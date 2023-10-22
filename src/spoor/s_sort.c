#include"spoor_internal.h"
#include<stdlib.h>

int64_t spoor_time_compare(struct tm *time1, struct tm *time2)
{
    int64_t result = 0;
    result += ((time1->tm_year != -1) * time1->tm_year - (time2->tm_year != -1) * time2->tm_year) * 60 * 24 * 31 * 12;
    result += ((time1->tm_mon != -1) * time1->tm_mon - (time2->tm_mon != -1) * time2->tm_mon) * 60 * 24 * 31;
    result += ((time1->tm_mday != -1) * time1->tm_mday - (time2->tm_mday != -1) * time2->tm_mday) * 60 * 24;
    result += ((time1->tm_hour != -1) * time1->tm_hour - (time2->tm_hour != -1) * time2->tm_hour) * 60;
    result += ((time1->tm_min != -1) * time1->tm_min - (time2->tm_min != -1) * time2->tm_min);

    return result;
}

int sort_func(const void *data0, const void *data1)
{
    int32_t result = spoor_time_compare(&((SpoorObject *)data0)->deadline.end, &((SpoorObject *)data1)->deadline.end);
#if 1
    if (result == 0)
        result = spoor_time_compare(&((SpoorObject *)data0)->schedule.start, &((SpoorObject *)data1)->schedule.start);
#endif

    return result;
}

void spoor_sort_objects_by_deadline(SpoorObject *spoor_objects, uint32_t spoor_objects_count)
{
    qsort(spoor_objects, spoor_objects_count, sizeof(*spoor_objects), sort_func);
#if 0
    uint32_t i, j;

    struct tm tmp;

    for (i = 1; i < spoor_objects_count; i++)
    {
        tmp = spoor_objects[i].deadline.end;

        for (j = i; j > 0 && spoor_time_compare(&spoor_objects[j - 1].deadline.end, &tmp) > 0; j--)
            spoor_objects[j].deadline.end = spoor_objects[j - 1].deadline.end;
        spoor_objects[j].deadline.end = tmp;
    }
#endif
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
