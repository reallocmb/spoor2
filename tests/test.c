#include<eenheid.h>
#include<s_object.c>

__EENHEID_INIT__
SUITE(spoor_object.c)
{
    TEST(spoor_time_date_create())
    {
        time_t current_time = time(NULL);
        struct tm *today = localtime(&current_time);

        struct tm date;
        char argument[50];
        strcpy(argument, "d");
        spoor_time_date_create(argument, strlen(argument), &date);
        eenheid_assert_int32(date.tm_hour, -1);
        eenheid_assert_int32(date.tm_min, -1);

        strcpy(argument, "5d");
        spoor_time_date_create(argument, strlen(argument), &date);
        current_time = time(NULL);
        today = localtime(&current_time);
        eenheid_assert_int32(date.tm_mday, today->tm_mday + 5);
        eenheid_assert_int32(date.tm_hour, -1);
        eenheid_assert_int32(date.tm_min, -1);

        strcpy(argument, "d6161");
        spoor_time_date_create(argument, strlen(argument), &date);
        current_time = time(NULL);
        today = localtime(&current_time);
        eenheid_assert_int32(date.tm_hour, -1);
        eenheid_assert_int32(date.tm_min, -1);

        TEST_END
    }

    TEST(arguments_next())
    {
        char *arguments = "arg um ent";
        int32_t argument_length = arguments_next(&arguments, 0);
        eenheid_assert_int32(argument_length, 3);
        argument_length = arguments_next(&arguments, argument_length);
        eenheid_assert_int32(argument_length, 2);
        eenheid_assert_str(arguments, "um ent");
        argument_length = arguments_next(&arguments, argument_length);
        eenheid_assert_int32(argument_length, 3);
        eenheid_assert_str(arguments, "ent");

        argument_length = arguments_next(&arguments, argument_length);
        eenheid_assert_int32(argument_length, 0);
        TEST_END
    }

    TEST(spoor_time_create())
    {
        char argument[] = "9d900";
        SpoorTime spoor_time;
        spoor_time_create(argument, strlen(argument), &spoor_time);

        TEST_END
    }

    TEST(spoor_object_create())
    {
        TEST_END
    }

    TEST(spoor_object_edit())
    {
        TEST_END
    }
}
__EENHEID_END__
