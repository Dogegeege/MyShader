set(imguizmo_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo)
set(LIBRARY_NAME imguizmo)

file(GLOB imguizmo_sources CONFIGURE_DEPENDS
    "${imguizmo_SOURCE_DIR_}/ImGuizmo.cpp"
    "${imguizmo_SOURCE_DIR_}/ImGuizmo.h"
    )#主文件

add_library(${LIBRARY_NAME} SHARED ${imguizmo_sources} )
target_include_directories(${LIBRARY_NAME}  PUBLIC  ${imguizmo_SOURCE_DIR_} )
target_link_libraries(${LIBRARY_NAME}  PUBLIC imgui)
