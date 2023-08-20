#include"spoor_internal.h"

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<termios.h>
#include<unistd.h>
#include<sys/ioctl.h>

const char UI_TYPES[][17] = {
    "TASK",
    "PROJECT",
    "EVENT",
    "APPOINTMENT",
    "GOAL",
    "HABIT",
};

const char UI_STATUS[][50] = {
    "\x1B[31mNOT STARTED\x1B[m",
    "\x1B[33mIN PROGRESS\x1B[m",
    "\x1B[32mCOMPLETED\x1B[m",
};

void screen_clear(void)
{
    fprintf(stdout, "\033[2J");
}

void cursor_show(void)
{
    fprintf(stdout, "\033[?25h");
}

void cursor_hide(void)
{
    fprintf(stdout, "\033[?25l");
}

void cursor_move(int x, int y)
{
    fprintf(stdout, "\033[%d;%dH", y, x);
}

void time_format_parse_deadline(SpoorTime *spoor_time, char *time_format)
{
    if (spoor_time->start.tm_year == -1)
    {
        sprintf(time_format, "--.--.---- --:--");
        return;
    }

    char time_format_start[6];

    if (spoor_time->start.tm_hour == -1 || spoor_time->start.tm_min == -1)
        sprintf(time_format_start, "--:--");
    else
        sprintf(time_format_start, "%s%d:%s%d",
                (spoor_time->start.tm_hour < 10) ?"0" :"", spoor_time->start.tm_hour,
                (spoor_time->start.tm_min < 10) ?"0" : "", spoor_time->start.tm_min);

    sprintf(time_format, "%s%d.%s%d.%d %s",
            (spoor_time->start.tm_mday < 10) ?"0" :"",
            spoor_time->start.tm_mday,
            (spoor_time->start.tm_mon + 1 < 10) ?"0" :"",
            spoor_time->start.tm_mon + 1,
            spoor_time->start.tm_year + 1900,
            time_format_start);
}

void time_format_parse_schedule(SpoorTime *spoor_time, char *time_format)
{
    if (spoor_time->start.tm_year == -1)
    {
        sprintf(time_format, "--.--.---- --:-- --:--");
        return;
    }

    char time_format_start[6] = { 0 };
    char time_format_end[6] = { 0 };

    if (spoor_time->start.tm_hour == -1 || spoor_time->start.tm_min == -1)
        sprintf(time_format_start, "--:--");
    else
        sprintf(time_format_start, "%s%d:%s%d",
                (spoor_time->start.tm_hour < 10) ?"0" :"", spoor_time->start.tm_hour,
                (spoor_time->start.tm_min < 10) ?"0" : "", spoor_time->start.tm_min);

    if (spoor_time->end.tm_hour == -1 || spoor_time->end.tm_min == -1)
        sprintf(time_format_end, "--:--");
    else
        sprintf(time_format_end, "%s%d:%s%d",
                (spoor_time->end.tm_hour < 10) ?"0" :"", spoor_time->end.tm_hour,
                (spoor_time->end.tm_min < 10) ?"0" : "", spoor_time->end.tm_min);

    sprintf(time_format, "%s%d.%s%d.%d %s %s",
            (spoor_time->start.tm_mday < 10) ?"0" :"",
            spoor_time->start.tm_mday,
            (spoor_time->start.tm_mon + 1 < 10) ?"0" :"",
            spoor_time->start.tm_mon + 1,
            spoor_time->start.tm_year + 1900,
            time_format_start,
            time_format_end);
}

