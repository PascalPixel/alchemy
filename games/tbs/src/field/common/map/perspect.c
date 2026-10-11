//--------------------------------------------------------------------------
// decode_staged_buffer.c
// マップ：展開したブロックのデータを gMapBlocks にほどく
//--------------------------------------------------------------------------

#include "types.h"
#include "ram_buffer.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "iwram_call.h"
#include "dma.h"
#include "ramcode.h"
#include "io_reg.h"
#include "global_cells.h"
#include "map_scroll.h"
#include "resource.h"
#include "resource_ids.h"
#include "flag_ids.h"
#include "edition.h"
#include "mapcopy.h"
#include "map.h"
#include "fixed_point_position.h"
#include "mapframe.h"
#include "heap_state.h"
#include "btldisp.h"
#include "project.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "transform.h"
#include "frame.h"
#include "palqueue.h"

extern u32 gMapBlocks[];
extern u8 gMapCellBuffer[];

//--------------------------------------------------------------------------
// 展開バッファの [1] が方式、[2] からが 16 ビットのデータ
// size : 方式のバイトを含む長さ（偶数ならデータなし）
//   方式 0 : そのまま写す
//   方式 1 : 上位バイトの列のあとに下位バイトの列。前の値と XOR して戻す
//   方式 2 : 前の値と XOR して戻す
//--------------------------------------------------------------------------
void Tilemap_DecodeStagedBuffer(s32 size)
{
	u16 *dst;
	u16 *src;
	s32	 n;
	s32	 i;

	src = (u16 *)(Ram_MapCellBuffer + 2);
	dst = (u16 *)gMapBlocks;
	n = (size - 1) / 2;
	if (size & 1)
	{
		switch (((u8 *)gMapCellBuffer)[1])
		{
		case 0:
			for (i = 0; i < n; i++)
				*dst++ = *src++;
			break;
		case 1:
		{
			u32 prev = 0;
			u8 *hi = Ram_MapCellBuffer + 2;
			u8 *lo = hi + n;

			for (i = 0; i < n; i++)
			{
				u32 v = ((*hi++ << 8) | *lo++) ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		case 2:
		{
			s32 prev = 0;

			for (i = 0; i < n; i++)
			{
				s32 v = *src++ ^ prev;
				*dst++ = v;
				prev = v;
			}
			break;
		}
		}
	}
}

typedef void (*ConvertFn)(void *dst, const void *src, const void *saved);

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
    /* FAKEMATCH: needed to match: the do-while block keeps the allocation next to the copy that uses it */
    do {
        saved = (u8 *)Runtime_BumpAllocateAlternatePool(size);
        Iwram_CopyWords(saved, Ram_MapCellBuffer, size);
    } while (0);
    RamCode_Load(routine, Tile_ConvertMap, Tile_ConvertMapCodeSize);
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
    dest = base + rowmod * 2 + colmod * 2;
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
    if (header->animation != 0) {
        src = (u8 *)header + header->animation;
        Resource_DecodeType01(src, Ram_MapCollision + 0x1000);
        MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
    }
    if (header->blend != 0) {
        src = (u8 *)header + header->blend;
        Resource_DecodeType01(src, Ram_MapCollision + 0x1e00);
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
        scroll_x = source->scroll_x << 12;
        scroll_y = source->scroll_y << 12;
        layer->scale_x = scroll_x;
        layer->scale_y = scroll_y;
        layer->speed_x = source->speed_x << 12;
        layer->speed_y = source->speed_y << 12;
        layer->mask_x = source->period_x;
        layer->mask_y = source->period_y;
        layer->phase_x = 0;
        layer->phase_y = 0;
        layer->cells = (struct MapCell *)Ram_MapCellBuffer + (y >> 1) * 128 + (x >> 1);
        layer->x = Iwram_MulQ16(*scale_x, scroll_x) + base_x;
        layer->y = Iwram_MulQ16(*scale_y, scroll_y) + base_y;
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
            /* FAKEMATCH: needed to match: these four copies call the fixed entry itself; through Iwram_CopyWords the scene header and the work move from r5 and r8 to r8 and sl (237 of 357 instructions differ), with or without the function's other devices. */
            Iwram_CopyWordsEntry((void *)BG_PLTT, buf, 0x1c0);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[2] + (u32)&ResourceId_Map001), buf);
            Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[3] + (u32)&ResourceId_Map001), buf);
            Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), buf, 0x4000);
            Resource_DecodeType2(Resource_GetTableEntry(entry->resources[4] + (u32)&ResourceId_Map001), buf);
            Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), buf, 0x4000);
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
            u32 val = *src++;
            u32 cell = val & 0xfff;
            u32 offset;
            u32 tmp3;
            u32 n;
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
            /* FAKEMATCH: cell and i are ordinary locals; three measured forms
             * replace the r5 pin on cell and the asm barrier on its scale.
             * 1. i = 0 here, not after the offset: i's live length (54 -> 60
             *    insns, 22 refs) drops its global-allocation priority below
             *    cell's (15 refs over 30), so cell takes r5 and i r6 as the
             *    native does (i = 0 after the offset: differ 59; at the top of
             *    the body: 32).
             * 2. The scale goes into the x-loop scratch tmp11 inside the layer
             *    loop: tmp11's first reference is outside that loop, so loop.c
             *    (reg_in_basic_block_p under maybe_never) leaves the shift in
             *    the conditional block; a fresh `u32 bytes = cell * 8` is
             *    invariant and hoisted before the window tests (differ 158).
             * 3. The scale comes after the first destination: cell's r5 is
             *    still live when reload materializes 0x6002800 for the folded
             *    add, so reload picks r2 (scale first: r5, differ 4).
             * The scr/blk variables and the dest = (u8 *)scr / tiles = (u8 *)blk
             * copies keep the layer loop at 31 real insns, above loop.c's
             * threshold of 30, so the four address constants and the 0x800
             * step stay in the loop (plain expressions: 29 insns, hoisted,
             * differ 113-153).
             * 4. The two word copies read and write through volatile casts: the
             *    VRAM store is hardware, and the volatile load from the block
             *    buffer keeps its add apart from the load (a non-volatile load
             *    combines into ldr [r, r]); these four volatiles replaced two pins
             *    and two barriers and raised the volatile count (2026-10-11). */
            i = 0;
            tmp15 = y & 15;
            tmp3 = tmp15;
            pos = tile;
            tmp13 = tmp3 << 5;
            tmp12 = tmp13;
            n = tmp12;
            n += tmp6;
            tmp13 = 4 * n;
            tmp11 = tmp13;
            tmp6 = tmp11;
            offset = tmp6;
            for (; i < 3; i++) {
                if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y && pos->y + 12 > (s32)y) {
                    u32 scr, blk;
                    u8 *dest, *tiles;

                    scr = 0x06002800;
                    dest = (u8 *)scr;
                    dest += offset;
                    tmp11 = cell * 8;
                    blk = (u32)Ram_MapBlocks;
                    tiles = (u8 *)blk;
                    tiles += tmp11;
                    *(volatile u32 *)dest = *(volatile u32 *)tiles;

                    blk = (u32)Ram_MapBlocks + 4;
                    tiles = (u8 *)blk;
                    tiles += tmp11;
                    scr = 0x06002840;
                    dest = (u8 *)scr;
                    dest += offset;
                    *(volatile u32 *)dest = *(volatile u32 *)tiles;
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

    /* FAKEMATCH: the conditional cancelling updates end CSE before the
     * comparison, preserving its ip copy while source stays in r0. Both
     * updates disappear; removing them loses the copy (53 -> 52 English
     * instructions, five rows differ). */
    if (record) {
        source ^= test;
        source ^= test;
    }
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

                /* FAKEMATCH: the same ip copy after the wait. 2026-10-11: the native compares a copy of source held in ip at both tests; the copy's pseudo must reach regclass with only the copy and the branch as its insns, which CSE's copy swap or combine's substitution removes in every plain form tried (best, a do-while on the field re-read with stop in a local: differ 12). */
                __asm__("" : "+r"(next));
                if (next == stop)
                    break;
            }
        }
    }
}

