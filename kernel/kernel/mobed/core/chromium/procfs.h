#ifndef _CHROMIUM_PROCFS_H
#define _CHROMIUM_PROCFS_H

#define PROC_FILE_NAME	"chromium"

extern struct file_operations chromium_proc_op;

int m_show(struct seq_file *m, void *v);
int mobed_open(struct inode *inode, struct file *file);

#endif /* _MOBED_PROCFS_H */
