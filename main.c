#include "raylib.h"

int main(void) {
    // 1. Set up anti-aliasing
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // Initializing with 0,0 auto-detects the monitor's native resolution
    InitWindow(0, 0, "grug-factory engine");
    
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // 2. Initialize the 2D Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f }; // What the camera is looking at in the world
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }; // Where the target is drawn on screen (center)
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    const int tileSize = 64; // Size of a single grid square

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // --- 3. Handle Input ---
        
        // WASD Movement (scaled by zoom so panning feels consistent at all zoom levels)
        float moveSpeed = 500.0f / camera.zoom * dt;
        if (IsKeyDown(KEY_W)) camera.target.y -= moveSpeed;
        if (IsKeyDown(KEY_S)) camera.target.y += moveSpeed;
        if (IsKeyDown(KEY_A)) camera.target.x -= moveSpeed;
        if (IsKeyDown(KEY_D)) camera.target.x += moveSpeed;

        if (IsKeyPressed(KEY_F)) ToggleFullscreen();

        // Mouse-centered scroll wheel zoom
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            // Get the world point that is under the mouse BEFORE zooming
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

            // Apply zoom
            const float zoomIncrement = 0.125f;
            camera.zoom += wheel * zoomIncrement;

            // Clamp zoom limits to prevent extreme zoom-in or zoom-out
            if (camera.zoom < 0.25f) camera.zoom = 0.25f;
            if (camera.zoom > 2.0f) camera.zoom = 2.0f;

            // Get the world point that is under the mouse AFTER zooming
            Vector2 mouseWorldPosNew = GetScreenToWorld2D(GetMousePosition(), camera);

            // Shift the camera target by the difference so the mouse stays over the exact same world spot
            camera.target.x += (mouseWorldPos.x - mouseWorldPosNew.x);
            camera.target.y += (mouseWorldPos.y - mouseWorldPosNew.y);
        }

        // --- 4. Draw Phase ---
        BeginDrawing();
        
        // Dark mode background
        ClearBackground((Color){ 20, 20, 20, 255 }); 

        // Start drawing in World Space (Camera applies translations and scaling)
        BeginMode2D(camera);

        // Calculate the visible area in world space
        Vector2 screenTopLeft = GetScreenToWorld2D((Vector2){ 0, 0 }, camera);
        Vector2 screenBottomRight = GetScreenToWorld2D((Vector2){ screenWidth, screenHeight }, camera);

        // Figure out the closest grid alignment to the left/top edges
        int startX = (int)(screenTopLeft.x / tileSize) - 1;
        int endX = (int)(screenBottomRight.x / tileSize) + 1;
        
        int startY = (int)(screenTopLeft.y / tileSize) - 1;
        int endY = (int)(screenBottomRight.y / tileSize) + 1;

        Color gridColor = (Color){ 45, 45, 45, 255 }; // Faint gray lines

        // Draw vertical grid lines
        for (int x = startX; x <= endX; x++) {
            DrawLine(x * tileSize, startY * tileSize, x * tileSize, endY * tileSize, gridColor);
        }
        // Draw horizontal grid lines
        for (int y = startY; y <= endY; y++) {
            DrawLine(startX * tileSize, y * tileSize, endX * tileSize, y * tileSize, gridColor);
        }

        // Draw a red square at the exact center of the world (0, 0) as an anchor point
        DrawRectangle(0, 0, tileSize, tileSize, RED);

        EndMode2D();
        // End of World Space

        // Start drawing in Screen Space (UI Layer)
        
        // Get coordinates for the UI
        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
        
        DrawText(TextFormat("Camera Target: (%.0f, %.0f)", camera.target.x, camera.target.y), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("Mouse World: (%.0f, %.0f)", mouseWorld.x, mouseWorld.y), 10, 35, 20, GRAY);
        DrawText(TextFormat("Zoom: %.2fx", camera.zoom), 10, 60, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();
}
