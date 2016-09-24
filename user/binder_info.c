#include "../kernel/include/uapi/linux/types.h"
//#include <uapi/linux/types.h>
#include "../kernel/include/hw2/binder_utils.h"
//#include <linux/types.h>
#include "../kernel/include/linux/types.h"
#include <string.h>

int main(int argc, char **argv)
{
	//Stub implementation
	pid_t pid;
	char *command;
	size_t *size;
	binder_stats stats;
	void *buf;
	void *peer;
	
	buf = malloc(4096);

	
	pid = (pid_t) argv[0];
	strcpy(command, argv[1]);
	
	if (strcmp(command, "start") 
		if (binder_rec(pid, 1) == -1) {
                	fprintf(stderr, "Error processing pid %u: %s\n",
                pid, strerror(errno));
                }
	else if (strcmp(command, "print") {
		if(binder_stats(pid, stats, buf, size)) == -1) {
			fprintf(stderr, "Error processing pid %u: %s\n",
                pid, strerror(errno));
		}
		printf("%s (%u):\t%u bytes\t%u transactions\n",
                stats.comm, pid, stats.bytes, stats.nr_trans);
		for (peer = buf; *peer != NULL; peer = peer + sizeof(struct binder_perr)) {
			printf("\t\t%s\t%u\t%u\n", ->comm, peer->pid, peer->uid);
}
	}
	else if (strcmp(command, "stop")
		if (binder_rec(pid, 0)== -1) {
                        fprintf(stderr, "Error processing pid %u: %s\n",
                pid, strerror(errno));
                }
	else
		printf("Error: Invalid argument\n");
 

	return 0;
}
