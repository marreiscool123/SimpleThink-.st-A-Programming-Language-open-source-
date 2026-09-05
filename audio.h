#ifndef ST_AUDIO_H
#define ST_AUDIO_H

/*
    Simple Think Audio Library
*/

int st_audio_init(void);
void st_audio_shutdown(void);

int st_audio_play(const char* filename);
int st_audio_play_loop(const char* filename);

void st_audio_stop(void);
void st_audio_pause(void);
void st_audio_resume(void);

int st_audio_is_playing(void);

void st_audio_set_volume(int volume);
int st_audio_get_volume(void);

#endif