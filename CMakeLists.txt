cmake_minimum_required(VERSION 3.23)

# 项目名称和 C++ 标准
set(TARGET_NAME LearnOpenGL)
project(${TARGET_NAME})
set(CMAKE_CXX_STANDARD 17)

#设置编译条件

# if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#     target_compile_options(${TARGET_NAME} PRIVATE
#         -Wall
#         -Wextra
#         -Wpedantic
#         -std=c++20
#     )
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#     target_compile_options(${TARGET_NAME} PRIVATE
#         /W4
#         /std:c++17
#     )
# endif ()

#可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")


####################################################################################################
# 添加子目录（第三方库）

#强制设置动态库
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#!注意需要在添加子项目前设置动态库的保存路径
add_subdirectory(3rdParty)


####################################################################################################


#头文件
file(GLOB PROJECT_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    #include/Application
)

# 第三方库头文件
file(GLOB THIRD_PARTY_HEADERS
    3rdParty/glad/include
    3rdParty/glfw/include
    3rdParty/glm-1.0.1
    3rdParty/imgui-docking
    3rdParty/stb_image
    3rdParty/assimp/include
    ${CMAKE_BINARY_DIR}/3rdParty/assimp/include/assimp
    
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
    #assets/shader/*.geom
    #....
)


####################################################################################################
# 添加可执行文件
add_executable(${TARGET_NAME} ${PROJECT_SOURCES})  


# 添加头文件搜索路径
# 设置为 PUBLIC 是为了浏览子模块时候头文件不报错
target_include_directories(${TARGET_NAME} PUBLIC
    #${THIRD_PARTY_HEADERS}
    3rdParty/glad/include
    3rdParty/glfw/include
    3rdParty/glm-1.0.1
    3rdParty/imgui-docking
    3rdParty/stb_image
    3rdParty/assimp/include
    ${CMAKE_BINARY_DIR}/3rdParty/assimp/include/assimp
    

    #${PROJECT_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include

)

# 添加文件分组
source_group("headers" FILES ${PROJECT_HEADERS})
source_group("shaders" FILES ${PROJECT_SHADERS})
source_group("sources" FILES ${PROJECT_SOURCES})
source_group("3rdPartyHeaders" FILES ${THIRD_PARTY_HEADERS})




####################################################################################################
# 链接第三方库

#link_directories(${CMAKE_BINARY_DIR}/3rdParty/assimp/lib)


target_link_libraries(${TARGET_NAME} PUBLIC glad)
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)
target_link_libraries(${TARGET_NAME} PUBLIC stb_image) 
target_link_libraries(${TARGET_NAME} PUBLIC assimp)




