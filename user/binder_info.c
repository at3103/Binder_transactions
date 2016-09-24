#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "../kernel/include/hw2/binder_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	/*Stub implementation*/
	pid_t pid;
	char *command = NULL;
	size_t *size = NULL;
	struct binder_stats stats;
	void *buf;
	struct binder_peer *peer;
	size_t l;

	buf = malloc(4096);
	l = sizeof(struct binder_peer);
	peer = (struct binder_peer *)buf;
	pid = (pid_t) atoi(argv[0]);

	strcpy(command, argv[1]);

	if (strcmp(command, "start")) {
		if (syscall(244, pid, 1) == -1) {
			fprintf(stderr, "Error processing pid %u: %s\n",
				pid, strerror(errno));
		}
	} else if (strcmp(command, "print")) {
		if (syscall(245, pid, stats, buf, size) == -1) {
			fprintf(stderr, "Error processing pid %u: %s\n",
				pid, strerror(errno));
		}
		printf("%s (%u):\t%u bytes\t%u transactions\n",
			stats.comm, pid, stats.bytes, stats.nr_trans);
		for (; peer != NULL; peer += l) {
			printf("\t\t%s\t%u\t%u\n", peer->comm,
				peer->pid, peer->uid);
		}
	} else if (strcmp(command, "stop")) {
		if (syscall(244, pid, 0) == -1) {
				fprintf(stderr, "Error processing pid %u: %s\n",
					pid, strerror(errno));
			}
	} else
		printf("Error: Invalid argument\n");

	free(buf);
	return 0;
}
