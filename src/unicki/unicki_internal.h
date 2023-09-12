#ifndef UNICKI_INTERNAL_H
#define UNICKI_INTERNAL_H

#include<stdint.h>

#define UNICKI_TESTS_ALLOC 2

enum { PASSED, FAILED, PENDING };

typedef struct {
    char *suite_old;
    char *test_old;
    char *suite;
    char *test;
    uint8_t status;
    uint32_t passed_count;
    uint32_t failed_count;
    uint32_t pending_count;
} UnickiTest;

#endif