//--------------------------------------------------------------------------

// mapcopy.c

// マップ：セルの矩形の写し

//--------------------------------------------------------------------------

#if GAME_TLA

//--------------------------------------------------------------------------
// セルの矩形を、ブロック番号と属性ごと写す
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyCellsRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	Map_CopyMetatileIndicesRect(src_x, src_y, dst_x, dst_y, width, height);
	Map_CopyCellAttributeRect(src_x, src_y, width, height, dst_x, dst_y);
}
#else
#include "map_scroll.h"
#include "ram_buffer.h"

extern u8 gMapCellBuffer[];

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff	   // ブロック番号
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000 // 属性

// レイヤーの表示範囲の左上（セル）
struct TilePos_2
{
	s32 x, y;
};

//--------------------------------------------------------------------------
// セル（16×16 ドット）の矩形をそのまま写す
// 写した先が BG レイヤーの表示範囲（16×12 セル）にあれば、
// VRAM の BG マップにもブロックの 2×2 キャラを書く
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyMetatileCellsRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	s32					   y = dst_y;
	u32					  *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
	u32					  *dst = &((u32 *)Ram_MapCellBuffer)[y * 128 + dst_x];
	struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
	struct TilePos_2		   view[3];
	s32					   i;
	struct TilePos_2		  *pos = view;
	register s32		   x;

	// 3 つのレイヤーの表示範囲（スクロール位置をセル単位に）
	i = 2;
	while (i >= 0)
	{
		register s32 tmp5;

		tmp5 = layer->x >> 20;
		pos->x = tmp5;
		tmp5 = (*layer).y;
		pos->y = tmp5 >> 20;
		pos++;
		layer++;
		i--;
	}
	if (y < dst_y + height)
	{
		do
		{
			for (x = dst_x; x < dst_x + width; x++)
			{
				u32			  val = *src++;
				u32			  cell;
				register u32  offset;
				u32			  tmp3;
				u32			 *dest;
				u32			  n;
				register u32 *tiles;
				u32			  tmp6;
				u32			  tmp12;
				u32			  tmp13;

				dst[0] = val;
				cell = val & 0xfff;
				tmp12 = x & 15;
				tmp6 = tmp12;
				dst += 1;
				tmp3 = y & 15;
				tmp13 = tmp3 << 5;
				pos = view;
				tmp12 = tmp13;
				n = tmp12;
				n += tmp6;
				tmp13 = n * 4;
				i = 0;
				tmp6 = tmp13;
				offset = tmp6;
				// 表示中のレイヤーなら、VRAM の BG マップ（レイヤーごとに 0x800）へ
				for (; i < 3; i += 1)
				{
					if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y && pos->y + 12 > y)
					{
						// ブロック（8 バイト）の上の段の 2 キャラと、下の段（0x40 先）の 2 キャラ
						u32 *tmp;
						u32	 tmp2;
						u32	 tmp4;
						u32 *tmp7;
						u32	 tmp10;
						u32	 tmp14;
						u32	 tmp16;
						/* FAKEMATCH: restoring the unscaled cell keeps loop.c's
						 * shift inside the successful layer branch. The restoration
						 * folds away, preserving the native 36-byte frame and registers. */
						register u32 bytes = cell * 8;
						u32			 tmp8;
						register u32 tmp17;

						cell = bytes / 8;
						tmp = (u32 *)Ram_MapBlocks;
						tmp8 = 0x06002800 + offset;
						tiles = tmp;
						tmp2 = tmp8;
						tmp = (u32 *)tmp2;
						dest = tmp;
						tiles = (u32 *)((u8 *)tiles + bytes);
						tmp16 = *tiles;
						tmp14 = tmp16;
						tmp17 = 0x06002840 + offset;
						tmp10 = tmp14;
						*dest = tmp10;
						tmp7 = (u32 *)(Ram_MapBlocks + 4);
						tiles = tmp7;
						tiles = (u32 *)((u8 *)tiles + bytes);
						tmp8 = tmp17;
						tmp14 = tmp8;
						tmp17 = tiles[0];
						tmp16 = tmp17;
						tmp10 = tmp16;
						tmp4 = tmp10;
						tmp2 = tmp4;
						tmp4 = tmp14;
						tmp7 = (u32 *)tmp4;
						dest = tmp7;
						*dest = tmp2;
						break;
					}
					n = 0x800; // つぎのレイヤーの BG マップ
					offset += n;
					pos++;
				}
			}
			src = src + (128 - width);
			dst = 128 - width + dst;
			y++;
		} while (y < dst_y + height);
	}
}

