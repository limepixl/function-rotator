cmake_minimum_required(VERSION 3.14)

project(function-rotator)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(Assimp REQUIRED)

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} glfw GL GLEW assimp)