void spoor_ui_object_show(void)
{
#if 0
    SpoorObject spoor_objects[500];
    uint32_t spoor_objects_count = 0;
    spoor_objects_count = spoor_object_storage_load(spoor_objects);

    uint32_t i;
    for (i = 0; i < spoor_objects_count; i++)
    {
        spoor_debug_spoor_object_print(&spoor_objects[i]);
    }
    
#else
    struct termios old;
    struct termios new;

    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    new.c_cc[VMIN] = 1;
    new.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);

    SpoorObject spoor_objects[500];
    uint32_t spoor_objects_count = 0;
    spoor_objects_count = spoor_object_storage_load(spoor_objects);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    uint32_t window_rows = w.ws_row;

    uint32_t offset = 0;

    cursor_hide();
    bool command_mode = false;
    char command_buffer[200] = { 0 };
    uint8_t command_buffer_pos = 0;
    while (1)
    {
        spoor_sort_objects(spoor_objects, spoor_objects_count);

        /* window size update */
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        window_rows = w.ws_row;

        /* print title bar */
        cursor_move(0, 0);
        screen_clear();
        fprintf(stdout, "%-5s%-30s%-18s%-24s%-13s%-13s%-12s", "I", "TITLE", "DEADLINE", "SCHEDULE", "STATUS", "TYPE", "PROJECT");
        cursor_move(0, 2);
        fprintf(stdout, "--------------------------------------------------------------------------------------------------------------");

        /* print spoor_objects */
        char time_format_deadline[50] = { 0 };
        char time_format_schedule[50] = { 0 };

        uint32_t i;
        for (i = 0; i < spoor_objects_count && i < window_rows - 5; i++)
        {
            time_format_parse_deadline(&spoor_objects[i + offset].deadline, time_format_deadline);
            time_format_parse_schedule(&spoor_objects[i + offset].schedule, time_format_schedule);
            cursor_move(0, 3 + i);
            fprintf(stdout,
                    "%-5d%-30s%-18s%-24s%-21s%-13s%s",
                    i,
                    spoor_objects[i + offset].title,
                    time_format_deadline,
                    time_format_schedule,
                    UI_STATUS[spoor_objects[i + offset].status],
                    UI_TYPES[spoor_objects[i + offset].type],
                    spoor_objects[i + offset].parent_title);
        }

        /* print status bar */
        cursor_move(0, window_rows - 2);
        fprintf(stdout, "---------------------------------------------------------------------------------------------------------------");
        cursor_move(0, window_rows -1);
        fprintf(stdout, "%s", command_buffer);
        cursor_move(0, window_rows);
        fprintf(stdout, "-- Elements: [%d - %d](%d)", offset, offset + window_rows - 5, spoor_objects_count);

        /* key input */
        if (command_mode)
        {
            uint32_t c = getchar();
            if (c == 0x7f)
            {
                command_buffer[--command_buffer_pos] = 0;
            }
            else if (c == '\n')
            {
                if (strncmp(command_buffer + 1, "c", 1) == 0)
                {
                    SpoorObject *spoor_object = spoor_object_create(command_buffer + 2);
                    spoor_storage_save(spoor_object);
                    spoor_objects_count = spoor_object_storage_load(spoor_objects);
                }
                else
                {
                    uint32_t index = 0;
                    uint32_t p = 0;
                    while (command_buffer[1 + p] >= 0x30 && command_buffer[1 + p] <= 0x39)
                    {
                        index *= 10;
                        index += command_buffer[1 + p] - 0x30;
                        p++;
                    }

                    if (strncmp(command_buffer + 1 + p, "dl", 2) == 0)
                    {
                        spoor_object_deadline_set(&spoor_objects[index + offset], command_buffer + 3 + p);
                        spoor_storage_change(&spoor_objects[index + offset]);
                    }
                    else if (strncmp(command_buffer + 1 + p, "d", 1) == 0)
                    {
                        spoor_storage_delete(&spoor_objects[index + offset]);
                        spoor_objects_count = spoor_object_storage_load(spoor_objects);
                    }
                    else if (strncmp(command_buffer + 1 + p, "p", 1) == 0)
                    {
                        uint16_t status = 0;
                        uint16_t space = (command_buffer[2 + p] == ' ') ?1 :0;
                        if (strncmp(command_buffer + 2 + p + space, "c", 1) == 0)
                            status = STATUS_COMPLETED;
                        else if (strncmp(command_buffer + 2 + p + space, "ip", 2) == 0)
                            status = STATUS_IN_PROGRESS;
                        else if (strncmp(command_buffer + 2 + p + space, "ns", 2) == 0)
                            status = STATUS_NOT_STARTED;

                        spoor_object_progress_change(&spoor_objects[index + offset], status);
                        spoor_storage_change(&spoor_objects[index + offset]);
                    }
                    else if (strncmp(command_buffer + 1 + p, "s", 1) == 0)
                    {
                        spoor_object_schedule_set(&spoor_objects[index + offset], command_buffer + 2 + p);
                        spoor_storage_change(&spoor_objects[index + offset]);
                    }
                    else if (strncmp(command_buffer + 1 + p, "t", 1) == 0)
                    {
                        uint16_t type = 0;
                        uint16_t space = (command_buffer[2 + p] == ' ') ?1 :0;
                        if (strncmp(command_buffer + 2 + p + space, "t", 1) == 0)
                            type = TYPE_TASK;
                        else if (strncmp(command_buffer + 2 + p + space, "p", 1) == 0)
                            type = TYPE_PROJECT;
                        else if (strncmp(command_buffer + 2 + p + space, "e", 1) == 0)
                            type = TYPE_EVENT;
                        else if (strncmp(command_buffer + 2 + p + space, "a", 1) == 0)
                            type = TYPE_APPOINTMENT;
                        else if (strncmp(command_buffer + 2 + p + space, "g", 1) == 0)
                            type = TYPE_GOAL;
                        else if (strncmp(command_buffer + 2 + p + space, "h", 1) == 0)
                            type = TYPE_HABIT;

                        spoor_objects[index + offset].type = type;
                        spoor_storage_change(&spoor_objects[index + offset]);
                    }
                    else
                    {
                        screen_clear();
                        cursor_move(0, 0);
                        spoor_debug_spoor_object_print(&spoor_objects[index + offset]);
                        getchar();
                    }
                }
                memset(command_buffer, 0, 200);
                command_mode = false;
                command_buffer_pos = 0;
            }
            else
                command_buffer[command_buffer_pos++] = c;
        }
        else
        {
            uint32_t c = getchar();
            if (c == ':')
            {
                command_mode = true;
                command_buffer[0] = ':';
                command_buffer_pos++;
            }
            if (c == 'f')
                offset += (window_rows - 4) / 2;
            if (c == 'b')
            {
                if (((int64_t)offset - (window_rows - 4) / 2) >= 0)
                    offset -= (window_rows - 4) / 2;
            }

            if (c == 'q')
            {
                cursor_move(0, 0);
                screen_clear();
                /* todo clear the database */
                /* like delete old id's and shrink it */

                break;
            }
        }
    }
    cursor_show();

    printf("\e[m");
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
#endif
}