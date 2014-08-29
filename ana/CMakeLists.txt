project(ana)

file(GLOB ana_sources ${PROJECT_SOURCE_DIR}/*.cc)
add_executable(event_recon ${ana_sources})
target_link_libraries(event_recon ${ROOT_LIBRARIES})

install(TARGETS event_recon DESTINATION bin)

