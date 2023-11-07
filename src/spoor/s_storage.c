#include"spoor_internal.h"
#include"../redbas/redbas.h"

#include<stdio.h>
#include<string.h>
#include<dirent.h>

void storage_db_path_clean(SpoorObject *spoor_object, char *db_path_clean)
{
    if (spoor_object->deadline.end.tm_year == -1)
    {
        strcpy(db_path_clean, "000000");
    }
    else
    {
        uint32_t year = spoor_object->deadline.end.tm_year + 1900;
        db_path_clean[3] = year % 10 + 0x30;
        year /= 10;
        db_path_clean[2] = year % 10 + 0x30;
        year /= 10;
        db_path_clean[1] = year % 10 + 0x30;
        year /= 10;
        db_path_clean[0] = year % 10 + 0x30;

        if (spoor_object->deadline.end.tm_mon + 1 < 10)
        {
            db_path_clean[4] = '0';
            db_path_clean[5] = spoor_object->deadline.end.tm_mon + 1 + 0x30;
        }
        else
        {
            db_path_clean[4] = (spoor_object->deadline.end.tm_mon + 1) / 10 + 0x30;
            db_path_clean[5] = (spoor_object->deadline.end.tm_mon + 1) % 10 + 0x30;
        }
    }

    db_path_clean[6] = 0;
}

void storage_db_path(SpoorObject *spoor_object, char *db_path)
{
    storage_db_path_clean(spoor_object, db_path);
    strcpy(db_path + 6, ".rdb");
    db_path[10] = 0;
}

void spoor_storage_save(SpoorObject *spoor_objects, SpoorObject *spoor_object)
{
    char db_path[11];
    storage_db_path(spoor_object, db_path);

    RedbasDB *db = redbas_db_open(db_path, sizeof(*spoor_object));
    spoor_object->id = redbas_db_items(db);
    char location[7];
    if (spoor_object->parent_location[0] == '-')
    {
        SpoorObject *spoor_object_head = &spoor_objects[spoor_object->parent_id];
        if (spoor_objects[spoor_object->parent_id].child_id == 0xffffffff)
        {
            spoor_object->parent_id = spoor_object_head->id;
            storage_db_path_clean(spoor_object_head, location);
            strcpy(spoor_object->parent_location, location);
            strcpy(spoor_object->parent_title, spoor_object_head->title);

            spoor_object_head->child_id = spoor_object->id;
            storage_db_path_clean(spoor_object, location);
            strcpy(spoor_object_head->child_location, location);
            spoor_storage_change(spoor_object_head);
        }
        else
        {
            spoor_object->child_id = 0xffffffff;
            spoor_object->child_id_next = spoor_object_head->child_id;
            strcpy(spoor_object->child_location_next, spoor_object_head->child_location_next);
            strcpy(spoor_object->parent_title, spoor_object_head->title);

            spoor_object_head->child_id = spoor_object->id;
            storage_db_path_clean(spoor_object, location);
            strcpy(spoor_object_head->child_location, location);
            spoor_storage_change(spoor_object_head);
        }
    }
    redbas_db_store(db, spoor_object, sizeof(*spoor_object));
    redbas_db_close(db);
}

uint32_t spoor_object_storage_load(SpoorObject *spoor_objects, SpoorFilter *spoor_filter)
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

                /* filter */
#if 0
                printf("stc: %ld\n%ld\n", spoor_time_compare(&spoor_objects[items_total + i].deadline.end, &spoor_filter->spoor_time.start),
                        spoor_time_compare(&spoor_objects[items_total + i].deadline.end, &spoor_filter->spoor_time.end));
                if (spoor_time_compare(&spoor_objects[items_total + i].deadline.end, &spoor_filter->spoor_time.start) < 0 ||
                    spoor_time_compare(&spoor_objects[items_total + i].deadline.end, &spoor_filter->spoor_time.end) > 0)
                    items_total--;
#endif
            }

            items_total += items;

            redbas_db_close(db);
        }
    }

    closedir(dir);
    return items_total;
}

uint32_t spoor_object_storage_load_filter_time_span(SpoorObject *spoor_objects, SpoorTime *spoor_time_span)
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

                /* time span filter */
                if (spoor_time_compare(&spoor_objects[items_total + i].deadline.end, &spoor_time_span->start) >= 0)
                    items_total--;
            }

            items_total += items;

            redbas_db_close(db);
        }
    }

    closedir(dir);
    spoor_sort_objects_by_deadline(spoor_objects, items_total);
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

void spoor_storage_clean_up(void)
{
    DIR *dir = opendir(".");
    struct dirent *ptr;
    while ((ptr = readdir(dir)) != NULL)
    {
        if (strncmp(ptr->d_name + 6, ".rdb", 3) == 0)
        {
            SpoorObject spoor_object;
            RedbasDB *db = redbas_db_open(ptr->d_name, sizeof(spoor_object));
            RedbasDB *db_tmp = redbas_db_open(".tmp_db", sizeof(spoor_object));

            uint32_t items = redbas_db_items(db);
            uint32_t i;
            uint32_t spoor_object_index;
            for (i = 0, spoor_object_index = 0; i < items; i++)
            {

                redbas_db_restore_cursor_set(db, i);
                redbas_db_restore(db, &spoor_object, sizeof(spoor_object));
                if (spoor_object.id != SPOOR_OBJECT_DELETED_ID)
                {
                    spoor_object.id = spoor_object_index;
                    redbas_db_store(db_tmp, &spoor_object, sizeof(spoor_object));
                    spoor_object_index++;
                }
            }

            redbas_db_close(db_tmp);
            redbas_db_close(db);
            rename(".tmp_db", ptr->d_name);
        }
    }

    closedir(dir);
}

