set(INCLUDE_FOLDER "3rdParty")

##############################################################################
#glad

if (NOT TARGET glad)
    include(glad.cmake)
    set_target_properties(glad PROPERTIES FOLDER ${INCLUDE_FOLDER}/glad)
endif ()

##############################################################################
#glm

##glm不需要编译

##############################################################################
#glfw

if (NOT TARGET glfw)
    option(GLFW_BUILD_EXAMPLES "" OFF)  # 禁用示例
    option(GLFW_BUILD_TESTS "" OFF)     # 禁用测试
    option(GLFW_BUILD_DOCS "" OFF)      # 禁用文档
    option(GLFW_INSTALL "" OFF)         # 禁用安装
    option(BUILD_SHARED_LIBS ON)        # 启用动态库构建


    add_subdirectory(glfw)
    set_target_properties(glfw PROPERTIES FOLDER ${INCLUDE_FOLDER}/glfw)
    set_target_properties(update_mappings PROPERTIES FOLDER ${INCLUDE_FOLDER}/glfw)
endif ()

##############################################################################
#imgui

if (NOT TARGET imgui)
    include(imgui.cmake)
    set_target_properties(imgui PROPERTIES FOLDER ${INCLUDE_FOLDER}/imgui)
endif ()

##############################################################################
#stb_image

if (NOT TARGET stb_image)
    include(stb_image.cmake)
    set_target_properties(stb_image PROPERTIES FOLDER ${INCLUDE_FOLDER}/stb_image)
endif ()

##############################################################################
#assimp

if (NOT TARGET assimp)
    option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
    option(ASSIMP_BUILD_SAMPLES OFF)
    option(ASSIMP_BUILD_TESTS OFF)# 关闭测试和安装以减少编译时间
    option(BUILD_SHARED_LIBS ON)#ON生成动态库
    #option(ASSIMP_BUILD_ZLIB "Use internal ZLIB" ON) # 使用 Assimp 内置 ZLIB‌:ml-citation{ref="4" data="citationList"}
    #option(ASSIMP_BUILD_MINIZIP ON)  # 启用 ZIP 压缩支持‌:ml-citation{ref="6" data="citationList"}

    set(ASSIMP_BUILD_ALL_IMPORTERS ON CACHE BOOL "Enable all importers" FORCE)    # 启用所有导入器
    set(ASSIMP_BUILD_ZLIB ON)

    set(ASSIMP_INSTALL OFF CACHE BOOL "Disable installation" FORCE)

    # 确保不跳过任何依赖
    set(ASSIMP_BUILD_MINIZIP OFF CACHE BOOL "Disable minizip" FORCE)

    add_subdirectory(assimp)
    # 为 assimp 设置单独的编译选项，禁用警告报错
    #set_target_properties(assimp PROPERTIES COMPILE_OPTIONS "-Wno-error")
    option(ASSIMP_WARNINGS_AS_ERRORS OFF)

endif ()