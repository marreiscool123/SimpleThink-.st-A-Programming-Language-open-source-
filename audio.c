#include "audio.h"

#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

static int st_audio_initialized = 0;
static int st_audio_playing = 0;
static int st_audio_volume = 100;

int st_audio_init(void)
{
    st_audio_initialized = 1;
    st_audio_playing = 0;
    st_audio_volume = 100;

    return 1;
}

void st_audio_shutdown(void)
{
    if (!st_audio_initialized)
        return;

    st_audio_stop();

    st_audio_initialized = 0;
}

int st_audio_play(const char* filename)
{
    if (!st_audio_initialized || filename == NULL)
        return 0;

    if (!PlaySoundA(
        filename,
        NULL,
        SND_FILENAME | SND_ASYNC
    ))
    {
        return 0;
    }

    st_audio_playing = 1;

    return 1;
}

int st_audio_play_loop(const char* filename)
{
    if (!st_audio_initialized || filename == NULL)
        return 0;

    if (!PlaySoundA(
        filename,
        NULL,
        SND_FILENAME | SND_ASYNC | SND_LOOP
    ))
    {
        return 0;
    }

    st_audio_playing = 1;

    return 1;
}

void st_audio_stop(void)
{
    PlaySoundA(
        NULL,
        NULL,
        0
    );

    st_audio_playing = 0;
}

void st_audio_pause(void)
{
    /*
        The Windows PlaySound API does not provide
        reliable pause/resume controls for every
        audio format, so this currently stops playback.
    */
    st_audio_stop();
}

void st_audio_resume(void)
{
    /*
        Resume requires the previously played filename,
        which can be added to the audio system later.
    */
}

int st_audio_is_playing(void)
{
    return st_audio_playing;
}

void st_audio_set_volume(int volume)
{
    if (volume < 0)
        volume = 0;

    if (volume > 100)
        volume = 100;

    st_audio_volume = volume;
}

int st_audio_get_volume(void)
{
    return st_audio_volume;
}