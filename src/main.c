#include <stdio.h>
#include <unistd.h>

#include "files_locking.h"

int main(int argc, char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return -1;
    }

    char * filenameToLock = argv[1];
    struct LockedFile * lockedFile = acquireLock(filenameToLock);
    if (lockedFile == NULL) {
        perror("failed to acquire lock: ");
        return -1;
    }

    sleep(5);

    releaseLock(lockedFile);
    return 0;
}
