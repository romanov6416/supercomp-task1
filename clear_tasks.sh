#!/usr/bin/env bash
for task_id in $(squeue | grep "$(echo $USER | cut -c1-7)" | awk '{ print $1 }' )
do
scancel $task_id
done