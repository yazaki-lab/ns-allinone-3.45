# Install script for directory: /Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/src

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/antenna/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/aodv/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/applications/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/bridge/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/brite/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/buildings/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/click/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/config-store/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/core/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/csma/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/csma-layout/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/dsdv/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/dsr/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/energy/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/flow-monitor/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/internet/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/internet-apps/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/lr-wpan/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/lte/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/mesh/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/mobility/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/netanim/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/network/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/nix-vector-routing/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/olsr/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/openflow/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/point-to-point/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/point-to-point-layout/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/propagation/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/sixlowpan/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/spectrum/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/stats/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/test/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/topology-read/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/traffic-control/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/uan/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/virtual-net-device/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/wifi/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/wimax/cmake_install.cmake")
  include("/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/zigbee/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/Users/kamikawamasahiro/Desktop/ns-allinone-3.45/ns-3.45/cmake-build/src/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
