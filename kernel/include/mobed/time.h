#ifndef __MOBED_TIME_H_
#define __MOBED_TIME_H_

#include <linux/ktime.h>

struct mobed_time {
	bool initialized;

	u64 before;
	u64 now;
};

extern void mobed_init_time(struct mobed_time *time);
extern void mobed_update_time(struct mobed_time *time);
extern u64 mobed_get_elapsed_time_ms(struct mobed_time *time);

extern u64 mobed_get_timestamp_ms(void);
extern u64 mobed_get_timestamp_us(void);

#endif
