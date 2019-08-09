#!/bin/csh -f 
#******************************************************************************
#          Script for running WASA Monte-Carlo     
#          $Id: wmc.sh 277 2008-10-30 10:31:03Z hejny $
#     Modified 2015-04 by rundel
#******************************************************************************
 date
 hostname
# Following variables should be defined:
# $WASA_ROOT $SYSTEM 
if ($WASA_ROOT =="") then
    echo "WASA_ROOT not defined exit"
    exit 1
endif
setenv RWLIB_ROOT  $WASA_ROOT/../RWlib
setenv LD_LIBRARY_PATH $RWLIB_ROOT/lib:${LD_LIBRARY_PATH}
echo $LD_LIBRARY_PATH

setenv UNAME $SYSTEM
 if ( $UNAME == "OSF1" ) then
  echo "You are working under: " $UNAME
  setenv NICE        '/usr/bin/nice -n 4'
  setenv TIME        '/bin/time'
 endif

 if ( $UNAME == "Linux" ) then
  echo "You are working under: " $UNAME
  setenv NICE        '/usr/bin/nice -4 '
  setenv TIME        '/usr/bin/time '
 endif

 if ( $UNAME == "" ) then
   echo "UserVariable SYSTEM is not set. Should be set to Linux or OSF1"
   exit 1
 endif
 echo "Setup links for Monte-Carlo program"
 rm -f fort.* >& /dev/null
 rm -f epio41 epio42 pluto.root >& /dev/null
 rm -f etap.ems >& /dev/null
#INPUT AND OUTPUT
 ln -s  $1 pluto.root
 ln -s  $2 etap.ems
# kinematic event input  text file
 ln -s  $WASA_ROOT/examples/evg/etap.out       fort.31 
 ln -s  etap.epo           epio41 
 ln -s  etap.ems           epio42
# ALIGMENT FILES
$WASA_ROOT/alig/links.sh
ln -s $WASA_ROOT/alig/al4cosy0_mc_009.dat042 fort.13
# USER CARDS 
 m4 -I$WASA_ROOT/alig/m4 -P wmc.dat.m4 > wmc.dat
 ln -s wmc.dat fort.4 

 echo "Running Main Program WMC"
$NICE $TIME $WASA_ROOT/examples/wmc3/src/wmc.exe
 rm -f fort.* >& /dev/null 
 rm -f epio41 epio42 pluto.root >& /dev/null
 if ( -z test.epo ) then
    rm -f test.epo >& /dev/null
 endif
 echo ""
 date
 exit 0
