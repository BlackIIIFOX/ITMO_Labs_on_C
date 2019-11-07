#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

pid_t pid;
uid_t uid;
gid_t gid;

void save_ids()
{
	pid = getpid();
	uid = getuid();
	gid = getgid();

}

int main(int argc, char const *argv[])
{
	

	return 0;
}