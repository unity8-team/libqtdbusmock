'''Hostname1 mock template
'''

# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3 of the License, or (at your option) any
# later version.  See http://www.gnu.org/copyleft/lgpl.html for the full text
# of the license.

__author__ = 'Pete Woods'
__email__ = 'pete.woods@canonical.com'
__copyright__ = '(c) 2016 Canonical Ltd.'
__license__ = 'LGPL 3+'

import dbus
import uuid
import binascii

from dbusmock import MOCK_IFACE
import dbusmock


BUS_NAME = 'org.freedesktop.hostname1'
MAIN_IFACE = 'org.freedesktop.hostname1'
MAIN_OBJ = '/org/freedesktop/hostname1'
SYSTEM_BUS = True


def load(mock, parameters):
    mock.AddProperties(MAIN_IFACE, {
       'Hostname': parameters.get('Hostname', 'my-computer'),
       'StaticHostname': parameters.get('StaticHostname', 'ubuntu'),
       'PrettyHostname': parameters.get('PrettyHostname', ''),
       'IconName': parameters.get('IconName', 'computer-vm'), # e.g. "computer-laptop", "computer-desktop"
       'Chassis': parameters.get('Chassis', 'vm'), # currently defined set: "desktop", "laptop", "server", "tablet", "handset", "vm" and "container"
       'Deployment': parameters.get('Deployment', ''),
       'Location': parameters.get('Location', ''),
       'KernelName': parameters.get('KernelName', 'Linux'),
       'KernelRelease': parameters.get('KernelRelease', '4.4.0-53-generic'),
       'KernelVersion': parameters.get('KernelVersion', '#74-Ubuntu SMP Fri Dec 2 15:59:10 UTC 2016'),
       'OperatingSystemPrettyName': parameters.get('OperatingSystemPrettyName', 'Ubuntu 16.04.1 LTS'),
       'OperatingSystemCPEName': parameters.get('OperatingSystemCPEName', ''),
    })

    mock.AddMethods(MAIN_IFACE, [
        ('SetHostname', 'sb', '', "ret = self.Set('%s', 'Hostname', args[0])" % MAIN_IFACE),
        ('SetStaticHostname', 'sb', '', "ret = self.Set('%s', 'StaticHostname', args[0])" % MAIN_IFACE),
        ('SetPrettyHostname', 'sb', '', "ret = self.Set('%s', 'PrettyHostname', args[0])" % MAIN_IFACE),
        ('SetIconName', 'sb', '', "ret = self.Set('%s', 'IconName', args[0])" % MAIN_IFACE),
        ('SetChassis', 'sb', '', "ret = self.Set('%s', 'Chassis', args[0])" % MAIN_IFACE),
        ('SetDeployment', 'sb', '', "ret = self.Set('%s', 'Deployment', args[0])" % MAIN_IFACE),
        ('SetLocation', 'sb', '', "ret = self.Set('%s', 'Location', args[0])" % MAIN_IFACE),
    ])
