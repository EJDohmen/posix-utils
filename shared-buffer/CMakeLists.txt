project(shared-buffer)



add_library(${PROJECT_NAME}
${PROJECT_SOURCE_DIR}/src/Shared_Buffer.cpp
${PROJECT_SOURCE_DIR}/src/Shared_Ring_Buffer.cpp

)

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)