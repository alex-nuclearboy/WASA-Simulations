#/bin/bash
mkdir $WASA_ROOT/alig/m4/B009
cp B009/* $WASA_ROOT/alig/m4/B009
mkdir $WASA_ROOT/alig/m4/B010
cp B010/* $WASA_ROOT/alig/m4/B010
cp al4cosy0_mc_009.dat042.m4 $WASA_ROOT/alig/m4
cp al4cosy0_mc_010.dat042.m4 $WASA_ROOT/alig/m4
cp mkalig.sh $WASA_ROOT/alig/m4
(cd $WASA_ROOT/alig/m4;./mkalig.sh)
