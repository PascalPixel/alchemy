#include "FAR_RUNTIME.H"
#include "TYPES.H"
#include "UI.H"

s32 UiWork_FinalizeFar(void *handle);

s32 UiWindow_UpdateOrCreate(s32 *handle, s32 first, s32 second, s32 third, s32 fourth, s32 flags)
{
    s32 current;
    s32 masked;

    current = *handle;
    if (current != 0) {
        if (0x100 & flags) {
            return 0;
        }
        RenderOutput_RedrawSavedRectFar(current);
        return 0;
    }
    masked = flags & 0xff;
    *handle = UiWindow_CreateFar(first, second, third, fourth, masked);
    return 1;
}

void UiWindow_CloseIfOpen(void **handle)
{
    if (*handle != NULL) {
        UiWork_FinalizeFar(*handle);
        *handle = NULL;
    }
}
