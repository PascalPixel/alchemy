#include "types.h"

s32 UiWindow_Close(void *handle);

void UiWindow_CloseIfOpen(void **handle)
{
    if (*handle != NULL) {
        UiWindow_Close(*handle);
        *handle = NULL;
    }
}
