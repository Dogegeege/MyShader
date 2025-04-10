cmake_minimum_required(VERSION 3.23)

#C++标准
set(CMAKE_CXX_STANDARD 20)
set(TARGET_NAME LearnOpenGL)

#设置编译条件
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MTd")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++20")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

####################################################################################################

project(${TARGET_NAME})

add_subdirectory(3rdParty)


#可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")






#可执行文件
file(GLOB PROJECT_SOURCES
    src/*.cpp
)
add_executable(${TARGET_NAME} ${PROJECT_SOURCES})  

#着色器文件
file(
    GLOB PROJECT_SHADERS
    assets/shader/*.vert
    assets/shader/*.vs
    assets/shader/*.vsh

    assets/shader/*.frag
    assets/shader/*.fs
    assets/shader/*.fsh

    assets/shader/*.geom
    assets/shader/*.geom

)

#头文件
file(
    GLOB PROJECT_HEADERS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Application
)

file(
    GLOB 3RD_HEADERS
    ${PROJECT_SOURCE_DIR}/3rdParty/glad/include
    ${PROJECT_SOURCE_DIR}/3rdParty/glfw/include
    ${PROJECT_SOURCE_DIR}/3rdParty/glm-1.0.1
    ${PROJECT_SOURCE_DIR}/3rdParty/imgui-docking
    ${PROJECT_SOURCE_DIR}/3rdParty/stb_image
)

# 添加头文件搜索路径
target_include_directories(${TARGET_NAME} PRIVATE
    ${3RD_HEADERS}
    ${PROJECT_HEADERS}
)

# 添加文件分组
source_group("headers" FILES ${PROJECT_HEADERS})
source_group("shaders" FILES ${PROJECT_SHADERS})
source_group("sources" FILES ${PROJECT_SOURCES})
source_group("vendors" FILES ${3RD_HEADERS})


#目标链接文件
target_link_libraries(${TARGET_NAME} PUBLIC glad)
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)
target_link_libraries(${TARGET_NAME} PUBLIC stb_image) 

