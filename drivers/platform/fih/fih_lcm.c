#define DEBUG

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
//#include <fih/hwid.h>
#include <linux/uaccess.h>
#include "fih_lcm.h"

//SW4-HL-Display-HDR-Ping-00+{_20180323
//HDR Ping
extern int HDR_enable;
extern int fih_hdr_ping (void);
//SW4-HL-Display-HDR-Ping-00+}_20180323

//SW4-HL-Display-HDR-SetFsCurr-00+{_20180515
extern int fih_get_fs_curr (void);
extern int fih_set_fs_curr (int fs_curr);
//SW4-HL-Display-HDR-SetFsCurr-00+}_20180515

char fih_awer_cnt[32] = "unknown";
void fih_awer_cnt_set(char *info)
{
	strcpy(fih_awer_cnt, info);
}

static int fih_awer_cnt_read_proc(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", fih_awer_cnt);
	return 0;
}

static int fih_awer_cnt_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, fih_awer_cnt_read_proc, NULL);
}

static struct file_operations awer_cnt_operations = {
	.open		= fih_awer_cnt_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

char fih_awer_status[32] = "unknown";
void fih_awer_status_set(char *info)
{
	strcpy(fih_awer_status, info);
}

static int fih_awer_status_read_proc(struct seq_file *m, void *v)
{
	seq_printf(m, "%s\n", fih_awer_status);
	return 0;
}

static int fih_awer_status_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, fih_awer_status_read_proc, NULL);
}

static struct file_operations awer_status_operations = {
	.open		= fih_awer_status_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

//SW4-HL-Display-HDR-Ping-00+{_20180323
//**********************************************
//* HDR Ping
//**********************************************
static int fih_hdr_chip_info_read(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", fih_hdr_ping());

	return 0;
}

static int fih_hdr_chip_info_open(struct inode *inode, struct file *file)
{
	return single_open(file, fih_hdr_chip_info_read, NULL);
};

static struct file_operations hdr_file_ops = {
	.owner   = THIS_MODULE,
	.open    = fih_hdr_chip_info_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};
//SW4-HL-Display-HDR-Ping-00+}_20180323

//SW4-HL-Display-HDR-SetFsCurr-00+{_20180515
static int fih_lcm_read_fs_curr_settings(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", fih_get_fs_curr());

	return 0;
}

static ssize_t fih_lcm_write_fs_curr_settings(struct file *file, const char __user *buffer,
	size_t count, loff_t *ppos)
{
	unsigned char tmp[2];
	unsigned int size;
	unsigned int res;

	size = (count > sizeof(tmp))? sizeof(tmp):count;

	if (copy_from_user(tmp, buffer, size))
	{
		pr_err("%s: copy_from_user fail\n", __func__);
		return -EFAULT;
	}

	//SW4-HL-Display-CE&CTWillNotBeExecutedUntilPanelInitIsDone-01*{_20150428
	if(strstr(saved_command_line, "androidboot.fihmode=0") != NULL ||
	   strstr(saved_command_line, "androidboot.fihmode=3") != NULL)
	{
		res = fih_set_fs_curr(simple_strtoull(tmp, NULL, 0));
		if (res < 0)
		{
			return res;
		}
	}
	//SW4-HL-Display-CE&CTWillNotBeExecutedUntilPanelInitIsDone-01*}_20150428

	return size;
}

static int fih_lcm_fs_curr_settings_open(struct inode *inode, struct file *file)
{
	return single_open(file, fih_lcm_read_fs_curr_settings, NULL);
};

static struct file_operations fs_curr_file_ops = {
	.owner   = THIS_MODULE,
	.open    = fih_lcm_fs_curr_settings_open,
	.read    = seq_read,
	.write	 = fih_lcm_write_fs_curr_settings,
	.llseek  = seq_lseek,
	.release = single_release
};
//SW4-HL-Display-HDR-SetFsCurr-00+}_20180515

struct {
	char *name;
	struct file_operations *ops;
} *p, LCM0_awer_cnt[] = {
	{"AllHWList/LCM0/awer_cnt", &awer_cnt_operations},
	{NULL}, },
	LCM0_awer_status[] = {
	{"AllHWList/LCM0/awer_status", &awer_status_operations},
	{NULL}, },
	LCM0_hdr[] = {								//SW4-HL-Display-HDR-Ping-00+_20180323
	{"AllHWList/LCM0/hdr_ping", &hdr_file_ops},
	{NULL}, },
	LCM0_fs_curr[] = {								//SW4-HL-Display-HDR-SetFsCurr-00+_20180515
	{"AllHWList/LCM0/fs_curr", &fs_curr_file_ops},
	{NULL},
	};

static int __init fih_lcm_init(void)
{
	struct proc_dir_entry *lcm0_dir;
	struct proc_dir_entry *ent;

	pr_debug("\n\n*** [HL] %s, AAA HDR_enable = %d ***\n\n", __func__, HDR_enable);
	lcm0_dir = proc_mkdir("AllHWList/LCM0", NULL);

	ent = proc_create((LCM0_awer_cnt->name) + 15, 0, lcm0_dir, LCM0_awer_cnt->ops);
	if (ent == NULL)
	{
		pr_err("\n\nUnable to create /proc/%s", LCM0_awer_cnt->name);
	}
	pr_debug("\n\n*** [LCM] %s, succeed to create proc/%s ***\n\n", __func__, LCM0_awer_cnt->name);

	ent = proc_create((LCM0_awer_status->name) + 15, 0, lcm0_dir, LCM0_awer_status->ops);
	if (ent == NULL)
	{
		pr_err("\n\nUnable to create /proc/%s", LCM0_awer_status->name);
	}
	pr_err("\n\n*** [LCM] %s, succeed to create proc/%s ***\n\n", __func__, LCM0_awer_status->name);

	//SW4-HL-Display-HDR-Ping-00+{_20180323
	//HDR Ping
	if (HDR_enable)
	{
		ent = proc_create((LCM0_hdr->name) + 15, 0, lcm0_dir, LCM0_hdr->ops);
		if (ent == NULL)
		{
			pr_err("\n\nUnable to create /proc/%s", LCM0_hdr->name);
		}
		pr_debug("\n\n*** [HL] %s, succeed to create proc/%s ***\n\n", __func__, LCM0_hdr->name);

		//SW4-HL-Display-HDR-SetFsCurr-00+{_20180515
		ent = proc_create((LCM0_fs_curr->name) + 15, 0, lcm0_dir, LCM0_fs_curr->ops);
		if (ent == NULL)
		{
			pr_err("\n\nUnable to create /proc/%s", LCM0_fs_curr->name);
		}
		pr_debug("\n\n*** [HL] %s, succeed to create proc/%s ***\n\n", __func__, LCM0_fs_curr->name);
		//SW4-HL-Display-HDR-SetFsCurr-00+}_20180515
	}
	//SW4-HL-Display-HDR-Ping-00+}_20180323

	return (0);
}

static void __exit fih_lcm_exit(void)
{
	remove_proc_entry(LCM0_awer_cnt->name, NULL);
	remove_proc_entry(LCM0_awer_status->name, NULL);

	//SW4-HL-Display-HDR-Ping-00+{_20180323
	//HDR Ping
	if (HDR_enable)
	{
		remove_proc_entry(LCM0_hdr->name, NULL);
		remove_proc_entry(LCM0_fs_curr->name, NULL);	//SW4-HL-Display-HDR-SetFsCurr-00+_20180515
	}
	//SW4-HL-Display-HDR-Ping-00+}_20180323
}

late_initcall(fih_lcm_init);
module_exit(fih_lcm_exit);
