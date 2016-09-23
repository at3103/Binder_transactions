#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/printk.h>
#include <hw2/binder_utils.h>

struct binder_proc_data *_init_binder_trans_node(pid_t pid, int state)
{
	struct binder_proc_data *result = (struct binder_proc_data *)
		kmalloc(sizeof(struct binder_proc_data), __GFP_WAIT);
	struct task_struct *task = find_task_by_vpid(pid);

	result->peers_head = (struct binder_peers_wrapper *)NULL;
	result->peers_tail = (struct binder_peers_wrapper *)NULL;
	strcpy(result->stats.comm, task->comm);
	result->stats.nr_trans = 0; 
	result->stats.bytes = 0;
	result->pid = pid;
	INIT_LIST_HEAD(&(result->list));

	return result;
}

SYSCALL_DEFINE2(binder_rec, pid_t, pid, int, state)
{
	struct list_head *current_n, *found;
	struct binder_proc_data *data_node;

	found = (struct list_head *)NULL;
	if (binder_trans_tail == (struct binder_proc_data *)NULL) {
		if (state == 1) {
			binder_trans_head = _init_binder_trans_node(pid, state); 
			binder_trans_tail = binder_trans_head;
		} else
			return 0;
	}
	list_for_each(current_n, &binder_trans_head->list) {
		data_node = list_entry(current_n, struct binder_proc_data, list);
		if (data_node->pid == pid) {
			found = current_n;
			break;
		}
	}
	if (found == (struct list_head *)NULL) {
		if (state == 1) {
			data_node = _init_binder_trans_node(pid, state); 
			list_add(&(data_node->list), &(binder_trans_tail->list));
			binder_trans_tail = data_node;
		} else
			return 0;
	} else
		data_node->state = state;
	printk("\n\n%s\n\n", binder_trans_tail->stats.comm);

	return 0;
}

SYSCALL_DEFINE4(binder_stats, pid_t, pid, struct binder_stats *, stats,
		void *, buf, size_t *, size)
{
	struct list_head *current_n, *found = (struct list_head *)NULL;
	struct binder_proc_data *data_node;
	struct binder_peers_wrapper *peers_node;
	void *curbuf = buf;
	long result = 0L;

	if (binder_trans_tail == (struct binder_proc_data *)NULL)
		return -1;
	list_for_each(current_n, &(binder_trans_head->list)) {
		data_node = list_entry(current_n, struct binder_proc_data, list);
		if(data_node->pid == pid) {
			found = current_n;
			break;
		}
	}
	if (found == (struct list_head *)NULL)
		return -1;
	memcpy(stats, &(data_node->stats), sizeof(struct binder_stats));
	if (*size < sizeof(struct binder_peer))
		return -1;
	list_for_each_entry(peers_node, &(data_node->peers_head->list), list) {
		memcpy(curbuf, &(peers_node->peer), sizeof(struct binder_peer));
		result++;
		curbuf += sizeof(struct binder_peer);
		if (*size < curbuf - buf + sizeof(struct binder_peer))
			break;
	}
	*size = curbuf - buf;

	return result;
}
