#include"spoor_internal.h"

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<time.h>

void spoor_time_deadline_create(char *argument, uint32_t argument_length, SpoorTime *spoor_time)
{
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
        current_time += count * 60 * 60 * 24 * sign;

    spoor_time->end = *localtime(&current_time);

    /* hour & minute */
    if (argument[i] == 0)
    {
        spoor_time->end.tm_hour = -1;
        spoor_time->end.tm_min = -1;
    }
    else if (argument[i] == '-')
    {
        i++;
        count = 0;
        while (argument[i] >= 0x30 && argument[i] <= 0x39)
        {
            count *= 10;
            count += argument[i] - 0x30;
            i++;
        }

        mode = argument[i];
        i++;
        /* todo */
    }
    else
    {
        while (argument[i] >= 0x30 && argument[i] <= 0x39)
        {
            minute *= 10;
            minute += argument[i] - 0x30;
            i++;
        }

        hour = minute / 100;
        minute = minute % 100;

        spoor_time->end.tm_hour = hour;
        spoor_time->end.tm_min = minute;
    }

}

void spoor_time_deadline_create_old(char *argument, uint32_t argument_length, SpoorTime *spoor_time)
{
    uint32_t count = 0;
    char mode = 0;
    int32_t hour = 0;
    int32_t minute = 0;
    int32_t sign = 1;

    /* count */
    uint32_t i = 0;

    if (argument[0] == '-')
    {
        sign = -1;
        i++;
    }

    for (; i < argument_length; i++)
    {
        if (argument[i] >= 0x30 && argument[i] <= 0x39)
        {
            count *= 10;
            count += argument[i] - 0x30;
        }
        else
            break;
    }

    /* mode */
    mode = argument[i];
    i++;

    /* time */
    time_t current_time;
    current_time = time(NULL);

    if (mode == 'd')
        current_time += count * 60 * 60 * 24 * sign;

    if (mode == 'w')
        current_time += count * 60 * 60 * 24 * 7 * sign;

    spoor_time->end = *localtime(&current_time);
    if (mode == 'm')
    {
        spoor_time->end.tm_mon += count * sign;
        if (spoor_time->end.tm_mon <= -1)
        {
            spoor_time->end.tm_year -= (spoor_time->end.tm_mon * -1) /  12;
            spoor_time->end.tm_mon -= (spoor_time->end.tm_mon * -1) % 12;
        }

        if (spoor_time->end.tm_mon >= 12)
        {
            spoor_time->end.tm_year += spoor_time->end.tm_mon / 12;
            spoor_time->end.tm_mon += spoor_time->end.tm_mon % 12;
        }
    }

    /* hour minute */
    if (i < argument_length && argument[i] != '-')
    {
        for (; i < argument_length; i++)
        {
            if (argument[i] >= 0x30 && argument[i] <= 0x39)
            {
                minute *= 10;
                minute += argument[i] - 0x30;
            }
            else
                break;
        }

        hour = minute / 100;
        minute = minute % 100;
    }
    else
    {
        hour = -1;
        minute = -1;
    }

    spoor_time->end.tm_hour = hour;
    spoor_time->end.tm_min = minute;

    spoor_time->start = spoor_time->end;
    /* spoor time end */
    if (argument[i] == '-')
    {
        i++;
        /* count */
        if (argument[i] == '-')
        {
            sign = -1;
            i++;
        }
        else
            sign = 1;

        for (; i < argument_length; i++)
        {
            if (argument[i] >= 0x30 && argument[i] <= 0x39)
            {
                count *= 10;
                count += argument[i] - 0x30;
            }
            else
                break;
        }

        /* mode */
        mode = argument[i];
        i++;

        /* time */
        current_time = time(NULL);

        if (mode == 'd')
            current_time += count * 60 * 60 * 24 * sign;

        if (mode == 'w')
            current_time += count * 60 * 60 * 24 * 7 * sign;

        spoor_time->end = *localtime(&current_time);
        if (mode == 'm')
        {
            spoor_time->end.tm_mon += count * sign;
            if (spoor_time->end.tm_mon <= -1)
            {
                spoor_time->end.tm_year -= (spoor_time->end.tm_mon * -1) /  12;
                spoor_time->end.tm_mon -= (spoor_time->end.tm_mon * -1) % 12;
            }

            if (spoor_time->end.tm_mon >= 12)
            {
                spoor_time->end.tm_year += spoor_time->end.tm_mon / 12;
                spoor_time->end.tm_mon += spoor_time->end.tm_mon % 12;
            }
        }


        if (i < argument_length)
        {
            minute = 0;
            for (; i < argument_length; i++)
            {
                if (argument[i] >= 0x30 && argument[i] <= 0x39)
                {
                    minute *= 10;
                    minute += argument[i] - 0x30;
                }
                else
                    break;
            }
            hour = minute / 100;
            minute = minute % 100;
        }
        else
        {
            hour = -1;
            minute = -1;
        }

        spoor_time->end.tm_hour = hour;
        spoor_time->end.tm_min = minute;
    }
}

