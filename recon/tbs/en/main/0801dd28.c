/* Draft, not exact (2026-10-01, slice-2): 308 of 308 bytes, 62 instructions
   differ (score 1675), all register choice and the order it forces.
   The reference keeps the buffer start in lr and the tile table in sl, then
   the mask 15 in sl; here the table takes lr and the buffer start sl. The
   three get equal weight from the allocator (3, 2 and 3 references), so the
   order follows their live lengths, and the reference's buffer start comes
   first. Its first buffer address is also a low register copied into ip
   (add r1, sp, #4; mov ip, r1; mov lr, ip), where this C reloads sp + 4
   straight into ip. Tried and worse: one start variable (dst = start = buf),
   array indexing in all three loops (94), static inline helpers (93).
   The slot search matches except its registers: the reference counts in
   r4 and loads 127 with movs; here a pool word is used because 127 loses
   its register. */
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
    u8 *dst;
    u32 i;

    {
        u8 *vram = (u8 *)(0x06000000 + slot * 32);

        dst = buf;
        for (i = 0; i < 32; i++) {
            u32 value = *vram++;

            dst[0] = value & 15;
            dst[1] = value >> 4;
            dst += 2;
        }
    }
    {
        u8 *tile = table + index * 32;

        dst = buf;
        for (i = 0; i < 32; i++) {
            u32 value = *tile++;
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
        u8 *src;

        dst = buf;
        src = buf;
        for (i = 0; i < 32; i++) {
            *dst++ = src[0] | (src[1] << 4);
            src += 2;
        }
    }
    if ((s8)slot >= 0) {
        u32 n;

        for (n = 0; n < 128; n++) {
            u32 value = work->next;

            work->next = (value + 1) & 127;
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
