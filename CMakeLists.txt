cmake_minimum_required(VERSION 3.14)
project(MinimalTetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# 1. Raylib (solo necesario para app y tests)
include(FetchContent)
FetchContent_Declare(raylib GIT_REPOSITORY https://github.com/raysan5/raylib.git GIT_TAG master)
FetchContent_MakeAvailable(raylib)

# 2. Biblioteca de lógica (sin raylib)
file(GLOB_RECURSE CORE_SOURCES "src/*.cpp")
add_library(tetris_core STATIC ${CORE_SOURCES})
target_include_directories(tetris_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
# NO enlazamos raylib aquí

# 3. Ejecutable principal
add_executable(${PROJECT_NAME} app/main.cpp app/GameRenderer.cpp app/InputHandler.cpp app/AudioManager.cpp app/Colors.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE tetris_core raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 4. Copiar recursos (igual que antes)
if (${PLATFORM} MATCHES "Web")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_WEB)
    target_link_options(${PROJECT_NAME} PRIVATE 
        -sUSE_GLFW=3 -O2 -sINITIAL_MEMORY=67108864
        --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/src/shell.html
        --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/resources@resources
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_DESKTOP)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
    )
endif()

# 5. Tests
enable_testing()
add_subdirectory(tests)