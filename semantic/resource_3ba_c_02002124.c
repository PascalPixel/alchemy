typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_3ba palette-loader owner at 0x02002124, complete 148-byte span
 * through its six-word pool before the next prologue at 0x020021b8.
 * Control flow is the reviewed resource_3bb:23bc homolog; every import and
 * data address below is resolved independently for this overlay.
 */

extern s32 Func_08000170(s32 size);
extern void Func_08000178(s32 handle);
extern void Func_080001a8(s32 source, s32 handle);
extern void Func_080001c8(s32 cursor, s32 size, s32 source, s32 mode);
extern s32 Func_080001d0(void);
extern s32 Func_08000290(s32 resource);

void Func_02002124(s32 actor)
{
    volatile u16 *cursor = (volatile u16 *)0x0200c57c;
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    u8 *table = (u8 *)0x0200be44;
    s32 handle = Func_08000170(0x1ca0);
    s32 table_byte;

    if ((short)*cursor == -1)
        *cursor = (u16)Func_080001d0();

    table_byte = table[actor];
    if (actor == 8)
        actor = 4;

    Func_080001a8(Func_08000290(0xe7), handle);

    dma3[0] = (u32)(handle + table_byte);
    dma3[1] = 0x050003e0;
    dma3[2] = 0x84000800;

    Func_080001c8((short)*cursor, 0x400,
                  actor * 1024 + handle + 160, 0);

    while ((dma3[2] & 0x80000000) != 0) {
        /* Wait for DMA3 to finish. */
    }

    Func_08000178(handle);
}
