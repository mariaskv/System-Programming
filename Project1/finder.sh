#!/bin/bash
	declare IFS=' ' 
	for type in ./Output/*.out ; do
		declare -i COUNTER=0
		while read -ra t; do
			if [[ $t == *$1 ]]
				then
					COUNTER=$((COUNTER+1)) 
				fi
		done < "$type"
		echo "In file: $type the TLD: $1 was found $COUNTER times."
	done
