#ifndef SPOOR_INTERNAL_H
#define SPOOR_INTERNAL_H

#include<time.h>
#include<stdint.h>

typedef enum {
    STATUS_NOT_STARTED,
    STATUS_IN_PROGRESS,
    STATUS_COMPLETED,
} SpoorStatus;

typedef enum {
    TYPE_TASK,
    TYPE_PROJECT,
    TYPE_EVENT,
    TYPE_APPOINTMENT,
    TYPE_GOAL,
    TYPE_HABIT,
} SpoorType;

typedef struct SpoorTime {
    struct tm start;
    struct tm end;
    struct SpoorTime *next;
} SpoorTime;

typedef struct {
    char parent_id[100];
    char id[100];
} SpoorChild;

#define SPOOR_OBJECT_DELETED_ID 0xffffffff

typedef struct SpoorObject {
    uint32_t id;
    char title[250];
    char parent_title[250];
    SpoorTime deadline;
    SpoorTime schedule;
    SpoorTime tracked;
    SpoorTime complete;
    SpoorTime created;
    SpoorStatus status;
    SpoorType type;
    uint32_t child_id;
    uint32_t child_id_next;
    char child_location[7];
    char child_location_next[7];
} SpoorObject;

typedef struct {
    SpoorTime spoor_time;
} SpoorFilter;

typedef struct {
} SpoorSort;

SpoorObject *spoor_object_create(char *arguments);
void spoor_object_edit(SpoorObject * spoor_object, char *arguments);

void mdb_func_error_check(int error, char *func_name, int line, char *file);
void spoor_debug_spoor_object_print(SpoorObject *spoor_object);

uint32_t spoor_object_storage_load(SpoorObject *spoor_objects, SpoorFilter *spoor_filter);
void spoor_sort_objects(SpoorObject *spoor_objects, uint32_t spoor_objects_count);

void spoor_object_progress_change(SpoorObject *spoor_object, SpoorStatus status);
void spoor_storage_save(SpoorObject *spoor_object);

void spoor_storage_change(SpoorObject *spoor_object);
void spoor_storage_delete(SpoorObject *spoor_object);
void spoor_object_schedule_set(SpoorObject *spoor_object, char *command);
void spoor_object_deadline_set(SpoorObject *spoor_object, char *command);

void spoor_storage_clean_up(void);
uint32_t spoor_object_storage_load_filter_time_span(SpoorObject *spoor_objects, SpoorTime *time_span);

void spoor_time_span_create(SpoorTime *spoor_time_span, char *command);

/* sort */
int64_t spoor_time_compare(struct tm *time1, struct tm *time2);
void spoor_sort_objects_by_title(SpoorObject *spoor_objects, uint32_t spoor_objects_count);
void spoor_sort_objects_by_deadline(SpoorObject *spoor_objects, uint32_t spoor_objects_count);

void spoor_time_deadline_create(char *argument, uint32_t argument_length, SpoorTime *spoor_time);

void storage_db_path_clean(SpoorObject *spoor_object, char *db_path_clean);
void spoor_object_children_append(SpoorObject *spoor_object_head, SpoorObject *spoor_object)

#endif
