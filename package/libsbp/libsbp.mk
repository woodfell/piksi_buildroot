################################################################################
#
# libsbp
#
################################################################################

LIBSBP_VERSION = 2b723bd51e3adb6af40a5a4ebc186a7e2ddd89ce
LIBSBP_SITE = https://github.com/swift-nav/libsbp
LIBSBP_SITE_METHOD = git
LIBSBP_INSTALL_STAGING = YES
LIBSBP_SUBDIR = c

$(eval $(cmake-package))
