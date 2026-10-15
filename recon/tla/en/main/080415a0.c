/* Near miss, differ 8 of 144 (four reordered rows), 2026-10-15: The Broken
 * Seal's UiWindow_OverlayTilePattern; this game finds the free tile in a
 * do-while (as a for loop the first pass is peeled). Remaining: the native
 * stores the mirror argument to its stack slot right after sub sp (this
 * after the first two copies) and in the search loop masks the next cursor
 * and stores it before it zero-extends the slot (ands; lsls; strh; lsrs,
 * where this gives lsls; ands; lsrs; strh). The tile cursor is the
 * halfword at the work's start.
 */
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "dma.h"
#include "io_reg.h"

extern char ResourceId_WindowTiles;
void *Resource_GetTableEntry(s32 id);

void Func_080415a0(u16 *entry, u16 *mirror, s32 pattern, u8 *remap)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    u8 pixels[128];
    u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_WindowTiles);
    u32 slot = *(u8 *)entry;
    u8 *src;
    u8 *dst;
    u32 n;
    u32 i;

    dst = pixels;
    src = (u8 *)(0x06000000 + 32 * slot);
    for (i = 0; i < 32; i++) {
        n = *src++;
        *dst++ = n & 15;
        *dst++ = n >> 4;
    }

    dst = pixels;
    src = sheet + pattern * 32;
    for (i = 0; i < 32; i++) {
        u32 value = *src++;
        u32 color = remap[value & 15];

        if (color != 0)
            *dst = color;
        dst++;
        color = remap[value >> 4];
        if (color != 0)
            *dst = color;
        dst++;
    }

    {
        u8 *p;

        dst = pixels;
        for (i = 0, p = dst; i < 32; i++) {
            u32 value = p[0] | p[1] << 4;

            p += 2;
            *dst++ = value;
        }
    }

    if ((s8)slot >= 0) {
        n = 0;
        do {
            u32 value = *(u16 *)work;
            u32 following = (value + 1) % 128;

            slot = (u8)value;
            *(u16 *)work = following;
            if (work->tileAttributes[slot] == 0)
                break;
            n++;
        } while (n < 128);
        work->tileAttributes[slot] = 1;
        slot |= 0x80;
        *entry = slot | 0xf000;
        *mirror = slot | 0xf000;
    }
    Dma_Set(pixels, (void *)(0x06000000 + slot * 32), 0x84000008, REG_DMA3);
}
