#include "time.h"

#include <windows.h>

long long st_time_now(void)
{
    FILETIME file_time;
    ULARGE_INTEGER value;

    GetSystemTimeAsFileTime(&file_time);

    value.LowPart = file_time.dwLowDateTime;
    value.HighPart = file_time.dwHighDateTime;

    /*
     * Windows FILETIME:
     *   100-nanosecond intervals since January 1, 1601.
     *
     * Unix time:
     *   seconds since January 1, 1970.
     *
     * Difference:
     *   11644473600 seconds
     */
    return (long long)(
        (value.QuadPart / 10000000ULL) - 11644473600ULL
    );
}

void st_time_current(STTime* current)
{
    SYSTEMTIME system_time;

    if (current == NULL)
        return;

    GetLocalTime(&system_time);

    current->year = (int)system_time.wYear;
    current->month = (int)system_time.wMonth;
    current->day = (int)system_time.wDay;
    current->hour = (int)system_time.wHour;
    current->minute = (int)system_time.wMinute;
    current->second = (int)system_time.wSecond;
}

int st_time_year(void)
{
    STTime current;

    st_time_current(&current);

    return current.year;
}

int st_time_month(void)
{
    STTime current;

    st_time_current(&current);

    return current.month;
}

int st_time_day(void)
{
    STTime current;

    st_time_current(&current);

    return current.day;
}

int st_time_hour(void)
{
    STTime current;

    st_time_current(&current);

    return current.hour;
}

int st_time_minute(void)
{
    STTime current;

    st_time_current(&current);

    return current.minute;
}

int st_time_second(void)
{
    STTime current;

    st_time_current(&current);

    return current.second;
}

void st_time_sleep(unsigned int milliseconds)
{
    Sleep(milliseconds);
}

double st_time_seconds(void)
{
    static LARGE_INTEGER frequency;
    static int initialized = 0;
    LARGE_INTEGER counter;

    if (!initialized)
    {
        if (!QueryPerformanceFrequency(&frequency))
            return 0.0;

        initialized = 1;
    }

    if (!QueryPerformanceCounter(&counter))
        return 0.0;

    return (double)counter.QuadPart /
           (double)frequency.QuadPart;
}