#ifndef __EAROM_TIME_H_
#define __EAROM_TIME_H_

#include <linux/ktime.h>
#include <linux/time.h>
#include <linux/jiffies.h>

struct earom_time {
	bool initialized;

	u64 before;
	u64 now;
};

extern void earom_init_time(struct earom_time *time);
extern void earom_update_time(struct earom_time *time);
extern u64 earom_get_elapsed_time_ms(struct earom_time *time);

extern u64 earom_get_monotonic_timestamp_ms(void);
extern u64 earom_get_monotonic_timestamp_us(void);
extern u64 earom_get_monotonic_timestamp_ns(void);

#endif
