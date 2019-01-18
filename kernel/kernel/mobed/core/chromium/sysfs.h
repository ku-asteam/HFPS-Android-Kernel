#ifndef _CHROMIUM_SYSFS_H_
#define _CHROMIUM_SYSFS_H_

#include <linux/version.h>
#include <linux/slab.h>

//extern struct kobject *mobed_kobj;

// for sysfs
#define SYSFS_ENTRY_NAME	"chromium"
#define mobed_attr(_name)	\
static struct kobj_attribute _name##_attr = {	\
	.attr	= {	\
		.name = __stringify (_name),	\
		.mode = 0644,	\
	},	\
	.show	= _name##_show,	\
	.store	= _name##_store,	\
}

#endif

