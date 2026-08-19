/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Compatibility shim for the legacy Android wakelock API.
 *
 * The old wakelock interface (struct wake_lock / wake_lock() /
 * wake_unlock()) was removed from the mainline kernel.  These helpers
 * map onto the standard wakeup_source API so legacy MediaTek drivers
 * keep working without modification.
 */
#ifndef _LINUX_WAKELOCK_H
#define _LINUX_WAKELOCK_H

#include <linux/jiffies.h>
#include <linux/pm_wakeup.h>

#define WAKE_LOCK_SUSPEND	0
#define WAKE_LOCK_IDLE		1

struct wake_lock {
	struct wakeup_source ws;
};

static inline void wake_lock_init(struct wake_lock *lock, int type,
				  const char *name)
{
	wakeup_source_init(&lock->ws, name);
}

static inline void wake_lock_destroy(struct wake_lock *lock)
{
	wakeup_source_trash(&lock->ws);
}

static inline void wake_lock(struct wake_lock *lock)
{
	__pm_stay_awake(&lock->ws);
}

static inline void wake_lock_timeout(struct wake_lock *lock, long timeout)
{
	__pm_wakeup_event(&lock->ws, jiffies_to_msecs(timeout));
}

static inline void wake_unlock(struct wake_lock *lock)
{
	__pm_relax(&lock->ws);
}

static inline int wake_lock_active(struct wake_lock *lock)
{
	return lock->ws.active;
}

#endif /* _LINUX_WAKELOCK_H */
