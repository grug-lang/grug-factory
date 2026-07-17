#include "raylib.h"
#include <stdlib.h> 
#include <math.h>

#include "grug.h"

typedef struct {
    int x; 
    int y; 
} Tile;

int main(void) {
    // --- 0. Grug Integration Test ---
    // Prove we can link against and call functions from the grug-rs static library
    grug_default_settings();

    // --- 1. Window Setup ---
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "grug-factory engine");

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // 2. Initialize the 2D Camera
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };
    camera.zoom = 1.0f;

    SetTargetFPS(60);
    const int tileSize = 64;

    // Tile management
    Tile* tiles = NULL;
    int tileCount = 0;
    int tileCapacity = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // --- 3. Handle Input ---
        float moveSpeed = 500.0f / camera.zoom * dt;
        if (IsKeyDown(KEY_W)) camera.target.y -= moveSpeed;
        if (IsKeyDown(KEY_S)) camera.target.y += moveSpeed;
        if (IsKeyDown(KEY_A)) camera.target.x -= moveSpeed;
        if (IsKeyDown(KEY_D)) camera.target.x += moveSpeed;
        if (IsKeyPressed(KEY_F)) ToggleFullscreen();

        // Zoom logic
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.zoom += wheel * 0.125f;
            if (camera.zoom < 0.25f) camera.zoom = 0.25f;
            if (camera.zoom > 2.0f) camera.zoom = 2.0f;
            Vector2 mouseWorldPosNew = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.target.x += (mouseWorldPos.x - mouseWorldPosNew.x);
            camera.target.y += (mouseWorldPos.y - mouseWorldPosNew.y);
        }

        // --- 4. Tile Placement/Removal ---
        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
        int gridX = (int)floorf(mouseWorld.x / tileSize);
        int gridY = (int)floorf(mouseWorld.y / tileSize);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            bool exists = false;
            for (int i = 0; i < tileCount; i++) {
                if (tiles[i].x == gridX && tiles[i].y == gridY) { exists = true; break; }
            }
            if (!exists) {
                if (tileCount == tileCapacity) {
                    tileCapacity = (tileCapacity == 0) ? 10 : tileCapacity * 2;
                    tiles = realloc(tiles, tileCapacity * sizeof(Tile));
                }
                tiles[tileCount++] = (Tile){ gridX, gridY };
            }
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            for (int i = 0; i < tileCount; i++) {
                if (tiles[i].x == gridX && tiles[i].y == gridY) {
                    tiles[i] = tiles[tileCount - 1];
                    tileCount--;
                    break;
                }
            }
        }

        // --- 5. Draw Phase ---
        BeginDrawing();
        ClearBackground((Color){ 20, 20, 20, 255 });

        BeginMode2D(camera);

        // Draw Grid
        Vector2 screenTopLeft = GetScreenToWorld2D((Vector2){ 0, 0 }, camera);
        Vector2 screenBottomRight = GetScreenToWorld2D((Vector2){ screenWidth, screenHeight }, camera);
        int startX = (int)floorf(screenTopLeft.x / tileSize) - 1;
        int endX = (int)floorf(screenBottomRight.x / tileSize) + 1;
        int startY = (int)floorf(screenTopLeft.y / tileSize) - 1;
        int endY = (int)floorf(screenBottomRight.y / tileSize) + 1;

        for (int x = startX; x <= endX; x++) DrawLine(x * tileSize, startY * tileSize, x * tileSize, endY * tileSize, (Color){ 45, 45, 45, 255 });
        for (int y = startY; y <= endY; y++) DrawLine(startX * tileSize, y * tileSize, endX * tileSize, y * tileSize, (Color){ 45, 45, 45, 255 });

        // Draw Tiles
        for (int i = 0; i < tileCount; i++) {
            DrawRectangle(tiles[i].x * tileSize, tiles[i].y * tileSize, tileSize, tileSize, RED);
        }

        EndMode2D();

        // UI Layer
        DrawText(TextFormat("Tile Coord: (%d, %d)", gridX, gridY), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("Total Tiles: %d", tileCount), 10, 35, 20, GRAY);
        DrawText(TextFormat("Zoom: %.2fx", camera.zoom), 10, 60, 20, GRAY);

        EndDrawing();
    }

    free(tiles);
    CloseWindow();
}
