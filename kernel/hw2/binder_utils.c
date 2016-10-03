#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <hw2/binder_utils.h>

/* Initialize a new binder_proc_data. */
struct binder_proc_data *_init_binder_trans_node(pid_t pid, int state)
{
	struct binder_proc_data *result;
	struct task_struct *task;

	rcu_read_lock();
	task = find_task_by_vpid(pid);
	rcu_read_unlock();

	if (task == (struct task_struct *)NULL)
		return (struct binder_proc_data *)NULL;
	result = (struct binder_proc_data *)
		kmalloc(sizeof(struct binder_proc_data), GFP_KERNEL);
	result->peers_head = (struct binder_peers_wrapper *)NULL;
	strcpy(result->stats.comm, task->comm);
	result->stats.nr_trans = 0;
	result->stats.bytes = 0;
	result->pid = pid;
	result->state = state;
	INIT_LIST_HEAD(&(result->list));

	return result;
}

/* Completely release memory of a binder_proc_data. */
void _free_node(struct binder_proc_data *node)
{
	struct binder_peers_wrapper *helperval;
	struct list_head *current_n = &(node->peers_head->list), *helper;

	if (node->peers_head != (struct binder_peers_wrapper *)NULL)
		while (!list_empty(current_n)) {
			helper = current_n->next;
			helperval = list_entry(current_n,
					       struct binder_peers_wrapper,
					       list);
			list_del(current_n);
			kfree(helperval);
			current_n = helper;
		}
	kfree(node);
}

SYSCALL_DEFINE2(binder_rec, pid_t, pid, int, state)
{
	struct list_head *current_n, *found = (struct list_head *)NULL;
	struct binder_proc_data *data_node;

	spin_lock_irq(&my_binder_spin_lock);

	/* Init binder_trans_head if needed. */
	if (binder_trans_head == (struct binder_proc_data *)NULL) {
		if (state == 1) {
			binder_trans_head =
			_init_binder_trans_node(pid, state);
			if (binder_trans_head ==
			    (struct binder_proc_data *)NULL) {
				spin_unlock_irq(&my_binder_spin_lock);
				return -ESRCH;
			}
		} else {
			spin_unlock_irq(&my_binder_spin_lock);
			return 0;
		}
	}
	list_for_each(current_n, &binder_trans_head->list) {
		data_node = list_entry(current_n,
			struct binder_proc_data, list);
		if (data_node->pid == pid) {
			found = current_n;
			break;
		}
	}
	/* New node add/existing node delete. */
	if (found == (struct list_head *)NULL) {
		if (state == 1) {
			data_node = _init_binder_trans_node(pid, state);
			if (data_node == (struct binder_proc_data *)NULL) {
				spin_unlock_irq(&my_binder_spin_lock);
				return -ESRCH;
			}
			list_add_tail(&(data_node->list),
				&(binder_trans_head->list));
		} else {
			spin_unlock_irq(&my_binder_spin_lock);
			return 0;
		}
	} else if (state == 0) {
		list_del(&(data_node->list));
		_free_node(data_node);
	}
	spin_unlock_irq(&my_binder_spin_lock);

	return 0;
}

SYSCALL_DEFINE4(binder_stats, pid_t, pid, struct binder_stats *, stats,
		void *, buf, size_t *, size)
{
	struct list_head *current_n, *found = (struct list_head *)NULL;
	struct binder_proc_data *data_node;
	struct binder_peers_wrapper *peers_node;
	void *curbuf = (void *)NULL;
	size_t *sz = NULL;
	long result = 0L;
	int cpy_res;
	long check1;
	long check2;

	sz = kmalloc(sizeof(size), GFP_KERNEL);

	check1 = copy_from_user(sz, size, sizeof(size));

	check2 = access_ok(VERIFY_READ, buf, (unsigned long) *sz);

	if (check2)
		curbuf = buf;

	if (buf == NULL) {
		kfree(sz);
		return -EINVAL;
	}

	if (*size < 1024 || *size < sizeof(struct binder_peer)) {
		kfree(sz);
		return -EINVAL;
	}

	if (check1 || !check2) {
		kfree(sz);
		return -EFAULT;
	}

	/* Find target node and sanity checks. */
	if (binder_trans_head == (struct binder_proc_data *)NULL) {
		kfree(sz);
		return -ENODATA;
	}

	spin_lock_irq(&my_binder_spin_lock);
	list_for_each(current_n, &(binder_trans_head->list)) {
		data_node = list_entry(current_n,
			struct binder_proc_data, list);
		if (data_node->pid == pid) {
			found = current_n;
			break;
		}
	}
	if (found == (struct list_head *)NULL) {
		spin_unlock_irq(&my_binder_spin_lock);
		kfree(sz);
		return -ENODATA;
	}
	cpy_res = copy_to_user(stats, &(data_node->stats),
		sizeof(struct binder_stats));
	if (*size < sizeof(struct binder_peer) || cpy_res) {
		spin_unlock_irq(&my_binder_spin_lock);
		kfree(sz);
		return -ENOMEM;
	}

	/* Copy peer transactions. */
	if (data_node->peers_head == (struct binder_peers_wrapper *)NULL) {
		spin_unlock_irq(&my_binder_spin_lock);
		kfree(sz);
		return 0;
	}
	list_for_each_entry(peers_node, &(data_node->peers_head->list), list) {

		if (*size >= curbuf - buf + sizeof(struct binder_peer)) {
			cpy_res = copy_to_user(curbuf, &(peers_node->peer),
				sizeof(struct binder_peer));
			curbuf += sizeof(struct binder_peer);
		}
		result++;
		if (cpy_res) {
			kfree(sz);
			return -EFAULT;
		}
	}
	if (cpy_res) {
		spin_unlock_irq(&my_binder_spin_lock);
		kfree(sz);
		return -1;
	}
	*size = curbuf - buf;
	spin_unlock_irq(&my_binder_spin_lock);
	kfree(sz);
	return result;
}
