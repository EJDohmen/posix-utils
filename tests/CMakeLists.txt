project (run-unit-tests)



# Add an executable with the above sources
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE shared-buffer)
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::GTest GTest::Main)