//--------------------------------------------------------------------------
// セルの矩形の属性だけを写す。写す先のブロック番号はそのまま
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyCellAttributeRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	u32 *dst = MAP_CELLS;
	u32 *src = dst;
	u16	 row;
	u16	 col;

	dst += (dst_y << 7) + dst_x;
	src += (src_y << 7) + src_x;

	for (row = 0; row < height; row++)
	{
		u32 *dst_cell = dst + (row << 7);
		u32 *src_cell = src + (row << 7);

		for (col = 0; col < width; col++)
		{
			*dst_cell = (*dst_cell & MAP_CELL_TILE_MASK) | (*src_cell++ & MAP_CELL_ATTRIBUTE_MASK);
			dst_cell++;
		}
	}
}

//--------------------------------------------------------------------------
// セルの矩形のブロック番号だけを写す。写す先の属性はそのまま
// 表示範囲にあれば VRAM の BG マップも書きかえる
// src_x, src_y  : 写すもとの左上（セル）
// width, height : 大きさ（セル）
// dst_x, dst_y  : 写す先の左上（セル）
//--------------------------------------------------------------------------
void Map_CopyCellIndexRect(s32 src_x, s32 src_y, s32 width, s32 height, s32 dst_x, s32 dst_y)
{
	/* FAKEMATCH: needed to match: word/index temporaries preserve register allocation; the plain body allocates differently and uses a 40-byte frame. */
	s32					   y;
	u32					  *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
	u32					  *dst = &((u32 *)Ram_MapCellBuffer)[dst_y * 128 + dst_x];
	struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
	struct TilePos_2		   view[3];
	s32					   i;
	struct TilePos_2		  *pos = view;
	register s32		   x;

	// 3 つのレイヤーの表示範囲（スクロール位置をセル単位に）
	i = 2;
	while (i >= 0)
	{
		register s32 tmp5;

		tmp5 = layer->x >> 20;
		pos->x = tmp5;
		tmp5 = (*layer).y;
		pos->y = tmp5 >> 20;
		pos++;
		layer++;
		i--;
	}
	y = dst_y;
	if (y < dst_y + height)
	{
		do
		{
			for (x = dst_x; x < dst_x + width; x++)
			{
				u32			  val = *src++;
				u32			  cell;
				register u32  offset;
				u32			  tmp3;
				u32			  n;
				u32			  tmp6;
				u32			  tmp12;
				u32			  tmp13;

				cell = val & MAP_CELL_TILE_MASK;
				dst[0] = (dst[0] & MAP_CELL_ATTRIBUTE_MASK) | cell;
				tmp12 = x & 15;
				tmp6 = tmp12;
				dst += 1;
				/* FAKEMATCH: the r3 pin on the scaled cell and the consumed-base
				 * barrier are replaced by three measured forms.
				 * 1. i = 0 here, before the offset: i's live length (58 -> 60
				 *    insns, 22 refs) drops its global-allocation priority below
				 *    cell's (15 refs over 30), so cell keeps r5 and i r6 (i = 0
				 *    after pos = view: differ 38, cell in r6).
				 * 2. The scale goes into the x-loop scratch tmp13 inside the layer
				 *    loop: its first reference is outside that loop, so loop.c
				 *    (reg_in_basic_block_p under maybe_never) leaves the shift in
				 *    the conditional block instead of hoisting the invariant
				 *    before the window tests; being block-local and untied, it
				 *    takes r3 like the pinned form (scale into n: differ 52).
				 * 3. The scale comes after the first destination: cell's r5 is
				 *    still live when reload materializes 0x6002800 for the folded
				 *    add, so reload picks r2 instead of its round-robin r5 (scale
				 *    first: differ 54).
				 * The scr/blk variables and the dest = (u8 *)scr / tiles = (u8 *)blk
				 * copies keep the layer loop above loop.c's threshold of 30 real
				 * insns, so the four address constants stay in the loop.
				 * 4. The two word copies read and write through volatile casts: the
				 *    VRAM store is hardware, and the volatile load from the block
				 *    buffer keeps its add apart from the load (a non-volatile load
				 *    combines into ldr [r, r]); these four volatiles replaced two pins
				 *    and two barriers and raised the volatile count (2026-10-11). */
				i = 0;
				tmp3 = y & 15;
				tmp13 = tmp3 << 5;
				pos = view;
				tmp12 = tmp13;
				n = tmp12;
				n += tmp6;
				tmp13 = n * 4;
				tmp6 = tmp13;
				offset = tmp6;
				// 表示中のレイヤーなら、VRAM の BG マップ（レイヤーごとに 0x800）へ
				for (; i < 3; i += 1)
				{
					if (pos->x <= x && 16 + (*pos).x > x && pos->y <= y && pos->y + 12 > y)
					{
						// ブロック（8 バイト）の上の段の 2 キャラと、下の段（0x40 先）の 2 キャラ
						u32 scr, blk;
						u8 *dest, *tiles;

						scr = 0x06002800;
						dest = (u8 *)scr;
						dest += offset;
						tmp13 = cell * 8;
						blk = (u32)Ram_MapBlocks;
						tiles = (u8 *)blk;
						tiles += tmp13;
						*(volatile u32 *)dest = *(volatile u32 *)tiles;

						blk = (u32)Ram_MapBlocks + 4;
						tiles = (u8 *)blk;
						tiles += tmp13;
						scr = 0x06002840;
						dest = (u8 *)scr;
						dest += offset;
						*(volatile u32 *)dest = *(volatile u32 *)tiles;
						break;
					}
					n = 0x800; // つぎのレイヤーの BG マップ
					offset += n;
					pos++;
				}
			}
			src = src + (128 - width);
			dst = 128 - width + dst;
			y++;
		} while (y < dst_y + height);
	}
}
#endif

