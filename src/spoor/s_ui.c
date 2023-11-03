#if 0
#define UI_GRAPHAPP
#endif

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
    if (spoor_time->end.tm_year == -1)
    {
        sprintf(time_format, "--.--.---- --:--");
        return;
    }

    char time_format_start[6];

    if (spoor_time->end.tm_hour == -1 || spoor_time->end.tm_min == -1)
        sprintf(time_format_start, "--:--");
    else
        sprintf(time_format_start, "%s%d:%s%d",
                (spoor_time->end.tm_hour < 10) ?"0" :"", spoor_time->end.tm_hour,
                (spoor_time->end.tm_min < 10) ?"0" : "", spoor_time->end.tm_min);

    sprintf(time_format, "%s%d.%s%d.%d %s",
            (spoor_time->end.tm_mday < 10) ?"0" :"",
            spoor_time->end.tm_mday,
            (spoor_time->end.tm_mon + 1 < 10) ?"0" :"",
            spoor_time->end.tm_mon + 1,
            spoor_time->end.tm_year + 1900,
            time_format_start);
}

void time_format_parse_schedule(SpoorTime *spoor_time, char *time_format)
{
    if (spoor_time->end.tm_year == -1)
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

    if (spoor_time->start.tm_year == -1)
    {
        sprintf(time_format, "%s%d.%s%d.%d %s %s",
                (spoor_time->end.tm_mday < 10) ?"0" :"",
                spoor_time->end.tm_mday,
                (spoor_time->end.tm_mon + 1 < 10) ?"0" :"",
                spoor_time->end.tm_mon + 1,
                spoor_time->end.tm_year + 1900,
                time_format_end,
                time_format_end);
    }
    else
    {
        sprintf(time_format, "%s%d.%s%d.%d %s %s",
                (spoor_time->start.tm_mday < 10) ?"0" :"",
                spoor_time->start.tm_mday,
                (spoor_time->start.tm_mon + 1 < 10) ?"0" :"",
                spoor_time->start.tm_mon + 1,
                spoor_time->start.tm_year + 1900,
                time_format_start,
                time_format_end);
    }
}

#ifdef UI_GRAPHAPP

