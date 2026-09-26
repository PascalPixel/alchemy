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
extern s32 Func_080178b0(s32 character, struct UiGlyphMetrics *metrics);
extern s32 Func_080072fc(void *window, s32 character, s32 x, s32 y, u8 *resource);

s32 UiText_DrawGlyph(
    struct UiTextWindow *window,
    s32 character,
    s32 x,
    s32 y,
    s32 mode)
{
    struct UiTextWork *work;
    struct UiRenderOutput *output;
    struct UiGlyphMetrics metrics;
    u8 *buffer;
    u8 *resource;
    s32 glyph;
    s32 saved_x;
    s32 saved_y;
    s32 size;
    u16 global_x;
    u16 global_y;
    s32 result;
    s32 index;

    saved_x = x;
    saved_y = y;
    glyph = character;
    work = gWindowWork;
    global_x = work->free_count;
    global_y = work->color_a;
    if (mode != 1 && window->flags.bits.special != 0) {
        if (**(struct UiTextWindow ***)0x03001ee4 == window) {
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
        Dma_Set((void *)0x080155d0, buffer,
            0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
        result = Func_080072fc(window, glyph, saved_x, saved_y, resource);
        Runtime_BumpFree(buffer);
        return result;
    }

    result = 5;
    if (glyph == 32)
        return result;
    output = RenderOutput_AcquireFree();
    if (output == 0)
        return 0;
    output->one4 = 0;
    output->one5 = 1;
    index = (s32)(output - (struct UiRenderOutput *)((u8 *)work + 0x698)) * 4;

    if (mode == 1) {
        output->one5 = 2;
        result = 1;
    } else {
        switch (work->color_b) {
        case 3:
            output->one5 = 5;
            break;
        case 4:
            output->one5 = 6;
            *(u16 *)((u8 *)output + 12) = 8;
            break;
        case 5:
            output->one5 = 7;
            *(u16 *)((u8 *)output + 12) = 0;
            break;
        case 2:
            output->one5 = 4;
            *(u16 *)((u8 *)output + 12) = 0;
            break;
        }
        result = Func_080178b0(glyph, &metrics);
        if (result == 0)
            result = 1;
    }

    if (output->one5 == 2) {
        u16 *entry = (u16 *)((u8 *)work + 0x12b6);
        u16 slot;

        slot = *entry;
        if (slot == 99) {
            slot = Resource_FindFreeEntry();
            *entry = slot;
        }
        *(u16 *)((u8 *)output + 22) =
            (*(u16 *)((u8 *)output + 22) & -0x200)
            | (((window->x + window->width + 0xfffe) << 3) + 4 & 0x1ff);
        *(u8 *)((u8 *)output + 20) =
            (u8)(((u8)window->y + (u8)window->height + 254) << 3) - 1;
    } else {
        s32 slot = *(u16 *)((u8 *)work + 0x12b8) + index;
        Dma_Set(&metrics, (void *)(0x06010000 + (slot << 5)),
            0x84000020, (volatile u32 *)0x040000d4);
        *(u16 *)((u8 *)output + 20) =
            saved_y + (global_y >> 1) + (window->y << 3) + 0xfffe;
        *(u16 *)((u8 *)output + 22) =
            (saved_x + (global_x >> 1) + (window->x << 3) + 2) | 0x4000;
        output->table.half.low = slot;
    }
    output->sentinel = 254;
    output->x = *(u16 *)((u8 *)output + 22) & 0x1ff;
    output->y = *(u8 *)((u8 *)output + 20);
    output->index = index;
    output->next = 0;
    RenderOutput_AppendToList(window, output);
    return result;
}