#undef Scheduler_AddOrUpdateCallback

void Map_SetWorkFlagBits9To11(u32 v)
{
    struct MapState *state = gMapWork[0];
    u32 mask = v & 0xe00;
    u32 flags = state->flags;

    flags = (flags & 0xf1ff) | mask;
    state->flags = flags;
}

#ifndef MODERN // modern GCC keeps the headers' volatile declaration
#endif

static __inline__ void Io_Set16(s32 value, volatile u16 *reg)
{
    *reg = value;
}

static __inline__ void Io_Put16(volatile u16 *reg, s32 value)
{
    *reg = value;
}

typedef s32 (*RatioFn)(s32, s32);
typedef s32 (*PlaneFn)(void *camera, s32 *position, void *lines, void *out);

#define ABS(v) ((v) < 0 ? -(v) : (v))

s32 Resource_DecodeByteLz(const void *source, void *destination);

struct WorldCell {
    u32 tile : 24;
    u32 kind : 7;
    u32 flag : 1;
};

#define BG_PALETTE ((s16 *)BG_PLTT)
#define WORLD_CELLS ((struct WorldCell *)Ram_MapBlocks)

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update)
{
    struct PerspectiveWork *work = gMapWork[0];
    u32 *graphics = work->tiles;
    u16 *shown;
    u16 *buffer;
    u16 *source;
    u8 *blocks;
    u16 *screen;
    u32 i;
    u32 j;
    u32 id;

    x &= 1;
    y &= 1;
    shown = &work->quarter_tiles[(layer * 2 + y) * 2 + x];
    if (update == 0 && tile == *shown)
        return 0;
    *shown = tile;
    buffer = Runtime_AllocateHeapBlock(14, 0x400);
    Resource_DecodeByteLz((u8 *)graphics + graphics[tile], buffer);

    source = buffer;
    blocks = Ram_MapBlocks + (((layer * 2 + y) * 32 + x) << 6);
    for (i = 0; i < 16; i++) {
        Dma_Set(source, blocks, DMA_ENABLE32 | DMA_32BIT | 0x10, REG_DMA3);
        source += 32;
        blocks += 128;
    }

    if (update != 0) {
        screen = (u16 *)(BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
        source = buffer;
        for (i = 0; i < 16; i++) {
            for (j = 0; j < 16; j++) {
                u16 *bottom;

                id = *source;
                screen[0] = *(u16 *)(Ram_MapCellBuffer + id * 4);
                bottom = (u16 *)(Ram_MapCellBuffer + 2);
                bottom = (u16 *)((u8 *)bottom + id * 4);
                screen[32] = *bottom;
                screen++;
                source += 2;
            }
            screen += 48;
        }
    }
    Runtime_ReleaseHeapBlock(14);
    return 1;
}

s32 Map_InitializePerspectiveScene(void)
{
    struct PerspectiveWork *work;
    struct BattleCamera *camera;
    struct MapFrameWork *tiles;
    struct MapAffinePair (*lines)[160];
    s32 *position;
    s32 *distance;
    u16 *yaw;
    u16 *pitch;
    u16 *turn;
    volatile u32 fill;
    struct FixedPointPosition vector;
    s32 far_plane;
    s32 i;

    REG_DISPCNT &= ~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
    Blend_SetDarkenTarget0(0);
    work = Runtime_AllocateHeapBlock(8, sizeof(struct PerspectiveWork));
    fill = 0;
    Dma_Set(&fill, work, DMA_ENABLE32 | DMA_SRC_FIXED32 | DMA_32BIT | (sizeof(struct PerspectiveWork) / 4), REG_DMA3);
    work->view_x = 0;
    work->view_y = 0;
    work->scale_x = 0x200000;
    work->scale_y = 0x400000;
    work->limit_x = 0x1fe00000;
    work->limit_y = 0x1fe00000;
    work->script = NULL;
    work->tiles = Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapAnimation), Ram_MapCollision + 0x1000);
    MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
    Io_Set16(0x3f9e, &REG_BLDCNT);
    Io_Set16(0x1010, &REG_BLDALPHA);
    REG_BLDY = 0;
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), Ram_MapCellBuffer);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMetatileAttributes), Ram_MapCollision);
    work->fade = 0x1f00;
    work->fade_step = 0x80;
    Io_Set16(0xa80a, &REG_BG3CNT);
    Io_Set16(0xaa0e, &REG_BG2CNT);
    Io_Set16(0x0501, &REG_BG1CNT);
    /* FAKEMATCH: needed to match: loop note keeps BG2PA's constant after its address */
    do {
        Io_Put16(&REG_BG2PA, 0x100);
        REG_BG2PB = 0;
        REG_BG2PC = 0;
    } while (0);
    /* FAKEMATCH: needed to match: loop notes keep BG2PD's constant late and r0 first below */
    do {
        Io_Put16(&REG_BG2PD, 0x100);
        REG_BG2X = 0;
        REG_BG2Y = 0;
        Io_Put16(&REG_BG3PA, 0x100);
        REG_BG3PB = 0;
        REG_BG3PC = 0;
        Io_Put16(&REG_BG3PD, 0x100);
        REG_BG3X = 0;
        REG_BG3Y = 0;
    } while (0);

    camera = Runtime_AllocateBlock(12, sizeof(struct BattleCamera));
    tiles = Runtime_AllocateHeapBlock(7, sizeof(struct MapFrameWork));
    position = camera->pos;
    lines = tiles->pages;
    far_plane = 0x1fe0000;
    work->far_plane = far_plane;
    distance = &work->distance;
    *distance = far_plane;
    work->zoom = 0x10000;
    turn = &work->zoom_transition_frames;
    camera->unknown_18 = 0;
    camera->follow_pos = 0;
    *turn = 0;
    gProjection.center_x = 120;
    gProjection.center_y = 96;
    Camera_StoreSceneParameters(far_plane, far_plane >> 1, far_plane << 1);
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(position);
    yaw = &work->yaw;
    SceneTransform_ApplyYaw(*yaw);
    pitch = &work->pitch;
    SceneTransform_ApplyPitch(*pitch);
    vector.x = 0;
    vector.y = 0;
    vector.z = far_plane;
    Iwram_TransformVector((const s32 *)&vector, (s32 *)camera);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(camera->eye, position);
    RamCode_LoadSlot(46, Transform_UpdateVertices, &Transform_UpdateVerticesSize);
    WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(Trig_Cos(*pitch), Trig_Sin(*pitch)),
                  position, tiles->scanlines);
    gScanlineAngle = 0;
    gScanlinePitch = work->pitch;
    ((PlaneFn)gWorkSlot[46])(camera, position, tiles->scanlines,
                                 lines[gFrameCount & 1]);
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    Render_ResetTransformState();
    Io_Put16(pitch, 0xe000);
    *yaw = 0;
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(position);
    SceneTransform_ApplyYaw(*yaw);
    SceneTransform_ApplyPitch(*pitch);
    vector.x = 0;
    vector.y = 0;
    vector.z = *distance + 0x10000;
    Iwram_TransformVector((s32 *)&vector, camera->eye);
    REG_MOSAIC = 0;
    /* FAKEMATCH: needed to match: loop notes keep the display writes in source order */
    do {
        Io_Put16(&REG_DISPCNT, 0x42);
    } while (0);
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    gBgScroll[2].x = 0;
    gBgScroll[2].y = 0;
    gBgScroll[3].x = 0;
    gBgScroll[3].y = 0;
    work->window_top = 0;
    work->window_bottom = 159;
    Scheduler_AddOrUpdateCallback(WorldMap_UpdateView, 0xc85);
    Scheduler_AddOrUpdateCallback(MapAnimation_ApplyAffineFrame, 0x480);
    for (i = 255; i >= 0; i--)
        work->tile_ids[i] = i;
}

