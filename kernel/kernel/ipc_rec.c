/*
 *  Copyright (C) 2016 Columbia University
 *
 *  Author: W4118 Staff <w4118@lists.cs.columbia.edu>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, version 2 of the
 *  License.
 *
 *  OS w4118 fall 2016 IPC stats recording functionality.
 */
#include <linux/ipc_rec.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <hw2/binder_utils.h>

DEFINE_MUTEX(ipc_rec_lock);
struct binder_proc_data *binder_trans_head = (struct binder_proc_data *)NULL;

struct binder_peers_wrapper *_init_binder_peers_node(pid_t pid)
{
	struct binder_peers_wrapper *result = (struct binder_peers_wrapper *)
		kmalloc(sizeof(struct binder_proc_data), __GFP_WAIT);
	struct task_struct *task = find_task_by_vpid(pid);

	result->peer.uid = (uid_t)task->cred->gid;
	result->peer.pid = pid;
	strcpy(result->peer.comm, task->comm);
	INIT_LIST_HEAD(&(result->list));

	return result;
}

void _add_transaction(struct binder_proc_data *data_node,
		     int partner, int data_size, int trans_flag)
{
	struct binder_peers_wrapper *data_n;
	int found = 0;

	if (data_node->peers_tail == (struct binder_peers_wrapper *)NULL) {
		data_node->peers_head = _init_binder_peers_node((pid_t)partner);
		data_node->peers_tail = data_node->peers_head;
	}
	list_for_each_entry(data_n, &(data_node->peers_head->list), list) {
		if(data_n->peer.pid == (pid_t)partner)
			found = 1;
	}
	if(!found) {
		data_n = _init_binder_peers_node((pid_t)partner);
		list_add(&(data_n->list), &(data_node->peers_tail->list));
		data_node->peers_tail = data_n;
	}
	if(trans_flag) {
		data_node->stats.nr_trans++;
		data_node->stats.bytes += data_size;
	}
}

void binder_trans_notify(int from_proc, int to_proc, int data_size)
{
	struct list_head *current_n;
	struct binder_proc_data *data_node;
	int flag_proc = 0;

	if (binder_trans_head == (struct binder_proc_data *)NULL)
		return;
	list_for_each(current_n, &(binder_trans_head->list)) {
		data_node = list_entry(current_n, struct binder_proc_data,
				       list);
		if(data_node->state != 1)
			continue;
		if(data_node->pid == (pid_t)from_proc && from_proc == to_proc) {
			_add_transaction(data_node, from_proc, data_size, 1);
			break;
		}
		if(data_node->pid == from_proc) {
			_add_transaction(data_node, to_proc, data_size, 1);
			flag_proc++;
		}
		if(data_node->pid == to_proc) {
			_add_transaction(data_node, from_proc, data_size, 0);
			flag_proc++;
		}
		if (flag_proc == 2)
			break;
	}
}
