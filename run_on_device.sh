#!/bin/sh
CODE_DIR=indicator-network-prompt
USER=phablet
USER_ID=32011
PASSWORD=phablet
PACKAGE=indicator-network-prompt
#BINARY=
TARGET_IP=${TARGET_IP-127.0.0.1}
TARGET_SSH_PORT=${TARGET_SSH_PORT-2222}
RUN_OPTIONS=
SETUP=false
GDB=false
SUDO="echo $PASSWORD | sudo -S"
NUM_JOBS='$(( `grep -c ^processor /proc/cpuinfo` + 1 ))'

usage() {
    echo "usage: run_on_device [OPTIONS]\n"
    echo "Script to setup a build environment for the shell and sync build and run it on the device\n"
    echo "OPTIONS:"
    echo "  -s, --setup   Setup the build environment"
    echo ""
    echo "IMPORTANT:"
    echo " * Make sure to have the networking and PPAs setup on the device beforehand (phablet-deploy-networking && phablet-ppa-fetch)."
    echo " * Execute that script from a directory containing a branch the shell code."
    exit 1
}

exec_with_ssh() {
    ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -t $USER@$TARGET_IP -p $TARGET_SSH_PORT "bash -ic \"$@\""
}

exec_with_adb() {
    adb shell $@
}

adb_root() {
    adb root
    adb wait-for-device
}

install_ssh_key() {
    ssh-keygen -R $TARGET_IP
    HOME_DIR="/home/phablet"
    adb push ~/.ssh/id_rsa.pub $HOME_DIR/.ssh/authorized_keys
    adb shell chown $USER_ID:$USER_ID $HOME_DIR/.ssh
    adb shell chown $USER_ID:$USER_ID $HOME_DIR/.ssh/authorized_keys
    adb shell chmod 700 $HOME_DIR/.ssh
    adb shell chmod 600 $HOME_DIR/.ssh/authorized_keys
}

setup_adb_forwarding() {
    adb forward tcp:$TARGET_SSH_PORT tcp:22
    adb forward tcp:$TARGET_DEBUG_PORT tcp:$TARGET_DEBUG_PORT
}

install_dependencies() {
    exec_with_adb apt-get update
    exec_with_adb apt-get -y install openssh-server
    exec_with_ssh $SUDO apt-get -y install build-essential rsync bzr ccache gdb ninja-build devscripts equivs
}

sync_code() {
    [ -e .bzr ] && bzr export --uncommitted --format=dir /tmp/$CODE_DIR
    [ -e .git ] && git checkout-index -a -f --prefix=/tmp/$CODE_DIR/
    rsync -crlOzv --delete --exclude builddir -e "ssh -p $TARGET_SSH_PORT -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no" /tmp/$CODE_DIR/ $USER@$TARGET_IP:$CODE_DIR/
    rm -rf /tmp/$CODE_DIR
}

build() {
    exec_with_ssh "[ $CODE_DIR/debian/control -nt unity8-build-deps*deb ] && $SUDO mk-build-deps --tool=\\\"apt-get -y --no-install-recommends\\\" --build-dep --install $CODE_DIR/debian/control"
    exec_with_ssh PATH=/usr/lib/ccache:$PATH "cd $CODE_DIR/ && PATH=/usr/lib/ccache:$PATH ./build.sh"
}

run() {
    exec_with_ssh "cd $CODE_DIR/ && ./run.sh $ARGS -- $RUN_OPTIONS"
}

set -- `getopt -n$0 -u -a --longoptions="setup,gdb,help" "sgpkh" "$@"`

# FIXME: giving incorrect arguments does not call usage and exit
while [ $# -gt 0 ]
do
    case "$1" in
       -s|--setup)   SETUP=true;;
       -g|--gdb)     GDB=true;;
       -h|--help)    usage;;
       --)           shift;break;;
    esac
    shift
done


adb_root
[ "${TARGET_IP}" = "127.0.0.1" ] && setup_adb_forwarding

if [ "`adb shell 'grep -q Ubuntu /etc/lsb-release 2> /dev/null && echo -n FLIPPED'`" = "FLIPPED" ]; then
    if [ "${TARGET_IP}" != "127.0.0.1" ]; then
        echo "ERROR: Flipped image detected, adb over TCP/IP isn't supported, yet :/"
        echo "Unset TARGET_IP and try again."
        exit 2
    fi
fi

if $SETUP; then
    echo "Setting up environment for building shell.."
    install_ssh_key
    install_dependencies
    sync_code
else
    echo "Transferring code.."
    sync_code
    echo "Building.."
    build
    echo "Running.."
    run
fi

