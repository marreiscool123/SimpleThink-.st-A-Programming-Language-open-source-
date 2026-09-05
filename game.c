#include "game.h"

#include <windows.h>

int st_game_init(STGame* game, int width, int height, int target_fps)
{
    if (game == NULL)
        return 0;

    if (width <= 0)
        width = 800;

    if (height <= 0)
        height = 600;

    if (target_fps <= 0)
        target_fps = 60;

    game->running = 0;
    game->width = width;
    game->height = height;
    game->target_fps = target_fps;
    game->delta_time = 0.0;
    game->frame_count = 0;

    return 1;
}

void st_game_start(STGame* game)
{
    if (game == NULL)
        return;

    game->running = 1;
}

void st_game_stop(STGame* game)
{
    if (game == NULL)
        return;

    game->running = 0;
}

int st_game_is_running(const STGame* game)
{
    if (game == NULL)
        return 0;

    return game->running;
}

void st_game_update(STGame* game)
{
    static ULONGLONG previous_time = 0;
    ULONGLONG current_time;
    double target_frame_time;

    if (game == NULL || !game->running)
        return;

    current_time = GetTickCount64();

    if (previous_time == 0)
    {
        previous_time = current_time;
        game->delta_time = 0.0;
    }
    else
    {
        game->delta_time =
            (double)(current_time - previous_time) / 1000.0;

        previous_time = current_time;
    }

    game->frame_count++;

    target_frame_time =
        1.0 / (double)game->target_fps;

    if (game->delta_time < target_frame_time)
    {
        DWORD sleep_time;

        sleep_time = (DWORD)(
            (target_frame_time - game->delta_time) * 1000.0
        );

        if (sleep_time > 0)
            Sleep(sleep_time);
    }
}

void st_game_set_fps(STGame* game, int fps)
{
    if (game == NULL)
        return;

    if (fps <= 0)
        fps = 60;

    game->target_fps = fps;
}

double st_game_delta_time(const STGame* game)
{
    if (game == NULL)
        return 0.0;

    return game->delta_time;
}

unsigned long long st_game_frame_count(const STGame* game)
{
    if (game == NULL)
        return 0;

    return game->frame_count;
}

void st_game_shutdown(STGame* game)
{
    if (game == NULL)
        return;

    game->running = 0;
    game->delta_time = 0.0;
    game->frame_count = 0;
}