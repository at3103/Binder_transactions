#include <linux/types.h>

#ifndef BINDERS_R_THERE
#define BINDERS_R_THERE

struct binder_peer {
	uid_t uid;		/* UID of the communicating process */
	pid_t pid;		/* PID of the communicating process */
	char comm[16];		/* Name of communicating process */
};

struct binder_stats {
	char comm[16];		/* Name of recorded process */
	unsigned int nr_trans;	/* Total number of Binder transactions */
	unsigned int bytes;	/* Total number of bytes transferred */
};

#endif
