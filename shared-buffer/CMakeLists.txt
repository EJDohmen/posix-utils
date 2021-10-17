project(shared-buffer)



add_library(${PROJECT_NAME}
${PROJECT_SOURCE_DIR}/src/Shared_Ring_Buffer.cpp

)


target_link_libraries(${PROJECT_NAME} PUBLIC synchronization)
