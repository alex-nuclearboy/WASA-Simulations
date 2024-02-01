#!/bin/bash

if [ -z "${PLUTO_OUTPUT}" ] || [ -z "${WMC_DATA}" ]; then
	echo "Environment variables PLUTO_OUTPUT and WMC_DATA must be set."
	exit 1
fi

for run_number in $(seq 1 1 10); do
	root_file="${PLUTO_OUTPUT}/$1-${run_number}.root"
	ems_file="${WMC_DATA}/$1-${run_number}.ems"

	if [ -e "${root_file}" ]; then
		if [ -e "${ems_file}" ]; then
			echo "File $1-${run_number} has already been completed."
		else
			script_name="run_wmc-$1-${run_number}.sh"
			if [ -e "${script_name}" ]; then
				echo "File $1-${run_number} is currently being processed."
			else
			echo "... Starting a Monte Carlo simulation of the file $1-${run_number} ..."

			cat > "${script_name}" <<- EOM
				#!/bin/bash
				set -e
				mkdir -p "wmc_tmp_$1-${run_number}"
				cp -r run/* "wmc_tmp_$1-${run_number}"
				cd "wmc_tmp_$1-${run_number}"
				./wmc.sh "${PLUTO_OUTPUT}/$1-${run_number}.root" "${WMC_DATA}/$1-${run_number}.ems"
				cd ..
				rm -rf "wmc_tmp_$1-${run_number}"
				rm -f "${script_name}"
			EOM

			chmod u+x "${script_name}"
			./"${script_name}" >> output_wmc_$1-${run_number}.log 2>&1
			echo "Simulation of the file has been successfully completed at $(date). Results are available at ${WMC_DATA}/$1-${run_number}.ems."
			fi
		fi
	else
		echo "ROOT file $1-${run_number}.root does not exist"
	fi
done

