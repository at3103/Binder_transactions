#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "../kernel/include/hw2/binder_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	pid_t pid;
	size_t size;
	void *buf = NULL;
	struct binder_stats *stats = (struct binder_stats *)NULL;
	struct binder_peer *peer = (struct binder_peer *)NULL;
	int is_invalid = 0;
	long count, i;

	if (argc != 3) {
		is_invalid = 1;
		goto err_handle;
	}
	pid = (pid_t)atoi(argv[2]);
	if ((int)pid == 0) {
		is_invalid = 1;
		goto err_handle;
	}

	if (strcmp(argv[1], "start") == 0) {
		if (syscall(244, pid, 1) != 0)
			goto err_handle;
	} else if (strcmp(argv[1], "print") == 0) {
		count = syscall(245, pid, stats, buf, &size);
		if (count < 0L)
			goto err_handle;
		peer = (struct binder_peer *)buf;
		/*
		printf("%s (%u):\t%u bytes\t%u transactions\n",
		       stats->comm, pid, stats->bytes, stats->nr_trans);
		*/
		for (i = 0L; i < count; i++)
			printf("\t\t%s\t%u\t%u\n", peer[i].comm,
			       peer[i].pid, peer[i].uid);
	} else if (strcmp(argv[1], "stop") == 0) {
		if (syscall(244, pid, 0) != 0)
			goto err_handle;
	} else {
		is_invalid = 1;
		goto err_handle;
	}
	goto terminate;

err_handle:
	if (is_invalid)
		fprintf(stderr, "Error: Invalid argument\n");
	else
                fprintf(stderr, "Error processing pid %u: %s\n",
			pid, strerror(errno));
terminate:
	free(buf);
	free(stats);
	return 0;
}
