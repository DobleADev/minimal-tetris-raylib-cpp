cmake_minimum_required(VERSION 3.14)
project(MinimalTetris C CXX)

set(CMAKE_CXX_STANDARD 17)

# 1. --- Raylib FetchContent (Keep as is) ---
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(raylib)

# 2. --- Centralized Source Management ---
# Add every .cpp file in your src folder automatically
file(GLOB_RECURSE ALL_SOURCES "src/*.cpp")

# 3. --- Define the Target Name once ---
if (${PLATFORM} MATCHES "Web")
    set(TARGET_NAME "index")
else()
    set(TARGET_NAME ${PROJECT_NAME})
endif()

# 4. --- Create the Executable once ---
add_executable(${TARGET_NAME} ${ALL_SOURCES})
target_link_libraries(${TARGET_NAME} PRIVATE raylib)

# 5. --- Platform Specific Logic ---
if (${PLATFORM} MATCHES "Web")
    target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_WEB)
    
    target_link_options(${TARGET_NAME} PRIVATE 
        -sUSE_GLFW=3 
        -O2
        -sINITIAL_MEMORY=67108864
        --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/src/shell.html
        --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/resources@resources
    )
    set_target_properties(${TARGET_NAME} PROPERTIES SUFFIX ".html")
else()
    # Desktop specific settings
    target_compile_definitions(${TARGET_NAME} PRIVATE PLATFORM_DESKTOP)

    # COPIAR RECURSOS AL DIRECTORIO DE SALIDA
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${TARGET_NAME}>/resources"
    )
endif()
