set(imgui_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking)
set(LIBRARY_NAME imgui)

file(GLOB imgui_sources CONFIGURE_DEPENDS "${imgui_SOURCE_DIR_}/*.cpp")#主文件
file(GLOB imgui_impl CONFIGURE_DEPENDS
        "${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.cpp"
        "${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.h"
        "${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3.cpp"
        "${imgui_SOURCE_DIR_}/backends/imgui_impl_opengl3.h"
)#opengl3/glfw文件

#私有化包含头文件,顶层目录包含头文件需要重声明
add_library(${LIBRARY_NAME} STATIC ${imgui_sources} ${imgui_impl})
target_include_directories(${LIBRARY_NAME}  PRIVATE  ${imgui_SOURCE_DIR_} ${imgui_SOURCE_DIR_}/backends)
target_link_libraries(${LIBRARY_NAME}  PUBLIC glfw)
