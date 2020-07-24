#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "raycast.h"

enum error_code {BAD_WINDOW = 1, BAD_FONT};

#define PI 3.1415926

#define WIN_HEIGHT 600
#define WIN_WIDTH 800
#define WIN_BPP 32
#define FPS 60U

#define PERSON_SPEED 4.0F
#define VIEWING_ANGLE 2.0 * PI / 3.0
#define MOUSE_SCALE 100.0

#define WALLS (5 + 4)
#define RAYS 100

vector2d person_delta(double angle);
void intersection_cr(sfRenderWindow *window, sfVector2f pos, line wall[]);

int main(void)
{
    sfVideoMode video_mode = {WIN_WIDTH, WIN_HEIGHT, WIN_BPP};
    sfRenderWindow *window = sfRenderWindow_create(video_mode, "Nice", sfClose, NULL);
    if (!window)
        return BAD_WINDOW;

    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);
    sfRenderWindow_setFramerateLimit(window, FPS);

    sfFont *font = sfFont_createFromFile("font.ttf");
    if (!font)
        return BAD_FONT;

    sfVector2f circ_pos = {200.0, 200.0};
    sfCircleShape *circle = sfCircleShape_create();
    sfCircleShape_setFillColor(circle, sfGreen);
    sfCircleShape_setRadius(circle, 5.0);

    time_t t;
    srand((unsigned) time(&t));

    line wall[WALLS];
    sfVertex vert_wall[WALLS][2];
    for (size_t i = 0; i < WALLS - 4; i++)
    {
        vert_wall[i][0].position.x = (float) (wall[i].start.x = rand() % WIN_WIDTH);
        vert_wall[i][0].position.y = (float) (wall[i].start.y = rand() % WIN_HEIGHT);
        vert_wall[i][1].position.x = (float) (wall[i].end.x = rand() % WIN_WIDTH);
        vert_wall[i][1].position.y = (float) (wall[i].end.y = rand() % WIN_HEIGHT);
        vert_wall[i][0].color = vert_wall[i][1].color = sfRed;
    }

    vert_wall[WALLS - 4][0].position.x = (float) (wall[WALLS - 4].start.x = 0);
    vert_wall[WALLS - 4][0].position.y = (float) (wall[WALLS - 4].start.y = 0);
    vert_wall[WALLS - 4][1].position.x = (float) (wall[WALLS - 4].end.x = 0);
    vert_wall[WALLS - 4][1].position.y = (float) (wall[WALLS - 4].end.y = WIN_HEIGHT);

    vert_wall[WALLS - 3][0].position.x = (float) (wall[WALLS - 3].start.x = 0);
    vert_wall[WALLS - 3][0].position.y = (float) (wall[WALLS - 3].start.y = WIN_HEIGHT);
    vert_wall[WALLS - 3][1].position.x = (float) (wall[WALLS - 3].end.x = WIN_WIDTH);
    vert_wall[WALLS - 3][1].position.y = (float) (wall[WALLS - 3].end.y = WIN_HEIGHT);

    vert_wall[WALLS - 2][0].position.x = (float) (wall[WALLS - 2].start.x = WIN_WIDTH);
    vert_wall[WALLS - 2][0].position.y = (float) (wall[WALLS - 2].start.y = 0);
    vert_wall[WALLS - 2][1].position.x = (float) (wall[WALLS - 2].end.x = WIN_WIDTH);
    vert_wall[WALLS - 2][1].position.y = (float) (wall[WALLS - 2].end.y = WIN_HEIGHT);

    vert_wall[WALLS - 1][0].position.x = (float) (wall[WALLS - 1].start.x = 0);
    vert_wall[WALLS - 1][0].position.y = (float) (wall[WALLS - 1].start.y = 0);
    vert_wall[WALLS - 1][1].position.x = (float) (wall[WALLS - 1].end.x = WIN_WIDTH);
    vert_wall[WALLS - 1][1].position.y = (float) (wall[WALLS - 1].end.y = 0);

    // найс говнокод, скоро заменим

    sfEvent event;
    vector2d delta;
    double angle = 0;
    int mouse_pos = sfMouse_getPositionRenderWindow(window).x;
    while (sfRenderWindow_isOpen(window))
    {
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            if (event.type == sfEvtMouseMoved)
            {
                int new_mouse_pos = sfMouse_getPositionRenderWindow(window).x;
                angle -= (double) (mouse_pos - new_mouse_pos) / MOUSE_SCALE;
                mouse_pos = new_mouse_pos;
            }
        }
        delta = person_delta(angle);
        circ_pos.x += (float) delta.x;
        circ_pos.y += (float) delta.y;
        sfCircleShape_setPosition(circle, circ_pos);
        sfRenderWindow_clear(window, sfBlack);

        intersection_cr(window, circ_pos, wall);

        for (size_t i = 0; i < WALLS; i++)
            sfRenderWindow_drawPrimitives(window, vert_wall[i], 2, sfLines, NULL);

        sfRenderWindow_drawCircleShape(window, circle, NULL);

        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);

    return EXIT_SUCCESS;
}

vector2d person_delta(double angle)
{
    vector2d delta, delta_s;
    delta.x = delta.y = 0;
    if (sfKeyboard_isKeyPressed(sfKeyW))
        delta.y -= PERSON_SPEED;
    if (sfKeyboard_isKeyPressed(sfKeyS))
        delta.y += PERSON_SPEED;
    if (sfKeyboard_isKeyPressed(sfKeyA))
        delta.x -= PERSON_SPEED;
    if (sfKeyboard_isKeyPressed(sfKeyD))
        delta.x += PERSON_SPEED;

    if (delta.x && delta.y)
    {
        double SQRT_2 = sqrtf(2.0);
        delta.x /= SQRT_2; 
        delta.y /= SQRT_2;
    }

    delta_s.x = delta.x * cos(angle) - sin(angle) * delta.y;
    delta_s.y = delta.y * cos(angle) + sin(angle) * delta.x;

    return delta_s;
}

void intersection_cr(sfRenderWindow *window, sfVector2f pos, line wall[])
{
    sfVector2f look_for;
    vector2d sect_dot;
    line ray;
    sfVertex cast_line[2];
    _Bool hit;
    for (size_t i = 0; i < RAYS; i++)
    {
        double alpha = 2 * PI / RAYS * i;
        hit = false;
        look_for.x = pos.x + (float) cos(alpha) * 10;
        look_for.y = pos.y + (float) sin(alpha) * 10;
        line_init(&ray, pos.x, pos.y, look_for.x, look_for.y);
        sect_dot = intersect(&ray, wall, WALLS, &hit);
        if (hit)
        {
            cast_line[0].position = pos;
            cast_line[1].position.x = (float) sect_dot.x;
            cast_line[1].position.y = (float) sect_dot.y;
            cast_line[0].color = cast_line[1].color = sfBlue;
            sfRenderWindow_drawPrimitives(window, cast_line, 2, sfLines, NULL);
        }
    }
}
