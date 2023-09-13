#include"spoor_internal.h"

#include<stdio.h>
#include<string.h>

const char DEBUG_TYPES[][17] = {
    "TYPE_TASK",
    "TYPE_PROJECT",
    "TYPE_EVENT",
    "TYPE_APPOINTMENT",
    "TYPE_TARGET",
    "TYPE_HABIT"
};

const char DEBUG_STATUS[][19] = {
    "STATUS_NOT_STARTED",
    "STATUS_IN_PROGRESS",
    "STATUS_COMPLETED"
};

void spoor_time_hour_minute_format(char *format_buffer, struct tm *time)
{
    if (time->tm_hour == -1 || time->tm_min == -1)
        sprintf(format_buffer, "--:--");
    else
    {
        sprintf(format_buffer, "%s%d:%s%d",
                (time->tm_hour < 10) ?"0" :"", time->tm_hour,
                (time->tm_min < 10) ?"0" : "", time->tm_min);
    }
}

void spoor_time_year_format(char *format_buffer, int year)
{
    if (year == -1)
        sprintf(format_buffer, "----");
    else
    {
        sprintf(format_buffer, "%d", year + 1900);
    }
}

void spoor_time_mon_format(char *format_buffer, int mon)
{
    if (mon == - 1)
        sprintf(format_buffer, "--");
    else
    {
        sprintf(format_buffer, "%s%d",
                (mon < 10) ?"0" :"", mon + 1);
    }
}

void spoor_time_day_format(char *format_buffer, int day)
{
    if (day == -1)
        sprintf(format_buffer, "--");
    else
        sprintf(format_buffer, "%s%d",
                (day < 10) ?"" :"", day);
}

void debug_spoor_time_print(SpoorTime *spoor_time)
{
    char format_buffer_hour_min_start[6];
    char format_buffer_hour_min_end[6];
    char format_buffer_year_start[5];
    char format_buffer_year_end[5];
    char format_buffer_mon_start[3];
    char format_buffer_mon_end[3];
    char format_buffer_day_start[3];
    char format_buffer_day_end[3];

    spoor_time_hour_minute_format(format_buffer_hour_min_start, &spoor_time->start);
    spoor_time_hour_minute_format(format_buffer_hour_min_end, &spoor_time->end);

    spoor_time_year_format(format_buffer_year_start, spoor_time->start.tm_year);
    spoor_time_year_format(format_buffer_year_end, spoor_time->end.tm_year);

    spoor_time_mon_format(format_buffer_mon_start, spoor_time->start.tm_mon);
    spoor_time_mon_format(format_buffer_mon_end, spoor_time->end.tm_mon);

    spoor_time_day_format(format_buffer_day_start, spoor_time->start.tm_mday);
    spoor_time_day_format(format_buffer_day_end, spoor_time->end.tm_mday);

    printf("%s.%s.%s %s - %s.%s.%s %s\n",
            format_buffer_day_start,
            format_buffer_mon_start,
            format_buffer_year_start,
            format_buffer_hour_min_start,
            format_buffer_day_end,
            format_buffer_mon_end,
            format_buffer_year_end,
            format_buffer_hour_min_end);
}

void spoor_debug_spoor_object_print(SpoorObject *spoor_object)
{
    printf("[ SpoorObject --\n");
    printf("%-20s%d\n", "Key:", spoor_object->id);
    printf("%-20s%s\n", "Title:", spoor_object->title);
    printf("%-20s%s\n", "Status:", DEBUG_STATUS[spoor_object->status]);
    printf("%-20s", "Deadline: ");
    debug_spoor_time_print(&spoor_object->deadline);
    printf("%-20s", "Schedule: ");
    debug_spoor_time_print(&spoor_object->schedule);
    printf("%-20s", "Tracked: ");
    debug_spoor_time_print(&spoor_object->tracked);
    printf("%-20s", "Complete: ");
    debug_spoor_time_print(&spoor_object->complete);
    printf("%-20s", "Created: ");
    debug_spoor_time_print(&spoor_object->created);
    printf("%-20s%s\n","Status:", DEBUG_STATUS[spoor_object->status]);
    printf("%-20s%s\n", "Type:", DEBUG_TYPES[spoor_object->type]);
    printf("-- SpoorObject ]\n");
}
