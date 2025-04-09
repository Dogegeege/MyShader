set(glm_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/glm-1.0.1)
set(LIBRARY_NAME glm)

file(GLOB glm_sources CONFIGURE_DEPENDS "${glm_SOURCE_DIR_}/glm/glm.cppm")

add_library(${LIBRARY_NAME}  INTERFACE)


target_include_directories(${LIBRARY_NAME}  INTERFACE ${glm_SOURCE_DIR_})