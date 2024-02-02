#!/bin/csh -f
#******************************************************************************
# Script for running WASA Monte Carlo
# $Id: wmc.sh 277 2008-10-30 10:31:03Z hejny $
# Modified: 2015-04 by rundel
# Modified: 2024-02-01 by khreptak
#******************************************************************************

# Display start time and host information
echo "Starting WASA Monte Carlo simulation at: `date`"
echo "Running on host: `hostname`"

echo "-----------------------------------------------------------------------"
echo "      Initiating WASA Monte Carlo Simulation for: $1"
echo "-----------------------------------------------------------------------"

# Validate necessary environment variables
if ( ! $?WASA_ROOT ) then
    echo "ERROR: Environment variable WASA_ROOT is not defined. Exiting."
    exit 1
endif

if ( "$SYSTEM" == "" ) then
    echo "ERROR: System type (SYSTEM variable) is not defined. Set to Linux or OSF1. Exiting."
    exit 1
endif

# Check alignment file existence
if ( ! -f ${WASA_ROOT}/alig/al4cosy0_mc_009.dat042 ) then
    echo "ERROR: Alignment file not found: ${WASA_ROOT}/alig/al4cosy0_mc_009.dat042"
    echo "Please run the configuration script for the corresponding experimental setup first."
    exit 1
endif

# Setup environment variables
echo "Configuring environment variables..."
setenv RWLIB_ROOT	$WASA_ROOT/../RWlib
if ( ! -d "$RWLIB_ROOT" ) then
    echo "ERROR: RWLIB_ROOT directory ($RWLIB_ROOT) was not found. Exiting."
    exit 1
endif

setenv LD_LIBRARY_PATH $RWLIB_ROOT/lib:${LD_LIBRARY_PATH}
echo "Environment setup completed. LD_LIBRARY_PATH is set to: $LD_LIBRARY_PATH"

# System-specific configurations
echo "Configuring system-specific settings..."
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

echo "System configuration completed."

# Preparing simulation environment
echo "Preparing simulation environment..."
rm -f fort.* >& /dev/null
rm -f epio41 epio42 pluto.root >& /dev/null
rm -f etap.ems >& /dev/null

# Configure input and output
ln -s  $1 pluto.root
ln -s  $2 etap.ems

# Configure kinematic event input text file and alignment files
ln -s  $WASA_ROOT/examples/evg/etap.out     fort.31
ln -s  etap.epo     epio41
ln -s  etap.ems     epio42

# Linking alignment files
$WASA_ROOT/alig/links.sh
ln -s $WASA_ROOT/alig/al4cosy0_mc_009.dat042    fort.13

# Process user cards
m4 -I$WASA_ROOT/alig/m4 -P wmc.dat.m4 > wmc.dat
ln -s wmc.dat   fort.4

echo "Simulation environment prepared. Starting main program..."
$NICE $TIME $WASA_ROOT/examples/wmc3/src/wmc.exe

# Clean up the simulation environment
echo "Cleaning up temporary files..."
rm -f fort.* >& /dev/null
rm -f epio41 epio42 pluto.root etap.ems >& /dev/null
if ( -e test.epo ) then
    rm -f test.epo >& /dev/null
endif

# Display completion message
echo "WASA Monte Carlo simulation completed at: `date`"
exit 0
