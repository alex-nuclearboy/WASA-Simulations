#!/bin/bash

# Check if the WASA_ROOT environment variable is set
if [ -z "$WASA_ROOT" ]; then
    echo "ERROR: The environment variable WASA_ROOT is not set."
    exit 1
fi

echo "Setting up environment for B009..."
mkdir -p "$WASA_ROOT/alig/m4/B009"
cp B009/* "$WASA_ROOT/alig/m4/B009"
echo "Files for B009 have been copied successfully."

echo "Setting up environment for B010..."
mkdir -p "$WASA_ROOT/alig/m4/B010"
cp B010/* "$WASA_ROOT/alig/m4/B010"
echo "Files for B010 have been copied successfully."

echo "Copying configuration files to $WASA_ROOT/alig/m4..."
cp al4cosy0_mc_009.dat042.m4 "$WASA_ROOT/alig/m4"
cp al4cosy0_mc_010.dat042.m4 "$WASA_ROOT/alig/m4"
cp mkalig.sh "$WASA_ROOT/alig/m4"

echo "Executing mkalig.sh in $WASA_ROOT/alig/m4..."

(cd "$WASA_ROOT/alig/m4" && ./mkalig.sh) || {
    echo "ERROR: Failed to execute mkalig.sh"
    exit 1
}

echo "Setup completed successfully."
