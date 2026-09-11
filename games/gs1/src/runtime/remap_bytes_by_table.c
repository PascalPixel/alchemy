#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* runtime/remap_bytes_by_table.c */
/* runtime/remap_bytes_by_table.c */
extern u8 gRom[];

void Runtime_RemapBytesByTable(u8 *buf, s32 cnt)
{
    s32 n;
    u8 *p;
    u8 *tbl;

    p = buf;
    tbl = gRom;
    n = cnt - 1;
    if (n != -1) {
        do {
            n -= 1;
            *p = tbl[*p];
            p += 1;
        } while (n != -1);
    }
}

/* graphics/renumber_filler_entries.c */
extern u32 gUnk[];

void Graphics_RenumberFillerEntries(void)
{
    u32 *p = gUnk;
    u32 cnt = 128 << 7;
    u32 mask = 0xfff;
    s32 no = -1;

    do {
        u32 value = *p++;
        u32 idx = value & mask;

        if (idx == mask) {
            if (no != (s32)idx) {
                no++;
            }
            value = value + no - idx;
            p[-1] = value;
        }
        cnt--;
    } while (cnt != 0);
}

/* map/shared/apply_work_origin_and_span.c */
/* map/shared/apply_work_origin_and_span.c */
void Map_ApplyWorkOriginAndSpan(void)
{
    s32 first;
    s32 third;
    s32 second;
    s32 *p;

    p = **(s32 ***)ADDR_03001E70;
    first = 0;
    second = 0;
    third = 0;
    if (p != NULL) {
        first = *p++;
        second = *p++;
        third = *p;
    }
    Map_Apply(first, (s32)((u32)third - (u32)second));
    Map_Check();
}

/* map/shared/render_palette_mapped_block.c */
static __inline__ void CopyBlock(u32 *map, u8 *base, u32 rowmod, u32 colmod, u32 parity)
{
    volatile u16 *colors;
    u8 *destination;
    u32 index = ((*map << 20) >> 18) + parity;

    colors = (volatile u16 *)0x02020000;
    colors += index;
    destination = base + (rowmod + colmod + parity) * 2;
    *(u16 *)destination = *colors;
    colors = (volatile u16 *)0x02020004;
    colors += index;
    *(u16 *)(destination + 64) = *colors;
}

void Map_RenderPaletteMappedBlock(u32 a0, s32 a1, s32 a2)
{
    u8 *destination = (u8 *)(0x06002800 + (a0 << 11));
    u32 row = ((a2 / 2) & 0x7F) << 7;
    u32 rowmod = (a2 & 30) << 5;
    u32 col = (a1 / 2) & 0x7F;
    u32 colmod = a1 & 30;
    u32 parity = a1 & 1;
    u32 counter;

    for (counter = 0; counter <= 10; counter++) {
        u32 *map = (u32 *)0x02010000;

        map += row + col;
        CopyBlock(map, destination, rowmod, colmod, parity);

        row = (row + 128) & 0x3F80;
        rowmod = (rowmod + 64) & 0x3C0;
    }
}

/* map/shared/set_work_flag_bits9_11.c */
struct State_080108c4 {
    u8 filler0[0x14];
    u16 flags;
};

extern struct State_080108c4 *volatile gCam;

void Map_SetWorkFlagBits9To11(u32 v)
{
    struct State_080108c4 *state = gCam;
    u32 mask = v & 0xe00;
    u32 flags = state->flags;

    flags = (flags & 0xf1ff) | mask;
    state->flags = flags;
}

/* map/shared/set_window_cell_tile.c */
#define ABS(v) ((v) < 0 ? -(v) : (v))

struct MapTileWindow_08010d48 {
    s32 *position;
    u8 unknown_004[0x134];
    u16 tiles[16][16];
};

extern struct MapTileWindow_08010d48 *gCam;

void Map_Run(s32 x, s32 y, s32 px, s32 py)
{
    struct MapTileWindow_08010d48 *window;
    s32 *position;
    s32 origin_x;
    s32 origin_y;
    s32 tile;

    window = gCam;
    origin_x = 0;
    origin_y = 0;
    position = window->position;
    if (position != 0) {
        origin_x = *position++;
        origin_y = position[1];
    }

    origin_x >>= 24;
    origin_y >>= 24;
    x >>= 4;
    y >>= 4;
    px >>= 3;
    py >>= 3;
    tile = (y << 4) + x;
    window->tiles[(py / 2) & 15][(px / 2) & 15] = tile;

    if (ABS(origin_x - px) <= 1 && ABS(origin_y - py) <= 1) {
        Map_SetRange(0, px / 2, py / 2, tile, 1);
        Map_SetRange(1, px / 2, py / 2, tile + 0x140, 1);
    }
}

/* map/shared/render_palette_mapped_row.c */
static __inline__ void CopyEntry(u8 *map, u8 *destination)
{
    u32 palette = *(u16 *)map;
    volatile u16 *colors = (volatile u16 *)0x02010000;
    colors += palette * 2;
    *(u16 *)destination = *colors++;
    *(u16 *)(destination + 64) = *colors;
}

void Map_RenderPaletteMappedRow(u32 value)
{
    u8 *map;
    u8 *destination;
    u32 counter;

    map = (u8 *)(0x02020000 + ((((s32)value / 2) & 31) << 7));
    destination = (u8 *)(0x06004000 + ((value & 62) << 6));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);

    destination += 0xFC0;
    map += 0xF80;
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);
}

/* map/shared/render_palette_mapped_column.c */
static __inline__ void CopyEntry(u8 *map, u8 *destination)
{
    u32 palette = *(u16 *)map;
    volatile u16 *colors = (volatile u16 *)0x02010000;
    colors += palette * 2;
    *(u16 *)destination = *colors++;
    *(u16 *)(destination + 64) = *colors;
}

void Map_RenderPaletteMappedColumn(u32 value)
{
    u8 *map;
    u8 *destination;
    u32 counter;

    map = (u8 *)(0x02020000 + ((((s32)value / 2) & 31) << 2));
    destination = (u8 *)(0x06004000 + (value & 62));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 128;
        map += 128;
    } while (counter <= 63);
}
