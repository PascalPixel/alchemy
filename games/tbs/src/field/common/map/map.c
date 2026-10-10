#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "iwram_call.h"
#include "dma.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "global_cells.h"
#include "map_scroll.h"
#include "resource.h"
#include "resource_ids.h"
#include "flag_ids.h"

extern u8 gMapCellBuffer[];

typedef void (*ConvertFn)(void *dst, const void *src, const void *saved);

static __inline__ void CopyWords(void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: needed to match: a direct call changes Tilemap_ConvertBuffer from mov r6, r8 to mov r6, sl (52/56 assembly lines). */
    Iwram_CopyWords(dst, src, size);
}

void Graphics_RenumberFillerEntries(void)
{
    u32 *p = (u32 *)gMapCellBuffer;
    u32 cnt = 128 << 7;
    u32 mask = 0xfff;
    s32 no = -1;

    do {
        u32 value = *p++;
        u32 idx = value & mask;

        if (idx == mask) {
            if (no != idx) {
                no++;
            }
            value = value + no - idx;
            p[-1] = value;
        }
        cnt--;
    } while (cnt != 0);
}

void Tilemap_ConvertBuffer(void)
{
    u8 *saved;
    ConvertFn routine;
    u32 size;

    size = 0x8000;
    /* FAKEMATCH: needed to match: the two do-while blocks keep each allocation next to the transfer that uses it */
    do {
        saved = (u8 *)Runtime_BumpAllocateAlternatePool(size);
        CopyWords(saved, Ram_MapCellBuffer, size);
    } while (0);
    {
        u32 code_size = (u32)Tile_ConvertMapCodeSize;

        do {
            routine = (ConvertFn)MemAlloc(code_size);
            Dma_Set((void *)Tile_ConvertMap, routine, 0x84000000 | (code_size >> 2),
                    REG_DMA3);
        } while (0);
    }
    routine((gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
    MemFree(routine);
    MemFree(saved);
}

s32 GameFlag_TestFar(s32 flag);

#define Scheduler_AddOrUpdateCallback ((void (*)(void (*)(void), s32))Scheduler_AddOrUpdateCallback)

static __inline__ void CopyCell(u32 *map, u8 *base, u32 rowmod, u32 colmod)
{
    /* FAKEMATCH: needed to match: helper scope gives each cell its own table-pointer lifetime. */
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

static __inline__ void CopyCameraCell(u32 *map, u8 *base, u32 rowmod, u32 colmod)
{
    u32 *tiles;
    u8 *dest;
    u32 index = (*map << 20) >> 19;

    tiles = (u32 *)Ram_MapBlocks;
    tiles += index;
    /* FAKEMATCH: needed to match: integer address addition preserves the add operand order. */
    dest = (u8 *)((rowmod + colmod) * 2 + (u32)base);
    *(u32 *)dest = *tiles;
    tiles = (u32 *)(Ram_MapBlocks + 4);
    tiles += index;
    *(u32 *)(dest + 64) = *tiles;
}

struct SceneEntry {
    u16 resources[6];
};

extern struct SceneEntry Map_LayeredScenes[];

struct SceneLayerSource {
    u8 x;
    u8 y;
    s8 scroll_x;
    s8 scroll_y;
    s8 speed_x;
    s8 speed_y;
    u8 period_x;
    u8 period_y;
};

struct SceneHeader {
    u8 origin[4];
    u8 priority[3];
    u8 char_base[3];
    u16 unknown_0a;
    struct SceneLayerSource layers[3];
    s32 tiles;
    s32 collision;
    s32 tilemap;
    s32 animation;
    s32 blend;
    s32 script;
};

extern u8 gMapLayerData[];

s32 Map_LoadLayeredScene(s32 index)
{
    struct SceneEntry *entry;
    struct MapScrollWork *work;
    struct SceneHeader *header;
    struct MapLayerScroll *layer;
    struct SceneLayerSource *source;
    s32 *scale_x;
    s32 *scale_y;
    s32 i;
    u8 *buf;
    s32 backdrop;
    u8 *src;
    s32 cnt;

    REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
    Blend_SetDarkenTarget0(0);
    entry = &Map_LayeredScenes[index];
    work = Runtime_AllocateBlock(8, sizeof(struct MapScrollWork));
    Iwram_ClearWords(work, sizeof(struct MapScrollWork));
    header = (struct SceneHeader *)Resource_GetTableEntry(entry->resources[0] + (u32)&ResourceId_Map001);
    src = (u8 *)header + header->tiles;
    Tilemap_DecodeStagedBuffer(Resource_DecodeType01(src, Ram_MapCellBuffer + 1));
    src = (u8 *)header + header->collision;
    Resource_DecodeType01(src, Ram_MapCollision);
    src = (u8 *)header + header->tilemap;
    Resource_DecodeType01(src, Ram_MapCellBuffer);
    Tilemap_ConvertBuffer();
    /* FAKEMATCH: needed to match: passing optional offsets through the pointer keeps them in r0. */
    src = (u8 *)header->animation;
    if (src != 0) {
        Resource_DecodeType01((u8 *)header + (s32)src, Ram_MapCollision + 0x1000);
        MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
    }
    src = (u8 *)header->blend;
    if (src != 0) {
        Resource_DecodeType01((u8 *)header + (s32)src, Ram_MapCollision + 0x1e00);
        DisplayBlend_StartScript((u16 *)(Ram_MapCollision + 0x1e00));
    }
    work->script = (u8 *)header + header->script;
    work->min_x = header->origin[0] << 19;
    work->min_y = header->origin[1] << 19;
    work->max_x = header->origin[2] << 19;
    work->max_y = header->origin[3] << 19;
    scale_x = &work->view_x;
    *scale_x = 0;
    scale_y = &work->view_y;
    *scale_y = 0;
    work->enabled[0] = header->priority[0];
    work->enabled[1] = header->priority[1];
    work->enabled[2] = header->priority[2];
    layer = work->layers;
    source = header->layers;
    for (i = 0; i < 3; i++) {
        u32 x = source->x;
        u32 y = source->y;
        s32 scroll_x;
        s32 scroll_y;
        s32 base_x;
        s32 base_y;

        layer->offset_x = base_x = x << 19;
        layer->offset_y = base_y = y << 19;
        layer->speed_x = source->speed_x << 12;
        layer->speed_y = source->speed_y << 12;
        layer->mask_x = source->period_x;
        layer->mask_y = source->period_y;
        layer->phase_x = 0;
        layer->phase_y = 0;
        scroll_x = source->scroll_x << 12;
        scroll_y = source->scroll_y << 12;
        layer->scale_x = scroll_x;
        layer->scale_y = scroll_y;
        layer->cells = (struct MapCell *)Ram_MapCellBuffer + (y >> 1) * 128 + (x >> 1);
        {
            s32 v = Iwram_MulQ16(*scale_x, scroll_x);

            /* FAKEMATCH: empty asm with an r3 clobber after the reviewed IWRAM multiply call (which leaves r3 intact): register choice only, the layer loop keeps its native allocation (7 of 377 rows differ as a volatile barrier, 331 of 395 without) */
            asm("" : "+r"(v) : : "r3");
            layer->x = v + base_x;
        }
        {
            s32 v = Iwram_MulQ16(*scale_y, scroll_y);

            layer->y = v + base_y;
        }
        source++;
        layer++;
    }
    work->flags = DISPCNT_OBJ_ON;
    if (work->enabled[0] != 0)
        work->flags = DISPCNT_OBJ_ON | DISPCNT_BG3_ON;
    if (work->enabled[1] != 0)
        work->flags |= DISPCNT_BG2_ON;
    if (work->enabled[2] != 0)
        work->flags |= DISPCNT_BG1_ON;
    cnt = work->enabled[0] | BGCNT_CHAR_BASE(header->char_base[0]) | BGCNT_SCREEN_BASE(5);
    REG_BG3CNT = cnt;
    /* FAKEMATCH: needed to match: one-pass loops keep the second and third background-control writes in source order. */
    do {
        cnt = work->enabled[1] | BGCNT_CHAR_BASE(header->char_base[1]) | BGCNT_SCREEN_BASE(6);
        REG_BG2CNT = cnt;
    } while (0);
    do {
        cnt = work->enabled[2] | BGCNT_CHAR_BASE(header->char_base[2]) | BGCNT_SCREEN_BASE(7);
        REG_BG1CNT = cnt;
    } while (0);
    if (GameFlag_TestFar(FLAG_KEEP_LAYER_GRAPHICS) != 0) {
        GameFlag_ClearBitFar(FLAG_KEEP_LAYER_GRAPHICS);
    } else {
        buf = MemAlloc(0x4000);
        if (buf != NULL) {
            backdrop = *(s16 *)BG_PLTT;
            Resource_DecodeType01(Resource_GetTableEntry(entry->resources[1] + (u32)&ResourceId_Map001), buf);
            *(s16 *)buf = backdrop;
            Iwram_CopyWords((void *)BG_PLTT, buf, 0x1c0);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[2] + (u32)&ResourceId_Map001), buf);
            Iwram_CopyWords(BG_CHAR_BLOCK(1), buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[3] + (u32)&ResourceId_Map001), buf);
            Iwram_CopyWords(BG_CHAR_BLOCK(2), buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[4] + (u32)&ResourceId_Map001), buf);
            Iwram_CopyWords(BG_CHAR_BLOCK(3), buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[5] + (u32)&ResourceId_Map001), gMapLayerData);
            MemFree(buf);
        }
    }
    cnt = 0;
    REG_MOSAIC = cnt;
    REG_BLDCNT = cnt;
    cnt = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
    REG_DISPCNT = cnt;
    Scheduler_AddOrUpdateCallback(Map_UpdateLayerScroll, 0xc85);
    return 2;
}

