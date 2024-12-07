cmake_minimum_required(VERSION 3.13)

project(engine)

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}" SRC)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/mesh" SRC)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/glad/src" SRC)

add_library(${PROJECT_NAME} ${SRC})

set(CMAKE_PREFIX_PATH

    # "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/Assimp/lib/cmake/assimp-5.4"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/spdlog/lib/cmake/spdlog"
)

# find_package(assimp REQUIRED)
find_package(spdlog REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/glfw/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stb/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/glm/include"
)

target_link_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/glfw/lib"
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    spdlog::spdlog
    glfw3
)