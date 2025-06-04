set(glad_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/glad)
set(LIBRARY_NAME glad)

file(GLOB glad_sources CONFIGURE_DEPENDS "${glad_SOURCE_DIR_}/src/*.c")

add_library(${LIBRARY_NAME} SHARED ${glad_sources})

target_include_directories(${LIBRARY_NAME} PUBLIC ${glad_SOURCE_DIR_}/include)
