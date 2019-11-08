#ifndef STATUS_SERVER_H
#define STATUS_SERVER_H

#include <time.h>
#include <sys/types.h>
#include "settings.h"

typedef struct _STATUS_SERVER_t {
    time_t start_time;
    time_t work_time;
    double load_avg[NELEM];
    pid_t pid;
    uid_t uid;
    gid_t gid;
} STATUS_SERVER_t;

void print_server_info(STATUS_SERVER_t server_info);

#endif