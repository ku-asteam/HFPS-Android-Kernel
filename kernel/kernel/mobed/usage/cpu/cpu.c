/**
 * kernel/mobed/core/chormium/sysfs.c
 * 
 * Copyright (C) Mobile Embedded System Lab.
 *             AT Yonsei Univ., Korea
 *             Park, Seonghoon   <park.s@yonsei.ac.kr>
 *             
 */

#include <linux/cpumask.h>
#include <linux/spinlock.h>
#include <asm/cputime.h>
#include <linux/tick.h>
#include <linux/types.h>

#include <mobed/cpu.h>

static bool state = false;

static u32 freq_cur[NR_CPUS];


void mobed_cpu_enable(void)
{
	state = true;
}

void mobed_cpu_disable(void)
{
	state = false;
}

bool is_mobed_cpu_enabled(void)
{
	return state;
}

void mobed_cpu_init(void)
{
	int cpu=0;

	for(cpu=0; cpu!=NR_CPUS; cpu++){
		freq_cur[cpu] = 0;
	}
}

void mobed_cpu_update(struct cpufreq_policy *policy)
{
	int cpu;
	if(!policy) return;

	for_each_cpu(cpu, policy->related_cpus){
		freq_cur[cpu] = policy->cur;
	}
}

u32 mobed_freq_cur(int cpu)
{
	return freq_cur[cpu];
}
