################################################################################
#
# libswiftnav
#
################################################################################

LIBSWIFTNAV_VERSION = 7919a98ef601d52a738bb2ff589fa2911c824b61
LIBSWIFTNAV_SITE = git@github.com:woodfell/libswiftnav.git
LIBSWIFTNAV_SITE_METHOD = git
LIBSWIFTNAV_INSTALL_STAGING = YES

$(eval $(cmake-package))
