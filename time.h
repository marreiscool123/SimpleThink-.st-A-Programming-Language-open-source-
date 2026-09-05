#ifndef ST_TIME_H
#define ST_TIME_H

/*
    Simple Think Time Library
*/

typedef struct STTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} STTime;

long long st_time_now(void);

void st_time_current(STTime* time);

int st_time_year(void);
int st_time_month(void);
int st_time_day(void);

int st_time_hour(void);
int st_time_minute(void);
int st_time_second(void);

void st_time_sleep(unsigned int milliseconds);

double st_time_seconds(void);

#endif