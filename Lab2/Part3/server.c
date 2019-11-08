#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include "status_server.h"
#include "settings.h"

#define TIME_SLEEP_SECONDS 1
#define LISTEN_BACKLOG 50

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


STATUS_SERVER_t status_server;
struct sockaddr_un addr_server;

int main()
{
	status_server.start_time = time(NULL);

	status_server.pid = getpid();
	status_server.uid = getuid();
	status_server.gid = getgid();

	memset(&addr_server, 0, sizeof(struct sockaddr_un));
	addr_server.sun_family = AF_UNIX;

	strncpy(addr_server.sun_path, MY_SOCK_PATH, sizeof(addr_server.sun_path) - 1);

	int fd_socket;

	if((fd_socket = socket(AF_UNIX, SOCK_STREAM, PROTOCOL)) == -1)
	{
		handle_error("Exception: Failed create an endpoint for communication");
	}
	
	if(bind(fd_socket, (struct sockaddr *) &addr_server, sizeof(struct sockaddr_un)) == -1)
	{
		handle_error("Exception: Failed bind a socket");
	}

	if (listen(fd_socket, LISTEN_BACKLOG) == -1)
	{
		handle_error("Exception: Failed init listen");
    }

	fcntl(fd_socket, F_SETFL, fcntl(fd_socket, F_GETFL) | O_NONBLOCK);

	int enable = 1;
	if (setsockopt(fd_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		handle_error("Exception: Failed setsockopt");
	}

	while(1) 
    {
        // Get time from start.
        time_t current_time;

        if((current_time =  time(NULL)) < 0)
        {
        	handle_error("Exception: time_t overflow in server_time");
        }

        status_server.work_time = current_time - status_server.start_time;

        // Get avg.
        if(getloadavg(status_server.load_avg, NELEM) == -1)
        {
        	handle_error("Exception: Load average is unobtainable");
        }

        // If client connect
        int fd_handle;
        if((fd_handle = accept(fd_socket, NULL, NULL)) != -1) 
        {
        	//printf("Connect\n");
        	//print_server_info(status_server);
            write(fd_handle, &status_server, sizeof(status_server));
        }

        sleep(TIME_SLEEP_SECONDS);
    }
	
	return 0;
}