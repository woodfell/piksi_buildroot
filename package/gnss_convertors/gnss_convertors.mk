################################################################################
#
# gnss_convertors
#
################################################################################

GNSS_CONVERTORS_VERSION = ae99a44e1b89a4fac3b14658bc25bc0161881d38
GNSS_CONVERTORS_SITE = https://github.com/swift-nav/gnss-converters
GNSS_CONVERTORS_SITE_METHOD = git
GNSS_CONVERTORS_INSTALL_STAGING = YES
GNSS_CONVERTORS_SUBDIR = c
GNSS_CONVERTORS_DEPENDENCIES = libswiftnav librtcm libsbp

$(eval $(cmake-package))
