cmake_minimum_required(VERSION 3.14)
project(RaylibProjectTemplate C CXX)

set(CMAKE_CXX_STANDARD 17)

# --- Raylib FetchContent ---
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(raylib)

# --- Configuración de Fuentes ---
set(SOURCES src/main.cpp)

# --- Lógica de Plataforma ---
if (${PLATFORM} MATCHES "Web")
    # Para Web, el output DEBE ser index.html para Live Server
    add_executable(index ${SOURCES})
    target_compile_definitions(index PRIVATE PLATFORM_WEB)
    
    target_link_options(index PRIVATE 
        -sUSE_GLFW=3 
        -O2
        -sTOTAL_MEMORY=67108864
        --shell-file ${raylib_SOURCE_DIR}/src/shell.html
    )
    set_target_properties(index PROPERTIES SUFFIX ".html")
    target_link_libraries(index raylib)
else()
    # Para Desktop
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_link_libraries(${PROJECT_NAME} raylib)
endif()