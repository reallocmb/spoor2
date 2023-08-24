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

void debug_spoor_time_print(SpoorTime *spoor_time)
{
    char format_buffer_start[6];
    char format_buffer_end[6];
    char format_buffer_year[5];

    spoor_time_hour_minute_format(format_buffer_start, &spoor_time->start);
    spoor_time_hour_minute_format(format_buffer_end, &spoor_time->end);

    spoor_time_year_format(format_buffer_year, spoor_time->start.tm_year);

    printf("%s%d.%s%d.%s %s - %s%d.%s%d.%d %s\n",
            (spoor_time->start.tm_mday < 10) ?"0" :"",
            spoor_time->start.tm_mday,
            (spoor_time->start.tm_mon + 1 < 10) ?"0" :"",
            spoor_time->start.tm_mon + 1,
            format_buffer_year,
            format_buffer_start,
            (spoor_time->end.tm_mday < 10) ?"0" :"",
            spoor_time->end.tm_mday,
            (spoor_time->end.tm_mon + 1 < 10) ?"0" :"",
            spoor_time->end.tm_mon + 1,
            spoor_time->end.tm_year + 1900,
            format_buffer_end);
}

void spoor_debug_spoor_object_print(SpoorObject *spoor_object)
{
    printf("[ SpoorObject --\n");
    printf("%-20s%d\n", "Key:", spoor_object->id);
    printf("%-20s%s\n", "Title:", spoor_object->title);
    printf("%-20s%i\n", "Status:", spoor_object->status);
    printf("%-20s", "Deadline: ");
    debug_spoor_time_print(&spoor_object->deadline);
    printf("start: ");
    printf("%-5i", spoor_object->deadline.start.tm_year); 
    printf("%-5i", spoor_object->deadline.start.tm_mon); 
    printf("%-5i", spoor_object->deadline.start.tm_mday); 
    printf("%-5i", spoor_object->deadline.start.tm_hour); 
    printf("%-5i", spoor_object->deadline.start.tm_min); 
    printf("end: ");
    printf("%-5i", spoor_object->deadline.end.tm_year); 
    printf("%-5i", spoor_object->deadline.end.tm_mon); 
    printf("%-5i", spoor_object->deadline.end.tm_mday); 
    printf("%-5i", spoor_object->deadline.end.tm_hour); 
    printf("%i\n", spoor_object->deadline.end.tm_min); 
    printf("%-20s", "Schedule: ");
    debug_spoor_time_print(&spoor_object->schedule);
    printf("%-5i", spoor_object->schedule.start.tm_year); 
    printf("%-5i", spoor_object->schedule.start.tm_mon); 
    printf("%-5i", spoor_object->schedule.start.tm_mday); 
    printf("%-5i", spoor_object->schedule.start.tm_hour); 
    printf("%-5i", spoor_object->schedule.start.tm_min); 
    printf("end: ");
    printf("%-5i", spoor_object->schedule.end.tm_year); 
    printf("%-5i", spoor_object->schedule.end.tm_mon); 
    printf("%-5i", spoor_object->schedule.end.tm_mday); 
    printf("%-5i", spoor_object->schedule.end.tm_hour); 
    printf("%i\n", spoor_object->schedule.end.tm_min); 
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
