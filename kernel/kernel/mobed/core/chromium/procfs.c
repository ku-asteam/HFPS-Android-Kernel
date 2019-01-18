/**
 * kernel/mobed/core/chormium/procfs.c
 * 
 * Copyright (C) Mobile Embedded System Lab.
 *             AT Yonsei Univ., Korea
 *             Choi, Yonghun     <y.choi@yonsei.ac.kr>
 *             Park, Seonghoon   <park.s@yonsei.ac.kr>
 */

#include <linux/kernel.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
//#include <linux/seq_file.h>
//#include <linux/fs.h>
//#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>

#include <mobed/time.h>
#include <earom/common.h>
#include <earom/cpu.h>
#include <earom/gpu.h>

#define BUF_MAX 20000
#define TMP_BUF_MAX 2000

static char global_buffer[BUF_MAX];
static char temp_buffer[TMP_BUF_MAX];
static char temp_cpu_buffer[TMP_BUF_MAX];
static char temp_gpu_buffer[TMP_BUF_MAX];
static char temp_common_buffer[TMP_BUF_MAX];

static int chromium_frame_count = 0;

static int m_show(struct seq_file *m, void *v);
static int proc_entry_open(struct inode *inode, struct file *file);
static ssize_t chromium_write(struct file *_file, const char __user* user_buf, size_t count, loff_t *data);

struct file_operations chromium_proc_op = {
	.open		= proc_entry_open,
	.read		= seq_read,
	.write		= chromium_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};


static ssize_t chromium_write(struct file *_file, const char __user* user_buf, size_t count, loff_t *data)
{
	int ret = 0;
	//int cur_size = strlen(global_buffer);

	memset(temp_buffer, 0, TMP_BUF_MAX);
	memset(temp_cpu_buffer, 0, TMP_BUF_MAX);
	memset(temp_gpu_buffer, 0, TMP_BUF_MAX);
	memset(temp_common_buffer, 0, TMP_BUF_MAX);

	ret = copy_from_user(temp_buffer, user_buf, count);
	if (ret < 0) return 0;

	if (strlen(global_buffer) + strlen(temp_buffer) >= BUF_MAX) return 0;

	if (strcmp(temp_buffer, "*\n") == 0)
	{
		chromium_frame_count++;
		
	}
	else if (strcmp(temp_buffer, "start\n") == 0)
	{
		chromium_frame_count = 0;

		print_earom_common(temp_common_buffer);
		if (!earom_cpu_stats_initialized()){
		    earom_cpu_stats_enable();
		} else {
		    print_earom_cpu_stats(temp_cpu_buffer);
		}
		if (!earom_gpu_stats_initialized()){
		    earom_gpu_stats_enable();
		} else {
		    print_earom_gpu_stats(temp_gpu_buffer);
		}

		sprintf(global_buffer + strlen(global_buffer), "<Start>\n");
		sprintf(global_buffer + strlen(global_buffer), "%s", temp_common_buffer);
		sprintf(global_buffer + strlen(global_buffer), "%s", temp_cpu_buffer);
		sprintf(global_buffer + strlen(global_buffer), "%s", temp_gpu_buffer);
		sprintf(global_buffer + strlen(global_buffer), "</Start>\n");
	}
	else
	{
		sprintf(global_buffer + strlen(global_buffer), "%s", temp_buffer);
		sprintf(global_buffer + strlen(global_buffer), "frameCount %d\n", chromium_frame_count);

		print_earom_common(temp_common_buffer);
		if (!earom_cpu_stats_initialized()){
		    earom_cpu_stats_enable();
		} else {
		    print_earom_cpu_stats(temp_cpu_buffer);
		}
		if (!earom_gpu_stats_initialized()){
		    earom_gpu_stats_enable();
		} else {
		    print_earom_gpu_stats(temp_gpu_buffer);
		}

		sprintf(global_buffer + strlen(global_buffer), "<End>\n");
		sprintf(global_buffer + strlen(global_buffer), "%s", temp_common_buffer);
		sprintf(global_buffer + strlen(global_buffer), "%s", temp_cpu_buffer);
		sprintf(global_buffer + strlen(global_buffer), "%s", temp_gpu_buffer);	
		sprintf(global_buffer + strlen(global_buffer), "</End>\n\n");
	}
	return count;
}

static int m_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%s", global_buffer);
	chromium_frame_count = 0;

	memset(global_buffer, 0, BUF_MAX);

	return 0;
}

static int proc_entry_open(struct inode *inode, struct file *file)
{
	unsigned size = 256 * 1024;
	char *buf;
	struct seq_file *m;
	int res;

	buf = kmalloc(size, GFP_KERNEL);
	if (!buf) return -ENOMEM;

	res = single_open(file, m_show, NULL);

	if (!res) 
	{
		m = file->private_data;
		m->buf = buf;
		m->size = size;
	}
	else kfree(buf);

	return res;
}

