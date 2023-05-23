#ifndef ROCKET_COMMON_UTIL_H
#define ROCKET_COMMON_UTIL_H

#include <sys/types.h>
#include <unistd.h>

namespace tinyrpc {

    pid_t getPid();

    pid_t getThreadId();

}

#endif