void spoor_ui_object_show(void)
{
    App *app = app_new_app(0, NULL);
    Window *window = app_new_window(app, rect(0, 0, 500, 300), "SPOOR", STANDARD_WINDOW);

    app_show_window(window);

    app_main_loop(app);
}
#else
void spoor_ui_object_show(void)
{
#if 0
    SpoorFilter spoor_filter;
    SpoorObject spoor_objects[500];
    uint32_t spoor_objects_count = 0;
    spoor_objects_count = spoor_object_storage_load(spoor_objects, &spoor_filter);

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

    SpoorFilter spoor_filter;
    spoor_filter.spoor_time.start.tm_year = 123;
    spoor_filter.spoor_time.start.tm_mon = 9;
    spoor_filter.spoor_time.start.tm_mday = 22;
    spoor_filter.spoor_time.start.tm_hour = -1;
    spoor_filter.spoor_time.start.tm_min = -1;

    spoor_filter.spoor_time.end = spoor_filter.spoor_time.start;
    spoor_filter.spoor_time.end.tm_hour = -1;
    spoor_filter.spoor_time.end.tm_min = -1;
    SpoorObject spoor_objects[500];
    uint32_t spoor_objects_count = 0;
    spoor_objects_count = spoor_object_storage_load(spoor_objects, &spoor_filter);
#if 0
    SpoorTime spoor_span;
    spoor_span.start.tm_year = 123;
    spoor_span.start.tm_mon = 9;
    spoor_span.start.tm_mday = 20;

    spoor_span.end = spoor_span.start;
    spoor_objects_count = spoor_object_storage_load_filter_time_span(spoor_objects, &spoor_span);
#endif

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    uint32_t window_rows = w.ws_row;

    uint32_t offset = 0;

    cursor_hide();
    bool command_mode = false;
    char arguments[200] = { 0 };
    uint8_t arguments_pos = 0;
    while (1)
    {
        /* sorting */
        spoor_sort_objects_by_deadline(spoor_objects, spoor_objects_count);
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
        fprintf(stdout, "%s", arguments);
        cursor_move(0, window_rows);
        fprintf(stdout, "-- Elements: [%d - %d](%d)", offset, offset + window_rows - 5, spoor_objects_count);

        /* key input */
        if (command_mode)
        {
            uint32_t c = getchar();
            if (c == 0x7f)
            {
                arguments[--arguments_pos] = 0;
            }
            else if (c == '\n')
            {
                if (strncmp(arguments + 1, "q", 1) == 0)
                {
                    cursor_move(0, 0);
                    screen_clear();

                    spoor_storage_clean_up();
                    break;
                }
                else if (strncmp(arguments + 1, "c", 1) == 0)
                {
                    SpoorObject *spoor_object = spoor_object_create(arguments + 2);
                    if (spoor_object->child_id != 0xffffffff)
                        spoor_object_children_append(&spoor_objects[spoor_object->child_id], spoor_object);
                    else
                        spoor_storage_save(spoor_object);

                    free(spoor_object);
                    spoor_objects_count = spoor_object_storage_load(spoor_objects, &spoor_filter);
                }
                else if (arguments[1] == 'h')
                {
                    screen_clear();
                    cursor_move(0, 0);
                    printf("--- HELP PAGE ---\n");
                    printf("commands:\n"
                            ":c[title],[deadline] [schedule] [status || type]\t\tcreate object\n"
                            "type: t = TASK\n"
                            "type: e = EVENT\n"
                            "type: a = APPOINTMENT\n"
                            "type: g = GOAL\n"
                            "type: h = HABIT\n"
                            "status: c = COMPLETED\n"
                            "status: ip = IN PROGRESS\n"
                            "status: ns = NOT STARTED\n"
                            ":[index] [title],[deadline] [schedule] [status || type]\t\tedit object by index");
                    getchar();
                }
                else
                {
                    /* edit spoor object */

                    /* index */
                    uint32_t index = 0;
                    uint32_t p = 0;
                    while (arguments[1 + p] >= 0x30 && arguments[1 + p] <= 0x39)
                    {
                        index *= 10;
                        index += arguments[1 + p] - 0x30;
                        p++;
                    }
                    if (arguments[p + 1] == 'e')
                    {
                        SpoorObject old = spoor_objects[index + offset];
                        spoor_object_edit(&spoor_objects[index + offset], arguments + p + 2);
                        if (spoor_object->child_id != 0xffffffff)
                            spoor_object_children_append(&spoor_objects[spoor_object->child_id], spoor_object);
                        else
                        {
                            if (old.deadline.end.tm_year == spoor_objects[index + offset].deadline.end.tm_year &&
                                    old.deadline.end.tm_mon == spoor_objects[index + offset].deadline.end.tm_mon)
                                spoor_storage_change(&spoor_objects[index + offset]);
                            else
                            {
                                spoor_storage_delete(&old);
                                spoor_storage_save(&spoor_objects[index + offset]);
                            }
                        }
                        spoor_objects_count = spoor_object_storage_load(spoor_objects, &spoor_filter);
                    }
                    else if (arguments[p + 1] == 'd')
                    {
                        spoor_storage_delete(&spoor_objects[index + offset]);
                        spoor_objects_count = spoor_object_storage_load(spoor_objects, &spoor_filter);
                    }
                    else
                    {
                        screen_clear();
                        cursor_move(0, 0);
                        spoor_debug_spoor_object_print(&spoor_objects[index + offset]);
                        getchar();
                    }
                }
                memset(arguments, 0, 200);
                command_mode = false;
                arguments_pos = 0;
            }
            else
                arguments[arguments_pos++] = c;
        }
        else
        {
            uint32_t c = getchar();
            if (c == ':')
            {
                command_mode = true;
                arguments[0] = ':';
                arguments_pos++;
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

                spoor_storage_clean_up();

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
#endif
