#pragma once

#include "types.h"

#define SYS_WRITE   64
#define SYS_GETPID  172

extern struct task_struct *current;

void sys_write(unsigned int fd, const char* buf, size_t count);
void sys_getpid(void);






