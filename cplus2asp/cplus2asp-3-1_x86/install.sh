#!/bin/bash

DEF_BIN_PATH_ROOT="/usr/local/bin"
DEF_LIB_PATH_ROOT="/usr/local/lib"
DEF_BIN_PATH_NONROOT="$HOME/bin"
DEF_LIB_PATH_NONROOT="$HOME/lib"

LIB_PATH=""
BIN_PATH=""
IS_ROOT=0

if [[ $EUID -ne 0 ]]; then

	echo "This script has NOT been run as root."
	echo "Selecting user-specific install location..."
	IS_ROOT=0
	LIB_PATH=$DEF_LIB_PATH_NONROOT
	BIN_PATH=$DEF_BIN_PATH_NONROOT

else

	echo "This script has been run as root."
	echo "Selecting system-wide install location..."
	IS_ROOT=1
	LIB_PATH=$DEF_LIB_PATH_ROOT
	BIN_PATH=$DEF_BIN_PATH_ROOT
fi

read -p "Select a location to install program binaries [$BIN_PATH]: " SEL
[ ! -z "$SEL" ] && BIN_PATH=$SEL;
read -p "Select a location to install program library files [$LIB_PATH]: " SEL
[ ! -z "$SEL" ] && LIB_PATH=$SEL;

echo ""
echo -e "Binary install path:  \t\t$BIN_PATH"
echo -e "Library install path: \t\t$LIB_PATH"

while true; do
    read -p "Do you wish to continue (Y/n) [$CONTINUE]? " yn
    [ -z "$yn" ] && break;
    case $yn in
        [Yy]* ) CONTINUE="Y"; break;;
        [Nn]* ) CONTINUE="n"; break;;
        * ) echo "Please answer yes (Y) or no (n).";;
    esac
done

if [ "$CONTINUE" != "Y" ] ; then
	echo "Installation canceled. Exiting with no changes."
	exit;
fi

mkdir -p $LIB_PATH
mkdir -p $BIN_PATH

cp -r lib/gringo.so $LIB_PATH/

cp bin/cplus2asp $BIN_PATH/.
cp bin/cplus2asp3.bin $BIN_PATH/.
cp bin/f2lp $BIN_PATH/.
cp bin/clingo3to4 $BIN_PATH/.
cp bin/clingo $BIN_PATH/.
cp bin/as2transition $BIN_PATH/.

# Fix permissions

if [ $IS_ROOT -eq 1 ] ; then
	chown -R root:root $LIB_PATH/gringo.so
	chmod -R a+r $LIB_PATH/gringo.so
	
	chown root:root $BIN_PATH/cplus2asp
	chmod a+rx $BIN_PATH/cplus2asp

	chown root:root $BIN_PATH/cplus2asp3.bin
	chmod a+rx $BIN_PATH/cplus2asp3.bin


	chown root:root $BIN_PATH/f2lp
	chmod a+rx $BIN_PATH/f2lp


	chown root:root $BIN_PATH/clingo3to4
	chmod a+rx $BIN_PATH/clingo3to4


	chown root:root $BIN_PATH/clingo
	chmod a+rx $BIN_PATH/clingo

	chown root:root $BIN_PATH/as2transition
	chmod a+rx $BIN_PATH/as2transition

else
	chmod -R ug+r $LIB_PATH/gringo.so
	chmod ug+rx $BIN_PATH/cplus2asp
	chmod ug+rx $BIN_PATH/cplus2asp3.bin
	chmod ug+rx $BIN_PATH/f2lp
	chmod ug+rx $BIN_PATH/clingo3to4
	chmod ug+rx $BIN_PATH/clingo
	chmod ug+rx $BIN_PATH/as2transition
	
fi


