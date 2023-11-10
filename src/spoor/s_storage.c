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

void spoor_storage_object_append(SpoorObject *spoor_object_parent, SpoorObject *spoor_object)
{
    char location[11];
    /* check if link is triggerd */
    if (spoor_object->parent_location[0] == '-')
    {
        if (spoor_object_parent->child_id == 0xffffffff)
        {
            spoor_object->parent_id = spoor_object_parent->id;
            storage_db_path(spoor_object_parent, location);
            strcpy(spoor_object->parent_location, location);
            strcpy(spoor_object->parent_title, spoor_object_parent->title);

            spoor_object_parent->child_id = spoor_object->id;
            storage_db_path(spoor_object, location);
            strcpy(spoor_object_parent->child_location, location);
            spoor_storage_change(spoor_object_parent);
        }
        else
        {
            spoor_object->parent_id = spoor_object_parent->id;
            spoor_object->child_id_next = spoor_object_parent->child_id;
            strcpy(spoor_object->child_location_next, spoor_object_parent->child_location);
            storage_db_path(spoor_object_parent, location);
            strcpy(spoor_object->parent_location, location);
            strcpy(spoor_object->parent_title, spoor_object_parent->title);

            spoor_object_parent->child_id = spoor_object->id;
            storage_db_path(spoor_object, location);
            strcpy(spoor_object_parent->child_location, location);
            spoor_storage_change(spoor_object_parent);
        }
    }
}

void spoor_storage_save(SpoorObject *spoor_objects, SpoorObject *spoor_object)
{
    char db_path[11];
    storage_db_path(spoor_object, db_path);

    spoor_storage_object_append(&spoor_objects[spoor_object->parent_id], spoor_object);

    RedbasDB *db = redbas_db_open(db_path, sizeof(*spoor_object));
    spoor_object->id = redbas_db_items(db);
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

void spoor_storage_object_remove(SpoorObject *spoor_object)
{
    char db_path[11];
    /* is spoor_object part of a list */
    if (spoor_object->parent_id != 0xffffffff)
    {
        SpoorObject spoor_object_prev;
        RedbasDB *db;
        db = redbas_db_open(spoor_object->parent_location, sizeof(spoor_object_prev));
        redbas_db_restore_cursor_set(db, spoor_object->parent_id);
        redbas_db_restore(db, &spoor_object_prev, sizeof(spoor_object_prev));
        redbas_db_close(db);

        if (spoor_object_prev.child_id == spoor_object->id)
        {
            if (spoor_object->child_id_next == 0xffffffff)
            {
                spoor_object_prev.child_id = 0xffffffff;
                spoor_object_prev.child_location[0] = 0;
            }
            else
            {
                spoor_object_prev.child_id = spoor_object->child_id_next;
                strcpy(spoor_object_prev.child_location, db_path);
            }
        }
        else
        {
            uint32_t index = spoor_object_prev.child_id;
            char *location = spoor_object_prev.child_location;

            while (!(index ==  spoor_object->id && strcmp(location, db_path) == 0))
            {
                db = redbas_db_open(location, sizeof(spoor_object_prev));
                redbas_db_restore_cursor_set(db, index);
                redbas_db_restore(db, &spoor_object_prev, sizeof(spoor_object_prev));
                redbas_db_close(db);

                index = spoor_object_prev.child_id_next;
                location = spoor_object_prev.child_location_next;
            }

            if (spoor_object->child_id_next == 0xffffffff)
            {
                spoor_object_prev.child_id_next = 0xffffffff;
                spoor_object_prev.child_location_next[0] = 0;
            }
            else
            {
                spoor_object_prev.child_id_next = spoor_object->child_id_next;
                strcpy(spoor_object_prev.child_location_next, spoor_object->child_location_next);
            }
        }

        storage_db_path(&spoor_object_prev, db_path);
        db = redbas_db_open(db_path, sizeof(spoor_object_prev));
        redbas_db_change(db, &spoor_object_prev, sizeof(spoor_object_prev), spoor_object_prev.id);
        redbas_db_close(db);
    }

    /* is spoor_object is parent of list */
    if (spoor_object->child_id != 0xffffffff)
    {
        SpoorObject spoor_object_ptr;
        RedbasDB *db;
        storage_db_path(spoor_object, db_path);
        db = redbas_db_open(db_path, sizeof(spoor_object_ptr));
        redbas_db_restore_cursor_set(db, spoor_object->id);
        redbas_db_restore(db, &spoor_object_ptr, sizeof(spoor_object_ptr));
        redbas_db_close(db);

        uint32_t index = spoor_object_ptr.child_id;
        char *location = spoor_object_ptr.child_location;

        while (index != 0xffffffff)
        {
            db = redbas_db_open(location, sizeof(*spoor_object));
            redbas_db_restore_cursor_set(db, index);
            redbas_db_restore(db, &spoor_object_ptr, sizeof(spoor_object_ptr));

            index = spoor_object_ptr.child_id_next;
            location = spoor_object_ptr.child_location_next;

            SpoorObject spoor_object_change;
            spoor_object_change = spoor_object_ptr;
            spoor_object_change.child_id_next = 0xffffffff;
            spoor_object_change.child_location_next[0] = 0;
            spoor_object_change.parent_id = 0xffffffff;
            spoor_object_change.parent_location[0] = 0;
            spoor_object_change.parent_title[0] = 0;

            redbas_db_change(db, &spoor_object_change, sizeof(spoor_object_change), spoor_object_change.id);
            redbas_db_close(db);
        }
    }
}

void spoor_storage_delete(SpoorObject *spoor_object)
{
    char db_path[11];
    storage_db_path(spoor_object, db_path);

    spoor_storage_object_remove(spoor_object);

    uint32_t db_id = spoor_object->id;
    spoor_object->id = SPOOR_OBJECT_DELETED_ID;

    storage_db_path(spoor_object, db_path);
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

