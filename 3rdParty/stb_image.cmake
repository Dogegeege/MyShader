set(stb_image_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
set(LIBRARY_NAME stb_image)

file(GLOB glm_sources CONFIGURE_DEPENDS "${stb_image_SOURCE_DIR_}/glm/glm.cppm")

add_library(${LIBRARY_NAME} SHARED ${stb_image_SOURCE_DIR_}/stb_image.cpp)