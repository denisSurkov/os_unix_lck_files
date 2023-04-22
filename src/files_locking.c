#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "files_locking.h"

#define LOCK_EXTENSION ".lck"


int openLockFile(char * lockFilename) {
    int fd = -1;

    while (fd < 0) {
        fd = open(lockFilename, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (errno == EEXIST) continue;

        if (fd < 0) {
            perror("failed to open file .lck while acquiring lock: ");
            return -1;
        }
    }

    return fd;
}

int writePidToLockFile(int lockFd) {
    char payload[32];
    int cnt = snprintf(payload, 32, "%d", getpid());
    int writeRes = write(lockFd, payload, cnt);
    if (writeRes == -1) {
        perror("failed to write pid to .lck file: ");
        return -1;
    }
    return 0;
}

int checkCorrectPid(int lockFd) {
    char payload[32];
    lseek(lockFd, 0, SEEK_SET);
    int readSize = read(lockFd, payload, 32);
    if (readSize == -1) {
        perror("failed to read pid .lck file while closing: ");
        return -1;
    }

    unsigned int pidFromFile = (unsigned int) strtoul(payload, NULL, 10);
    if (pidFromFile != getpid()) {
        fprintf(stderr, "wrong pid in .lck file progam: %d, file: %d", getpid(), pidFromFile);
        return -1;
    }

    return 0;
}

void freeLockMemory(struct LockedFile * lockedFile) {
    free(lockedFile->lockFilename);
    free(lockedFile->filename);
    free(lockedFile);
}


struct LockedFile * acquireLock(char * filename) {
    struct LockedFile * lockedFile = malloc(sizeof(struct LockedFile));
    lockedFile->filename = malloc(sizeof(char) * (strlen(filename)  + 1));
    strcpy(lockedFile->filename, filename);

    lockedFile->lockFilename = malloc(sizeof(char) * (strlen(filename) + strlen(LOCK_EXTENSION) + 1));
    strcpy(lockedFile->lockFilename, filename);
    strcat(lockedFile->lockFilename, LOCK_EXTENSION);

    lockedFile->lockFd = -1;
    lockedFile->fd = -1;

    int lockFd = openLockFile(lockedFile->lockFilename);
    if (lockFd == -1) {
        freeLockMemory(lockedFile);
        return NULL;
    }

    int writeRes = writePidToLockFile(lockFd);
    if (writeRes == -1) {
        releaseLock(lockedFile);
        return NULL;
    }

    lockedFile->lockFd = lockFd;
    int fd = open(filename, O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("failed to open original file: ");
        releaseLock(lockedFile);
        return NULL;
    }

    lockedFile->fd = fd;

    return lockedFile;
}

int releaseLock(struct LockedFile * lockedFile) {
    if (lockedFile->fd != -1) {
        close(lockedFile->fd);
    }

    if (checkCorrectPid(lockedFile -> lockFd) != 0) {
        close(lockedFile -> lockFd);
        freeLockMemory(lockedFile);
        return -1;
    }

    if (lockedFile -> lockFd != -1) {
        close(lockedFile -> lockFd);
    }

    if (remove(lockedFile->lockFilename) != 0) {
        perror("remove: ");
        freeLockMemory(lockedFile);
        return -1;
    }

    freeLockMemory(lockedFile);
    return 0;
}
