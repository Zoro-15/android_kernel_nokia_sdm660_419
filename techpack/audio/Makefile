# auto-detect subdirs
ifeq ($(CONFIG_ARCH_SDXPOORWILLS), y)
include $(srctree)/techpack/audio/config/sdxpoorwillsauto.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/sdxpoorwillsauto.conf)
endif
ifeq ($(CONFIG_ARCH_SM8150), y)
include $(srctree)/techpack/audio/config/sm8150auto.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/sm8150auto.conf)
endif
ifeq ($(CONFIG_ARCH_SDMSHRIKE), y)
include $(srctree)/techpack/audio/config/sm8150auto.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/sm8150auto.conf)
endif
ifeq ($(CONFIG_ARCH_KONA), y)
include $(srctree)/techpack/audio/config/konaauto.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/konaauto.conf)
endif
ifeq ($(CONFIG_ARCH_LITO), y)
include $(srctree)/techpack/audio/config/litoauto.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/litoauto.conf)
endif
ifeq ($(CONFIG_ARCH_BENGAL), y)
include $(srctree)/techpack/audio/config/bengalauto.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/bengalauto.conf)
endif
ifeq ($(CONFIG_ARCH_SDM660), y)
include $(srctree)/techpack/audio/config/sdm660auto.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/sdm660auto.conf)
ifeq ($(CONFIG_LONGCHEER_DAREDEVIL), y)
include $(srctree)/techpack/audio/config/ddv_sld.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/ddv_sld.conf)
endif
ifeq ($(CONFIG_LONGCHEER_STARLORD), y)
include $(srctree)/techpack/audio/config/ddv_sld.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/ddv_sld.conf)
endif
ifeq ($(CONFIG_FIH_ONYX), y)
include $(srctree)/techpack/audio/config/b2n_ctl.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/b2n_ctl.conf)
endif
ifeq ($(CONFIG_FIH_CRYSTAL), y)
include $(srctree)/techpack/audio/config/b2n_ctl.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/b2n_ctl.conf)
endif
ifeq ($(CONFIG_FIH_DRAGON), y)
include $(srctree)/techpack/audio/config/drg_pl2.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/drg_pl2.conf)
endif
ifeq ($(CONFIG_FIH_PLATE2), y)
include $(srctree)/techpack/audio/config/drg_pl2.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/drg_pl2.conf)
endif
ifeq ($(CONFIG_FIH_TAISHAN), y)
include $(srctree)/techpack/audio/config/tas.conf
export $(shell sed 's/=.*//' $(srctree)/techpack/audio/config/tas.conf)
endif
endif

# Use USERINCLUDE when you must reference the UAPI directories only.
USERINCLUDE     += \
                -I$(srctree)/techpack/audio/include/uapi \
                -I$(srctree)/techpack/audio/include

# Use LINUXINCLUDE when you must reference the include/ directory.
# Needed to be compatible with the O= option
LINUXINCLUDE    += \
                -I$(srctree)/techpack/audio/include/uapi \
                -I$(srctree)/techpack/audio/include

ifeq ($(CONFIG_ARCH_SDXPOORWILLS), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/sdxpoorwillsautoconf.h
endif
ifeq ($(CONFIG_ARCH_SM8150), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/sm8150autoconf.h
endif
ifeq ($(CONFIG_ARCH_SDMSHRIKE), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/sm8150autoconf.h
endif
ifeq ($(CONFIG_ARCH_KONA), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/konaautoconf.h
endif
ifeq ($(CONFIG_ARCH_LITO), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/litoautoconf.h
endif
ifeq ($(CONFIG_ARCH_BENGAL), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/bengalautoconf.h
endif
ifeq ($(CONFIG_ARCH_SDM660), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/sdm660autoconf.h
ifeq ($(CONFIG_LONGCHEER_DAREDEVIL), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/ddv_sld.h
endif
ifeq ($(CONFIG_LONGCHEER_STARLORD), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/ddv_sld.h
endif
ifeq ($(CONFIG_FIH_ONYX), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/b2n_ctl.h
endif
ifeq ($(CONFIG_FIH_CRYSTAL), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/b2n_ctl.h
endif
ifeq ($(CONFIG_FIH_DRAGON), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/drg_pl2.h
endif
ifeq ($(CONFIG_FIH_PLATE2), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/drg_pl2.h
endif
ifeq ($(CONFIG_FIH_TAISHAN), y)
LINUXINCLUDE    += \
                -include $(srctree)/techpack/audio/config/tas.h
endif
endif
obj-y += soc/
obj-y += dsp/
obj-y += ipc/
obj-y += asoc/
