#include "status_server.h"
#include <stdio.h>
#include <stdlib.h>

void print_server_info(STATUS_SERVER_t server_info)
{
    printf("Process ID: %d\n", server_info.pid);
    printf("Process user ID: %d\n", server_info.uid);
    printf("Process group ID: %d\n", server_info.gid);
    printf("Start time: %lu\n", server_info.start_time);
    printf("Working time: %lu\n", server_info.work_time);
    printf("Load average for 1 minute: %lf\n", server_info.load_avg[0]);
    printf("Load average for 5 minutes: %lf\n", server_info.load_avg[1]);
    printf("Load average for 15 minutes: %lf\n", server_info.load_avg[2]);

    /*printf("Process ID: %d\n", server_info->pid);
    printf("Process user ID: %d\n", server_info->uid);
    printf("Process group ID: %d\n", server_info->gid);
    printf("Start time: %lu\n", server_info->start_time);
    printf("Working time: %lu\n", server_info->work_time);
    printf("Load average for 1 minute: %lf\n", server_info->load_avg[0]);
    printf("Load average for 5 minutes: %lf\n", server_info->load_avg[1]);
    printf("Load average for 15 minutes: %lf\n", server_info->load_avg[2]);*/
}