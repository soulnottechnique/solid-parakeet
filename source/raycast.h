#ifndef RAYCAST_H
#define RAYCAST_H

#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define SQR(x) ((x) * (x))

#define EPS 1e-6

typedef struct 
{
    double x, y;
} vector2d;

typedef struct 
{
    vector2d start, end;
} line; 


void line_init(line *lin, double x1, double y1, double x2, double y2);
vector2d intersect(line *ray, line lin[], size_t n, _Bool *hit);

#endif