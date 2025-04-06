# 添加imgui源文件
file(GLOB IMGUI_SRC
    *.cpp
    *.h
)

# 添加imgui库
add_library(imgui STATIC ${IMGUI_SRC})

# 链接头文件路径
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 只需要backends中OpenGL3的源文件
file(GLOB IMGUI_BACKENDS_SRC
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_glfw.h
)

# 将backends文件添加到imgui库
target_sources(imgui PRIVATE ${IMGUI_BACKENDS_SRC})

# 链接backends头文件路径
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/backends)
