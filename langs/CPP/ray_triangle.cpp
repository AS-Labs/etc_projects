#include "raylib.h"

int main() {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Draw Triangle");

    // Main game loop
    while (!WindowShouldClose()) {
        // Update

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw triangle
        Vector2 points[3] = {
            { screenWidth / 2.0f, 100.0f },
            { screenWidth / 2.0f - 200.0f, screenHeight - 100.0f },
            { screenWidth / 2.0f + 200.0f, screenHeight - 100.0f }
        };
        DrawTriangle(points[0], points[1], points[2], RED);

        EndDrawing();
    }

    // Clean up
    CloseWindow();

    return 0;
}

