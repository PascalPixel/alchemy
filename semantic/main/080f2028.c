#include "layout_guard.h"
#include "types.h"

struct OamEntry_080f2028 {
    u32 attributes01;
    u32 attribute2;
};

struct OamWork_080f2028 {
    u8 padding00[8];
    s32 visibility_limit;
    s32 tick;
    u8 padding10[4];
    s32 scroll;
    struct OamEntry_080f2028 entries[120];
};

struct Dma_080f2028 {
    const void *source;
    void *destination;
    u32 control;
};

LAYOUT_SIZE_GUARD(OamEntry080f2028_Size, struct OamEntry_080f2028, 8);
LAYOUT_OFFSET_GUARD(
    OamWork080f2028_VisibilityLimit,
    struct OamWork_080f2028,
    visibility_limit,
    0x08);
LAYOUT_OFFSET_GUARD(
    OamWork080f2028_Tick,
    struct OamWork_080f2028,
    tick,
    0x0c);
LAYOUT_OFFSET_GUARD(
    OamWork080f2028_Scroll,
    struct OamWork_080f2028,
    scroll,
    0x14);
LAYOUT_OFFSET_GUARD(
    OamWork080f2028_Entries,
    struct OamWork_080f2028,
    entries,
    0x18);

extern struct OamWork_080f2028 *Data_03001efc;
extern u8 Data_03001d20;

s32 Func_080022ec(s32 value, s32 divisor);

static volatile u16 *const BlendControl_080f2028 =
    (volatile u16 *)0x04000050;
static volatile u16 *const BlendAlpha_080f2028 =
    (volatile u16 *)0x04000052;
static volatile struct Dma_080f2028 *const Dma3_080f2028 =
    (volatile struct Dma_080f2028 *)0x040000D4;
static void *const Oam_080f2028 = (void *)0x07000000;

static u8 WrappedCoordinate_080f2028(s32 coordinate) {
    return (u8)coordinate;
}

static u32 PackedAttributes_080f2028(s32 x, s32 y, u32 base) {
    return ((u32)x << 16) | WrappedCoordinate_080f2028(y) | base;
}

static s32 AnimatedRowY_080f2028(
    s32 row,
    s32 tile_size,
    s32 origin_y,
    s32 vertical_span
) {
    return WrappedCoordinate_080f2028(
        Func_080022ec(vertical_span * (row - 0x68), 0x50) +
        origin_y - tile_size);
}

static void Draw16PixelQuad_080f2028(
    struct OamEntry_080f2028 *entries,
    s32 first_entry,
    s32 row,
    s32 y,
    u32 tile
) {
    s32 left = row - 0x0c;
    s32 right = row + 0x04;

    entries[first_entry + 0].attributes01 =
        PackedAttributes_080f2028(left, y, 0x40002400);
    entries[first_entry + 1].attributes01 =
        PackedAttributes_080f2028(right, y, 0x50002400);
    entries[first_entry + 2].attributes01 =
        PackedAttributes_080f2028(left, y + 0x10, 0x60002400);
    entries[first_entry + 3].attributes01 =
        PackedAttributes_080f2028(right, y + 0x10, 0x70002400);
    entries[first_entry + 0].attribute2 = tile;
    entries[first_entry + 1].attribute2 = tile;
    entries[first_entry + 2].attribute2 = tile;
    entries[first_entry + 3].attribute2 = tile;
}

static void Draw32PixelQuad_080f2028(
    struct OamEntry_080f2028 *entries,
    s32 first_entry,
    s32 row,
    s32 y,
    u32 tile
) {
    s32 left = row - 0x1c;
    s32 right = row + 0x04;

    entries[first_entry + 0].attributes01 =
        PackedAttributes_080f2028(left, y, 0x80002400);
    entries[first_entry + 1].attributes01 =
        PackedAttributes_080f2028(right, y, 0x90002400);
    entries[first_entry + 2].attributes01 =
        PackedAttributes_080f2028(left, y + 0x20, 0xa0002400);
    entries[first_entry + 3].attributes01 =
        PackedAttributes_080f2028(right, y + 0x20, 0xb0002400);
    entries[first_entry + 0].attribute2 = tile;
    entries[first_entry + 1].attribute2 = tile;
    entries[first_entry + 2].attribute2 = tile;
    entries[first_entry + 3].attribute2 = tile;
}

