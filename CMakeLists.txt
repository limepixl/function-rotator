cmake_minimum_required(VERSION 3.14)

project(function-rotator)

file(GLOB SRC "*.h" "src/*.cpp" "thirdparty/glad/src/glad.c")

include_directories(thirdparty/glad/include thirdparty/glm)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} glfw dl)