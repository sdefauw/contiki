#!/bin/bash

while IFS=$'\n' read -r line || [[ -n "$line" ]];     
do
	l=($line)
	node=${l[0]}
	typefire="6l${l[1]}"
	echo "PUT Firmware $typefire in $node"
done <node.txt       