void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py)
{
    struct PerspectiveWork *window;
    s32 *position;
    s32 origin_x;
    s32 origin_y;
    s32 tile;

    window = gMapWork[0];
    origin_x = 0;
    origin_y = 0;
    position = window->origin;
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
    window->tile_ids[(((py / 2) & 15) << 4) + ((px / 2) & 15)] = tile;

    if (ABS(origin_x - px) <= 1 && ABS(origin_y - py) <= 1) {
        Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
        Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x140, 1);
    }
}

void WorldMap_LoadGraphics(s32 x, s32 z)
{
    struct PerspectiveWork *state;
    u32 *resources;
    u8 *buffer;
    u8 *tiles;
    u32 *cursor;
    u32 tile;
    s32 variant;
    s32 row;
    s32 col;
    s16 value;
    volatile u32 fill;

    variant = 0;
    buffer = MemAlloc(0x200);
    state = gMapWork[0];
    if (WORLD_CELLS[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)].kind == 21)
        variant = 1;
    resources = WorldMap_GraphicsResources[variant];
    state->resources = resources;
    value = BG_PALETTE[0];
    Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, BG_PALETTE, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    tiles = gBgTileBuffer;
    Resource_DecodeType01(Resource_GetTableEntry(resources[1]), tiles);
    Dma_Set(tiles, BG_CHAR_BLOCK(2), DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
    Dma_Set(Ram_BgTileBuffer + 0x2000, BG_CHAR_BLOCK(2) + 0x2000, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
    Dma_Set(Ram_BgTileBuffer + 0x4000, BG_CHAR_BLOCK(3), DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
    Dma_Set(Ram_BgTileBuffer + 0x6000, BG_CHAR_BLOCK(3) + 0x2000, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[5]), gMapLayerData);
    fill = 0xf07ff07f;
    Dma_Set((const void *)&fill, BG_SCREEN_BLOCK(5), DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x180, REG_DMA3);
    cursor = (u32 *)BG_SCREEN_BLOCK(6);
    tile = 0x01a901a8;
    for (row = 0; row < 20; row++) {
        for (col = 0; col < 15; col++) {
            *cursor++ = tile;
            tile += 0x00020002;
        }
        cursor++;
    }
    if (variant == 1) {
        state->tile_ids[10] = 0x10a;
        state->tile_ids[11] = 0x10b;
        state->tile_ids[12] = 0x10c;
        state->tile_ids[26] = 0x11a;
        state->tile_ids[27] = 0x11b;
        state->tile_ids[28] = 0x11c;
        Map_UpdateCurrentTileBlock();
    }
    MemFree(buffer);
}

void MapAnimation_ApplyAffineFrame(void)
{
    struct MapAffinePair (*lines)[160] = ((struct MapFrameWork *)WORK_SLOT(7))->pages;
    s32 dispcnt = (s16)(REG_DISPCNT & 0xfff8);
    u32 *dst = (u32 *)&REG_BG2PA;
    struct PerspectiveWork *work = WORK_SLOT(8);
    volatile u16 *channel = REG_DMA0;
    u32 *src;
    u32 mode;
    u32 top;
    u32 control;

    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    if (lines != NULL) {
        src = (u32 *)lines[gFrameCount & 1];
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst = *src++;
        Dma_Set(src, (void *)&REG_BG2PA, 0xa6600008, (volatile u32 *)channel);
    }
    work->shown_top = work->window_top;
    work->shown_bottom = work->window_bottom;
    top = work->shown_top;
    mode = 0;
    if (top < 200) {
        u16 bottom = work->shown_bottom;

        mode = (bottom != 0) << 1;
        if (top <= bottom) {
            mode = 0;
            if (top == 0)
                mode = 2;
        }
    }
    dispcnt |= mode;
    control = (u16)dispcnt;
    REG_DISPCNT = control;
    work->window_pending = 0;
}

#define Camera_StoreSceneParameters ((void (*)(s32,s32,s32))Camera_StoreSceneParameters)
#define SceneTransform_ApplyPosition ((void (*)(s32*))SceneTransform_ApplyPosition)

static __inline__ void CopyEntry(u8 *map, u8 *destination)
{
    u32 palette = *(u16 *)map;
    u16 *colors = (u16 *)Ram_MapCellBuffer;

    colors += palette * 2;
    *(u16 *)destination = *colors++;
    *(u16 *)(destination + 64) = *colors;
}

u32 Random16(void);

void Map_RenderPaletteMappedRow(u32 value)
{
    u8 *map;
    u8 *destination;
    u32 counter;

    map = Ram_MapBlocks + ((((s32)value / 2) & 31) << 7);
    destination = (u8 *)(0x06004000 + ((value & 62) << 6));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);

    destination += 0xfc0;
    map += 0xf80;
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 2;
        map += 4;
    } while (counter <= 31);
}

