cmake_minimum_required(VERSION 3.23)

# 项目名称和 C++ 标准
set(TARGET_NAME LearnOpenGL)
project(${TARGET_NAME})
set(CMAKE_CXX_STANDARD 20)

#设置编译条件
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MTd")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++20")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

#可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")


####################################################################################################
# 添加子目录（第三方库）

add_subdirectory(3rdParty)

####################################################################################################


#头文件
file(GLOB PROJECT_HEADERS
    include
    include/Application
)

# 第三方库头文件
file(GLOB THIRD_PARTY_HEADERS
    3rdParty/glad/include
    3rdParty/glfw/include
    3rdParty/glm-1.0.1
    3rdParty/imgui-docking
    3rdParty/stb_image
)


#可执行文件
file(GLOB PROJECT_SOURCES
    src/*.cpp
)

#着色器文件
file(GLOB PROJECT_SHADERS
    assets/shader/*.vert
    assets/shader/*.vs
    assets/shader/*.vsh

    assets/shader/*.frag
    assets/shader/*.fs
    assets/shader/*.fsh

    assets/shader/*.geom
    assets/shader/*.geom
    #....
)


####################################################################################################
# 添加可执行文件
add_executable(${TARGET_NAME} ${PROJECT_SOURCES})  


# 添加头文件搜索路径
target_include_directories(${TARGET_NAME} PRIVATE
    ${THIRD_PARTY_HEADERS}
    ${PROJECT_HEADERS}
)

# 添加文件分组
source_group("headers" FILES ${PROJECT_HEADERS})
source_group("shaders" FILES ${PROJECT_SHADERS})
source_group("sources" FILES ${PROJECT_SOURCES})
source_group("3rdPartyHeaders" FILES ${THIRD_PARTY_HEADERS})


####################################################################################################
# 链接第三方库
target_link_libraries(${TARGET_NAME} PUBLIC glad)
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)
target_link_libraries(${TARGET_NAME} PUBLIC stb_image) 

