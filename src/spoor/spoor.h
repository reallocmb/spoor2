#ifndef SPOOR_H
#define SPOOR_H

typedef void SpoorObject;
typedef void SpoorDatabase;
typedef void SpoorFilter;

#include<stdint.h>

SpoorObject *spoor_arguments_parse(int argc, char **argv); /* parse the command arguments and create a new SpoorObject */
uint32_t spoor_object_size(void);
void spoor_object_progress_change(SpoorObject *spoor_object);

/* Data Storage */
void spoor_storage_save(SpoorObject *spoor_object);

SpoorDatabase *spoor_storage_init(void);
void spoor_storage_close(SpoorDatabase *spoor_database);
int spoor_storage_object_save(SpoorDatabase *spoor_database, SpoorObject *spoor_object);

/* Data Sort */
void spoor_sort_objects(SpoorObject *spoor_objects, uint32_t spoor_objects_count);

/* UI */
void spoor_ui_object_show(void);

/* Debugging */
#ifdef DEBUG
void spoor_debug_spoor_object_print(SpoorObject *spoor_object); /* print an SpoorObject to the console */
void spoor_debug_storage_print_all(SpoorDatabase *spoor_object);
#endif

#endif
