#include <linux/types.h>
#ifndef YIYAYIYAYO
#include <linux/list.h>
#include <linux/spinlock.h>
#endif

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

#ifndef YIYAYIYAYO
struct binder_peers_wrapper {
	struct binder_peer peer;
	struct list_head list;
};

struct binder_proc_data {
	struct binder_peers_wrapper *peers_head, *peers_tail;
	struct binder_stats stats;
	int state;
	pid_t pid;
	struct list_head list;
};

extern struct binder_proc_data *binder_trans_head;
extern struct binder_proc_data *binder_trans_tail;
extern spinlock_t my_binder_spin_lock;
#endif

#endif
