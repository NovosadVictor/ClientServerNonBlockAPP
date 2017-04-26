#!/bin/bash



seq 1 2 | while read $i;
do echo -en "\rwaiting...[.--]";
sleep 1;
echo -en "\rwaiting...[-.-]";
sleep 1;
echo -en "\rwaiting...[--.]";
sleep 1;


done