void spoor_time_schedule_create(char *argument, uint32_t argument_length, SpoorTime *spoor_time)
{
    uint32_t count = 0;
    char mode = 0;
    int32_t hour = 0;
    int32_t minute = 0;
    int32_t sign = 1;

    /* count */
    uint32_t i = 0;

    if (argument[0] == '-')
    {
        sign = -1;
        i++;
    }

    for (; i < argument_length; i++)
    {
        if (argument[i] >= 0x30 && argument[i] <= 0x39)
        {
            count *= 10;
            count += argument[i] - 0x30;
        }
        else
            break;
    }

    /* mode */
    mode = argument[i];
    i++;

    /* time */
    time_t current_time;
    current_time = time(NULL);

    if (mode == 'd')
        current_time += count * 60 * 60 * 24 * sign;

    if (mode == 'w')
        current_time += count * 60 * 60 * 24 * 7 * sign;

    spoor_time->start = *localtime(&current_time);
    if (mode == 'm')
    {
        spoor_time->start.tm_mon += count * sign;
        if (spoor_time->start.tm_mon <= -1)
        {
            spoor_time->start.tm_year -= (spoor_time->start.tm_mon * -1) /  12;
            spoor_time->start.tm_mon -= (spoor_time->start.tm_mon * -1) % 12;
        }

        if (spoor_time->start.tm_mon >= 12)
        {
            spoor_time->start.tm_year += spoor_time->start.tm_mon / 12;
            spoor_time->start.tm_mon += spoor_time->start.tm_mon % 12;
        }
    }

    /* hour minute */
    for (; i < argument_length; i++)
    {
        if (argument[i] >= 0x30 && argument[i] <= 0x39)
        {
            minute *= 10;
            minute += argument[i] - 0x30;
        }
        else
            break;
    }

    hour = minute / 100;
    minute = minute % 100;

    spoor_time->start.tm_hour = hour;
    spoor_time->start.tm_min = minute;

    /* spoor time end */
    if (argument[i] == '-')
    {
        hour = 0;
        minute = 0;

        i++;
        for (; i < argument_length; i++)
        {
            if (argument[i] >= 0x30 && argument[i] <= 0x39)
            {
                minute *= 10;
                minute += argument[i] - 0x30;
            }
            else
                break;
        }

        hour = minute / 100;
        minute = minute % 100;
    }
    else
    {
        hour = -1;
        minute = -1;
        spoor_time->end.tm_year = -1;
        spoor_time->end.tm_mon = -1;
        spoor_time->end.tm_mday = -1;
    }

    spoor_time->end = spoor_time->start;
    spoor_time->end.tm_hour = hour;
    spoor_time->end.tm_min = minute;
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

SpoorObject *spoor_object_create(char *arguments)
{
    SpoorObject *spoor_object = malloc(sizeof(*spoor_object));
    assert(spoor_object != NULL);
    memset(spoor_object, 0, sizeof(*spoor_object));

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
    uint16_t type = TYPE_EVENT;

    spoor_object->deadline.start.tm_year = -1;
    spoor_object->schedule.start.tm_year = -1;

    uint8_t deadline_schedule_switch = 0;
    void (*spoor_time_func)(char *argument, uint32_t argument_length, SpoorTime *spoor_time) = spoor_time_deadline_create;
    
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
        else
        {
            spoor_time_func(arguments, argument_lenght, &spoor_object->deadline + deadline_schedule_switch * sizeof(spoor_object->deadline));
            deadline_schedule_switch++;
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

#if 0
SpoorObject *spoor_arguments_parse(uint16_t argc, char **argv)
{
    SpoorObject *spoor_object = malloc(sizeof(*spoor_object));
    assert(spoor_object != NULL);
    memset(spoor_object, 0, sizeof(*spoor_object));

    /* create title */
    char *title;
    if (argc < 1)
        return spoor_object;

    title = argv[0];
    uint16_t i;
    for (i = 0; i < argc - 1; i++)
    {
        uint32_t argv_len = strlen(argv[i]);

        /* check argv is not title */
        if (argv[i][argv_len - 1] == ',')
        {
            argv[i][argv_len - 1] = 0;
            i++;
            break;
        }

        argv[i][argv_len] = ' ';
    }

    memcpy(spoor_object->title, title, strlen(title));
    spoor_object->title[strlen(title)] = 0;

    /* create time arguments */
    if (i < argc && argc != 1)
        spoortime_parse(argv[i], strlen(argv[i]), &spoor_object->deadline);
    else
        spoor_object->deadline.start.tm_year = -1;
    i++;
    if (i < argc)
        spoortime_parse(argv[i], strlen(argv[i]), &spoor_object->schedule);
    else
        spoor_object->schedule.start.tm_year = -1;


    spoor_object->type = TYPE_EVENT;
    spoor_object->status = STATUS_NOT_STARTED;

    /* create id */
    spoor_object->id = 0;


    /* created time */
    time_t current_time;
    current_time = time(NULL);
    spoor_object->created.start = *localtime(&current_time);

    /* parent */
    spoor_object->parent_title[0] = '-';
    spoor_object->parent_title[1] = 0;

    return spoor_object;
}
#endif

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
