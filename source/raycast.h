#ifndef RAYCAST_H
#define RAYCAST_H

#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define SQR(x) ((x) * (x))

#define EPS 0.1F

typedef struct 
{
    sfVector2f start, end;
} line; 


void line_init(line *lin, float x1, float y1, float x2, float y2);
sfVector2f intersect(line *ray, line lin[], size_t n, _Bool *hit);

#endif