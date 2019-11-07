#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/mman.h>
#include "status_server.h"
#include "settings.h"

#define TIME_SLEEP 1
#define MMAP_OFFSET 0

STATUS_SERVER_t* status_server;

void save_ids()
{
	status_server->pid = getpid();
	status_server->uid = getuid();
	status_server->gid = getgid();
}

int main()
{
    unlink(MEMORY_MAPPED_FILE);
    int fd;
    if((fd = open(MEMORY_MAPPED_FILE, O_RDWR | O_CREAT)) == -1) 
    {
        printf("Exception: failed open memory mapped file %s\n", MEMORY_MAPPED_FILE);
        perror("");
        return -1;
    }

    if(MAP_FAILED == (status_server = (STATUS_SERVER_t *)mmap(NULL, sizeof(STATUS_SERVER_t),
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, MMAP_OFFSET))) { /* IPC transport */
        perror("Exception: mmap exception");
        return -1;
    }
    if(truncate(MEMORY_MAPPED_FILE, sizeof(STATUS_SERVER_t)) == -1)
    {
        perror("Exception: truncate exception");
        return -1;
    }

    status_server->start_time = time(NULL);
    if(status_server->start_time < 0) 
    {
        perror("Exception: time_t overflow in server_time");
        return -1;
    }

    save_ids();

	while(1) 
    {
        // Get time from start.
        time_t current_time = time(NULL);
        if(current_time < 0)
        {
            perror("Exception: time_t overflow in server_time");
            return -1;
        }

        status_server->work_time = current_time - status_server->start_time;

        // Get avg.
        if(getloadavg(status_server->load_avg, NELEM) == -1)
        {
            perror("Exception: load average is unobtainable");
            return -1;
        }

        sleep(TIME_SLEEP);
    }

	return 0;
}