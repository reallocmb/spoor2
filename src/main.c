#define DEBUG

#include"spoor/spoor.h"
#include"redbas/redbas.h"

#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>

int main(int argc, char **argv)
{
#ifdef _WIN32
#else
    mkdir("/home/mb/.spoor", 0777);
    chdir("/home/mb/.spoor");
#endif

    if (argc <= 1)
        spoor_ui_object_show();
    else if (strcmp(argv[1], "-c") == 0)
    {
        SpoorObject *spoor_object = spoor_arguments_parse(argc - 2, argv + 2);
        spoor_debug_spoor_object_print(spoor_object);

        spoor_storage_save(spoor_object);
        free(spoor_object);
    }

    return 0;
}
