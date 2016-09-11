cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(PbF2Calo)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package(Geant4 REQUIRED ui_all)
find_package(ROOT REQUIRED TreePlayer)

include(${ROOT_USE_FILE})
include(${Geant4_USE_FILE})

set(CMAKE_CXX11_STANDARD_COMPILE_OPTION)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -std=c++14 -Wextra -Wno-shadow -g -O3")

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/action
    ${PROJECT_SOURCE_DIR}/src/geometry
    ${PROJECT_SOURCE_DIR}/src/geometry/calorimeter
    ${PROJECT_SOURCE_DIR}/src/physics
    ${PROJECT_SOURCE_DIR}/src/product
    ${PROJECT_SOURCE_DIR}/src/product/energy_deposit_writer
    ${PROJECT_SOURCE_DIR}/src/product/gdml_writer
    ${PROJECT_SOURCE_DIR}/src/product/photon_detector
    ${PROJECT_SOURCE_DIR}/src/product/photon_origin_writer
    ${PROJECT_SOURCE_DIR}/src/product/muon_track_sniffer
    ${PROJECT_SOURCE_DIR}/src/utility
    ${Geant4_INCLUDE_DIR}
    ${ROOT_INCLUDE_DIR}
    contrib/json11
    /usr/local/include
)

file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/src/*.cc)
list(APPEND sources contrib/json11/json11.cpp)

file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/src/*.h)


message(STATUS ${ROOT_LIBRARIES})

add_executable(PbF2Calo PbF2Calo.cc ${sources} ${headers})
target_link_libraries(PbF2Calo ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

add_subdirectory(ana)

file(GLOB_RECURSE confs ${PROJECT_SOURCE_DIR}/src/ *.json)

set(macs
    run.mac
)

install(TARGETS PbF2Calo DESTINATION bin)
install(FILES ${confs} DESTINATION conf)
install(FILES ${macs} DESTINATION .)
