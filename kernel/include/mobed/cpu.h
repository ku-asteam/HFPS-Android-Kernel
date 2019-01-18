#ifndef __MOBED_CPU__
#define __MOBED_CPU__

#include <linux/cpufreq.h>

extern void mobed_cpu_enable(void);
extern void mobed_cpu_disable(void);
extern bool is_mobed_cpu_enabled(void);

extern void mobed_cpu_init(void);
extern void mobed_cpu_update(struct cpufreq_policy *policy);

extern u32 mobed_freq_cur(int cpu);

#endif
