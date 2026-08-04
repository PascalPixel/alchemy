#include "types.h"

/* Initialize the palette buffer, start its task, and clear both phase words. */
extern void Func_080001a8(void *source, void *destination);
extern s32 Func_080001d0(void);
extern void Func_080001c8(s32 slot, s32 bytes, void *source);
extern void Func_080000d0(void *callback, s32 frame_budget);

void Func_02001c1c(void)
{
    s32 slot;

    Func_080001a8((void *)0x02009fa0, (void *)0x0200a730);
    slot = Func_080001d0();
    *(u16 *)0x0200a6d0 = (u16)slot;
    Func_080001c8((s16)slot, 0x480, 0);
    *(u16 *)0x0200a6be = 0;
    *(u16 *)0x0200b030 = 0;
    Func_080000d0((void *)0x020098a5, 0xc76);
}
