#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"

void *Runtime_BumpAllocateAlternatePool(s32 size);
s32 Func_080f07f0(void *resource, s32 offset, s32 mode);

extern u16 Data_02004c00;
extern u16 Data_02004c08;
extern u16 Data_02004c04;

/* Allocate the scrolling display's object table, clear OBJ VRAM and fill its
   last tiles with colour 1, then lay out the entries: three strips of eight
   tall objects, a 16 x 6 grid of wide objects stepping 8 down and 32 across
   with 4 tiles each, and eight parked objects. Reset the scroll position and
   group, schedule the step and group callbacks, and load the 32 tile groups. */
void DisplayScroll_InitObjectTable(void)
{
    volatile u32 fill;
    u32 *entry;
    u32 i;
    u32 j;

    *(void **)0x02004c0c = Runtime_BumpAllocateAlternatePool(0x400);
    fill = 0;
    Dma_Set((void *)&fill, (void *)0x06010000, 0x85001800, (volatile u32 *)0x040000d4);
    fill = 0x11111111;
    Dma_Set((void *)&fill, (void *)0x06016000, 0x85000040, (volatile u32 *)0x040000d4);

    entry = *(u32 **)0x02004c0c;
    for (i = 0; i < 8; i++) {
        u32 *q = entry;

        *q++ = (i << 21) | 0x80004000;
        *q = 0x300;
        entry += 2;
    }
    for (i = 0; i < 8; i++) {
        u32 *q = entry;

        *q++ = (i << 21) | 0x80004088;
        *q = 0x300;
        entry += 2;
    }
    for (i = 0; i < 8; i++) {
        u32 *q = entry;

        *q++ = (i << 21) | 0x40004098;
        *q = 0x300;
        entry += 2;
    }
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 6; j++) {
            u32 *q = entry;
            u32 tile = (i * 3) * 8 + j * 4;

            *q++ = (i * 8 + 16) | ((j * 32 + 24) << 16) | 0x40004000;
            *q = tile;
            entry += 2;
        }
    }
    for (i = 0; i < 8; i++) {
        u32 *q = entry;

        *q++ = 0x00c000c0;
        *q = 0x300;
        entry += 2;
    }
    Data_02004c00 = 0;
    Data_02004c08 = 0;
    Data_02004c04 = 0;
    Scheduler_AddOrUpdateCallback(0x080f0539, 0x480);
    Scheduler_AddOrUpdateCallback(0x080f0615, 0xc80);
    for (i = 0; i < 32; i++)
        Func_080f07f0(*(void **)0x080f1220, i * 24, 1);
}
