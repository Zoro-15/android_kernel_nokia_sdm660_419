/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2017-2019 The Linux Foundation. All rights reserved.
 */

#ifndef __STEP_CHG_H__
#define __STEP_CHG_H__

#if defined(CONFIG_FIH_SDM630_SDM660_PROJS)
// FIHTDC, IdaChiang, add for LCM status }}
#define MAX_STEP_CHG_ENTRIES 24  //8*3 =24 
#else
#define MAX_STEP_CHG_ENTRIES	8
#endif

#if defined(CONFIG_FIH_SDM630_SDM660_PROJS)
enum step_chg_cfg_idx {
	STEP_CHG_CFG = 0,
	JEITA_FCC_CFG,
	JEITA_FV_CFG,
};
#endif

struct step_chg_jeita_param {
	u32			psy_prop;
	char			*prop_name;
	int			hysteresis;
	bool			use_bms;
};

struct range_data {
	int low_threshold;
	int high_threshold;
	u32 value;
};

int qcom_step_chg_init(struct device *dev,
		bool step_chg_enable, bool sw_jeita_enable, bool jeita_arb_en);
void qcom_step_chg_deinit(void);
int read_range_data_from_node(struct device_node *node,
		const char *prop_str, struct range_data *ranges,
		int max_threshold, u32 max_value);

#if defined(CONFIG_FIH_SDM630_SDM660_PROJS)
int fih_set_step_chg_cfg(int *cfg, int cfg_len, int mode);
int fih_set_step_chg_hysteresis(int hysteresis, int mode);
#endif

#endif /* __STEP_CHG_H__ */