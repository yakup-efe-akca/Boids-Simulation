# Boids Simulation in C

A 2D flocking simulation implemented in C using the raylib graphics library.

The project demonstrates how simple local behaviors can create complex flocking movement.

## Features

- Multiple boids with randomized positions and velocities
- Separation, Alignment, and Cohesion behaviors
- Speed limiting
- Screen wrapping
- Real-time visualization using raylib
- FPS monitoring

## Requirements

- C compiler (Clang or GCC)
- raylib
- pkg-config

## Building and Running

Compile the program:

```bash
clang Boids.c -o Boids $(pkg-config --cflags --libs raylib)
```

Run the simulation:

```bash
./Boids
```

## Algorithm

Each boid follows three basic rules:

- **Separation:** Avoid getting too close to neighboring boids.
- **Alignment:** Adjust velocity toward the average velocity of nearby boids.
- **Cohesion:** Move toward the average position of nearby boids.

The current implementation uses an O(n²) neighbor search.

## Future Improvements

- Spatial grid optimization
- Dynamic arrays for grid cells
- Performance comparison between different data structures