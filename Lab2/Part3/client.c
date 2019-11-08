#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "status_server.h"
#include "settings.h"

int main(void) {
    int fd;
    STATUS_SERVER_t server_info;
    struct sockaddr_un addr;
    if((fd = socket(AF_UNIX, SOCK_STREAM, PROTOCOL)) == -1) {
        perror("Exception: Failed create an endpoint for communication");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, MY_SOCK_PATH);

    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Exception: Bad connection to socket");
        return -1;
    }

    read(fd, &server_info, sizeof(STATUS_SERVER_t));
    print_server_info(server_info);
    close(fd);
    return 0;
}