#define DEBUG

#ifndef TESTS
#include"spoor/spoor.h"
#include"redbas/redbas.h"

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>

int main(int argc, char **argv)
{
    /* change current directory to database directory */
    char *home_directory = getenv("HOME");
    char database_path[strlen(home_directory) + 7 + 1];
#ifdef RELEASE
    strcpy(database_path, home_directory);
    strcpy(database_path + strlen(home_directory), "/.spoor");
#else 
    strcpy(database_path, ".spoor");
#endif
    mkdir(database_path, 0777);
    chdir(database_path);

    if (argc <= 1)
        spoor_ui_object_show();
    else if (strcmp(argv[1], "-v") == 0)
    {
        printf("SPOOR VERSION: %d.%d.%d\n", SPOOR_VERSION_MAJOR, SPOOR_VERSION_MINOR, SPOOR_VERSION_PATCH);
    }
    else if (strcmp(argv[1], "-c") == 0)
    {
        char *arguments = spoor_object_argv_to_command(argc - 2, argv + 2);
        printf("Arguments %s\n", arguments);
        SpoorObject *spoor_object = spoor_object_create(arguments);
        spoor_debug_spoor_object_print(spoor_object);
        free(spoor_object);
    }

    return 0;
}
#else

#include"unicki/unicki.h"
#include"spoor/spoor_internal.h"
#include<string.h>

TEST_INIT
TEST(suite, spoor_time_deadline_create())
{
    time_t current_time = time(NULL);
    struct tm today = *localtime(&current_time);

    SpoorTime spoor_time;
    char argument[50];
    strcpy(argument, "d");
    spoor_time_deadline_create(argument, strlen(argument), &spoor_time);
    unicki_assert(spoor_time.end.tm_year == today.tm_year);
    unicki_assert(spoor_time.end.tm_mday == today.tm_mday);
    unicki_assert(spoor_time.end.tm_mon == today.tm_mon);
    unicki_assert(spoor_time.end.tm_hour == -1);
    unicki_assert(spoor_time.end.tm_min == -1);

    unicki_assert(spoor_time.start.tm_year == -1);
    unicki_assert(spoor_time.start.tm_mday == -1);
    unicki_assert(spoor_time.start.tm_mon == -1);
    unicki_assert(spoor_time.start.tm_hour == -1);
    unicki_assert(spoor_time.start.tm_min == -1);

    strcpy(argument, "d-d");
    spoor_time_deadline_create(argument, strlen(argument), &spoor_time);
    unicki_assert(spoor_time.end.tm_year == today.tm_year);
    unicki_assert(spoor_time.end.tm_mday == today.tm_mday);
    unicki_assert(spoor_time.end.tm_mon == today.tm_mon);
    unicki_assert(spoor_time.end.tm_hour == -1);
    unicki_assert(spoor_time.end.tm_min == -1);

    unicki_assert(spoor_time.start.tm_year == today.tm_year);
    unicki_assert(spoor_time.start.tm_mday == today.tm_mday);
    unicki_assert(spoor_time.start.tm_mon == today.tm_mon);
    unicki_assert(spoor_time.start.tm_hour == -1);
    unicki_assert(spoor_time.start.tm_min == -1);

    strcpy(argument, "d915");
    spoor_time_deadline_create(argument, strlen(argument), &spoor_time);
    unicki_assert(spoor_time.end.tm_year == today.tm_year);
    unicki_assert(spoor_time.end.tm_mday == today.tm_mday);
    unicki_assert(spoor_time.end.tm_mon == today.tm_mon);
    unicki_assert(spoor_time.end.tm_hour == 9);
    unicki_assert(spoor_time.end.tm_min == 15);

    unicki_assert(spoor_time.start.tm_year == -1);
    unicki_assert(spoor_time.start.tm_mday == -1);
    unicki_assert(spoor_time.start.tm_mon == -1);
    unicki_assert(spoor_time.start.tm_hour == -1);
    unicki_assert(spoor_time.start.tm_min == -1);

    strcpy(argument, "d915-d1948");
    spoor_time_deadline_create(argument, strlen(argument), &spoor_time);
    unicki_assert(spoor_time.end.tm_year == today.tm_year);
    unicki_assert(spoor_time.end.tm_mday == today.tm_mday);
    unicki_assert(spoor_time.end.tm_mon == today.tm_mon);
    unicki_assert(spoor_time.end.tm_hour == 19);
    unicki_assert(spoor_time.end.tm_min == 48);

    unicki_assert(spoor_time.start.tm_year == today.tm_year);
    unicki_assert(spoor_time.start.tm_mday == today.tm_mday);
    unicki_assert(spoor_time.start.tm_mon == today.tm_mon);
    unicki_assert(spoor_time.start.tm_hour == 9);
    unicki_assert(spoor_time.start.tm_min == 15);

    strcpy(argument, "5d");
    current_time += 5 * 60 * 60 * 24;
    today = *localtime(&current_time);
    spoor_time_deadline_create(argument, strlen(argument), &spoor_time);
    unicki_assert(spoor_time.end.tm_year == today.tm_year);
    unicki_assert(spoor_time.end.tm_mday == today.tm_mday);
    unicki_assert(spoor_time.end.tm_mon == today.tm_mon);
    unicki_assert(spoor_time.end.tm_hour == -1);
    unicki_assert(spoor_time.end.tm_min == -1);

    unicki_assert(spoor_time.start.tm_year == -1);
    unicki_assert(spoor_time.start.tm_mday == -1);
    unicki_assert(spoor_time.start.tm_mon == -1);
    unicki_assert(spoor_time.start.tm_hour == -1);
    unicki_assert(spoor_time.start.tm_min == -1);

    current_time = time(NULL);
    strcpy(argument, "-5d");
    current_time -= 5 * 60 * 60 * 24;
    today = *localtime(&current_time);
    spoor_time_deadline_create(argument, strlen(argument), &spoor_time);
    unicki_assert(spoor_time.end.tm_year == today.tm_year);
    unicki_assert(spoor_time.end.tm_mday == today.tm_mday);
    unicki_assert(spoor_time.end.tm_mon == today.tm_mon);
    unicki_assert(spoor_time.end.tm_hour == -1);
    unicki_assert(spoor_time.end.tm_min == -1);

    unicki_assert(spoor_time.start.tm_year == -1);
    unicki_assert(spoor_time.start.tm_mday == -1);
    unicki_assert(spoor_time.start.tm_mon == -1);
    unicki_assert(spoor_time.start.tm_hour == -1);
    unicki_assert(spoor_time.start.tm_min == -1);
}
TEST_END


#endif
