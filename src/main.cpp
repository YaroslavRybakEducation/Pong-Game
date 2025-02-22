#include "raylib.h"

// Screen settings
const int screenWidth = 800;
const int screenHeight = 600;

// size paddles and balls
const int paddleWidth = 10;
const int paddleHeight = 100;
const int ballSize = 10;

enum GameState { MENU, PLAYING, GAME_OVER };

int main() {
    InitWindow(screenWidth, screenHeight, "Pong Game");
    SetTargetFPS(60);

    // Player settings
    Rectangle leftPaddle = { 50, screenHeight / 2 - paddleHeight / 2, paddleWidth, paddleHeight };
    Rectangle rightPaddle = { screenWidth - 60, screenHeight / 2 - paddleHeight / 2, paddleWidth, paddleHeight };
    Vector2 ballPosition = { screenWidth / 2, screenHeight / 2 };
    Vector2 ballSpeed = { 4.0f, 4.0f };

    int leftScore = 0;
    int rightScore = 0;
    GameState gameState = MENU;

    while (!WindowShouldClose()) {
        if (gameState == MENU) {
            if (IsKeyPressed(KEY_ENTER)) {
                gameState = PLAYING;
                leftScore = 0;
                rightScore = 0;
                ballPosition = { screenWidth / 2, screenHeight / 2 };
            }

            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("PONG", screenWidth / 2 - 60, 100, 40, GREEN);
            DrawText("Press ENTER to start the game", screenWidth / 2 - 180, 200, 20, WHITE);
            EndDrawing();

        } else if (gameState == PLAYING) {
            // --- UPDATING ---
            // Moving the player
            if (IsKeyDown(KEY_W) && leftPaddle.y > 0) leftPaddle.y -= 5.0f;
            if (IsKeyDown(KEY_S) && leftPaddle.y < screenHeight - paddleHeight) leftPaddle.y += 5.0f;

            // Moving the alternate player
            if (IsKeyDown(KEY_UP) && rightPaddle.y > 0) rightPaddle.y -= 5.0f;
            if (IsKeyDown(KEY_DOWN) && rightPaddle.y < screenHeight - paddleHeight) rightPaddle.y += 5.0f;

            // Updating move ball
            ballPosition.x += ballSpeed.x;
            ballPosition.y += ballSpeed.y;

            // Checking collisions with Balls
            if (ballPosition.y <= 0 || ballPosition.y >= screenHeight - ballSize)
                ballSpeed.y *= -1;

            // Checking collisions with Paddle
            if (CheckCollisionCircleRec(ballPosition, ballSize, leftPaddle) ||
                CheckCollisionCircleRec(ballPosition, ballSize, rightPaddle)) {
                ballSpeed.x *= -1;
            }

            // Reset the ball when players increasing score
            if (ballPosition.x <= 0) {
                rightScore++;
                ballPosition = { screenWidth / 2, screenHeight / 2 };
                ballSpeed = { 4.0f, 4.0f };
            }
            if (ballPosition.x >= screenWidth) {
                leftScore++;
                ballPosition = { screenWidth / 2, screenHeight / 2 };
                ballSpeed = { 4.0f, 4.0f };
            }

            // Checking game over
            if (leftScore == 5 || rightScore == 5) {
                gameState = GAME_OVER;
            }

            // --- DRAWING ---
            BeginDrawing();
            ClearBackground(BLACK);

            DrawRectangleRec(leftPaddle, WHITE);
            DrawRectangleRec(rightPaddle, WHITE);
            DrawCircleV(ballPosition, ballSize, WHITE);
            DrawText("PONG", screenWidth / 2 - 40, 10, 20, GREEN);

            // Rendering Score
            DrawText(TextFormat("%d", leftScore), screenWidth / 4, 20, 30, WHITE);
            DrawText(TextFormat("%d", rightScore), screenWidth * 3 / 4, 20, 30, WHITE);

            EndDrawing();
        } else if (gameState == GAME_OVER) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("GAME OVER!", screenWidth / 2 - 100, 100, 30, RED);
            DrawText(leftScore == 5 ? "Player win!" : "Alternate Player wins!", screenWidth / 2 - 150, 150, 20, WHITE);
            DrawText("Press ENTER, To return to main menu", screenWidth / 2 - 200, 200, 20, WHITE);
            if (IsKeyPressed(KEY_ENTER)) {
                gameState = MENU;
            }
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
