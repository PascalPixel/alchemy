#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "RAM_BUFFER.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "MAP_SCROLL.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
void Map_SetCameraCenter(s32 x, s32 y);

/* FAKEMATCH: helper scope gives each cell its own table-pointer lifetime,
   following the independently matched neighbouring column renderer. */
static __inline__ void CopyCell(u32 *map, u8 *base, u32 rowmod, u32 colmod)
{
    u32 *tiles;
    u8 *dest;
    u32 index = (*map << 20) >> 19;

    tiles = (u32 *)Ram_MapBlocks;
    tiles += index;
    dest = base + (rowmod + colmod) * 2;
    *(u32 *)dest = *tiles;
    tiles = (u32 *)(Ram_MapBlocks + 4);
    tiles += index;
    *(u32 *)(dest + 64) = *tiles;
}

static __inline__ void CopyBlock(u32 *map, u8 *base, u32 rowmod, u32 colmod, u32 parity)
{
    u16 *colors;
    u8 *destination;
    u32 index = ((*map << 20) >> 18) + parity;

    colors = (u16 *)Ram_MapBlocks;
    colors += index;
    destination = base + (rowmod + colmod + parity) * 2;
    *(u16 *)destination = *colors;
    colors = (u16 *)(Ram_MapBlocks + 4);
    colors += index;
    *(u16 *)(destination + 64) = *colors;
}

extern struct MapRenderWork *gMapWork;
void Map_UpdateLayerScroll(void);

static __inline__ void CopyCameraCell(u32 *map, u8 *base, u32 rowmod, u32 colmod)
{
    u32 *tiles;
    u8 *dest;
    u32 index = (*map << 20) >> 19;

    tiles = (u32 *)Ram_MapBlocks;
    tiles += index;
    /* FAKEMATCH: integer address addition preserves the add operand order. */
    dest = (u8 *)((rowmod + colmod) * 2 + (u32)base);
    *(u32 *)dest = *tiles;
    tiles = (u32 *)(Ram_MapBlocks + 4);
    tiles += index;
    *(u32 *)(dest + 64) = *tiles;
}

void Map_ApplyWorkOriginAndSpan(void)
{
    s32 first;
    s32 third;
    s32 second;
    s32 *p;

    p = **(s32 ***)((u32)&gCam);
    first = 0;
    second = 0;
    third = 0;
    if (p != NULL) {
        first = *p++;
        second = *p++;
        third = *p;
    }
    Map_SetCameraCenter(first, (s32)((u32)third - (u32)second));
    Map_UpdateLayerScroll();
}

/* Exact candidate: whole owner [0800fec8, 0800ff54), 140 bytes, four own pool
   words. Baseline 2026-09-26: 144 bytes, 64 differing halfwords, 59
   aligned edits. No callees. Raw caller 08010000 selects this row update
   on a vertical scroll boundary and ff54 on a horizontal boundary.
   H1: use the adopted ff54 family's typed per-cell helper and explicit
   outer row/base lifetimes, with paired word stores for this row owner.
   Prediction: only r8/sl saved, base in ip, all four pools reloaded where
   the reference owns them. Accept only exact 140 bytes plus landing gates.
   H1 result: exact 140/140 bytes, zero differing halfwords/aligned edits,
   topology equal; complete normalized comparison read. The same-source
   family evidence is the adopted ff54 implementation, not another project.
   The ff54 owner is unchanged. */
void Map_RenderMetatileRow(u32 a0, s32 a1, s32 a2)
{
    u8 *dest = (u8 *)(0x06002800 + (a0 << 11));
    u32 row = ((a2 / 2) & 0x7F) << 7;
    u32 rowmod = (a2 & 30) << 5;
    u32 col = (a1 / 2) & 0x7F;
    u32 colmod = a1 & 30;
    u32 counter;

    for (counter = 0; counter <= 15; counter++) {
        u32 *map = (u32 *)Ram_MapCellBuffer;

        map += row + col;
        CopyCell(map, dest, rowmod, colmod);
        col = (col + 1) & 0x7F;
        colmod = (colmod + 2) & 30;
    }
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
        u32 *map = (u32 *)Ram_MapCellBuffer;

        map += row + col;
        CopyBlock(map, destination, rowmod, colmod, parity);

        row = (row + 128) & 0x3F80;
        rowmod = (rowmod + 64) & 0x3C0;
    }
}

