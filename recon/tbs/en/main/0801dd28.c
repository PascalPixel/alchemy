/* Draft, not exact (2026-10-02, slice-2): 308 of 308 bytes, 9 instructions
   differ (score 260). What closed the rest: the registers the reference
   shares name the variables the source shared (one source pointer for the
   first two loops, one variable for the first loop's byte and the search
   count), and the search wraps with % 128.
   Remaining: the reference keeps the buffer start in lr and the tile table
   in sl; here they are swapped (7 register names). The allocator ranks the
   table 2 references over 21 instructions, the buffer start 3 over 36, so
   the table goes first; four more instructions of life for the table, or a
   fourth reference to the start, would turn it. And the search stores the
   next position one instruction early (before the slot's second shift). */
#include "DMA.H"

struct PaletteSlotWork {
    u8 unknown_000[0xda0];
    u8 used[0x100];
    u16 next;
};

extern struct PaletteSlotWork *gWindowWork;

u8 *Resource_GetTableEntry(s32 index);
extern u8 ResourceId_WindowTiles;

void Func_0801dd28(u16 *entry, u16 *mirror, s32 index, u8 *remap)
{
    struct PaletteSlotWork *work = gWindowWork;
    u8 *table = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    u32 slot = *(u8 *)entry;
    u8 buf[128];
    u8 *src;
    u8 *dst;
    u32 n;
    u32 i;

    {
        dst = buf;
        src = (u8 *)(0x06000000 + slot * 32);
        for (i = 0; i < 32; i++) {
            n = *src++;

            dst[0] = n & 15;
            dst[1] = n >> 4;
            dst += 2;
        }
    }
    {
        dst = buf;
        src = table + index * 32;
        for (i = 0; i < 32; i++) {
            u32 value = *src++;
            u32 color;

            color = remap[value & 15];
            if (color != 0)
                *dst = color;
            dst++;
            color = remap[value >> 4];
            if (color != 0)
                *dst = color;
            dst++;
        }
    }
    {
        u8 *p;

        dst = buf;
        for (i = 0, p = dst; i < 32; i++) {
            u32 value = p[0];

            value |= p[1] << 4;
            p += 2;
            *dst++ = value;
        }
    }
    if ((s8)slot >= 0) {
        for (n = 0; n < 128; n++) {
            u32 value = work->next;

            work->next = (value + 1) % 128;
            slot = (u8)value;
            if (work->used[slot] == 0)
                break;
        }
        work->used[slot] = 1;
        slot |= 0x80;
        *entry = slot | 0xf000;
        *mirror = slot | 0xf000;
    }
    Dma_Set(buf, (void *)(0x06000000 + slot * 32), 0x84000008, (volatile u32 *)0x040000d4);
}
