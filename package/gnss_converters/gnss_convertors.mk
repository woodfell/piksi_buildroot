################################################################################
#
# gnss_converters
#
################################################################################

GNSS_CONVERTERS_VERSION = 36c5784af7e37e88bda622cdd7b2ee1302a2ef10
GNSS_CONVERTERS_SITE = https://github.com/swift-nav/gnss-converters
GNSS_CONVERTERS_SITE_METHOD = git
GNSS_CONVERTERS_INSTALL_STAGING = YES
GNSS_CONVERTERS_SUBDIR = c
GNSS_CONVERTERS_DEPENDENCIES = libswiftnav librtcm libsbp

$(eval $(cmake-package))
