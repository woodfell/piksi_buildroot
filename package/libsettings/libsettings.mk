################################################################################
#
# libsettings
#
################################################################################

LIBSETTINGS_VERSION = 1fdccef302c1cf63c740907bc22460af6c87dabc
LIBSETTINGS_SITE = git@github.com:woodfell/libsettings.git
LIBSETTINGS_SITE_METHOD = git
LIBSETTINGS_INSTALL_STAGING = YES
LIBSETTINGS_DEPENDENCIES = libsbp

define LIBSETTINGS_BUILD_CMDS
    $(MAKE) -C $(@D) settings
endef

$(eval $(cmake-package))
