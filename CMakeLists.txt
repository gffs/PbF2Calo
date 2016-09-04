cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

project(PbF2Calo)

find_package(Geant4 REQUIRED ui_all)
find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})
set(CMAKE_CXX11_STANDARD_COMPILE_OPTION)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -std=c++14 -Wextra -Wno-shadow -g -O3")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)
include_directories(${Geant4_INCLUDE_DIR})
include_directories(${ROOT_INCLUDE_DIR})
include_directories(contrib/json11)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
list(APPEND sources contrib/json11/json11.cpp)

file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB headers_src ${PROJECT_SOURCE_DIR}/src/*.h)
list(APPEND headers ${headers_src})

add_executable(PbF2Calo PbF2Calo.cc ${sources} ${headers})
target_link_libraries(PbF2Calo ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

add_subdirectory(ana)


set(PbF2Calo_SCRIPTS
    run.mac
    uw.json
    artg4.json
)

foreach(_script ${PbF2Calo_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

install(TARGETS PbF2Calo DESTINATION bin)
