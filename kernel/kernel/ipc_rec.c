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
#include <hw2/binder_utils.h>
#include <linux/slab.h>
#include <linux/gfp.h>

DEFINE_MUTEX(ipc_rec_lock);

void add_transaction(struct binder_proc_data *data_node, int partner, int data_size, int trans_flag)
{
	struct binder_peers_wrapper *data_n;
	int found = 0;

	list_for_each_entry(data_n, &(data_node->peers).list, list) {
		if (data_n->peer.pid == partner)
			found = 1;
	}
	if (!found) {
		/*TODO: Get name and uid*/
		data_n = kmalloc(sizeof(struct binder_peers_wrapper), __GFP_WAIT);
		data_n->peer.pid = partner;
		list_add(&(data_n->list), &(data_node->peers).list);
	}

	if (trans_flag == 1) {
		data_node->stats.nr_trans++;
		data_node->stats.bytes += data_size;
	}

	return 0;
}

void binder_trans_notify(int from_proc, int to_proc, int data_size)
{
	struct list_head *current_n;
	struct binder_proc_data *data_node;
	int flag_proc = 0;

	list_for_each(current_n, &binder_trans_list.list) {
		data_node = list_entry(current_n, struct binder_proc_data, list);
		if (data_node->state != 1)
			continue;
		if (data_node->pid == from_proc && from_proc == to_proc && flag_proc == 0) {
			add_transaction(data_node, from_proc, data_size, 1);
			break;
		} else
		if (data_node->pid == from_proc) {
			add_transaction(data_node, from_proc, data_size, 1);
			flag_proc++;
		} else
		if (data_node->pid == to_proc) {
			add_transaction(data_node, to_proc, data_size, 0);
			flag_proc++;
		}
		if (flag_proc == 2)
			break;
	}

	return 0;
}
