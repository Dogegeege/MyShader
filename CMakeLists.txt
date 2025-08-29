cmake_minimum_required(VERSION 3.10)

# 项目名称和 C++ 标准
set(TARGET_NAME LearnOpenGL)
set(CMAKE_CXX_STANDARD 17)

#全局默认编译条件
# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")



#可执行文件输出路径
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

project(${TARGET_NAME})



####################################################################################################
# 添加子目录（第三方库）

#强制设置动态库
# set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)


# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#!注意需要在添加子项目前设置动态库的保存路径
#add_subdirectory(3rdParty)


####################################################################################################


#头文件
# file(GLOB PROJECT_HEADERS
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/global/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/math/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/material/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/camera/*.h    
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/cameracontrol/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/texture/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/loader/*.h
#     #include/Application
# )

# 第三方库头文件
# file(GLOB THIRD_PARTY_HEADERS
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw/include/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm-1.0.1/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui-docking/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb_image/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/assimp/include/*.h
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imguizmo/*.h

#     ${CMAKE_BINARY_DIR}/3rdParty/assimp/include/assimp/*.h
    
# )


#可执行文件
file(GLOB PROJECT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main_test.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/math/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/material/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/camera/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/cameracontrol/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/texture/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/src/loader/*.cpp

    #${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb_image/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imguizmo/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui-docking/*.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui-docking/backends/imgui_impl_glfw.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui-docking/backends/imgui_impl_opengl3.cpp



)

#着色器文件
# file(GLOB PROJECT_SHADERS
#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/shader/*.vert
#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/shader/*.vs
#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/shader/*.vsh

#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/shader/*.frag
#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/shader/*.fs
#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/shader/*.fsh

#     ${CMAKE_CURRENT_SOURCE_DIR}/assets/shader/*.geom
#     #assets/shader/*.geom
#     #....
# )


####################################################################################################
# 添加可执行文件
add_executable(${TARGET_NAME} ${PROJECT_SOURCES})  


# 添加头文件搜索路径
# 设置为 PUBLIC 是为了浏览子模块时候头文件不报错
# target_include_directories(${TARGET_NAME} PUBLIC
#    #${THIRD_PARTY_HEADERS}
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw/include
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm-1.0.1
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui-docking
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb_image
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/assimp/include
#     ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imguizmo


#     ${CMAKE_BINARY_DIR}/3rdParty/assimp/include/assimp
    
#     #${PROJECT_HEADERS}
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/core
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/global
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/util
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/math
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/camera
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/cameracontrol
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/texture
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/material
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/loader
    
# )

# 添加文件分组
# source_group("headers" FILES ${PROJECT_HEADERS})
# source_group("shaders" FILES ${PROJECT_SHADERS})
# source_group("sources" FILES ${PROJECT_SOURCES})
# source_group("3rdPartyHeaders" FILES ${THIRD_PARTY_HEADERS})




####################################################################################################
# 链接第三方库

# target_link_libraries(${TARGET_NAME} PUBLIC stb_image) 


# target_link_libraries(${TARGET_NAME} PUBLIC glad)
# target_link_libraries(${TARGET_NAME} PUBLIC glfw)
# target_link_libraries(${TARGET_NAME} PRIVATE imgui)
# target_link_libraries(${TARGET_NAME} PUBLIC assimp)
# target_link_libraries(${TARGET_NAME} PUBLIC imguizmo)



#本目标设置编译条件
# if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#     target_compile_options(${TARGET_NAME} PRIVATE
#         #-Wall
#         -g
#         #-O2
#         #-Wextra
#         #-Wpedantic
#         #-std=c++17
#     )
# elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#     target_compile_options(${TARGET_NAME} PRIVATE
#         /W4
#         /std:c++17
#     )
# endif ()

