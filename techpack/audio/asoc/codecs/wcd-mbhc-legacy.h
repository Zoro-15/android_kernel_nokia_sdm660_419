/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (c) 2014-2018, The Linux Foundation. All rights reserved.
 */
#ifndef __WCD_MBHC_LEGACY_H__
#define __WCD_MBHC_LEGACY_H__

#include <asoc/wcdcal-hwdep.h>
#include <asoc/wcd-mbhc-v2.h>

#if IS_ENABLED(CONFIG_SND_SOC_WCD_MBHC_LEGACY)
void wcd_mbhc_legacy_init(struct wcd_mbhc *mbhc);
#if defined(CONFIG_FIH_SDM630_SDM660_PROJS) || defined (CONFIG_LONGCHEER_SDM660_PROJS)
void wcd_enable_mbhc_supply(struct wcd_mbhc *mbhc,
			enum wcd_mbhc_plug_type plug_type);
#endif
#else
static inline void wcd_mbhc_legacy_init(struct wcd_mbhc *mbhc)
{
}
#endif

#endif /* __WCD_MBHC_LEGACY_H__ */
