cmake_minimum_required(VERSION 3.27)
project(SAPPHIRE)

include(FetchContent)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_LOCAL_EMERALD "Use local EMERALD instead of GitHub release" ON)

# ---------------- SDL3 ----------------
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-3.2.4   # pick the latest stable tag
)
FetchContent_MakeAvailable(SDL3)

# ---------------- IMGUI ----------------
FetchContent_Declare(
    ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG        v1.91.7-docking
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ImGui)

FetchContent_Declare(
    ImPlot
    GIT_REPOSITORY https://github.com/epezent/implot
    GIT_TAG        v0.16
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ImPlot)

# ---------------- EMERALD ----------------
if(USE_LOCAL_EMERALD AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../EMERALD")
    message(STATUS "Using local EMERALD from ../EMERALD")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../EMERALD ${CMAKE_BINARY_DIR}/emerald_build)
else()
    message(STATUS "Fetching EMERALD from GitHub release")
    FetchContent_Declare(
        EMERALD
        GIT_REPOSITORY https://github.com/DurnezG/EMERALD.git
    )
    FetchContent_MakeAvailable(EMERALD)
endif()

# ---------------- Source files ----------------
set(IMGUI_SRC
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp")

set(IMPLOT_SRC
    "${implot_SOURCE_DIR}/implot.cpp"
    "${implot_SOURCE_DIR}/implot_items.cpp")

set(SRC_FILES
    "src/Sapphire.cpp"
)

add_executable(${PROJECT_NAME} 
    ${SRC_FILES} 
    ${IMGUI_SRC} 
    ${IMPLOT_SRC}
)

# ---------------- Linking ----------------
target_link_libraries(${PROJECT_NAME} PUBLIC
    EMERALD
    SDL3::SDL3
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${implot_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}
)

if(TARGET EMERALD)
    get_target_property(EMERALD_INCLUDE_DIRS RUBY INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${PROJECT_NAME} PUBLIC ${EMERALD_INCLUDE_DIRS})
else()
    target_include_directories(${PROJECT_NAME} PUBLIC "${emerald_SOURCE_DIR}/include")
endif()

# ---------------- Warnings ----------------
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# ---------------- Copy SDL3.dll on Windows ----------------
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL3::SDL3>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()


#cmake_minimum_required(VERSION 3.27)
#project(SAPPHIRE)
#
#include(FetchContent)
#
## Use C++20
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
#option(USE_LOCAL_EMERALD "Use local EMERALD instead of GitHub release" ON)
#
#FetchContent_Declare(
#      ImGui
#      GIT_REPOSITORY https://github.com/ocornut/imgui
#      GIT_TAG        v1.91.7-docking
#      GIT_SHALLOW TRUE
#)
#FetchContent_MakeAvailable(ImGui)
#
#FetchContent_Declare(
#      ImPlot
#      GIT_REPOSITORY https://github.com/epezent/implot
#      GIT_TAG        v0.16
#      GIT_SHALLOW TRUE
#)
#FetchContent_MakeAvailable(ImPlot)
#
#if(USE_LOCAL_EMERALD AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../EMERALD")
#    message(STATUS "Using local EMERALD from ../EMERALD")
#    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../EMERALD ${CMAKE_BINARY_DIR}/emerald_build)
#else()
#    message(STATUS "Fetching EMERALD from GitHub release")
#    include(FetchContent)
#    FetchContent_Declare(
#        EMERALD
#        GIT_REPOSITORY https://github.com/DurnezG/EMERALD.git
#    )
#    FetchContent_MakeAvailable(EMERALD)
#endif()
#
#set(IMGUI_SRC
#    "${imgui_SOURCE_DIR}/imgui.cpp"
#    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
#    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
#    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
#    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
#    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp"
#    "${imgui_SOURCE_DIR}/imgui_demo.cpp")
#
#set(IMPLOT_SRC
#    "${implot_SOURCE_DIR}/implot.cpp"
#    "${implot_SOURCE_DIR}/implot_items.cpp")
#
#Set( SRC_INCLUDES
#    
#)
#
#set( SRC_FILES
#    "src/Sapphire.cpp"
#)
#
#add_executable(${PROJECT_NAME} 
#    ${SRC_FILES} 
#    ${IMGUI_SRC} 
#    ${IMPLOT_SRC})
#
#target_link_libraries(${PROJECT_NAME} PUBLIC EMERALD)
#
#
#target_include_directories(${PROJECT_NAME} PUBLIC 
#    ${implot_SOURCE_DIR} 
#    ${imgui_SOURCE_DIR}
#)
#
#if(TARGET EMERALD)
#    get_target_property(EMERALD_INCLUDE_DIRS RUBY INTERFACE_INCLUDE_DIRECTORIES)
#    target_include_directories(${PROJECT_NAME} PUBLIC ${EMERALD_INCLUDE_DIRS})
#else()
#    # Fallback: manually add the include path if target property is not set
#    target_include_directories(${PROJECT_NAME} PUBLIC "${emerald_SOURCE_DIR}/include")
#endif()
#
## compile at max warning level + treat warnings as errors
#if (MSVC)
#    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
#else()
#    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
#endif()
