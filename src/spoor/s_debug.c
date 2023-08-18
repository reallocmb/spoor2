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

void spoor_time_hour_minute_format(char *format_buffer, SpoorTime *spoor_time)
{
    char format_buffer_start[6];
    char format_buffer_end[6];

    sprintf(format_buffer_start, "%s%d:%s%d",
            (spoor_time->start.tm_hour < 10) ?"0" :"", spoor_time->start.tm_hour,
            (spoor_time->start.tm_min < 10) ?"0" : "", spoor_time->start.tm_min);

    if (spoor_time->end.tm_hour == -1 || spoor_time->end.tm_min == -1)
        sprintf(format_buffer_end, "--:--");
    else
    {
        sprintf(format_buffer_end, "%s%d:%s%d",
                (spoor_time->end.tm_hour < 10) ?"0" :"", spoor_time->end.tm_hour,
                (spoor_time->end.tm_min < 10) ?"0" : "", spoor_time->end.tm_min);
    }

    sprintf(format_buffer, "%s - %s", format_buffer_start, format_buffer_end);
}

void debug_spoor_time_print(SpoorTime *spoor_time)
{
    if (spoor_time == NULL)
        return;
    char time_format_buffer[14];
    spoor_time_hour_minute_format(time_format_buffer, spoor_time);

    printf("%s%d.%s%d.%d - %s\n",
            (spoor_time->start.tm_mday < 10) ?"0" :"",
            spoor_time->start.tm_mday,
            (spoor_time->start.tm_mon + 1 < 10) ?"0" :"",
            spoor_time->start.tm_mon + 1,
            spoor_time->start.tm_year + 1900,
            time_format_buffer);
}

void spoor_debug_spoor_object_print(SpoorObject *spoor_object)
{
    printf("[ SpoorObject --\n");
    printf("%-20s%d\n", "Key:", spoor_object->id);
    printf("%-20s%s\n", "Title:", spoor_object->title);
    printf("%-20s%i\n", "Status:", spoor_object->status);
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
