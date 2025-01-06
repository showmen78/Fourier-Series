#include "raylib.h"
#include <vector>
#include <cmath>
#include <string>
#include <raymath.h>

// Screen constants
const int screenWidth = 1000;
const int screenHeight = 600;

// Point structure to store (x, y) positions
struct Point {
    float x, y;
};

int main() {
    // Initialize Raylib window
    InitWindow(screenWidth, screenHeight, "Fourier Transform in C++");
    SetTargetFPS(60);

    std::vector<Point> wave; // Store the points of the wave
    float time = 0.0f;       // Time variable for animation
    int numCircles = 5;      // Initial number of circles (can be changed with slider)

    // Slider properties
    Rectangle slider = { 800, 50, 150, 20 }; // Slider rectangle
    bool isDragging = false;                 // Dragging state
    int maxCircles = 20;                     // Maximum number of circles

    while (!WindowShouldClose()) {
        // Update slider input
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, slider)) {
            isDragging = true;
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            isDragging = false;
        }

        if (isDragging) {
            float normalized = (mouse.x - slider.x) / slider.width; // Map mouse position to [0, 1]
            numCircles = (int)(normalized * maxCircles);            // Map normalized to circle count
            numCircles = Clamp(numCircles, 1, maxCircles);         // Clamp to valid range
        }

        // Update the simulation
        float x = 150;  // Starting x position for circles
        float y = screenHeight / 2; // Starting y position for circles

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw the slider
        DrawRectangleRec(slider, GRAY);
        DrawRectangle(slider.x + (numCircles / (float)maxCircles) * slider.width - 5, slider.y - 5, 10, 30, WHITE);
        DrawText(TextFormat("Circles: %d", numCircles), slider.x, slider.y - 30, 20, YELLOW);

        // Calculate Fourier series and draw circles
        for (int i = 0; i < numCircles; i++) {
            int n = 2 * i + 1;  // Odd harmonics for Fourier series
            float radius = 75 * (4 / (n * PI));
            float prevX = x;
            float prevY = y;

            // Update position
            x += radius * cos(n * time);
            y += radius * sin(n * time);

            // Draw the circles and connecting lines
            DrawCircleLines(prevX, prevY, radius, GREEN);
            DrawLine(prevX, prevY, x, y, WHITE);
        }

        // Store the latest point in the wave
        wave.insert(wave.begin(), { x, y });
        if (wave.size() > 500) wave.pop_back(); // Limit wave size

        // Draw the wave
        for (size_t i = 0; i < wave.size() - 1; i++) {
            DrawLine(wave[i].x + 400, wave[i].y, wave[i + 1].x + 400, wave[i + 1].y, BLUE);
        }

        // Draw connecting line from the last circle to the wave
        DrawLine(x, y, wave[0].x + 400, wave[0].y, RED);

        // Update time
        time += 0.03;

        EndDrawing();
    }

    CloseWindow(); // Close the Raylib window
    return 0;
}
