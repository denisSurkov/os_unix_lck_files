#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "files_locking.h"

#define LOCK_EXTENSION ".lck"

int lockFile(char * filename) {
    int fd = -1;

    char lockFilename[strlen(filename) + strlen(LOCK_EXTENSION)];
    strcpy(lockFilename, filename);
    strcat(lockFilename, LOCK_EXTENSION);

    while (fd < 0) {
        fd = open(lockFilename, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (errno == EEXIST) continue;

        if (fd < 0) {
            perror("failed to open file .lck while acquiring lock: ");
            return -1;
        }
    }

    char payload[20];
    int cnt = snprintf(payload, 20, "%zu", getpid());
    int writeRes = write(fd, payload, cnt);
    if (writeRes == -1) {
        perror("failed to write pid to .lck file: ");
        return -1;
    }

    return fd;
}

int releaseFile(int fd, char * filename) {
    //5.  Проверить, что файл .lck ещё существует и в нём записан свой pid. Если это так, то удалить файл, если нет — завершиться с сообщением об ошибке.
    int closeRes = close(fd);

    char lockFilename[strlen(filename) + strlen(LOCK_EXTENSION)];
    strcpy(lockFilename, filename);
    strcat(lockFilename, LOCK_EXTENSION);

    if (remove(lockFilename) != 0) {
        perror("remove: ");
    }

    return 0;
}

