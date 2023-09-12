#ifndef UNICKI_H
#define UNICKI_H

#include"unicki_internal.h"

#include<stdbool.h>

#define UNICKI_VERSION_MAJOR 0
#define UNICKI_VERSION_MINOR 0
#define UNICKI_VERSION_PATCH 0

void unicki_assert(UnickiTest *unicki_test, bool condition, char *file, uint32_t line);

void unicki_test_create(UnickiTest *unicki_test, char *suite, char *test);
void unicki_stats_print(UnickiTest *unicki_test);

#define TEST_INIT \
    UnickiTest unicki_test = { \
        .test = "", \
        .suite = "", \
        .test_old = "", \
        .suite_old = "" \
    }; \
int main(void) \
{ \


#define TEST(suite, test) unicki_test_create(&unicki_test, #suite, #test); unicki_test.pending_count++;

#define unicki_assert(condition) unicki_assert(&unicki_test, condition, __FILE__, __LINE__)
#define TEST_END \
    unicki_stats_print(&unicki_test); \
    return 0; \
} 

#endif
