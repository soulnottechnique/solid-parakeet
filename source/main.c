#include <SFML/Graphics.h>
#include <stdlib.h>
#include <stdio.h>

#define BAD_WINDOW 1
#define BAD_FONT 2

#define WIN_HEIGHT 600
#define WIN_WIDTH 800
#define WIN_BPP 32

#define FPS 30U

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
    sfVector2f text_pos = {300.0, 250.0};
    sfText_setString(text, "Hi man..");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 70);
    sfText_setPosition(text, text_pos);

    sfEvent event;

    while (sfRenderWindow_isOpen(window))
    {
        while (sfRenderWindow_pollEvent(window, &event))
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawText(window, text, NULL);
        sfRenderWindow_display(window);
    }

    sfText_destroy(text);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);

    return EXIT_SUCCESS;
}
