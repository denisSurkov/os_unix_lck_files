#include <stdio.h>
#include <unistd.h>

#include "files_locking.h"

int main(int argc, char * argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s filename [-s sleep_time]\n", argv[0]);
        return -1;
    }

    int sleepTime = getTimeToSleep();

    char * filenameToLock = argv[1];
    struct LockedFile * lockedFile = acquireLock(filenameToLock);
    if (lockedFile == NULL) {
        perror("failed to acquire lock: ");
        return -1;
    }

    if (sleepTime == 0) {
        char buffer[4096];
        int countRead = read(STDIN_FILENO, buffer, 4096);
        if (countRead != 0) {
            write(lockedFile->fd, buffer, countRead);
        }
    }

    if (sleepTime > 0) {
        sleep(sleepTime);
    }

    return releaseLock(lockedFile);
}


int getTimeToSleep(int argc, char * argv[]) {
    int timeToSleep = 0;

    int timeToSleepArgument = getopt(argc, argv, "s:");
    if (timeToSleepArgument == 's') {
        timeToSleep = (int) strtol(optarg, NULL, 10);
    }

    return timeToSleep;
}
