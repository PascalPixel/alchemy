#include "types.h"
#include "dma.h"

extern const void *DisplayScroll_LineTable[];
extern u32 *DisplayScroll_Objects;
s32 DisplayScroll_DrawLine(const u8 *text, s32 slot, s32 align);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Func_081a0cac(void);
void Func_081a0d78(void);

void Func_081a0fb8(void)
{
    s32 i;
    u32 *objects;

    for (i = 0; i < 32; i++)
        DisplayScroll_DrawLine(DisplayScroll_LineTable[0], i * 24, 1);
    Scheduler_RemoveCallback(Func_081a0cac);
    Scheduler_RemoveCallback(Func_081a0d78);
    objects = DisplayScroll_Objects;
    for (i = 0; i != 0x100; i++)
        *objects++ = 0;
    Dma_Set(DisplayScroll_Objects, (void *)0x07000000, 0x84000100, (volatile u32 *)0x040000d4);
}
