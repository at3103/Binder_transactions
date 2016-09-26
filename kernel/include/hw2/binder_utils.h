#include <linux/types.h>
#include <linux/list.h>

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

struct binder_peers_wrapper {
	struct binder_peer peer;
	struct list_head list;
};

struct binder_proc_data {
	struct binder_peers_wrapper peers;
	struct binder_stats stats;
	int state;
	int pid;
	struct list_head list;
};

extern struct binder_proc_data binder_list;

#endif
