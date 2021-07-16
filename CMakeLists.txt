cmake_minimum_required(VERSION 3.0.0)
project(new_project VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB TARGET_SRC "./src/*.cpp" )

add_executable(main ${TARGET_SRC})

# Copy every resource file to the build directory
file(COPY "./Resources/" DESTINATION ${PROJECT_BINARY_DIR}/Resources)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
