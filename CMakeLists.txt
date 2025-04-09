cmake_minimum_required(VERSION 3.23)

#C++标准
set(CMAKE_CXX_STANDARD 20)
set(TARGET_NAME LearnOpenGL)

project(${TARGET_NAME})

add_subdirectory(include)


#可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")






#可执行文件
file(GLOB SRC_FILE
    src/*
    src/Application/*
)
add_executable(${TARGET_NAME} ${SRC_FILE})  


# 添加头文件搜索路径
target_include_directories(${TARGET_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}/include/glad/include
    ${PROJECT_SOURCE_DIR}/include/glfw/include
    ${PROJECT_SOURCE_DIR}/include/glm-1.0.1
    ${PROJECT_SOURCE_DIR}/include/imgui-docking
    ${PROJECT_SOURCE_DIR}/include/stb_image

)


#目标链接文件
target_link_libraries(${TARGET_NAME} PUBLIC glad)
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC glm)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)
target_link_libraries(${TARGET_NAME} PUBLIC stb_image) 

