#include "raylib.h"

const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
    // Initialization
    InitWindow(WIDTH, HEIGHT, "Vulkan Triangle");

    // Main loop
    while (!WindowShouldClose()) {
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTriangle((Vector2){400, 100}, (Vector2){300, 300}, (Vector2){500, 300}, RED);
        EndDrawing();
    }

    // De-Initialization
    CloseWindow();

    return 0;
}

