# Install script for directory: /Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/build/lib/libns3.45-wifi-optimized.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.45-wifi-optimized.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.45-wifi-optimized.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/build/lib"
      -add_rpath "/usr/local/lib:$ORIGIN/:$ORIGIN/../lib:/usr/local/lib64:$ORIGIN/:$ORIGIN/../lib64"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.45-wifi-optimized.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.45-wifi-optimized.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/helper/athstats-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/helper/spectrum-wifi-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/helper/wifi-co-trace-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/helper/wifi-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/helper/wifi-mac-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/helper/wifi-radio-energy-model-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/helper/yans-wifi-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/helper/wifi-phy-rx-trace-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/helper/wifi-tx-stats-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/addba-extension.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/adhoc-wifi-mac.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ampdu-subframe-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ampdu-tag.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/amsdu-subframe-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ap-wifi-mac.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/block-ack-agreement.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/block-ack-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/block-ack-type.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/block-ack-window.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/capability-information.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/channel-access-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ctrl-headers.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/edca-parameter-set.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/advanced-ap-emlsr-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/advanced-emlsr-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/ap-emlsr-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/common-info-basic-mle.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/common-info-probe-req-mle.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/default-ap-emlsr-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/default-emlsr-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/eht-capabilities.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/eht-configuration.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/eht-frame-exchange-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/eht-operation.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/tid-to-link-mapping-element.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/eht-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/eht-ppdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/eht-ru.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/emlsr-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/eht/multi-link-element.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/error-rate-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/extended-capabilities.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/fcfs-wifi-queue-scheduler.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/frame-capture-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/frame-exchange-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/gcr-group-address.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/gcr-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/constant-obss-pd-algorithm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/he-6ghz-band-capabilities.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/he-capabilities.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/he-configuration.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/he-frame-exchange-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/he-operation.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/he-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/he-ppdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/he-ru.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/mu-edca-parameter-set.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/mu-snr-tag.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/multi-user-scheduler.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/obss-pd-algorithm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/he/rr-multi-user-scheduler.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ht/ht-capabilities.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ht/ht-configuration.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ht/ht-frame-exchange-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ht/ht-operation.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ht/ht-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ht/ht-ppdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/interference-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/mac-rx-middle.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/mac-tx-middle.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/mgt-action-headers.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/mgt-headers.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/mpdu-aggregator.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/msdu-aggregator.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/nist-error-rate-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-ht/dsss-error-rate-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-ht/dsss-parameter-set.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-ht/dsss-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-ht/dsss-ppdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-ht/erp-information.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-ht/erp-ofdm-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-ht/erp-ofdm-ppdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-ht/ofdm-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-ht/ofdm-ppdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/non-inheritance.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/originator-block-ack-agreement.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/phy-entity.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/preamble-detection-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/qos-frame-exchange-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/qos-txop.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/qos-utils.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/aarf-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/aarfcd-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/amrr-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/aparf-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/arf-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/cara-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/constant-rate-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/ideal-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/minstrel-ht-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/minstrel-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/onoe-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/parf-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/rraa-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/rrpaa-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/rate-control/thompson-sampling-wifi-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/recipient-block-ack-agreement.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/reduced-neighbor-report.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/reference/error-rate-tables.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/simple-frame-capture-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/snr-tag.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/spectrum-wifi-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/ssid.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/sta-wifi-mac.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/status-code.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/supported-rates.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/table-based-error-rate-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/threshold-preamble-detection-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/tim.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/txop.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/vht/vht-capabilities.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/vht/vht-configuration.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/vht/vht-frame-exchange-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/vht/vht-operation.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/vht/vht-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/vht/vht-ppdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-ack-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-acknowledgment.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-assoc-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-bandwidth-filter.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-constants.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-default-ack-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-default-assoc-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-default-gcr-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-default-protection-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-information-element.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mac-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mac-queue-container.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mac-queue-elem.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mac-queue-scheduler-impl.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mac-queue-scheduler.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mac-queue.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mac-trailer.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mac.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mgt-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mode.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-mpdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-net-device.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-opt-field.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-phy-band.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-phy-common.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-phy-listener.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-phy-operating-channel.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-phy-state-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-phy-state.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-ppdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-protection-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-protection.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-psdu.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-radio-energy-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-remote-station-info.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-remote-station-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-ru.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-spectrum-phy-interface.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-spectrum-signal-parameters.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-standards.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-tx-current-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-tx-parameters.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-tx-timer.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-tx-vector.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-types.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-units.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-utils.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/yans-error-rate-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/yans-wifi-channel.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/yans-wifi-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/model/wifi-spectrum-value-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src/wifi/test/wifi-mlo-test.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/build/include/ns3/wifi-module.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-cache/src/wifi/examples/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-cache/src/wifi/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
