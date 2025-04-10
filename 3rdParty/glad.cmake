set(glad_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/glad)
set(LIBRARY_NAME glad)

file(GLOB glad_sources CONFIGURE_DEPENDS "${glad_SOURCE_DIR_}/src/*.c")

add_library(glad STATIC ${glad_sources})

#私有化包含头文件,顶层目录包含头文件需要重声明
target_include_directories(glad PRIVATE ${glad_SOURCE_DIR_}/include)
