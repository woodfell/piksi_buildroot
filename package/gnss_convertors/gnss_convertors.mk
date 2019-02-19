################################################################################
#
# gnss_convertors
#
################################################################################

GNSS_CONVERTORS_VERSION = cccc64d78dad10d0422f9b73d76bade1e3c3496c
GNSS_CONVERTORS_SITE = https://github.com/swift-nav/gnss-converters
GNSS_CONVERTORS_SITE_METHOD = git
GNSS_CONVERTORS_INSTALL_STAGING = YES
GNSS_CONVERTORS_SUBDIR = c
GNSS_CONVERTORS_DEPENDENCIES = libswiftnav librtcm libsbp

$(eval $(cmake-package))
