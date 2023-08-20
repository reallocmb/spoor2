#include"spoor_internal.h"
#include"../redbas/redbas.h"

#include<stdio.h>
#include<string.h>
#include<dirent.h>

void storage_db_path(SpoorObject *spoor_object, char *db_path)
{
    if (spoor_object->deadline.start.tm_year == -1)
    {
        strcpy(db_path, "000000");
    }
    else
    {
        uint32_t year = spoor_object->deadline.start.tm_year + 1900;
        db_path[3] = year % 10 + 0x30;
        year /= 10;
        db_path[2] = year % 10 + 0x30;
        year /= 10;
        db_path[1] = year % 10 + 0x30;
        year /= 10;
        db_path[0] = year % 10 + 0x30;

        if (spoor_object->deadline.start.tm_mon + 1 < 10)
        {
            db_path[4] = '0';
            db_path[5] = spoor_object->deadline.start.tm_mon + 1 + 0x30;
        }
        else
        {
            db_path[4] = (spoor_object->deadline.start.tm_mon + 1) / 10 + 0x30;
            db_path[5] = (spoor_object->deadline.start.tm_mon + 1) % 10 + 0x30;
        }
    }
    strcpy(db_path + 6, ".rdb");
    db_path[10] = 0;
}

void spoor_storage_save(SpoorObject *spoor_object)
{
    char db_path[11];
    storage_db_path(spoor_object, db_path);

    RedbasDB *db = redbas_db_open(db_path, sizeof(*spoor_object));
    spoor_object->id = redbas_db_items(db);
    redbas_db_store(db, spoor_object, sizeof(*spoor_object));
    redbas_db_close(db);
}

uint32_t spoor_object_storage_load(SpoorObject *spoor_objects)
{
    DIR *dir = opendir(".");
    struct dirent *ptr;
    uint32_t items_total = 0;
    uint32_t i;
    while ((ptr = readdir(dir)) != NULL)
    {
        if (strncmp(ptr->d_name + 6, ".rdb", 3) == 0)
        {
            RedbasDB *db = redbas_db_open(ptr->d_name, sizeof(*spoor_objects));
            uint32_t items = redbas_db_items(db);
            for (i = 0; i < items; i++)
            {
                redbas_db_restore_cursor_set(db, i);
                redbas_db_restore(db, &spoor_objects[items_total + i], sizeof(*spoor_objects));
                /* skip temporly deleted elements */
                if (spoor_objects[items_total + i].id == SPOOR_OBJECT_DELETED_ID)
                    items_total--;
            }

            items_total += items;

            redbas_db_close(db);
        }
    }

    closedir(dir);
    return items_total;
}

void spoor_storage_change(SpoorObject *spoor_object)
{
    char db_path[11];
    storage_db_path(spoor_object, db_path);

    RedbasDB *db = redbas_db_open(db_path, sizeof(*spoor_object));
    redbas_db_change(db, spoor_object, sizeof(*spoor_object), spoor_object->id);
    redbas_db_close(db);
}

void spoor_storage_delete(SpoorObject *spoor_object)
{
    char db_path[11];
    storage_db_path(spoor_object, db_path);

    uint32_t db_id = spoor_object->id;
    spoor_object->id = SPOOR_OBJECT_DELETED_ID;

    RedbasDB *db = redbas_db_open(db_path, sizeof(*spoor_object));
    redbas_db_change(db, spoor_object, sizeof(*spoor_object), db_id);
    redbas_db_close(db);
}