void Map_RenderPaletteMappedColumn(u32 value)
{
    u8 *map;
    u8 *destination;
    u32 counter;

    map = Ram_MapBlocks + ((((s32)value / 2) & 31) << 2);
    destination = (u8 *)(0x06004000 + (value & 62));
    counter = 0;
    do {
        CopyEntry(map, destination);
        counter++;
        destination += 128;
        map += 128;
    } while (counter <= 63);
}

void WorldMap_UpdateView(void)
{
    struct BattleCamera *cam = WORK_SLOT(HEAP_SLOT_CAMERA);
    struct MapFrameWork *map = WORK_SLOT(7);
    struct PerspectiveWork *view = WORK_SLOT(8);
    s32 *pos = cam->pos;
    s32 *target = view->origin;
    struct MapAffinePair (*buffer)[160] = map->pages;
    s32 distance = view->far_plane;
    s32 height = view->distance;
    struct FixedPointPosition local;

    Map_UpdateCurrentTileBlockUntilBlocked();
    if (target != NULL) {
        s32 x;
        s32 y;
        s32 col;
        s32 row;

        y = target[2];
        x = target[0];
        if (view->shake_x != 0) {
            s32 r = Random16();
            s32 amp;

            r -= Random16();
            amp = view->shake_x;
            x += Iwram_MulQ16(amp, r);
            view->shake_x = Iwram_MulQ16(amp, view->shake_decay);
        }
        if (view->shake_y != 0) {
            s32 r = Random16();
            s32 amp;

            r -= Random16();
            amp = view->shake_y;
            y += Iwram_MulQ16(amp, r);
            view->shake_y = Iwram_MulQ16(amp, view->shake_decay);
        }
        col = x / 0x100000;
        row = y / 0x100000;
        if ((view->view_x ^ x) & 0x100000) {
            if (view->view_x < x)
                Map_RenderPaletteMappedColumn(col + 16);
            else
                Map_RenderPaletteMappedColumn(col - 16);
        }
        if ((view->view_y ^ y) & 0x100000) {
            if (view->view_y < y)
                Map_RenderPaletteMappedRow(row + 12);
            else
                Map_RenderPaletteMappedRow(row - 18);
        }
        view->view_x = x;
        view->view_y = y;
    }
    gProjection.center_x = 120;
    gProjection.center_y = 96;
    Camera_StoreSceneParameters(distance, height / 2, height * 2);
    pos[0] = *target++;
    pos[1] = 0;
    pos[2] = target[1];
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(pos);
    SceneTransform_ApplyYaw(view->yaw);
    SceneTransform_ApplyPitch(view->pitch);
    local.x = 0;
    local.y = 0;
    local.z = height + 0x10000;
    Iwram_TransformVector((s32 *)&local, (s32 *)cam);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork((u8 *)cam, pos);
    if (gScanlinePitch != view->pitch) {
        s32 c = Trig_Cos(view->pitch);
        s32 s = Trig_Sin(view->pitch);

        WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(c, s), pos, map->scanlines);
        gScanlineAngle = 0;
        gScanlinePitch = view->pitch;
    }
    ((s32 (*)(void *, s32 *, void *, void *))gWorkSlot[46])(cam, pos, map->scanlines, buffer[gFrameCount & 1]);
}

void Map_UpdateCurrentTileBlock(void)
{
    struct PerspectiveWork *window = gMapWork[0];
    s32 x0 = 0;
    s32 y0 = 0;
    u32 layer;
    u32 row;
    u32 col;
    s32 bias;
    s32 tile;

    if (window->origin != NULL) {
        s32 *p = window->origin;

        x0 = *p++;
        y0 = p[1];
    }
    x0 = (x0 - 0x1000000) >> 25;
    y0 = (y0 - 0x1400000) >> 25;
    layer = 0;
    bias = 0;
    for (; layer < 2; layer++) {
        for (row = 0; row < 2; row++) {
            for (col = 0; col < 2; col++) {
                tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
                tile = window->tile_ids[tile];
                tile += bias;
                Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 1);
            }
        }
        bias += 320;
    }
}

