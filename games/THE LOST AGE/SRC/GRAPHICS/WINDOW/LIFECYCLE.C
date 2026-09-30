#include "TYPES.H"

s32 UiWork_FinalizeFar(void *handle);

void UiWindow_CloseIfOpen(void **handle)
{
    if (*handle != NULL) {
        UiWork_FinalizeFar(*handle);
        *handle = NULL;
    }
}
