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

	pid = (pid_t)atoi(argv[0]);


	if (argc != 3)
		is_invalid = 1;


	if ((int)pid == 0)
		is_invalid = 1;

	if (strcmp(argv[1], "start") == 0) {

		if (syscall(244, pid, 1) != 0)
			is_invalid = 2;

	} else if (strcmp(argv[1], "print") == 0) {

		count = syscall(245, pid, stats, buf, &size);

		if (count < 0L)
			is_invalid = 2;

		peer = (struct binder_peer *)buf;
		printf("%s (%u):\t%u bytes\t%u transactions\n",
		       stats->comm, pid, stats->bytes, stats->nr_trans);

		for (i = 0L; i < count; i++)
			printf("\t\t%s\t%u\t%u\n", peer[i].comm,
			       peer[i].pid, peer[i].uid);

		free(peer);

	} else if (strcmp(argv[1], "stop") == 0) {
		if (syscall(244, pid, 0) != 0)
			is_invalid = 2;

	} else
		is_invalid = 1;

/*err_handle*/
if (is_invalid == 1)
	fprintf(stderr, "Error: Invalid argument\n");

else if (is_invalid == 2)
	fprintf(stderr, "Error processing pid %u: %s\n",
	pid, strerror(errno));

/*terminate*/
free(buf);
free(stats);
return 0;
}
