#!/bin/bash

JOB_NAME=$1
IS_RUNNING=true

TOTAL_RUN=0
SUCCESS_COUNT=0

function trapHandler() {
    IS_RUNNING=false
    wait -n
    echo -e "[$JOB_NAME]\ttotal: ${TOTAL_RUN}\tsuccess: ${SUCCESS_COUNT}" | ./os_unix_lck_files statistics
}

trap trapHandler SIGINT

echo "staring ${JOB_NAME}"
while $IS_RUNNING; do
  if [[ ! $(./os_unix_lck_files test -s 1 & wait -n) ]]; then
    SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
  fi
  echo "after run ${JOB_NAME}"
  TOTAL_RUN=$((TOTAL_RUN + 1))
done


