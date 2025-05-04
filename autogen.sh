#!/bin/sh

# See Issue #38 -- Choose qmake first
QMAKE_BIN="$(which qmake 2> /dev/null)"
if [ ! -f "$QMAKE_BIN" ]; then
    # -- If it's not there, try qmake-qt5 second
    QMAKE_BIN="$(which qmake-qt5 2> /dev/null)"
fi

if [ ! -f "$QMAKE_BIN" ]; then
    # If it's still not there...
    echo "Did not find qmake. Please make sure either"
    echo "qmake or qmake-qt5 is provided by your system."
    exit 1
fi

QMAKE_SPEC="linux-g++"
if [ "$(uname -s)" = "Darwin" ]; then
  QMAKE_SPEC="macx-clang"
fi

QT_SELECT=qt5 $QMAKE_BIN qjournalctl.pro -r -spec $QMAKE_SPEC CONFIG+=release "PREFIX=\"$PREFIX\""

