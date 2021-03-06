cmake_minimum_required(VERSION 3.5)
project(cyfw_examples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

add_subdirectory(cyfw)
include_directories(${CYFW_INCLUDE_DIR})


add_executable(00-hello examples/00-hello.cpp)
target_link_libraries(00-hello cyfw)

add_executable(01-triangle examples/01-triangle.cpp)
target_link_libraries(01-triangle cyfw)

add_executable(02-shadertoy examples/02-shadertoy.cpp)
target_link_libraries(02-shadertoy cyfw)

add_executable(03-cube examples/03-cube.cpp)
target_link_libraries(03-cube cyfw)

add_executable(04-text examples/04-text.cpp)
target_link_libraries(04-text cyfw)

add_executable(05-video examples/05-video.cpp)
target_link_libraries(05-video cyfw)

add_executable(06-gui-demo examples/06-gui-demo.cpp)
target_link_libraries(06-gui-demo cyfw)

embed_resources(Fonts examples/resources/a-scp-r.ttf)
add_executable(07-editor examples/07-editor.cpp ${Fonts})
target_link_libraries(07-editor cyfw)

add_executable(08-basic3d examples/08-basic3d.cpp)
target_link_libraries(08-basic3d cyfw)

add_executable(09-more3d examples/09-more3d.cpp)
target_link_libraries(09-more3d cyfw)

add_executable(10-multieditor examples/10-multieditor.cpp ${Fonts})
target_link_libraries(10-multieditor cyfw)
