#!/bin/bash

PROCESS_COUNT=10
SLEEP_TIME="5m"

tasks=()

function taskRunner() {
    local jobName=$1

    local isRunning=true

    local totalRun=0
    local successCount=0

    function trapHandler() {
        isRunning=false
        wait -n
        echo -e "[$jobName]\ttotal: ${totalRun}\tsuccess: ${successCount}" | ./os_unix_lck_files results.txt
    }

    trap trapHandler SIGINT

    echo -e "\e[33m[${jobName}]\e[0m\tStating ${jobName}"
    while $isRunning; do
      if [[ ! $(./os_unix_lck_files test -s 1) ]]; then
        successCount=$((successCount + 1))
      fi
      echo -e "\e[33m[${jobName}]\e[0m\tAfter run ${jobName}"
      totalRun=$((totalRun + 1))
    done
}

function startAllProcess() {
    for i in $(seq ${PROCESS_COUNT}) ; do
        taskRunner "job-${i}" &
        tasks+=($!)
    done
}

function killAllProcess() {
    for i in "${!tasks[@]}"; do
      kill -s SIGINT "${tasks[$i]}"
    done
}

function waitAllProcessToSaveStatistics() {
    for i in "${!tasks[@]}"; do
        wait "${tasks[$i]}"
    done
}

echo -e "\e[32m[+]\e[0m\tStarting runme.sh script"

echo -e "\e[32m[+]\e[0m\tSetting trap handler"
trap killAllProcess INT

echo -e "\e[32m[+]\e[0m\tStarting all process"
startAllProcess

echo -e "\e[32m[+]\e[0m\tGoing to sleep"
sleep ${SLEEP_TIME}

echo -e "\e[32m[+]\e[0m\tSending sigint to all tasks"
killAllProcess


echo -e "\e[32m[+]\e[0m\tWaiting for tasks to end (saving statistics)"
waitAllProcessToSaveStatistics

echo -e "\e[32m[+]\e[0m\e[0m\tTasks statistics:"
cat results.txt
