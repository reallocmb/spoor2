#include"spoor_internal.h"

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>

/* Input [count]['d'][time] */
void spoor_time_date_create(char *argument, uint32_t argument_length, struct tm *date)
{
    char last_char = argument[argument_length];
    argument[argument_length] = 0;

    /* d900-5d1000 */
    uint32_t count = 0;
    char mode = 0;
    int32_t hour = 0;
    int32_t minute = 0;
    int8_t sign = 1;
    /* count */
    uint32_t i = 0;

    /* count minus */
    if (argument[0] == '-')
    {
        sign = -1;
        i++;
    }

    while (argument[i] >= 0x30 && argument[i] <= 0x39)
    {
        count *= 10;
        count += argument[i] - 0x30;
        i++;
    }

    /* mode */
    mode = argument[i];
    i++;

    /* end time */
    time_t current_time;
    current_time = time(NULL);

    if (mode == 'd')
    {
        current_time += sign * 60 * 60 * 24 * (int32_t)count;
        *date = *localtime(&current_time);
    }
    else
    {
        minute = count;
        hour = minute / 100;
        minute = minute % 100;

        date->tm_hour = hour;
        date->tm_min = minute;

        if (!(hour >= 0 && hour <= 23 && minute >= 0 && minute <= 60))
        {
            date->tm_hour = -1;
            date->tm_min = -1;
        }

        return;
    }

    /* hour & minute */
    if (argument[i] != 0)
    {
        while (argument[i] >= 0x30 && argument[i] <= 0x39)
        {
            minute *= 10;
            minute += argument[i] - 0x30;
            i++;
        }

        hour = minute / 100;
        minute = minute % 100;

        date->tm_hour = hour;
        date->tm_min = minute;
    }
    else
    {
        date->tm_hour = -1;
        date->tm_min = -1;
    }

    if (!(hour >= 0 && hour <= 23 && minute >= 0 && minute <= 60))
    {
        date->tm_hour = -1;
        date->tm_min = -1;
    }

    argument[argument_length] = last_char;
}

void spoor_time_create(char *argument, uint32_t argument_length, SpoorTime *spoor_time)
{
    char last_c = argument[argument_length];
    argument[argument_length] = 0;

    char *ptr = strchr(argument + 1, '-');
    if (ptr)
    {
        spoor_time_date_create(argument, ptr - argument, &spoor_time->end);
        spoor_time->start = spoor_time->end;
        spoor_time_date_create(argument + (ptr - argument + 1), argument_length - (ptr - argument + 1), &spoor_time->end);
    }
    else
    {
        memset(&spoor_time->start, -1, sizeof(spoor_time->start));
        spoor_time_date_create(argument, argument_length, &spoor_time->end);
    }

    argument[argument_length] = last_c;
}

uint32_t arguments_next(char **arguments, uint32_t arguments_last_length)
{
    *arguments += arguments_last_length;
    if ((*arguments)[0] == ' ')
        (*arguments)++;

    if ((*arguments)[0] == 0)
        return 0;

    uint32_t i;
    for (i = 0; (*arguments)[i] != ' '; i++)
    {
        if ((*arguments)[i] == 0)
            return i;
    }

    return i;
}


char *spoor_object_argv_to_command(int argc, char **argv)
{
    /* create title */
    if (argc < 1)
        return NULL;

    uint16_t i;
    for (i = 0; i < argc - 1; i++)
    {
        uint32_t argv_len = strlen(argv[i]);

        argv[i][argv_len] = ' ';
    }

    return argv[0];
}


