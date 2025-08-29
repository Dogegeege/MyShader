set(stb_image_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
set(LIBRARY_NAME stb_image)

file(GLOB stb_image_sources CONFIGURE_DEPENDS 
    "${stb_image_SOURCE_DIR_}/stb_image.cpp"
    #"${stb_image_SOURCE_DIR_}/stb_image.h"
    )#主文件


add_library(${LIBRARY_NAME} SHARED ${stb_image_sources})

target_include_directories(${LIBRARY_NAME} PUBLIC ${stb_image_SOURCE_DIR_})