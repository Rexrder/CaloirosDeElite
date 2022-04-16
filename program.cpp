#include <stdbool.h>
#include "src\classes\Game.cpp"

int main()
{
    FreeConsole();
    bool end = false;
    al_init();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_acodec_addon();
    al_install_keyboard();
    al_install_audio();

    ALLEGRO_PATH *path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/logo.png");
    ALLEGRO_BITMAP *logo = al_load_bitmap(al_path_cstr(path, '/'));

    path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
    al_set_path_filename(path, "/res/sound/music/background.ogg");
    ALLEGRO_AUDIO_STREAM *b_music = al_load_audio_stream(al_path_cstr(path, '/'), 4, 1024);
    al_set_audio_stream_gain(b_music, 0.5);
    al_set_audio_stream_playmode(b_music, ALLEGRO_PLAYMODE_LOOP);

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30.0);
    ALLEGRO_TIMER *timer2 = al_create_timer(3.0 / 10.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_DISPLAY *disp = al_create_display(1200, 950);
    al_set_window_title(disp, "Caloiros De Elite");
    al_set_display_icon(disp, logo);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_timer_event_source(timer2));
    al_reserve_samples(6);

    bool redraw = true;
    ALLEGRO_EVENT event;
    srand(time(0));
    Game new_game(4);

    
    al_attach_audio_stream_to_mixer(b_music, al_get_default_mixer());
    al_set_audio_stream_playing(b_music, true);
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
                new_game.collisionHandler();
                end = new_game.moveEntities();
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

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            new_game.drawEntities();
            al_flip_display();

            redraw = false;
        }
    }
    new_game.~Game();

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
     al_destroy_audio_stream(b_music);

    return 0;
}