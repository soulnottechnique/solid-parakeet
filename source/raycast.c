#include "raycast.h"

void line_init(line *lin, float x1, float y1, float x2, float y2)
{
    lin->start.x = x1;
    lin->start.y = y1;
    lin->end.x = x2;
    lin->end.y = y2;
}

float y_by_x(line *lin, float x)
{
    return (lin->start.y - lin->end.y) / (lin->start.x - lin->end.x) * (x - lin->start.x) + lin->start.y;
}

void swap(float *a, float *b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

float sqrdist(sfVector2f a, sfVector2f b)
{
    return SQR(a.x - b.x) + SQR(a.y - b.y);
}

_Bool boundcheck(line *ray, line *lin, sfVector2f dot)
{
    float xmin, xmax, ymin, ymax, se, ds, de;
    xmin = lin->start.x;
    xmax = lin->end.x;
    ymin = lin->start.y;
    ymax = lin->end.y;
    se = sqrdist(ray->start, ray->end);
    ds = sqrdist(ray->start, dot);
    de = sqrdist(dot, ray->end);
    if (xmax < xmin)
        swap(&xmax, &xmin);
    if (ymax < ymin)
        swap(&ymax, &ymin);
    if (dot.x > xmin - EPS && dot.x < xmax + EPS && dot.y > ymin - EPS && dot.y < ymax + EPS)
        if (de >= se && ds < de)
            return true;
    return false;
}

sfVector2f intersect(line *ray, line lin[], size_t n, _Bool *hit)
{
    sfVector2f sect, close_sect;
    _Bool current_hit, uninit = true;
    *hit = false;
    for (size_t i = 0; i < n; i++)
    {
        current_hit = false;
        float kquot = (ray->start.x - ray->end.x) * (lin[i].start.y - lin[i].end.y);
        kquot -= (ray->start.y - ray->end.y) * (lin[i].start.x - lin[i].end.x);
        if (fabs(kquot) > EPS)
        { 
            sect.x = (ray->start.x * ray->end.y - ray->start.y * ray->end.x) * (lin[i].start.x - lin[i].end.x);
            sect.x -= (ray->start.x - ray->end.x) * (lin[i].start.x * lin[i].end.y - lin[i].start.y * lin[i].end.x);
            sect.x /= kquot;
            if (fabs(ray->start.x - ray->end.x) < EPS)
                sect.y = y_by_x(lin + i, sect.x);
            else 
                sect.y = y_by_x(ray, sect.x);
            if (boundcheck(ray, lin + i, sect))
            {
                *hit = current_hit = true;
                if (uninit || sqrdist(ray->start, sect) < sqrdist(ray->start, close_sect))
                {
                    uninit = false;
                    close_sect = sect;
                }
            }
        }
    }
    return close_sect;
}