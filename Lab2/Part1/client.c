#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "status_server.h"
#include "settings.h"

void print_server_info(STATUS_SERVER_t* server_info);

int main(void) {
    int fd;
    STATUS_SERVER_t* server_info;
    
    if((fd = open(MEMORY_MAPPED_FILE, O_RDWR)) == -1) 
    {
        printf("Exception: failed open memory mapped file %s\n", MEMORY_MAPPED_FILE);
        perror("");
        return -1;
    }

    if(MAP_FAILED == (server_info = (STATUS_SERVER_t*)mmap(NULL, sizeof(STATUS_SERVER_t),
            PROT_READ, MAP_SHARED, fd, 0))) 
    {
        perror("Exception: mmap exception");
        return -1;
    }

    print_server_info(server_info);
    munmap(server_info, sizeof(STATUS_SERVER_t));

    return 0;
}

void print_server_info(STATUS_SERVER_t* server_info){
    printf("Process ID: %li\n", (long)server_info -> pid);
    printf("Process user ID: %li\n", (long)server_info -> uid);
    printf("Process group ID: %li\n", (long)server_info -> pid);
    printf("Start time: %li\n", (long)server_info -> start_time);
    printf("Working time: %li\n", (long)server_info -> work_time);
    printf("Load average for 1 minute: %lf\n", server_info -> load_avg[0]);
    printf("Load average for 5 minutes: %lf\n", server_info -> load_avg[1]);
    printf("Load average for 15 minutes: %lf\n", server_info -> load_avg[2]);
}