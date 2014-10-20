/*

    heartbeat.c

    dmesg heartbeat module

    (c) 2014 Michael Bosse <metiscus@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/time.h>

static struct delayed_work dwork;
static struct workqueue_struct *queue;

MODULE_DESCRIPTION("code to print a heartbeat message to dmesg periodically");
MODULE_AUTHOR("Michael Bosse");
MODULE_LICENSE("GPL");

static unsigned int frequency = 30;
module_param(frequency, int, 0);
MODULE_PARM_DESC(frequency, "int: frequency of heartbeat print in dmesg");

static void heartbeat_work_function(struct work_struct *work)
{
	struct timeval tv;
	struct tm date;
	static const char const *day[]   = { "Sun", "Mon", "Tue", "Wed",
		"Thu", "Fri", "Sat" };
	static const char const *month[] = { "Jan", "Feb", "Mar", "Apr",
		"May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	do_gettimeofday(&tv);
	time_to_tm(tv.tv_sec, 0, &date);

	printk(KERN_INFO "The current date and time is %s %s %d %d:%d:%d %ld GMT\n",
		day[date.tm_wday], month[date.tm_mon], date.tm_mday,
		date.tm_hour, date.tm_min, date.tm_sec, date.tm_year + 1900);

	queue_delayed_work(queue, &dwork, HZ*60*frequency);
}

static int __init heartbeat_module_init(void)
{
	queue = create_singlethread_workqueue("heartbeat");

	INIT_DELAYED_WORK(&dwork, heartbeat_work_function);

	if (!queue_delayed_work(queue, &dwork, HZ * 60 * frequency))
		return -EBUSY;

	return 0;
}

static void __exit heartbeat_module_cleanup(void)
{
	if (!cancel_delayed_work(&dwork))
		flush_workqueue(queue);
	destroy_workqueue(queue);
}

module_init(heartbeat_module_init);
module_exit(heartbeat_module_cleanup);
