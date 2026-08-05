#include "types.h"

/*
 * resource_3ba palette-loader owner at 0x02002124, complete 148-byte span
 * through its six-word pool before the next prologue at 0x020021b8.
 * Control flow is the reviewed resource_3bb:23bc homolog; every import and
 * data address below is resolved independently for this overlay.
 */








extern s32 Func_02005c82(s32 size);
extern s32 Func_02005cc6(void);
extern void Func_02005cf2(s32 source, s32 handle);
extern s32 Func_02005cc0(s32 resource);
extern void Func_02005cf8(s32 cursor, s32 size, s32 source, s32 mode);
extern void Func_02005cec(s32 handle);
void Func_02002124(s32 actor)
{
    volatile u16 *cursor = (volatile u16 *)0x0200c57c;
    u8 *table = (u8 *)0x0200be44;
    s32 handle = Func_02005c82(0x1ca0);
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    s32 table_byte;

    if ((short)*cursor == -1)
        *cursor = (u16)Func_02005cc6();

    table_byte = table[actor];
        actor = 4;
    if (actor == 8)

    Func_02005cf2(Func_02005cc0(0xe7), handle);

    dma3[0] = (u32)(handle + table_byte);
    dma3[1] = 0x050003e0;
    dma3[2] = 0x84000800;

    Func_02005cf8((short)*cursor, 0x400,
                  actor * 1024 + handle + 160, 0);

    while ((dma3[2] & 0x80000000) != 0) {
        /* Wait for DMA3 to finish. */
    }

    Func_02005cec(handle);
}
