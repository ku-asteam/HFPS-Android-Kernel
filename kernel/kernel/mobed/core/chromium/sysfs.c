/**
 * kernel/mobed/core/chormium/sysfs.c
 * 
 * Copyright (C) Mobile Embedded System Lab.
 *             AT Yonsei Univ., Korea
 *             Park, Seonghoon   <park.s@yonsei.ac.kr>
 *             
 */

#include <linux/init.h>
#include <linux/module.h>

#include <linux/kallsyms.h>

//register as a driver
#include <linux/fs.h>
#include <linux/cdev.h>

#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/ioctl.h>

#include <linux/seq_file.h>

#include <asm/hardirq.h>

//sysfs
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>

#include "sysfs.h"
#include "procfs.h"

static struct kobject *chromium_kobj;

/**
 * state - control/show the state.
 *
 * show() returns the current state 
 */
static ssize_t 
state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) 
{
	char *msg = buf;
	msg += sprintf(msg, "%s \n", "status");
	
	return msg - buf;
}

static ssize_t 
state_store(struct kobject *kobj, 
		struct kobj_attribute *attr, 
		const char *buf, 
		size_t n)
{
	// TODO: fill something to do
	return 0;
}

mobed_attr(state);

static struct attribute *g[] = {
	&state_attr.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = g,
};

// init function
static int
__init mobed_init(void)
{
	int ret = 0;

	if(!proc_create(PROC_FILE_NAME, 0666, NULL, &chromium_proc_op))
		goto proc_create_error;

	// create sysfs entry
	chromium_kobj = kobject_create_and_add(SYSFS_ENTRY_NAME, NULL);

	if (!chromium_kobj)
		goto kobject_create_error;

	ret =  sysfs_create_group(chromium_kobj, &attr_group);
	if(ret < 0)
		goto sysfs_create_error;

	return 0;

proc_create_error:
	return -1;

kobject_create_error:
	return -1;

sysfs_create_error:
	return -1;
}


static int
__exit mobed_exit(void)
{
	return 0;
}

late_initcall(mobed_init);

