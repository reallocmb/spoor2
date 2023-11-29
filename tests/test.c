#define EENHEID_UNIT_TEST

#include<eenheid.h>
#include<string.h>

#include<s_storage.c>
#include<s_object.c>
#include<s_span_time.c>
#include<s_sort.c>

__EENHEID_INIT__
SUITE(s_spoor_object.c)
{
    TEST(spoor_time_date_create())
    {
        char argument[50];
        memcpy(argument, "d", 1);
        SpoorTime spoor_time;
        SpoorTime spoor_time_today_test = {
            .day = 30,
            .mon = 0,
            .year = 123
        };

        spoor_time_date_create(argument,
                               1,
                               &spoor_time,
                               &spoor_time_today_test);
        eenheid_assert_int32(spoor_time.day,
                             spoor_time_today_test.day);

        TEST_END
    }
}
__EENHEID_END__