void Map_ApplyWorkOriginAndSpan(void)
{
    struct MapScrollWork *work = gMapWork[0];
    s32 x;
    s32 y;
    s32 height;
    s32 *p;

    p = work->origin;
    x = 0;
    height = 0;
    y = 0;
    if (p != NULL) {
        x = *p++;
        height = *p++;
        y = *p;
    }
    Map_SetCameraCenter(x, y - height);
    Map_UpdateLayerScroll();
}

void Map_RenderMetatileRow(u32 a0, s32 a1, s32 a2)
{
    u8 *dest = BG_SCREEN_BLOCK(5) + (a0 << 11);
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
    u8 *destination = BG_SCREEN_BLOCK(5) + (a0 << 11);
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

    work = gMapWork[0];
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
    struct MapScrollWork *work = gMapWork[0];
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
            dest = BG_SCREEN_BLOCK(5) + (no << 11);
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

struct TilePos { s32 x, y; };

void Map_CopyMetatileIndicesRect(s32 src_x, s32 src_y,
    u32 dst_x, u32 dst_y, u32 width, u32 height)
{
    /* FAKEMATCH: needed to match: temporary sharing preserves register allocation and scheduling without changing the frame or instructions. */
    u32 *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
    u32 *dst = (u32 *)Ram_MapCellBuffer + (dst_y * 128 + dst_x);
    register struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
    struct TilePos tile[3];
    s32 i;
    struct TilePos *pos = tile;
    u32 y;
    register u32 x;

    for (i = 2; i >= 0; i--) {
        pos->x = layer->x >> 20;
        pos->y = layer->y >> 20;
        pos++;
        layer++;
    }
    for (y = dst_y; y < dst_y + height; y++) {
        for (x = dst_x; x < dst_x + width; x++) {
            /* FAKEMATCH: the cell value remains live in r5 while layers are tested. */
            register u32 cell __asm__ ("r5") = 0xfff & *src++;
            volatile u32 *dest;
            u32 offset;
            u32 tmp3;
            u32 n;
            register u32 *tiles;
            u32 tmp6;
            u32 tmp9;
            u32 tmp12;
            u32 tmp11;
            u32 tmp13;
            u32 tmp15;
            tmp9 = *dst;
            tmp3 = tmp9 & -0x1000;
            tmp12 = x & 15;
            dst[0] = cell | tmp3;
            tmp6 = tmp12;
            dst++;
            tmp15 = y & 15;
            tmp3 = tmp15;
            pos = tile;
            tmp13 = tmp3 << 5;
            tmp12 = tmp13;
            n = tmp12;
            n += tmp6;
            tmp13 = 4 * n;
            i = 0;
            tmp11 = tmp13;
            tmp6 = tmp11;
            offset = tmp6;
            for (; i < 3; i++) {
                if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y && pos->y + 12 > (s32)y) {
                    u32 *tmp;
                    u32 tmp2;
                    u32 tmp4;
                    u32 *tmp7;
                    u32 tmp10;
                    u32 tmp14;
                    u32 tmp18;
                    u32 tmp21;
                    u32 bytes;
                    u32 tmp16;
                    {
                        bytes = cell * 8;
                        tmp2 = 0x06002800;
                        tmp = (u32 *)Ram_MapBlocks;
                        tiles = tmp;
                        tmp16 = offset + tmp2;
                        /* FAKEMATCH: scaling before the destination and keeping cell live here removes the offset pin; without the cell input 28 instructions differ, and scaling after the destination swaps the scale and base load. */
                        __asm__("" : "+l"(bytes) : "l"(cell));
                    }
                    tmp21 = tmp16;
                    tmp18 = tmp21;
                    tmp2 = tmp18;
                    tmp = (u32 *)tmp2;
                    dest = tmp;
                    tiles = (u32 *)((u8 *)tiles + bytes);
                    tmp14 = *tiles;
                    tmp10 = tmp14;
                    *dest = tmp10;
                    tmp7 = (u32 *)(Ram_MapBlocks + 4);
                    tiles = tmp7;
                    tiles = (u32 *)((u8 *)tiles + bytes);
                    tmp16 = 0x06002840 + offset;
                    tmp18 = tmp16;
                    tmp14 = tmp18;
                    tmp21 = *tiles;
                    tmp10 = tmp21;
                    tmp4 = tmp10;
                    tmp2 = tmp4;
                    tmp4 = tmp14;
                    tmp7 = (u32 *)tmp4;
                    dest = tmp7;
                    *dest = tmp2;
                    break;
                }
                offset += 0x800;
                pos++;
            }
        }
        src += 128 - width;
        dst += 128 - width;
    }
}

struct MapCopyCommand {
    u16 source;
    s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));
typedef char MapCopyCommandSize[(sizeof(struct MapCopyCommand) == 10) ? 1 : -1];

void Map_PlayMetatileCopySequence(u16 *command, s32 destination_x, u32 destination_y)
{
    struct MapCopyCommand *record = (struct MapCopyCommand *)command;
    u32 source = record->source;
    u32 end = 0xffff;
    u32 test = source;

    /* FAKEMATCH: compares an ip copy of the entry and keeps the entry itself in r0. */
    __asm__("" : "+r"(test));
    if (test != end) {
        u32 stop = end;
        s16 *args = (s16 *)(command + 1);

        while (1) {
            s32 source_y = args[0];
            s32 width = args[1];
            s32 height = args[2];
            s32 delay = args[3];
            args += 5;

            Map_CopyMetatileIndicesRect(source, (u16)source_y,
                          destination_x, destination_y,
                          (u16)width, (u16)height);
            record++;
            WaitFrames((u16)delay);
            source = record->source;
            {
                u32 next = source;

                /* FAKEMATCH: the same ip copy after the wait. */
                __asm__("" : "+r"(next));
                if (next == stop)
                    break;
            }
        }
    }
}
