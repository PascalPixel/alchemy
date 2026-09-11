#include "types.h"
#include "scene.h"
#include "resource.h"

/* link/create_countdown_label_window.c */
s32 UiWindow_Create(s32, s32, s32, s32, s32);
void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);
extern u8 gRom[];
s32 Link_CreateCountdownLabelWindow(void)
{
    s32 handle = UiWindow_Create(0, 0, 6, 4, 6);

    UiText_DrawStringInWindow(gRom, handle, 0, 0);
    return handle;
}

/* resource/load_indexed_into_buffer.c */
extern u8 gVal[];

s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

s32 Resource_GetBuffer(s32 index, s32 value);

s32 Resource_LoadIndexedIntoBuffer(s32 arg0, s32 arg1)
{
    void *buffer = Runtime_AllocateHeapBlock(0x11, 0x608);
    u16 *base = GetResource((s32)gVal);
    void **slot = (void **)((u32)buffer + 0x604);
    void *target = (void *)((u32)base + base[arg1]);
    s32 ret;

    *slot = target;
    Sys_Apply(target, buffer);
    ret = Resource_GetBuffer(arg0, (s32)buffer);
    Sys_Do(0x11);
    return ret;
}
