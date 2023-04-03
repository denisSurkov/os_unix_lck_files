#ifndef OS_UNIX_LCK_FILES_FILES_LOCKING_H
#define OS_UNIX_LCK_FILES_FILES_LOCKING_H

int lockFile(char * filename);

int releaseFile(int fd, char * filename);;

#endif //OS_UNIX_LCK_FILES_FILES_LOCKING_H
