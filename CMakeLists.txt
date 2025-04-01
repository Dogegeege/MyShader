cmake_minimum_required(VERSION 3.23)
project(LearnOpenGL)
#C++标准
set(CMAKE_CXX_STANDARD 20)



#可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

#头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)



#库文件路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

find_library(GLFW_LIB glfw3 ${PROJECT_SOURCE_DIR}/lib)  # 在链接库中找到glfw3链接库文件。

if(NOT GLFW_LIB)    #如果找不到glfw的话这里会报错,本项目lib基于GCC-w64环境
    message(FATAL_ERROR "Cannot find glfw3 library")
endif()


#可执行文件
#file(GLOB SRC_FILE src/*.c src/*.cpp scr/*.h  src/glad/*.c )
file(GLOB SRC_FILE src/* include/glad/* include/stb_image/*)
add_executable(LearnOpenGL ${SRC_FILE})  

#目标链接文件
target_link_libraries(LearnOpenGL ${GLFW_LIB})