void Map_UpdateCurrentTileBlockUntilBlocked(void)
{
    struct PerspectiveWork *window = gMapWork[0];
    s32 x0 = 0;
    s32 y0 = 0;
    u32 layer;
    u32 row;
    u32 col;
    s32 tile;

    if (window->origin != NULL) {
        s32 *p = window->origin;

        x0 = *p++;
        y0 = p[1];
    }
    x0 = (x0 - 0x1000000) >> 25;
    y0 = (y0 - 0x1400000) >> 25;
    for (layer = 0; layer < 2; layer++) {
        for (row = 0; row < 2; row++) {
            for (col = 0; col < 2; col++) {
                tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
                tile = window->tile_ids[tile];
                tile += layer * 320;
                if (Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) != 0)
                    return;
            }
        }
    }
}

extern u8 gDecodeBuffer[];

void MapAnimation_PresentFrame(void)
{
    s32 control = 0x682;

    /* FAKEMATCH: needed to match: the do-while keeps the BG1CNT store before the DMA source and destination loads. */
    do {
        REG_BG1CNT = control;
    } while (0);
    Dma_Set((const void *)gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | 0x2580, REG_DMA3);
}

void MapAnimation_Start(void)
{
    struct MapFrameWork *pages = WORK_SLOT(7);
    struct PerspectiveWork *work = WORK_SLOT(8);
    s32 one = 1;

    work->animation_active = one;
    Scheduler_EnableCallbacks(MapAnimation_Update);
    Dma_Set(BG_CHAR_BLOCK(1), (void *)gDecodeBuffer, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
    WaitFrames(1);
    Resource_RunCopiedDecoder((s32)pages->pages[gFrameCount & one], (s32)gMapCellBuffer);
    work->window_top = 200;
    work->window_bottom = 255;
    gVBlankHook = MapAnimation_PresentFrame;
}

void Map_ShowBg1FromBuffer(void)
{
    u32 mode = 0x501;
    u16 *bg1cnt = (u16 *)&REG_BG1CNT;

    do { *bg1cnt = mode; } while (0); /* FAKEMATCH: needed to match: the wrap orders the store before the DMA operands. */
    Dma_Set((const void *)gBgTileBuffer, BG_CHAR_BLOCK(2), 0x84002000,
            REG_DMA3);
}

void Map_LoadAreaGraphics(void)
{
    struct PerspectiveWork *state;
    u32 *resources;
    u8 *buffer;
    s16 value;

    state = gMapWork[0];
    buffer = gMapCellBuffer;
    resources = state->resources;
    value = *(s16 *)BG_PLTT;
    Resource_DecodeByteLz(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 0x70, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[1]), gBgTileBuffer);
    Resource_DecodeType01(Resource_GetTableEntry(resources[2]), Ram_BgTileBuffer + 0x2000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
    gVBlankHook = Map_ShowBg1FromBuffer;
    state->window_top = 0;
    state->window_bottom = 159;
    WaitFrames(1);
    Resource_DecodeType01(Resource_GetTableEntry((u32)&ResourceId_DefaultMapCells), buffer);
    Map_UpdateCurrentTileBlock();
    state->animation_active = 0;
    Scheduler_DisableCallbacks(MapAnimation_Update);
    WaitFrames(1);
}

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct PerspectiveWork *work = gMapWork[0];
    gVBlankHook = Map_ShowBg1FromBuffer;
    work->window_top = 0;
    work->window_bottom = 0x9f;
    WaitFrames(1);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), gMapCellBuffer);
    Map_UpdateCurrentTileBlock();
    Scheduler_DisableCallbacks(MapAnimation_Update);
    WaitFrames(1);
}

