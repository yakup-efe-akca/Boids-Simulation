
#include "raylib.h"
#include <math.h>

#define NEIGHBOR_RADIUS 50
#define SEPARATION_RADIUS 25
#define SEPARATION_STRENGTH 100
#define ALIGNMENT_STRENGTH 1.0f
#define COHESION_STRENGTH 1.0f
#define MAX_SPEED 60.0f
#define BOID_COUNT 3000

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700
#define CELL_SIZE 50
#define GRID_COLS ((SCREEN_WIDTH + CELL_SIZE - 1) / CELL_SIZE)
#define GRID_ROWS ((SCREEN_HEIGHT + CELL_SIZE - 1) / CELL_SIZE)

#define MAX_BOIDS_PER_CELL 20
typedef struct {
    int boidIndices[MAX_BOIDS_PER_CELL];
    int count;
} Cell;

typedef struct
{
    float x;
    float y;
    float vx;
    float vy;
} Boid;

void wrapAround(Boid *b) {
    if (b->x < 0)
        b->x = SCREEN_WIDTH;
    else if (b->x > SCREEN_WIDTH)
        b->x = 0;

    if (b->y < 0)
        b->y = SCREEN_HEIGHT;
    else if (b->y > SCREEN_HEIGHT)
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
    Cell grid[GRID_ROWS][GRID_COLS];
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            grid[i][j].count = 0;
        }
    }

        for (int i = 0; i < count; i++) {
            int col = (int)(boid[i].x / CELL_SIZE);
            int row = (int)(boid[i].y / CELL_SIZE);
            
            if (col >= GRID_COLS)
                col = GRID_COLS - 1;
            if (row >= GRID_ROWS)
                row = GRID_ROWS - 1;
                
            Cell *cell = &grid[row][col];
            if (cell->count < MAX_BOIDS_PER_CELL) {
                cell->boidIndices[cell->count] = i;
                cell->count++;
            }
        }
    
    for (int i = 0; i < count; i++) {
        next[i] = boid[i];
        
        float separationX = 0;
        float separationY = 0;
        float alignmentX = 0;
        float alignmentY = 0;
        int neighborCount = 0;
        float centerX = 0;
        float centerY = 0;
        
        int col = (int)(boid[i].x / CELL_SIZE);
        int row = (int)(boid[i].y / CELL_SIZE);
        if (col >= GRID_COLS)
            col = GRID_COLS - 1;
        if (row >= GRID_ROWS)
            row = GRID_ROWS - 1;
            
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int r = row + dr;
                int c = col + dc;
                if (r > GRID_ROWS - 1 || r < 0 || c > GRID_COLS - 1 || c < 0)
                    continue;
                      
                Cell *cell = &grid[r][c];
                
                for (int k = 0; k < cell->count; k++) {
                    int j = cell->boidIndices[k];
                
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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Boids");
    SetTargetFPS(60);

    Boid boid[BOID_COUNT];

    for (int i = 0; i < BOID_COUNT; i++) {
        boid[i].x = GetRandomValue(0, SCREEN_WIDTH);
        boid[i].y = GetRandomValue(0, SCREEN_HEIGHT);
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
