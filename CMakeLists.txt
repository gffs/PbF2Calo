cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

project(PbF2Calo_app)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

find_package(ROOT REQUIRED)

include(${Geant4_USE_FILE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -std=c++1y -Wunreachable-code -pedantic -Wno-c++98-compat -Wno-shadow -Wno-deprecated-register -g -O3")

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)
include_directories(${Geant4_INCLUDE_DIR})
include_directories(${ROOT_INCLUDE_DIR})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB headers_src ${PROJECT_SOURCE_DIR}/src/*.h)
list(APPEND headers ${headers_src})

message(STATUS ${headers})

add_executable(PbF2Calo PbF2Calo.cc ${sources} ${headers})
target_link_libraries(PbF2Calo ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

set(PbF2Calo_SCRIPTS
    run.mac
  )

foreach(_script ${PbF2Calo_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

install(TARGETS PbF2Calo DESTINATION bin)

