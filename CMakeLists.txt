cmake_minimum_required(VERSION 3.25)

project(fins LANGUAGES C VERSION 1.0)

# compiler settings
if(MSVC)
    set(
        COMPILER_C_FLAGS
        -Ot
        -GT
        -volatile:iso
        -nologo
        -J
        -sdl
        -W3
        -WX-
        -Wv:18
    )
else()
    set(
        COMPILER_C_FLAGS
        -Wall
        -Wstrict-prototypes
        -Wshadow
        -Wpointer-arith
        -Wcast-qual
        -Wcast-align
        -Wwrite-strings
        -Wnested-externs
        -Wredundant-decls
        -funsigned-char
    )
endif()

# target
file(GLOB_RECURSE LIB_SRCS src/*.c)
file(GLOB_RECURSE LIB_HEADERS include/*.h)
add_library(fins ${LIB_SRCS})
target_compile_options(fins PRIVATE "${COMPILER_C_FLAGS}")
target_include_directories(
    fins PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>  
)
set_target_properties(
    fins PROPERTIES
    PUBLIC_HEADER "${LIB_HEADERS}"
    LINKER_LANGUAGE C
    PREFIX lib
)

# install logic
install(
    TARGETS ${PROJECT_NAME}
    EXPORT "${PROJECT_NAME}Targets"
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/fins
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION "share/${PROJECT_NAME}"
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

install(
    FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION share/${PROJECT_NAME}
)

install(
    EXPORT "${PROJECT_NAME}Targets"
    DESTINATION share/${PROJECT_NAME}
    NAMESPACE fins::
)
