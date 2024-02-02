#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status

# Check if necessary environment variables are set
if [ -z "${PLUTO_OUTPUT}" ] || [ -z "${WMC_DATA}" ]; then
    echo "ERROR: The environment variables PLUTO_OUTPUT and WMC_DATA must be defined."
    exit 1
fi

# Main loop to process each simulation run
for run_number in $(seq 1 1 10); do
    root_file="${PLUTO_OUTPUT}/$1-${run_number}.root"
    ems_file="${WMC_DATA}/$1-${run_number}.ems"    
    
    # Check if the input ROOT file exists
    if [ -e "${root_file}" ]; then
        # Skip processing if simulation results already exist
        if [ -e "${ems_file}" ]; then
            echo "INFO: Simulation for $1-${run_number} has already been completed and will be skipped."
            continue
        else
            script_name="wmc_run_$1-${run_number}.sh"
            # Check if the simulation is currently being processed
            if [ -e "${script_name}" ]; then
                echo "INFO: File $1-${run_number} is currently being processed."
                continue
            else
                echo "... Starting a WASA Monte Carlo simulation for $1-${run_number} ..."                
                cat > "${script_name}" <<- EOM
#!/bin/bash
# Automatically generated script for running Monte Carlo simulation
set -e
mkdir -p "wmc_tmp_$1-${run_number}"
cp -r run/* "wmc_tmp_$1-${run_number}"
cd "wmc_tmp_$1-${run_number}"
./wmc.sh "${root_file}" "${ems_file}"
cd ..
rm -rf "wmc_tmp_$1-${run_number}"
rm -f "${script_name}"
EOM
                chmod u+x "${script_name}"
                ./"${script_name}" >> output_wmc_$1-${run_number}.log 2>&1
                echo "The simulation for $1-${run_number} has been successfully completed at $(date). Results are available at ${ems_file}."
            fi
        fi
    else
        echo "ERROR: The ROOT file ${root_file} does not exist, skipping this simulation."
    fi
done
