#include "types.h"

s32 Func_08015018(void *handle);

void Func_080a1114(void **handle)
{
    if (*handle != NULL) {
        Func_08015018(*handle);
        *handle = NULL;
    }
}
