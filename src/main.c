#define DEBUG

#include"spoor/spoor.h"
#include"redbas/redbas.h"

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>

#ifndef TESTS
int main(int argc, char **argv)
{
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
        char *command = spoor_object_argv_to_command(argc - 2, argv + 2);
        printf("%s\n", command);
        SpoorObject *spoor_object = spoor_object_create(command);
        spoor_debug_spoor_object_print(spoor_object);
        spoor_storage_save(spoor_object);
        free(spoor_object);
    }

    return 0;
}
#else
int main(int argc, char **argv)
{
    printf("Tests\n");
    return 0;
}
#endif
