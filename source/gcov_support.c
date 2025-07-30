#include "gcov_support.h"

extern void *__gcov_ptrs[] __attribute__((weak));
extern void *__gcov_info_head __attribute__((weak));

void __gcov_flush(void)
{
    // This is a simplified flush stub for embedded systems
    // You would normally write this to memory buffer for gcda extraction
}
