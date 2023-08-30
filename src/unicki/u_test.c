#include"unicki_internal.h"

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

void unicki_assert(UnickiTest *unicki_test, bool condition, char *file, uint32_t line)
{
    if (condition)
    {
        if (strcmp(unicki_test->suite, unicki_test->suite_old) != 0)
        {
            putchar('\n');
            printf("Suite: %s:", unicki_test->suite);
        }
        if (strcmp(unicki_test->test, unicki_test->test_old) != 0)
        {
            putchar('\n');
            printf("\t%s:\n", unicki_test->test);
            putchar('\t');
        }
        printf("\x1b[32m\u2714\x1b[0m");
        unicki_test->passed_count++;
    }
    else
    {
        printf("\x1b[31m\u2718file: %s | line: %d\x1b[0m", file, line);
        unicki_test->failed_count++;
    }

    unicki_test->suite_old = unicki_test->suite;
    unicki_test->test_old = unicki_test->test;
}

void unicki_test_create(UnickiTest *unicki_test, char *suite, char *test)
{
    unicki_test->suite = suite;
    unicki_test->test = test;
}

void unicki_stats_print(UnickiTest *unicki_test)
{
    putchar('\n');
    printf("PASSED: %5d\n", unicki_test->passed_count);
    printf("FAILED: %5d\n", unicki_test->failed_count);
    printf("PENDING: %4d\n", unicki_test->pending_count - (unicki_test->passed_count + unicki_test->failed_count));
}
