#include"spoor_internal.h"

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

#if 0
int32_t spoor_time_compare(struct tm *time1, struct tm *time2)
{
    if (time1->tm_year > time2->tm_year)
    {
        return 1;
    }
    else if (time1->tm_year < time2->tm_year)
    {
        return -1;
    }
    else if (time1->tm_year == time2->tm_year)
    {
        if (time1->tm_mon > time2->tm_mon)
            return 1;
        else if (time1->tm_mon < time2->tm_mon)
            return -1;
        else if (time1->tm_mon == time2->tm_mon)
        {
            if (time1->tm_mday > time2->tm_mday)
                return 1;
            else if (time1->tm_mday < time2->tm_mday)
                return -1;
            else if (time1->tm_mday == time2->tm_mday)
            {
                if (time1->tm_hour > time2->tm_hour)
                    return 1;
                else if (time1->tm_hour < time2->tm_hour)
                    return -1;
                else if (time1->tm_hour == time2->tm_hour)
                {
                    if (time1->tm_min > time2->tm_min)
                        return 1;
                    else if (time1->tm_min < time2->tm_min)
                        return -1;
                    else if (time1->tm_min == time2->tm_min)
                    {
                        return 0;
                    }
                }
            }
        }
    }

    return 0;
}
#endif

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