void MapAnimation_Update(void)
{
    /* FAKEMATCH: needed to match: indexed channel expressions preserve ip/lr allocation and the sentinel/VRAM pool order. */
    struct MapState *state = gMapWork[0];
    u32 i;

    for (i = 0; i <= 15; i++) {
        const u16 *script;
        u32 op;
        u32 count;
        u32 dst;

        if ((state->anim + i)->start == NULL || (state->anim + i)->paused != 0)
            continue;
    next:
        if ((state->anim + i)->timer == 0) {
            script = (state->anim + i)->cursor;
            op = *script++;
            if (op == 0xffff) {
                (state->anim + i)->cursor = (state->anim + i)->start;
                goto next;
            }
            if ((op & 0xff00) == 0xfe00) {
                if ((op & 0xff) == 0xff)
                    continue;
                (state->anim + i)->cursor = (state->anim + i)->start + (op & 0xff) * 2;
                goto next;
            }
            count = *script++;
            dst = script[0];
            (state->anim + i)->timer = script[1];
            if (state->wide_tiles == 0) {
                if (op >= 0x600)
                    Dma_Set(Ram_DecodeBuffer + op * 32, BG_CHAR_BLOCK(1) + dst * 32, DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
                else
                    Dma_Set(BG_CHAR_BLOCK(1) + op * 32, BG_CHAR_BLOCK(1) + dst * 32, DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
            } else {
                if (op >= 0x200)
                    Dma_Set(Ram_MapBlocks + op * 64, BG_CHAR_BLOCK(2) + dst * 64, DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
                else
                    Dma_Set(BG_CHAR_BLOCK(2) + op * 64, BG_CHAR_BLOCK(2) + dst * 64, DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
            }
            (state->anim + i)->cursor += 4;
            goto next;
        } else {
            (state->anim + i)->timer--;
        }
    }
}

void Map_ClearLayerEntryFlag(u32 no)
{
    struct MapState *state = gMapWork[0];
    struct MapAnimation *anim = &state->anim[no];

    anim->paused = 0;
}

void Map_SetLayerEntryFlag(u32 no)
{
    struct MapState *state = gMapWork[0];
    struct MapAnimation *anim = &state->anim[no];

    anim->paused = 1;
}

void MapAnimation_StartChannels(const u16 *script)
{
    struct MapState *state;
    s32 count;
    u32 command;
    volatile u32 zero;

    state = gMapWork[0];
    count = 0;
    zero = 0;
    Dma_Set((const void *)&zero, state->anim, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x30, REG_DMA3);
    command = *script++;
    while (command != 0xffff) {
        if ((command & 0xff00) == 0xfd00) {
            struct MapAnimation *anim;
            u32 idx = command & 15;
            u32 flag = 0;
            if (command & 0x80)
                flag = 1;
            anim = &state->anim[idx];
            anim->start = script;
            anim->cursor = script;
            anim->timer = 0;
            anim->paused = flag;
            count++;
        }
        command = *script++;
    }
    if (count != 0)
        Scheduler_AddOrUpdateCallback(MapAnimation_Update, 0xc80);
}

void Map_EnableUpdateCallback(void)
{
    if (((struct MapScrollWork *)gMapWork[0])->animation_active == 0)
        Scheduler_EnableCallbacks(MapAnimation_Update);
}

void Map_DisableUpdateCallback(void)
{
    if (((struct MapScrollWork *)gMapWork[0])->animation_active == 0)
        Scheduler_DisableCallbacks(MapAnimation_Update);
}

void DisplayBlend_RunScript(void)
{
    struct MapState *work = gMapWork[0];
    struct BlendScriptState *state;
    u16 *cursor;
    u32 command;
    u32 value;
    u16 test;

    state = &work->blend;
    if (state->script == 0)
        return;
    if (state->paused != 0)
        return;

again:
    value = state->delay;
    test = value;
    if (test != 0)
        goto tick;

    cursor = state->cursor;
    command = *cursor;
    cursor++;
    if (command == 0xffff) {
        state->cursor = state->script;
        goto again;
    }

    if ((command & 0xff00) == 0xfe00) {
        value = command & 0xff;
        if (value == 0xff)
            return;
        state->cursor = state->script + value * 2;
        goto again;
    }

    if ((command & 0xf000) == 0x3000) {
        REG_BLDCNT = command;
        work->blend_control = command;
        state->cursor++;
        goto again;
    }

    if ((work->blend_control & 0xc0) == 0x40)
        REG_BLDALPHA = command;
    else
        REG_BLDY = command;
    state->delay = cursor[0];
    state->cursor += 2;
    goto again;

tick:
    state->delay = value + 0xffff;
}

void DisplayBlend_StartScript(u16 *script)
{
    struct BlendScriptState *state;
    s32 started;
    volatile u32 zero;

    started = 0;
    zero = 0;
    state = &((struct MapState *)gMapWork[0])->blend;
    Dma_Set((const void *)&zero, state, 0x85000003, REG_DMA3);
    if (*script != 0xffff) {
        state->script = script;
        state->cursor = script;
        state->delay = 0;
        state->paused = 0;
        started = 1;
    }
    if (started)
        Scheduler_AddOrUpdateCallback(DisplayBlend_RunScript, 0xc80);
}

void DisplayBlend_EnableRunScript(void)
{
    Scheduler_EnableCallbacks(DisplayBlend_RunScript);
}

void DisplayBlend_DisableRunScript(void)
{
    Scheduler_DisableCallbacks(DisplayBlend_RunScript);
}

void Runtime_AllocateAndClearQueue(void)
{
    struct PaletteSnapshotWork *queue;
    struct PaletteSnapshot *entry;
    u16 i;
    u16 j;

    queue = Runtime_AllocateBlock(28, sizeof(struct PaletteSnapshotWork));
    entry = queue->entries;
    for (i = 0; i != 4; i++) {
        entry->source = 0;
        entry->position = 0;
        entry->timer = 0;
        entry->delay = 0;
        entry->count = 0;
        for (j = 0; j != 16; j++) {
            entry->colors[j] = 0;
        }
        entry++;
    }
    queue->count = 0;
}

s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count)
{
    struct PaletteSnapshotWork *work;
    struct PaletteSnapshot *entry;
    u16 *source;
    u32 slot;
    u16 size;

    work = gPaletteWork;
    slot = work->count;
    if (slot > 3)
        return -1;
    entry = &work->entries[slot];
    source = (u16 *)BG_PLTT + (((u16)bank << 4) + (u16)index);
    size = count;
    entry->position = 0;
    entry->timer = 0;
    entry->count = size;
    entry->source = source;
    entry->delay = delay;
    Dma_Set(source, entry->colors, 0x80000000 | size, REG_DMA3);
    work->count++;
    return 0;
}

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    Scheduler_RemoveCallback(PaletteQueue_StepCycles);
    Runtime_ReleaseHeapBlock(0x1C);
}

void Scheduler_ScheduleCallbackBAfterFrames(void)
{
    Scheduler_AddOrUpdateCallback(PaletteQueue_StepCycles, 0xc80);
}

void PaletteQueue_StepCycles(void)
{
    struct PaletteSnapshotWork *work = gPaletteWork;
    u8 i;
    u16 count = work->count & 3;
    u16 buf[16];

    for (i = 0; i < count; i++) {
        struct PaletteSnapshot *cycle = &work->entries[i];
        if (cycle->timer == 0) {
            u16 start = cycle->position;
            u16 len = cycle->count;
            void *dest = cycle->source;
            u16 *src = cycle->colors;
            u8 j;
            for (j = len - start; j < len; j++)
                buf[j] = *src++;
            for (j = 0; j < len - start; j++)
                buf[j] = *src++;
            Dma_Set(buf, dest, 0x80000000 | len, REG_DMA3);
            start++;
            if (start >= len)
                start = 0;
            cycle->position = start;
            cycle->timer = cycle->delay;
        } else {
            cycle->timer--;
        }
    }
}