static s32 BuildAnimatedEntries_080f2028(
    struct OamWork_080f2028 *work,
    s32 origin_y,
    s32 vertical_span
) {
    const u8 *rows = (const u8 *)0x080F39AB;
    s32 row;
    s32 y;

    if (work->tick & 1) {
        row = rows[1];
        y = AnimatedRowY_080f2028(row, 0x10, origin_y, vertical_span);
        Draw16PixelQuad_080f2028(work->entries, 0, row, y, 0xe8);

        row = rows[3];
        y = AnimatedRowY_080f2028(row, 0x10, origin_y, vertical_span);
        Draw16PixelQuad_080f2028(work->entries, 4, row, y, 0xe0);

        row = rows[5];
        y = AnimatedRowY_080f2028(row, 0x20, origin_y, vertical_span);
        Draw32PixelQuad_080f2028(work->entries, 8, row, y, 0xa0);
        return 12;
    }

    row = rows[0];
    y = AnimatedRowY_080f2028(row, 0x10, origin_y, vertical_span);
    Draw16PixelQuad_080f2028(work->entries, 0, row, y, 0xe8);

    row = rows[2];
    y = AnimatedRowY_080f2028(row, 0x10, origin_y, vertical_span);
    work->entries[4].attributes01 =
        PackedAttributes_080f2028(row - 0x0c, y, 0x80002400);
    work->entries[4].attribute2 = 0x80;

    row = rows[4];
    y = AnimatedRowY_080f2028(row, 0x20, origin_y, vertical_span);
    Draw32PixelQuad_080f2028(work->entries, 5, row, y, 0xc0);
    return 9;
}

static void CopyOam_080f2028(
    const void *source,
    void *destination,
    u32 words
) {
    Dma3_080f2028->source = source;
    Dma3_080f2028->destination = destination;
    Dma3_080f2028->control = 0x84000000 | words;
}

/*
 * Build the frame's 120-entry OAM image, transfer it, append the final marker
 * at entry 120, then refresh the first four entries after the scratch write.
 */
void Func_080f2028(void) {
    struct OamWork_080f2028 *work = Data_03001efc;
    s32 camera_offset = *(volatile u16 *)0x03001AD6;
    s32 origin_y;
    s32 vertical_span;
    s32 entry_count = 0;

    if (Data_03001d20 == 0) {
        work->tick++;
        if ((work->tick & 3) == 0) {
            work->scroll++;
        }
    }

    origin_y = 0x30 - camera_offset;
    vertical_span = (0x90 - work->scroll) - origin_y;
    if (work->visibility_limit < 0x118) {
        entry_count = BuildAnimatedEntries_080f2028(
            work, origin_y, vertical_span);
    }

    while (entry_count <= 119) {
        work->entries[entry_count].attributes01 = 0x400020a0;
        entry_count++;
    }

    *BlendControl_080f2028 = 0x3f50;
    *BlendAlpha_080f2028 = 0x0e0e;
    CopyOam_080f2028(
        work->entries,
        Oam_080f2028,
        (u32)entry_count * 2);

    work->entries[12].attributes01 =
        WrappedCoordinate_080f2028(0x20 - camera_offset) | 0xc05c2000;
    work->entries[12].attribute2 = 0x0800;
    CopyOam_080f2028(
        &work->entries[12],
        (u8 *)Oam_080f2028 + entry_count * 8,
        2);
    CopyOam_080f2028(work->entries, Oam_080f2028, 8);
}