SpoorObject *spoor_object_create(char *arguments)
{
    SpoorObject *spoor_object = malloc(sizeof(*spoor_object));
    assert(spoor_object != NULL);
    memset(spoor_object, -1, sizeof(*spoor_object));

    /* create title */
    uint16_t i;

    if (arguments[0] == ' ')
        arguments++;

    for (i = 0; arguments[i] != ',' && arguments[i] != 0; i++)
    {
        spoor_object->title[i] = arguments[i];
    }
    spoor_object->title[i] = 0;
    i++;

    arguments += i;

    /* create time arguments & type & status */
    uint32_t argument_lenght = 0;

    uint16_t status = STATUS_NOT_STARTED; 
    uint16_t type = TYPE_TASK;

    uint8_t time_argument_count = 0;
    
    while ((argument_lenght = arguments_next(&arguments, argument_lenght)) != 0)
    {
        if (strncmp(arguments, "t", 1) == 0)
            type = TYPE_TASK;
        else if (strncmp(arguments, "p", 1) == 0)
            type = TYPE_PROJECT;
        else if (strncmp(arguments, "e", 1) == 0)
            type = TYPE_EVENT;
        else if (strncmp(arguments, "a", 1) == 0)
            type = TYPE_APPOINTMENT;
        else if (strncmp(arguments, "g", 1) == 0)
            type = TYPE_GOAL;
        else if (strncmp(arguments, "h", 1) == 0)
            type = TYPE_HABIT;
        else if (strncmp(arguments, "c", 1) == 0)
            status = STATUS_COMPLETED;
        else if (strncmp(arguments, "ip", 2) == 0)
            status = STATUS_IN_PROGRESS;
        else if (strncmp(arguments, "ns", 2) == 0)
            status = STATUS_NOT_STARTED;
        else if (strncmp(arguments, "-", 1) == 0)
            time_argument_count++;
        else
        {
            if (time_argument_count < 2)
            {
                spoor_time_create(arguments, argument_lenght, &spoor_object->deadline + time_argument_count);
                time_argument_count++;
            }
        }
    }

    spoor_object->type = type;
    spoor_object->status = status;

    /* create id */
    spoor_object->id = 0;
    
    /* tracked time */

    /* created time */
    time_t current_time;
    current_time = time(NULL);
    spoor_object->created.start = *localtime(&current_time);

    /* parent */
    spoor_object->parent_title[0] = '-';
    spoor_object->parent_title[1] = 0;

    return spoor_object;
}

void spoor_object_edit(SpoorObject *spoor_object, char *arguments)
{
    /* create title */
    uint16_t i;

    if (arguments[0] == ' ')
        arguments++;

    char *ptr = strchr(arguments, ',');
    if (ptr)
    {
        for (i = 0; arguments[i] != ',' && arguments[i] != 0; i++)
        {
            spoor_object->title[i] = arguments[i];
        }
        spoor_object->title[i] = 0;
        i++;

        arguments += i;
    }

    /* create time arguments & type & status */
    uint32_t argument_length = 0;
    uint8_t time_argument_count = 0;
    
    while ((argument_length = arguments_next(&arguments, argument_length)) != 0)
    {
        if (strncmp(arguments, "t", argument_length) == 0)
            spoor_object->type = TYPE_TASK;
        else if (strncmp(arguments, "p", argument_length) == 0)
            spoor_object->type = TYPE_PROJECT;
        else if (strncmp(arguments, "e", argument_length) == 0)
            spoor_object->type = TYPE_EVENT;
        else if (strncmp(arguments, "a", argument_length) == 0)
            spoor_object->type = TYPE_APPOINTMENT;
        else if (strncmp(arguments, "g", argument_length) == 0)
            spoor_object->type = TYPE_GOAL;
        else if (strncmp(arguments, "h", argument_length) == 0)
            spoor_object->type = TYPE_HABIT;
        else if (strncmp(arguments, "c", argument_length) == 0)
            spoor_object->status = STATUS_COMPLETED;
        else if (strncmp(arguments, "ip", argument_length) == 0)
            spoor_object->status = STATUS_IN_PROGRESS;
        else if (strncmp(arguments, "ns", argument_length) == 0)
            spoor_object->status = STATUS_NOT_STARTED;
        else if (strncmp(arguments, "-1", argument_length) == 0)
        {
            memset(&spoor_object->deadline + time_argument_count, -1, sizeof(spoor_object->deadline));
            time_argument_count++;
        }
        else if (strncmp(arguments, "-", 1) == 0)
            time_argument_count++;
        else
        {
            if (time_argument_count < 2)
            {
                spoor_time_create(arguments, argument_length, &spoor_object->deadline + time_argument_count);
                time_argument_count++;
            }
        }
    }
}

#if 0
uint32_t spoor_object_size(void)
{
    return sizeof(SpoorObject);
}

void spoor_object_progress_change(SpoorObject *spoor_object, SpoorStatus status)
{
    spoor_object->status = status;
}

void spoor_object_deadline_set(SpoorObject *spoor_object, char *command)
{
    if (command[0] == ' ')
        command++;

    if (command[0] == 'r')
        spoor_object->deadline.start.tm_year = -1;
    else
        spoor_time_schedule_create(command, strlen(command), &spoor_object->deadline);
}

void spoor_object_schedule_set(SpoorObject *spoor_object, char *command)
{
    if (command[0] == ' ')
        command++;

    if (command[0] == 'r')
        spoor_object->schedule.start.tm_year = -1;
    else
        spoor_time_schedule_create(command, strlen(command), &spoor_object->schedule);
}
#endif
