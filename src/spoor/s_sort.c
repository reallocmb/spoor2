#include"spoor_internal.h"
#include <stdlib.h>

int64_t spoor_time_compare(struct tm *time1, struct tm *time2)
{
    return (time1->tm_year * 100000) +
        (time1->tm_mon * 10000)+
        (time1->tm_mday * 100) +
        (time1->tm_hour * 10) +
        time1->tm_min -
        ((time2->tm_year * 100000) +
        (time2->tm_mon * 10000) +
        (time2->tm_mday * 100) +
        (time2->tm_hour * 10) +
        time2->tm_min);
}

void spoor_sort_objects_by_title(SpoorObject *spoor_objects, uint32_t spoor_objects_count)
{
    uint32_t i;
    SpoorObject temp;
    while (spoor_objects_count--)
    {
        for (i = 1; i <= spoor_objects_count; i++)
        {
            if (spoor_objects[i - 1].title[0] < spoor_objects[i].title[0])
            {
                temp = spoor_objects[i - 1];
                spoor_objects[i - 1] = spoor_objects[i];
                spoor_objects[i] = temp;
            }
        }
    }
}

void spoor_sort_objects(SpoorObject *spoor_objects, uint32_t spoor_objects_count)
{
    uint32_t i;
    SpoorObject temp;
    while (spoor_objects_count--)
    {
        for (i = 1; i <= spoor_objects_count; i++)
        {
            if (spoor_time_compare(&spoor_objects[i - 1].deadline.start, &spoor_objects[i].deadline.start) == 1)
            {
                temp = spoor_objects[i - 1];
                spoor_objects[i - 1] = spoor_objects[i];
                spoor_objects[i] = temp;
            }
        }
    }
}
