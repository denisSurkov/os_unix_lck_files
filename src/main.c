#include <stdio.h>
#include <unistd.h>

#include "files_locking.h"

int main(int argc, char * argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return -1;
    }

    char * filenameToLock = argv[1];
    int lockFd = lockFile(filenameToLock);
//4.  Выполнить чтение/запись исходного файла myfile, добавить сон для наглядности;

    sleep(5);
    releaseFile(lockFd, filenameToLock);
    return 0;
}
