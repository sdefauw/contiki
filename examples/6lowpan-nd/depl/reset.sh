#!/bin/bash

while IFS=$'\n' read -r line || [[ -n "$line" ]];
do
	l=($line)
	node=${l[0]}
	tty=${l[1]}
	typ=${l[2]}
	typefire="6l${l[3]}"
	echo "PUT Firmware $typefire in $node"
	echo "RESET $typefire: $node"
	tb_action reset --dev $tty -t sky &
done <node.txt
