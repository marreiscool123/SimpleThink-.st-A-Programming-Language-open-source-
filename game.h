#ifndef ST_GAME_H
#define ST_GAME_H

/*
    Simple Think Game Library
*/

typedef struct STGame
{
    int running;
    int width;
    int height;
    int target_fps;
    double delta_time;
    unsigned long long frame_count;
} STGame;

int st_game_init(STGame* game, int width, int height, int target_fps);
void st_game_start(STGame* game);
void st_game_stop(STGame* game);

int st_game_is_running(const STGame* game);

void st_game_update(STGame* game);
void st_game_set_fps(STGame* game, int fps);

double st_game_delta_time(const STGame* game);
unsigned long long st_game_frame_count(const STGame* game);

void st_game_shutdown(STGame* game);

#endif