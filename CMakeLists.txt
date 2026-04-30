cmake_minimum_required(VERSION 3.14)
project(portable_attributes
        VERSION 1.0.0
        LANGUAGES C
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# 创建 INTERFACE 库
add_library(portable_attributes INTERFACE)
add_library(portable_attributes::portable_attributes ALIAS portable_attributes)

# 设置包含目录
target_include_directories(portable_attributes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


# 安装头文件
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h"
)

# 导出目标
install(TARGETS portable_attributes
        EXPORT portable_attributesTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 导出目标文件（带命名空间）
install(EXPORT portable_attributesTargets
        FILE portable_attributesTargets.cmake
        NAMESPACE portable_attributes::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/portable_attributes
)

# 生成并安装 Config 文件
configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/portable_attributesConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/portable_attributesConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/portable_attributes
)

# 生成版本文件
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/portable_attributesConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/portable_attributesConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/portable_attributesConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/portable_attributes
)
