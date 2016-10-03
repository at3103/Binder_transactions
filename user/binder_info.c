#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#define INCLUDED_BY_TEST
#include "../kernel/include/hw2/binder_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	pid_t pid;
	size_t size = 5000;
	void *buf = NULL;
	struct binder_stats *stats = (struct binder_stats *)NULL;
	struct binder_peer *peer = (struct binder_peer *)NULL;
	int is_invalid = 0;
	long count, i;
	size_t size_stat = sizeof(struct binder_stats);

	if (argc != 3) {
		fprintf(stderr, "Error: Invalid argument\n");
		return 0;
	}
	pid = (pid_t)atoi(argv[2]);
	if ((int)pid <= 0) {
		is_invalid = 3;
	} else if (strcmp(argv[1], "start") == 0) {
		if (syscall(244, pid, 1) != 0)
			is_invalid = 2;
	} else if (strcmp(argv[1], "print") == 0) {
		buf = (void *)malloc(size);
		stats = (struct binder_stats *)malloc(size_stat);
		count = syscall(245, pid, stats, buf, &size);
		if (count < 0L) {
			is_invalid = 2;
		} else {
			peer = (struct binder_peer *)buf;
			printf("%s (%u):\t%u bytes\t%u transactions\n",
			       stats->comm, pid, stats->bytes, stats->nr_trans);
			for (i = 0L; i < size;
				i += sizeof(struct binder_peer)) {
				printf("\t\t%s\t%u\t%u\n", peer->comm,
				       peer->pid, peer->uid);
				peer++;
			}
		}

		free(buf);
		free(stats);
	} else if (strcmp(argv[1], "stop") == 0) {
		if (syscall(244, pid, 0) != 0)
			is_invalid = 2;
	} else
		is_invalid = 1;

	/* err_handle */
	if (is_invalid == 1)
		fprintf(stderr, "Error: Invalid argument\n");
	else if (is_invalid == 2)
		fprintf(stderr, "Error processing pid %u: %s\n",
			pid, strerror(errno));
	else if (is_invalid == 3)
		fprintf(stderr, "Error: Invalid pid %d\n ",
			pid);

	return 0;
}
