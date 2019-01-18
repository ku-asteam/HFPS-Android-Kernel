/**
 * kernel/mobed/core/chormium/sysfs.c
 * 
 * Copyright (C) Mobile Embedded System Lab.
 *             AT Yonsei Univ., Korea
 *             Park, Seonghoon   <park.s@yonsei.ac.kr>
 *             
 */

#include <linux/spinlock.h>
#include <linux/tick.h>
#include <linux/types.h>

#include <mobed/graphics.h>
#include <mobed/cpu.h>
#include <mobed/time.h>

static bool gfx_state = false;

/* ns * cpu_frequency(MHz) */
static u64 surfaceflinger[NR_CPUS];
static u64 mdss_fb0[NR_CPUS];
static u64 renderthead[NR_CPUS];

/* timestamp in us */
static u64 surfaceflinger_prev = 0;
static u64 mdss_fb0_prev = 0;
static u64 renderthead_prev = 0;

static spinlock_t gfx_update_lock[NR_CPUS];

void mobed_gfx_enable(void)
{
	gfx_state = true;
}

void mobed_gfx_disable(void)
{
	gfx_state = false;
}

bool is_mobed_gfx_enabled(void)
{
	return gfx_state;
}

void mobed_gfx_init(void)
{
	int cpu = 0;
	for(cpu=0; cpu!=NR_CPUS; cpu++){
		surfaceflinger[cpu] = mdss_fb0[cpu] = renderthead[cpu] = 0;
	}

	surfaceflinger_prev = mdss_fb0_prev = renderthead_prev = 0;

	for(cpu=0; cpu!=NR_CPUS; cpu++){
		spin_lock_init(&gfx_update_lock[cpu]);
	}
}

void mobed_surfaceflinger_update(int cpu, enum gfx_state state)
{
	u64 now = mobed_get_timestamp_us();
	u64 tmp = 0;

	switch(state){
		case GFX_IN:
			surfaceflinger_prev = now;
			break;

		case GFX_OUT:
			if(!surfaceflinger_prev) return;

			tmp = (now - surfaceflinger_prev) * mobed_freq_cur(cpu);
			tmp /= 1000;

			spin_lock(&gfx_update_lock[cpu]);
			surfaceflinger[cpu] += tmp;
			spin_unlock(&gfx_update_lock[cpu]);

			surfaceflinger_prev = 0;
			break;
	}
}

void mobed_mdss_fb0_update(int cpu, enum gfx_state state)
{
	u64 now = mobed_get_timestamp_us();
	u64 tmp = 0;

	switch(state){
		case GFX_IN:
			mdss_fb0_prev = now;
			break;

		case GFX_OUT:
			if(!mdss_fb0_prev) return;

			tmp = (now - mdss_fb0_prev) * mobed_freq_cur(cpu);
			tmp /= 1000;

			spin_lock(&gfx_update_lock[cpu]);
			mdss_fb0[cpu] += tmp;
			spin_unlock(&gfx_update_lock[cpu]);

			mdss_fb0_prev = 0;
			break;
	}
}

void mobed_renderthread_update(int cpu, enum gfx_state state)
{
	u64 now = mobed_get_timestamp_us();
	u64 tmp = 0;

	switch(state){
		case GFX_IN:
			renderthead_prev = now;
			break;

		case GFX_OUT:
			if(!renderthead_prev) return;

			tmp = (now - renderthead_prev) * mobed_freq_cur(cpu);
			tmp /= 1000;

			spin_lock(&gfx_update_lock[cpu]);
			renderthead[cpu] += tmp;
			spin_unlock(&gfx_update_lock[cpu]);

			renderthead_prev = 0;
			break;
	}
}

void mobed_graphics_proc_print(struct seq_file *m, void *v)
{
	int cpu;

	seq_printf(m, "timestamp %llu\n", mobed_get_timestamp_us());

	for(cpu=0; cpu!=NR_CPUS; cpu++){
		spin_lock(&gfx_update_lock[cpu]);

		seq_printf(m, "%s %d %llu\n", SURFACEFLINGER_COMM, cpu, surfaceflinger[cpu]);
		seq_printf(m, "%s %d %llu\n", MDSS_FB0_COMM, cpu, mdss_fb0[cpu]);
		seq_printf(m, "%s %d %llu\n", RENDERTHREAD_COMM, cpu, renderthead[cpu]);

		surfaceflinger[cpu] = mdss_fb0[cpu] = renderthead[cpu] = 0;

		spin_unlock(&gfx_update_lock[cpu]);
	}
}
