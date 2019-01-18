#ifndef __EAROM_COMMON_STATS__
#define __EAROM_COMMON_STATS__

#include <linux/seq_file.h>

extern void earom_proc_print_common(struct seq_file *m, void *v);
extern void print_earom_common(char *output);

#endif
