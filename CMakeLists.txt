cmake_minimum_required(VERSION 3.14)
project(portable_attributes VERSION 1.0.0 LANGUAGES C)

# 引入标准 CMake 模块用于安装和包配置
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# 创建 INTERFACE 库目标（header-only，无需编译）
add_library(portable_attributes INTERFACE)
add_library(portable_attributes::portable_attributes ALIAS portable_attributes)

# 配置包含目录，使用生成器表达式：
# - BUILD_INTERFACE：直接构建项目时使用
# - INSTALL_INTERFACE：使用已安装的包时使用
target_include_directories(portable_attributes INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# 安装头文件到标准包含目录
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h"
)

# 导出目标定义供下游项目使用
install(TARGETS portable_attributes
	EXPORT portable_attributesTargets
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# 生成并安装带命名空间的目标导出文件
install(EXPORT portable_attributesTargets
	FILE portable_attributesTargets.cmake
	NAMESPACE portable_attributes::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/portable_attributes
)

# 从模板生成包配置文件
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/portable_attributesConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/portable_attributesConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/portable_attributes
)

# 生成版本兼容性文件（支持同主版本兼容策略）
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/portable_attributesConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

# 安装包配置文件和版本文件，以支持 find_package() 功能
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/portable_attributesConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/portable_attributesConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/portable_attributes
)
