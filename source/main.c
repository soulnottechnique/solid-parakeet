#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

enum error_code {BAD_WINDOW = 1, BAD_FONT};

#define WIN_HEIGHT 600
#define WIN_WIDTH 800
#define WIN_BPP 32
#define FPS 30U

#define PERSON_SPEED 4.0F
#define MOUSE_SCALE 100

sfVector2f person_delta(float);

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

    sfText *text = sfText_create();
    sfVector2f text_pos = {300.0, 50.0};
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 70);
    sfText_setPosition(text, text_pos);
    sfText_setString(text, "0");

    sfVector2f circ_pos = {200.0, 200.0};
    sfCircleShape *circle = sfCircleShape_create();
    sfCircleShape_setFillColor(circle, sfGreen);
    sfCircleShape_setRadius(circle, 5.0);

    sfEvent event;
    sfVector2f delta;
    float angle = 0;
    int mouse_pos = sfMouse_getPositionRenderWindow(window).x;
    while (sfRenderWindow_isOpen(window))
    {
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            if (event.type == sfEvtMouseMoved)
            {
                int new_pos = sfMouse_getPositionRenderWindow(window).x;
                angle -= (float) (mouse_pos - new_pos) / MOUSE_SCALE;
                mouse_pos = new_pos;
                char angle_str[10];
                snprintf(angle_str, 10, "%lf", angle);
                sfText_setString(text, angle_str);
            }
        }

        delta = person_delta(angle);
        circ_pos.x += delta.x;
        circ_pos.y += delta.y;
        sfCircleShape_setPosition(circle, circ_pos);

        sfRenderWindow_clear(window, sfBlack);

        sfRenderWindow_drawText(window, text, NULL);
        sfRenderWindow_drawCircleShape(window, circle, NULL);

        sfRenderWindow_display(window);
    }

    sfRenderWindow_destroy(window);

    return EXIT_SUCCESS;
}

sfVector2f person_delta(float angle)
{
    sfVector2f delta, delta_s;
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
        float SQRT_2 = sqrtf(2.0);
        delta.x /= SQRT_2; 
        delta.y /= SQRT_2;
    }

    delta_s.x = delta.x * cosf(angle) - sinf(angle) * delta.y;
    delta_s.y = delta.y * cosf(angle) + sinf(angle) * delta.x;

    return delta_s;
}