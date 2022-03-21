#include <stdbool.h>
#include "src\classes\Game.cpp"

int main()
{
    bool end = false;
    al_init();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_TIMER *timer2 = al_create_timer(3.0 / 10.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(1100, 950);
    ALLEGRO_FONT *font = al_create_builtin_font();

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_timer_event_source(timer2));

    bool redraw = true;
    ALLEGRO_EVENT event;
    Game new_game;

    al_start_timer(timer);
    al_start_timer(timer2);
    while (!end)
    {
        al_wait_for_event(queue, &event);

        switch (event.type)
        {
        case ALLEGRO_EVENT_TIMER:
            if (event.timer.source == timer2)
            {
                new_game.animateEntities();
            }
            else
            {
                new_game.moveEntities();
                redraw = true;
            }
            break;

        case ALLEGRO_EVENT_KEY_DOWN:
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                end = true;
                break;
            }
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            end = true;
            break;
        }

        new_game.playerMovement(event);

        new_game.collisionHandler();

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            new_game.drawEntities();
            al_flip_display();

            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}