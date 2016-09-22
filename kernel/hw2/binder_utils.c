#include <linux/syscalls.h>
#include <linux/types.h>
#include <hw2/binder_utils.h>

SYSCALL_DEFINE2(binder_rec, pid_t, pid, int, state)
{
	return 0;
}

SYSCALL_DEFINE4(binder_stats, pid_t, pid, struct binder_stats, *stats,
		void, *buf, size_t, *size)
{
	return 0;
}
