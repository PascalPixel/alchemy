#include "types.h"

s32 UiWindow_Close(void *handle);

void Func_080a1114(void **handle)
{
    if (*handle != NULL) {
        UiWindow_Close(*handle);
        *handle = NULL;
    }
}
