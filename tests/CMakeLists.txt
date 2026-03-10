# Incluir Unity mediante FetchContent
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG master
)
FetchContent_MakeAvailable(unity)

# Ejecutable de pruebas
add_executable(tetris_tests test_tetris.cpp)
target_link_libraries(tetris_tests PRIVATE tetris_core unity)

# Registrar la prueba con CTest
add_test(NAME TetrisCoreTests COMMAND tetris_tests)