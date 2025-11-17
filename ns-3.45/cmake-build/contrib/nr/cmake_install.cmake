# Install script for directory: /Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr

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
    set(CMAKE_INSTALL_CONFIG_NAME "default")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/build/lib/libns3.45-nr-default.dylib")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.45-nr-default.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.45-nr-default.dylib")
    execute_process(COMMAND /usr/bin/install_name_tool
      -delete_rpath "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/build/lib"
      -add_rpath "/usr/local/lib:$ORIGIN/:$ORIGIN/../lib:/usr/local/lib64:$ORIGIN/:$ORIGIN/../lib64"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.45-nr-default.dylib")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" -x "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.45-nr-default.dylib")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/beamforming-helper-base.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/cc-bwp-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/file-scenario-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/grid-scenario-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/hexagonal-grid-scenario-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/ideal-beamforming-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/node-distribution-scenario-interface.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-bearer-stats-calculator.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-bearer-stats-connector.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-bearer-stats-simple.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-channel-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-epc-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-mac-rx-trace.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-mac-scheduling-stats.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-no-backhaul-epc-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-phy-rx-trace.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-point-to-point-epc-helper-base.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-point-to-point-epc-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-radio-environment-map-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-spectrum-value-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/nr-stats-calculator.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/realistic-beamforming-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/scenario-parameters.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/helper/three-gpp-ftp-m1-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/bandwidth-part-gnb.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/bandwidth-part-ue.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/beam-id.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/beam-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/beamforming-vector.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/bwp-manager-algorithm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/bwp-manager-gnb.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/bwp-manager-ue.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/ideal-beamforming-algorithm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/lena-error-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-a2-a4-rsrq-handover-algorithm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-a3-rsrp-handover-algorithm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-amc.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-anr-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-anr.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-as-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-asn1-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-cb-two-port.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-cb-type-one-sp.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-cb-type-one.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ccm-mac-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ccm-rrc-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ch-access-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-chunk-processor.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-common.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-component-carrier.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-control-messages.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-csi-rs-filter.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eesm-cc-t1.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eesm-cc-t2.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eesm-cc.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eesm-error-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eesm-ir-t1.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eesm-ir-t2.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eesm-ir.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eesm-t1.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eesm-t2.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-gnb-application.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-gnb-s1-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-gtpc-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-gtpu-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-mme-application.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-pgw-application.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-s11-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-s1ap-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-sgw-application.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-tft-classifier.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-tft.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-ue-nas.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-x2-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-x2-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-epc-x2.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eps-bearer-tag.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-eps-bearer.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-error-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-fh-control.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-fh-phy-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-fh-sched-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-gnb-cmac-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-gnb-component-carrier-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-gnb-cphy-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-gnb-mac.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-gnb-net-device.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-gnb-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-gnb-rrc.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-handover-algorithm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-handover-management-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-harq-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-initial-association.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-interference-base.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-interference.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-lte-amc.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-lte-mi-error-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-csched-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-harq-process.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-harq-vector.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-header-fs-dl.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-header-fs-ul.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-header-fs.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-header-vs-dl.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-header-vs-ul.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-header-vs.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-pdu-info.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-sched-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-cqi-management.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-harq-rr.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-lc-alg.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-lc-qos.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-lc-rr.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-lcg.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ns3.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ofdma-mr.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ofdma-pf.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ofdma-qos.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ofdma-rr.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ofdma-symbol-per-beam.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ofdma-random.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ofdma.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-srs-default.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-srs.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-tdma-mr.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-tdma-pf.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-tdma-qos.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-tdma-rr.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-tdma-random.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-tdma.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ue-info-mr.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ue-info-pf.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ue-info-qos.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ue-info-rr.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler-ue-info.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-scheduler.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mac-short-bsr-ce.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mimo-chunk-processor.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mimo-matrices.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-mimo-signal.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-net-device.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-no-op-component-carrier-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-no-op-handover-algorithm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-pdcp-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-pdcp-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-pdcp-tag.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-pdcp.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-phy-mac-common.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-phy-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-phy-tag.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-pm-search-fast.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-pm-search-full.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-pm-search-ideal.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-pm-search-sasaoka.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-pm-search.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-radio-bearer-info.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-radio-bearer-tag.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc-am-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc-am.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc-sdu-status-tag.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc-sequence-number.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc-tag.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc-tm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc-um.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rlc.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rrc-header.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rrc-protocol-ideal.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rrc-protocol-real.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-rrc-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-simple-ue-component-carrier-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-spectrum-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-spectrum-signal-parameters.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ue-ccm-rrc-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ue-cmac-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ue-component-carrier-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ue-cphy-sap.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ue-mac.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ue-net-device.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ue-phy.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ue-power-control.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-ue-rrc.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/nr-vendor-specific-parameters.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/realistic-beamforming-algorithm.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/realistic-bf-manager.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/resource-assignment-matrix.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/model/sfnsf.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/channels/nyu/nyu-channel-condition-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/channels/nyu/nyu-channel-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/channels/nyu/nyu-propagation-loss-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/channels/nyu/nyu-spectrum-propagation-loss-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/distance-based-three-gpp-spectrum-propagation-loss-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/fast-fading-constant-position-mobility-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/nr-json.hpp"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/parse-string-to-vector.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/helper/traffic-generator-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/helper/xr-traffic-mixer-helper.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/model/traffic-generator-3gpp-audio-data.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/model/traffic-generator-3gpp-generic-video.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/model/traffic-generator-3gpp-pose-control.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/model/traffic-generator-ftp-single.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/model/traffic-generator-ngmn-ftp-multi.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/model/traffic-generator-ngmn-gaming.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/model/traffic-generator-ngmn-video.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/model/traffic-generator-ngmn-voip.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/traffic-generators/model/traffic-generator.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/wraparound-model/hexagonal-wraparound-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/contrib/nr/utils/wraparound-three-gpp-spectrum-propagation-loss-model.h"
    "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/build/include/ns3/nr-module.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/contrib/nr/examples/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/contrib/nr/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
