# Boids Simulation in C

A 2D flocking simulation developed in C using the raylib graphics library.

The project implements the Boids algorithm to simulate collective movement using three fundamental behaviors: separation, alignment, and cohesion.

A spatial grid with dynamic arrays is used to optimize neighbor searches.

## Features

- Real-time 2D Boids simulation
- Separation, alignment, and cohesion behaviors
- Velocity limiting
- Screen wrap-around
- Spatial grid optimization for neighbor detection
- Dynamic arrays for grid cells
- Automatic memory allocation and deallocation
- FPS counter

## Boids Algorithm

Each Boid follows three rules:

**1. Separation**

Avoids getting too close to neighboring Boids.

**2. Alignment**

Adjusts velocity to match the average velocity of nearby Boids.

**3. Cohesion**

Moves toward the average position of neighboring Boids.

Combining these behaviors produces flocking patterns.

## Spatial Grid Optimization

Instead of comparing every Boid with every other Boid, the simulation divides the screen into grid cells.

Each Boid is assigned to a cell based on its position. Neighbor searches examine only the current cell and surrounding cells.

This reduces unnecessary distance calculations.

## Dynamic Array Implementation

Each grid cell stores Boid indices using a dynamically allocated array.

- `malloc()` allocates the initial memory.
- `realloc()` increases capacity when necessary.
- `free()` releases allocated memory when the simulation closes.

The capacity doubles when a cell becomes full, avoiding the previous fixed limit on Boids per cell.

## Technologies

- C
- raylib
- Spatial Grid
- Dynamic Arrays
- Dynamic Memory Management

## How to Run

### Requirements

- C compiler (Clang or GCC)
- raylib
- pkg-config

### Installation on macOS

Install dependencies with Homebrew:

```bash
brew install raylib pkg-config
```

### Compile

```bash
clang Boids.c -o Boids $(pkg-config --cflags --libs raylib) -lm
```

### Run

```bash
./Boids
```

## Controls

- **ESC:** Close the simulation window.

## Learning Objectives

This project focuses on applying C programming concepts to a graphical simulation, including:

- Structures and pointers
- Dynamic memory allocation
- Arrays and spatial partitioning
- Performance optimization
- Real-time simulation loops
- Vector-based movement calculations