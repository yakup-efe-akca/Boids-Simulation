
#include "raylib.h"
#include <math.h>

#define NEIGHBOR_RADIUS 50
#define SEPARATION_RADIUS 25
#define SEPARATION_STRENGTH 100
#define ALIGNMENT_STRENGTH 1.0f
#define COHESION_STRENGTH 1.0f
#define MAX_SPEED 60.0f
#define BOID_COUNT 100

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
} Boid;

void wrapAround(Boid *b) {
    if (b->x < 0)
        b->x = 1000;
    else if (b->x > 1000)
        b->x = 0;

    if (b->y < 0)
        b->y = 700;
    else if (b->y > 700)
        b->y = 0;
}

void limitSpeed(Boid *b) {
    float speedSq = b->vx * b->vx + b->vy * b->vy;

    if (speedSq > MAX_SPEED * MAX_SPEED) {
        float speed = sqrtf(speedSq);
        b->vx = b->vx / speed * MAX_SPEED;
        b->vy = b->vy / speed * MAX_SPEED;
    }
}

void updateFlock(Boid boid[], int count, float dt) {
    Boid next[BOID_COUNT];

    for (int i = 0; i < count; i++) {
        next[i] = boid[i];

        float separationX = 0;
        float separationY = 0;
        float alignmentX = 0;
        float alignmentY = 0;
        int neighborCount = 0;
        float centerX = 0;
        float centerY = 0;

        for (int j = 0; j < count; j++) {
            if (i == j)
                continue;

            float dx = boid[j].x - boid[i].x;
            float dy = boid[j].y - boid[i].y;
            float distSq = dx * dx + dy * dy;

            if (distSq < NEIGHBOR_RADIUS * NEIGHBOR_RADIUS) {
                alignmentX += boid[j].vx;
                alignmentY += boid[j].vy;
                neighborCount++;
                centerX += boid[j].x;
                centerY += boid[j].y;
            }

            if (distSq < SEPARATION_RADIUS * SEPARATION_RADIUS && distSq > 0) {
                separationX -= dx / distSq;
                separationY -= dy / distSq;
            }
        }

        float cohesionX = 0;
        float cohesionY = 0;

        if (neighborCount > 0) {
            alignmentX /= neighborCount;
            alignmentY /= neighborCount;

            centerX /= neighborCount;
            centerY /= neighborCount;

            alignmentX -= boid[i].vx;
            alignmentY -= boid[i].vy;

            cohesionX = centerX - boid[i].x;
            cohesionY = centerY - boid[i].y;
        }

        next[i].vx += separationX * SEPARATION_STRENGTH * dt;
        next[i].vy += separationY * SEPARATION_STRENGTH * dt;

        next[i].vx += alignmentX * ALIGNMENT_STRENGTH * dt;
        next[i].vy += alignmentY * ALIGNMENT_STRENGTH * dt;

        next[i].vx += cohesionX * COHESION_STRENGTH * dt;
        next[i].vy += cohesionY * COHESION_STRENGTH * dt;

        limitSpeed(&next[i]);
        
        next[i].x += next[i].vx * dt;
        next[i].y += next[i].vy * dt;
        wrapAround(&next[i]);
    }

    for (int i = 0; i < count; i++) {
        boid[i] = next[i];
    }
}

int main(void) {
    InitWindow(1000, 700, "Boids");
    SetTargetFPS(60);

    Boid boid[BOID_COUNT];

    for (int i = 0; i < BOID_COUNT; i++) {
        boid[i].x = GetRandomValue(0, 1000);
        boid[i].y = GetRandomValue(0, 700);
        boid[i].vx = GetRandomValue(-100, 100);
        boid[i].vy = GetRandomValue(-100, 100);
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        updateFlock(boid, BOID_COUNT, dt);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawFPS(10, 10);
        for (int i = 0; i < BOID_COUNT; i++) {
            DrawCircle(boid[i].x, boid[i].y, 4, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
