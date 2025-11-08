cmake_minimum_required(VERSION 3.13)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

if(NOT TARGET pico_stdlib)
    set(PICO_BOARD pico2_w CACHE STRING "Board type")
    include(pico_sdk_import.cmake)
    project(submodule-wifi C CXX ASM)
    pico_sdk_init()
endif()

add_subdirectory(submodules/logger)

add_library(submodule-wifi wifi-manager.cpp wifi-logger.cpp)

target_include_directories(submodule-wifi PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(submodule-wifi PUBLIC
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    submodule-logger
)

target_compile_features(submodule-wifi PUBLIC cxx_std_17)

set_target_properties(submodule-wifi PROPERTIES VERSION 1.0.0)
