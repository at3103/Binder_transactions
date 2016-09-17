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

DEFINE_MUTEX(ipc_rec_lock);

void binder_trans_notify(int from_proc, int to_proc, int data_size)
{
	/* Stub implementation */
}
