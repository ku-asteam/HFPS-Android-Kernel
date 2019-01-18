#include <linux/jiffies.h>
#include <mobed/time.h>

void mobed_init_time(struct mobed_time *time)
{
	if(time == NULL) return;

	time->initialized = true;

	time->before = 0;
	time->now = get_jiffies_64();
}


void mobed_update_time(struct mobed_time *time)
{
	if(time == NULL) return;
	time->before = time->now;
	time->now = get_jiffies_64();
}


u64 mobed_get_elapsed_time_ms(struct mobed_time *time)
{
	if(time == NULL) return -1;
	if(time->now < time->before) return -1;

	return (time->now - time->before) * 1000 / HZ;
}

u64 mobed_get_timestamp_ms(void)
{
	struct timespec ts;
	u64 msecs = 0;

	getnstimeofday(&ts);

	msecs = (unsigned long long)ts.tv_sec * 1000000 + ts.tv_nsec / 1000000;

	return msecs;
}

u64 mobed_get_timestamp_us(void)
{
	struct timespec ts;
	u64 msecs = 0;

	getnstimeofday(&ts);

	msecs = (unsigned long long)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;

	return msecs;
}
