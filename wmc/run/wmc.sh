#!/bin/csh -f
#******************************************************************************
# Script for running WASA Monte Carlo
# $Id: wmc.sh 277 2008-10-30 10:31:03Z hejny $
# Modified: 2015-04 by rundel
# Modified: 2024-02-01 by khreptak
#******************************************************************************

# Display current date and host name
date
hostname

echo "-----------------------------------------------------------------------"
echo "      WASA Monte Carlo Run $1"
echo "-----------------------------------------------------------------------"

# Ensure the WASA_ROOT and SYSTEM environment variables are defined
if ( ! $?WASA_ROOT ) then
    echo "Environment variable WASA_ROOT is not defined. Exiting."
    exit 1
endif

# Setup environment variables
setenv RWLIB_ROOT	$WASA_ROOT/../RWlib
if ( ! -d "$RWLIB_ROOT" ) then
    echo "RWLIB_ROOT directory was not found. Exiting."
    exit 1
endif
setenv LD_LIBRARY_PATH $RWLIB_ROOT/lib:${LD_LIBRARY_PATH}
echo "LD_LIBRARY_PATH is set to: $LD_LIBRARY_PATH"

# Determine the system and set variables accordingly
if ( "$SYSTEM" == "" ) then
    echo "UserVariable SYSTEM is not set. Should be set to Linux or OSF1. Exiting."
    exit 1
endif

setenv UNAME $SYSTEM

if ( $UNAME == "OSF1" ) then
    echo "You are working under: " $UNAME
    setenv NICE     '/usr/bin/nice -n 4'
    setenv TIME     '/bin/time'
endif

if ( $UNAME == "Linux" ) then
    echo "You are working under: " $UNAME
    setenv NICE     '/usr/bin/nice -4'
    setenv TIME     '/usr/bin/time'
endif

echo "Setup links for Monte-Carlo program"
rm -f fort.* >& /dev/null
rm -f epio41 epio42 pluto.root >& /dev/null
rm -f etap.ems >& /dev/null

# Input and Output
ln -s  $1 pluto.root
ln -s  $2 etap.ems

# Kinematic event input text file
ln -s  $WASA_ROOT/examples/evg/etap.out     fort.31
ln -s  etap.epo     epio41
ln -s  etap.ems     epio42

# ALIGMENT FILES
$WASA_ROOT/alig/links.sh
ln -s $WASA_ROOT/alig/al4cosy0_mc_009.dat042    fort.13

# USER CARDS
m4 -I$WASA_ROOT/alig/m4 -P wmc.dat.m4 > wmc.dat
ln -s wmc.dat   fort.4

echo "Running Main Program WMC"
$NICE $TIME $WASA_ROOT/examples/wmc3/src/wmc.exe

echo "Cleaning up temporary files"
rm -f fort.* >& /dev/null
rm -f epio41 epio42 pluto.root >& /dev/null
if ( -z test.epo ) then
    rm -f test.epo >& /dev/null
endif

# Display completion message
echo "Script execution completed."
date

# Exit the script
exit 0
