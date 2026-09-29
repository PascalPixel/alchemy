/* 2026-09-29 alchemy permute: score 7965 to 6875 on the permuter's scorer
   (0 is exact); remaining 52 register-only, 32 operand, 25 reordered, 18
   inserted, 26 deleted. Kept rewrites: 11x swap commutative operands, 11x
   introduce a temporary, 8x reorder local declarations, 7x reorder
   independent statements, 6x pointer arithmetic or indexing, 6x test truth
   or compare with zero, 5x add a same-width cast, 4x drop a same-width
   cast, 3x move an assignment into or out of a condition, 1x remove a
   temporary, 1x toggle register. FAKEMATCH: the permuter's temporaries,
   register hints and swapped operand orders below only steer allocation
   and scheduling; no programmer would write them, so they stay tagged
   until a natural spelling replaces them. */
/* Draft, not exact: 562 of 592 bytes, 274 differing halfwords.
   Complete owner: [0x08018cac, 0x08018efc). Recovered glyph allocation,
   DMA transfer and sprite attributes. Branch layout, pointer arithmetic,
   literal pools and live ranges still differ across the owner. */
#include "TYPES.H"
#include "DMA.H"

union UiTextWindowFlags {
    u16 value;
    struct {
        unsigned short unused : 3;
        unsigned short special : 1;
        unsigned short unknown : 12;
    } bits;
};

struct UiTextWindow {
    u8 unknown_00[8];
    u16 width;
    u8 height;
    u8 unknown_0b;
    u16 x;
    u16 y;
    u8 unknown_10[6];
    union UiTextWindowFlags flags;
};

struct UiTextWork {
    u8 unknown_00[0xea8];
    u16 color_a;
    u16 unknown_eaa;
    u16 color_b;
    u16 color_c;
    u16 entries[0x200];
    u16 free_count;
    u16 entry_count;
};

union UiRenderTable {
    u32 value;
    struct {
        u16 low;
        u16 high;
    } half;
    struct {
        unsigned short index : 10;
        unsigned short reserved : 6;
        u16 high;
    } bits;
};

struct UiRenderOutput {
    s32 next;
    u8 one4;
    u8 one5;
    u16 x;
    u16 y;
    u8 unknown_0a[4];
    u8 index;
    u8 sentinel;
    u8 unknown_10[4];
    s32 packed;
    union UiRenderTable table;
};

struct UiGlyphMetrics {
    u8 bytes[128];
};

extern struct UiTextWork *gWindowWork;
extern u8 *Resource_GetTableEntry(u32 resource);
extern void *Runtime_BumpAllocate(s32 size);
extern void Runtime_BumpFree(void *buffer);
extern void *RenderOutput_AcquireFree(void);
extern void RenderOutput_AppendToList(void *window, void *output);
extern s32 Resource_FindFreeEntry(void);
extern s32 UiText_RenderGlyphPair(s32 character, struct UiGlyphMetrics *metrics);
extern s32 _call_via_r6(void *window, s32 character, s32 x, s32 y, u8 *resource);

s32 UiText_DrawGlyph(
    struct UiTextWindow *window,
    s32 character,
    s32 x,
    s32 y,
    s32 mode)
{
    struct UiTextWork *work;
    struct UiGlyphMetrics metrics;
    struct UiRenderOutput *output;
    u8 *resource;
    s32 glyph;
    u8 *buffer;
    s32 size;
    s32 saved_x;
    u16 global_x;
    s32 saved_y;
    u16 global_y;
    s32 result;
    s32 index;
    u16 tmp8;

    saved_x = x;
    glyph = character;
    work = gWindowWork;
    global_x = work->free_count;
    saved_y = y;
    global_y = work->color_a;
    if (mode != 1 && window->flags.bits.special != 0) {
        if (*((struct UiTextWindow ***)0x03001ee4)[0] == window) {
            Resource_GetTableEntry(0x14);
            Resource_GetTableEntry(0x13);
            if (glyph == 32)
                return 3;
        }
        resource = Resource_GetTableEntry(0x13);
        if (glyph == 32)
            return 4;
        size = 0x318;
        buffer = Runtime_BumpAllocate(size);
        Dma_Set((void *)0x080155d0, buffer, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
        result = _call_via_r6(window, glyph, saved_x, saved_y, resource);
        Runtime_BumpFree(buffer);
        return result;
    }
    result = 5;
    if (glyph == 32)
        return result;
    if ((output = RenderOutput_AcquireFree()) != 0 == 0)
        return 0;
    output->one4 = 0;
    index = (output - (struct UiRenderOutput *)((u8 *)work + 0x698)) * 4;
    output->one5 = 1;
    if (1 == mode) {
        result = 1;
        output->one5 = 2;
    } else {
        switch (work->color_b) {
        case 3:
            output->one5 = 5;
            break;
        case 4:
            output->one5 = 6;
            *(u16 *)&((u8 *)output)[12] = 8;
            break;
        case 5:
            output->one5 = 7;
            *(u16 *)((s32)12 + (u8 *)output) = 0;
            break;
        case 2:
            (*output).one5 = 4;
            *(u16 *)((u8 *)output + 12) = 0;
            break;
        }
        if ((u32)!(result = UiText_RenderGlyphPair(glyph, &metrics)))
            result = 1;
    }
    if ((u8)output->one5 == 2) {
        u16 *entry = (u16 *)(0x12b6 + (u8 *)work);
        u16 slot;
        u8 tmp;
        s32 tmp2;
        register s32 tmp4;
        s32 tmp6;
        u16 tmp9;
        if (99 == (slot = *entry)) {
            slot = Resource_FindFreeEntry();
            *entry = slot;
        }
        tmp2 = window->x + window->width + 0xfffe;
        tmp6 = -0x200;
        tmp9 = *(u16 *)((u8 *)output + 22);
        *(u16 *)((u8 *)output + 22) = ((((u32)tmp2 << 3) + 4) & 0x1ff) | (tmp6 & tmp9);
        tmp = (u8)window->y;
        tmp4 = window->height + tmp + 254;
        *(u8 *)((u8 *)output + 20) = (u8)(tmp4 << 3) - 1;
    } else {
        s32 slot = *(u16 *)((u8 *)work + 0x12b8) + index;
        s32 tmp3;
        s32 tmp5;
        u8 *tmp7;
        s32 tmp10;
        Dma_Set(&metrics, (void *)(0x06010000 + (slot << 5)), 0x84000020, (volatile u32 *)0x040000d4);
        tmp10 = global_y >> 1;
        tmp3 = window->y << 3;
        tmp5 = saved_y + tmp10 + tmp3 + 0xfffe;
        *(u16 *)((u8 *)output + 20) = tmp5;
        tmp7 = (u8 *)output + 22;
        *(u16 *)tmp7 = (((*window).x << 3) + (saved_x + (global_x >> 1)) + 2) | 0x4000;
        output[0].table.half.low = slot;
    }
    output->sentinel = 254;
    tmp8 = *(u16 *)((u8 *)output + 22);
    output->x = 0x1ff & tmp8;
    output->y = *((u8 *)output + 20);
    output->index = index;
    output->next = 0;
    RenderOutput_AppendToList(window, output);
    return result;
}
