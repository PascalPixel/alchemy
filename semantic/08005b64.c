#include "types.h"

/* The object behind the pointer at 0x03001f1c. src/08005868.c already sees its
   first 64 bytes as opaque padding with a word at 64; the three tail stores
   here resolve that padding into three parallel per-index arrays. */
struct Record_03001f1c {
    u8 states[16];
    u8 kinds[16];
    u16 values[16];
    u32 payload[4];
};

u32 Func_08005868(s32 arg0);

u32 Func_08005b64(s32 index)
{
    u32 zero;
    u32 buffer[4];
    struct Record_03001f1c *record;

    record = *(struct Record_03001f1c **)0x03001f1c;

    {
        u32 *source = &zero;
        u32 *destination = buffer;
        u32 control = 0x85000004;
        u32 *dma = (u32 *)0x040000d4;

        *source = 0;
        dma[0] = (u32)source;
        dma[1] = (u32)destination;
        dma[2] = control;
    }
    {
        volatile u32 *status = (volatile u32 *)0x040000d4;

        while ((status[2] & 0x80000000) != 0)
            ;
    }
    {
        u32 source = 0x080079b8;
        u32 *destination = buffer;
        u32 control = 0x84000002;
        u32 *dma = (u32 *)0x040000d4;

        dma[0] = source;
        dma[1] = (u32)destination;
        dma[2] = control;
    }
    {
        volatile u32 *status = (volatile u32 *)0x040000d4;

        while ((status[2] & 0x80000000) != 0)
            ;
    }
    {
        u8 *source = (u8 *)buffer;
        u32 *destination = record->payload;
        u32 control = 0x84000004;
        u32 *dma = (u32 *)0x040000d4;

        source[7] = 16;
        *(u16 *)(source + 10) = 0;
        dma[0] = (u32)source;
        dma[1] = (u32)destination;
        dma[2] = control;
    }
    {
        volatile u32 *status = (volatile u32 *)0x040000d4;

        while ((status[2] & 0x80000000) != 0)
            ;
    }

    if (Func_08005868(index) != 0)
        return 1;
    record->states[index] = 0;
    record->kinds[index] = 16;
    record->values[index] = 0;
    return 0;
}
