#ifndef __EAROM_CPU_STATS_H_
#define __EAROM_CPU_STATS_H_

#include <linux/cpufreq.h>
#include <linux/seq_file.h>

#define CPU_FREQ_MAX 30

struct earom_cpufreq_policy {
	cpumask_var_t cpus;
	cpumask_var_t		related_cpus; /* Online + Offline CPUs */
	
	unsigned int		cpu;    /* cpu nr of CPU managing this policy */
	unsigned int		cur;	/* in kHz, only needed if cpufreq
								   govornors are used */
};

struct earom_cpu_stats {
	unsigned int cpu;
	unsigned int freq_num;
	int step;
	u32 freq_table[CPU_FREQ_MAX];

	cputime64_t cpu_idle;
	cputime64_t cpu_busy;
	cputime64_t cpu_wall;

	cputime64_t wall_per_freq[CPU_FREQ_MAX];
	cputime64_t busy_per_freq[CPU_FREQ_MAX];
	cputime64_t idle_per_freq[CPU_FREQ_MAX];
};

extern void earom_cpu_stats_enable(void);
extern void earom_cpu_stats_disable(void);
extern bool earom_cpu_stats_enabled(void);
extern bool earom_cpu_stats_initialized(void);

extern void proc_print_earom_cpu_stats(struct seq_file *m, void *v);
extern void print_earom_cpu_stats(char *output);

extern int earom_cpufreq_transition_handler(struct cpufreq_policy *policy);

extern int calc_earom_cpu_stats_each(int cpu);
extern int calc_earom_cpu_stats_each_no_update(int cpu);

extern void earom_cpu_stats_init(void);

extern int get_earom_cpu_frequency_num(int cpu);
extern int get_earom_cpu_step(int cpu);

#endif
