#ifndef OS_UNIX_LCK_FILES_FILES_LOCKING_H
#define OS_UNIX_LCK_FILES_FILES_LOCKING_H

struct LockedFile {
    int fd;
    char * filename;

    int lockFd;
    char * lockFilename;
};

struct LockedFile * acquireLock(char * filename);
int releaseLock(struct LockedFile * lockedFile);

#endif //OS_UNIX_LCK_FILES_FILES_LOCKING_H
