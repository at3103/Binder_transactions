#include <linux/syscalls.h>
#include <hw2/binder_utils.h>
#include <linux/slab.h>
#include <linux/gfp.h>

SYSCALL_DEFINE2(binder_rec, pid_t, pid, int, state)
{
	struct list_head *current_n, *found;
	struct binder_proc_data *data_node;

	found = NULL;
	if(binder_list.list.next == NULL) {
		INIT_LIST_HEAD(&binder_list.list);
	}
	list_for_each(current_n, &binder_list.list) {
		data_node = list_entry(current_n, struct binder_proc_data, list);
		if(data_node->pid == pid) {
			found = current_n;
			break;
		}
	}
	if(found == NULL || data_node == NULL) {
		data_node = kmalloc(sizeof(struct binder_proc_data), __GFP_WAIT);
		data_node->state = state;
		data_node->stats.nr_trans = 0;
		data_node->stats.bytes = 0;
		//TODO: Get process name
		//data_node->stats.comm = "Hello World!";
		INIT_LIST_HEAD(&(data_node->peers).list);
		list_add(&(data_node->list), &(binder_list.list));
	} else {
		data_node->state = state;
	}
	return 0;
}

SYSCALL_DEFINE4(binder_stats, pid_t, pid, struct binder_stats *, stats,
		void *, buf, size_t *, size)
{
	struct list_head *current_n, *found;
	struct binder_proc_data *data_node;
	struct binder_peers_wrapper *peers_node;
	void *curbuf = buf;

	if(binder_list.list.next == NULL)
		return -1;
	list_for_each(current_n, &binder_list.list) {
		data_node = list_entry(current_n, struct binder_proc_data, list);
		if(data_node->pid == pid) {
			found = current_n;
			break;
		}
	}
	memcpy(stats, &(data_node->stats), sizeof(struct binder_stats));

	list_for_each_entry(peers_node, &(data_node->peers).list, list) {
		memcpy(curbuf, &(peers_node->peer), sizeof(struct binder_peer));
		curbuf += sizeof(struct binder_peer);
	}
	(*size) = curbuf - buf;
	return 0;
}
