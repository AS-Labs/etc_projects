#include "raylib.h"

const int screenWidth = 800;
const int screenHeight = 450;
const float playerSpeed = 5.0f;
const float swingForce = 10.0f;

int main(void)
{
    // Initialization
    InitWindow(screenWidth, screenHeight, "Swinging Rectangle");

    Rectangle player = { screenWidth / 2 - 40, screenHeight / 2 - 40, 80, 80 };
    Vector2 playerPosition = { player.x + player.width / 2, player.y + player.height / 2 };
    Vector2 playerVelocity = { 0.0f, 0.0f };

    bool isSwinging = false;

    SetTargetFPS(60); // Set the desired framerate (frames-per-second)

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        if (IsKeyDown(KEY_W)) playerPosition.y -= playerSpeed;
        if (IsKeyDown(KEY_S)) playerPosition.y += playerSpeed;
        if (IsKeyDown(KEY_A)) playerPosition.x -= playerSpeed;
        if (IsKeyDown(KEY_D)) playerPosition.x += playerSpeed;

        if (IsKeyPressed(KEY_SPACE) && !isSwinging)
        {
            isSwinging = true;
            playerVelocity = (Vector2){ 0.0f, -swingForce };
        }

        if (isSwinging)
        {
            playerPosition.x += playerVelocity.x;
            playerPosition.y += playerVelocity.y;
            playerVelocity.y += 0.2f; // Apply gravity

            if (playerPosition.y > screenHeight) // Stop swinging when player hits ground
            {
                playerPosition.y = screenHeight;
                isSwinging = false;
            }
        }

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangleRec(player, RED);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

