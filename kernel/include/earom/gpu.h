#ifndef __EAROM_GPU_STATS_H_
#define __EAROM_GPU_STATS_H_

#include <linux/seq_file.h>

#define GPU_FREQ_MAX 30

struct earom_gpu_stats {
	u32 freq_table[GPU_FREQ_MAX];
	unsigned int freq_num;

	int step;

	u64 wall_per_freq[GPU_FREQ_MAX];
	u64 idle_per_freq[GPU_FREQ_MAX];
	u64 busy_per_freq[GPU_FREQ_MAX];
};

extern void earom_gpu_stats_enable(void);
extern void earom_gpu_stats_disable(void);
extern bool earom_gpu_stats_enabled(void);

extern bool earom_gpu_stats_initialized(void);

extern void proc_print_earom_gpu_stats(struct seq_file *m, void *v);
extern void print_earom_gpu_stats(char *output);

extern void earom_gpu_stats_init(int freq_num);
extern void earom_gpu_stats_update(int index, u32 busy, u32 idle);

#endif
