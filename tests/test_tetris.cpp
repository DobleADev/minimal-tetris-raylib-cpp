#include "unity.h"
#include "GameState.h"

GameState* state;

void setUp() {
    state = new GameState();
    state->Reset(); // Asegurar estado inicial
}

void tearDown() {
    delete state;
}

void test_moveLeft_valid() {
    // Asumiendo que la pieza actual está en columna 3 (por ejemplo)
    int initialCol = state->GetCurrentPiece().GetCol();
    state->HandleInput(InputAction::MoveLeft);
    TEST_ASSERT_EQUAL_INT(initialCol - 1, state->GetCurrentPiece().GetCol());
}

void test_moveLeft_invalid_wall() {
    // Mover hasta la izquierda hasta que no pueda
    for (int i = 0; i < 10; i++) state->HandleInput(InputAction::MoveLeft);
    int col = state->GetCurrentPiece().GetCol();
    state->HandleInput(InputAction::MoveLeft);
    TEST_ASSERT_EQUAL_INT(col, state->GetCurrentPiece().GetCol());
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_moveLeft_valid);
    RUN_TEST(test_moveLeft_invalid_wall);
    // ... más pruebas
    return UNITY_END();
}