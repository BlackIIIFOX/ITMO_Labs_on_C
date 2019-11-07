#ifndef STATUS_SERVER

#include <time.h>
#include <sys/types.h>
#include "settings.h"

typedef struct {
    time_t start_time;
    time_t work_time;
    double load_avg[NELEM];

    pid_t pid;
    uid_t uid;
    gid_t gid;
} STATUS_SERVER_t;

#endif