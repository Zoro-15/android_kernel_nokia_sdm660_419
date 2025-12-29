/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2011-2014, 2016, 2018, The Linux Foundation. All rights reserved.
 */

#ifndef _ASM_ARCH_MSM_RESTART_H_
#define _ASM_ARCH_MSM_RESTART_H_

#define RESTART_NORMAL 0x0
#define RESTART_DLOAD  0x1

void msm_set_restart_mode(int mode);
extern int pmic_reset_irq;

#if defined(CONFIG_FIH_SDM630_SDM660_PROJS)
/* FIH, to support fih apr */
unsigned int restart_reason_rd(void);
void restart_reason_wt(unsigned int rere);
/* FIH, to support fih apr */
#endif

#endif

