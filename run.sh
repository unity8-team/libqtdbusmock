#!/bin/bash

set -e

SOURCEDIR="$PWD"
BRANCHNAME=`basename $SOURCEDIR`
BUILDDIR="$SOURCEDIR/../$BRANCHNAME-build"

$BUILDDIR/src/indicator-network-prompt
