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
#include"spoor/spoor_internal.h"

#include<stdio.h>

int main(void)
{
    printf("Test Func: spoor_time_compare\n");
    struct tm time1 = { .tm_year = 2023, .tm_mon = 0, .tm_mday = 0, .tm_hour = 0, .tm_min = 0 };
    struct tm time2 = { .tm_year = 2024, .tm_mon = 11, .tm_mday = 30, .tm_hour = 23, .tm_min = 60 };

    int compare = spoor_time_compare(&time1, &time2);

    if (compare < 0)
        printf("\t\x1B[32mTest Passed\x1B[m\n");
    else
        printf("\t\x1B[31mTest Falied\x1B[m\n");

    compare = spoor_time_compare(&time2, &time1);

    if (compare > 0)
        printf("\t\x1B[32mTest Passed\x1B[m\n");
    else
        printf("\t\x1B[31mTest Falied\x1B[m\n");

    printf("Test Func: spoor_object_create\n");

    free(spoor_object);


    return 0;
}
#endif
