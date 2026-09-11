#include "types.h"
#include "scene.h"
#include "abi/link/create_countdown_label_window.h"

s32 UiWindow_Create(s32, s32, s32, s32, s32);
void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);
extern u8 gRom[];
s32 Link_CreateCountdownLabelWindow(void)
{
    s32 handle = UiWindow_Create(0, 0, 6, 4, 6);

    UiText_DrawStringInWindow(gRom, handle, 0, 0);
    return handle;
}
