set (imguizmo_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/imguizmo)
set (LIBRARY_NAME imguizmo)

# add_library (ImGui SHARED
# ${imguizmo_SOURCE_DIR_}/example/imgui.h
# ${imguizmo_SOURCE_DIR_}/example/imgui.cpp
# )
add_library (${LIBRARY_NAME} SHARED
    ${imguizmo_SOURCE_DIR_}/ImGuizmo.cpp
    ${imguizmo_SOURCE_DIR_}/ImGuizmo.h

    ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking/imgui.cpp

    # ${imguizmo_SOURCE_DIR_}/example/ImApp.h
    # ${imguizmo_SOURCE_DIR_}/example/main.cpp
)
target_include_directories (${LIBRARY_NAME} PUBLIC
    ${imguizmo_SOURCE_DIR_}
    ${imguizmo_SOURCE_DIR_}/example
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui-docking
)

target_link_libraries (${LIBRARY_NAME} PUBLIC imgui glfw)