void Map_UpdateLayerScroll(void)
{
    struct MapScrollWork *work;
    struct MapLayerScroll *layer;
    s32 *origin;
    s32 x, y;
    s32 min_x, min_y, max_x, max_y;
    s32 base_x, base_y;
    u32 i;

    work = *(struct MapScrollWork **)&gMapWork;
    origin = work->origin;
    layer = work->layers;
    if (origin == 0)
        return;

    base_x = *origin++ - 0x780000;
    {
        s32 height = *origin++;
        base_y = *origin - height - 0x600000;
    }
    min_x = work->min_x + work->shake_x;
    max_x = work->max_x - work->shake_x - 0xf00000;
    min_y = work->min_y + work->shake_y;
    max_y = work->max_y - work->shake_y - 0xa00000;
    if (min_x > max_x)
        max_x = min_x;
    if (min_y > max_y)
        max_y = min_y;
    if (base_x < min_x)
        base_x = min_x;
    if (base_x > max_x)
        base_x = max_x;
    if (base_y < min_y)
        base_y = min_y;
    if (base_y > max_y)
        base_y = max_y;

    if (work->shake_x != 0) {
        s32 first = Random16();
        s32 second = Random16();
        s32 amplitude = work->shake_x;
        base_x += Iwram_MulQ16(amplitude, first - second);
        work->shake_x = Iwram_MulQ16(amplitude, work->shake_decay);
    }
    if (work->shake_y != 0) {
        s32 first = Random16();
        s32 second = Random16();
        s32 amplitude = work->shake_y;
        base_y += Iwram_MulQ16(amplitude, first - second);
        work->shake_y = Iwram_MulQ16(amplitude, work->shake_decay);
    }
    work->view_x = base_x;
    work->view_y = base_y;
    for (i = 0; i < 3; i++, layer++) {
        base_x = Iwram_MulQ16(work->view_x, layer->scale_x);
        base_y = Iwram_MulQ16(work->view_y, layer->scale_y);
        if (layer->speed_x != 0) {
            layer->phase_x += layer->speed_x;
            base_x = (base_x + layer->phase_x) &
                (((u32)layer->mask_x << 19) | 0x7ffff);
        }
        if (layer->speed_y != 0) {
            layer->phase_y += layer->speed_y;
            base_y = (base_y + layer->phase_y) &
                (((u32)layer->mask_y << 19) | 0x7ffff);
        }
        base_x += layer->offset_x;
        base_y += layer->offset_y;
        x = base_x / 0x80000;
        y = base_y / 0x80000;
        if ((layer->x ^ base_x) & 0x80000) {
            if (layer->x < base_x)
                Map_RenderPaletteMappedBlock(i, x + 30, y);
            else
                Map_RenderPaletteMappedBlock(i, x, y);
        }
        if ((layer->y ^ base_y) & 0x100000) {
            if (layer->y < base_y)
                Map_RenderMetatileRow(i, x, y + 20);
            else
                Map_RenderMetatileRow(i, x, y);
        }
        gBgScroll[3 - i].x = base_x >> 16;
        gBgScroll[3 - i].y = base_y >> 16;
        layer->x = base_x;
        layer->y = base_y;
    }
}

void Map_SetCameraCenter(s32 x, s32 y)
{
    u32 no;
    u8 *dest;
    struct MapScrollWork *work = gCam;
    struct MapLayerScroll *layer = work->layers;

    x -= 0x780000;
    y -= 0x600000;
    if (x < work->min_x)
        x = work->min_x;
    if (x > work->max_x - 0xf00000)
        x = work->max_x - 0xf00000;
    if (y < work->min_y)
        y = work->min_y;
    if (y > work->max_y - 0xa00000)
        y = work->max_y - 0xa00000;
    work->view_x = x;
    work->view_y = y;

    for (no = 0; no < 3; no++) {
        if (work->enabled[no]) {
            u32 rows = 22;
            u32 row, col, rowmod, colmod, i, j;

            x = Iwram_MulQ16(work->view_x, layer->scale_x);
            y = Iwram_MulQ16(work->view_y, layer->scale_y);
            if (layer->speed_x) {
                layer->phase_x += layer->speed_x;
                x = (x + layer->phase_x) & ((layer->mask_x << 19) | 0x7ffff);
            }
            if (layer->speed_y) {
                layer->phase_y += layer->speed_y;
                y = (y + layer->phase_y) & ((layer->mask_y << 19) | 0x7ffff);
                rows = 32;
            }
            x += layer->offset_x;
            y += layer->offset_y;
            layer++;
            x /= 0x80000;
            y /= 0x80000;
            dest = (u8 *)(0x06002800 + (no << 11));
            row = ((y / 2) & 127) << 7;
            rowmod = (y & 30) << 5;
            for (i = 0; i < rows / 2; i++) {
                col = (x / 2) & 127;
                colmod = x & 30;
                for (j = 0; j <= 15; j++) {
                    u32 *map = (u32 *)Ram_MapCellBuffer;
                    map += row + col;
                    CopyCameraCell(map, dest, rowmod, colmod);
                    col = (col + 1) & 127;
                    colmod = (colmod + 2) & 30;
                }
                row = (row + 128) & 0x3f80;
                rowmod = (rowmod + 64) & 0x3c0;
            }
        }
    }
}
