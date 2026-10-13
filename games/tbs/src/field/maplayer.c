#include "unit/field/field.h"
#include "unit/field/script.h"
#include "unit/field/operands.h"
#include "unit/field/maplayer.h"
#include "unit/field/mapview.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/math.h"
#include "unit/runtime/heap.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "animspr.h"
#include "global_cells.h"
#include "dma.h"
#include "ramcode.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "iowrite.h"
#include "heap_state.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "object_dispatch.h"
#include "metadata_lookup.h"
#include "edition.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "btlsys.h"
#include "transform.h"
#include "script.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "script_object_runtime.h"
#include "fieldobj.h"
#include "fixed_math.h"
#include "keystate.h"
#include "fieldrun.h"
#include "event_runtime.h"
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "resource_ids.h"
#include "flag_ids.h"
#include "mapcopy.h"
#include "fixed_point_position.h"
#include "mapframe.h"
#include "btldisp.h"
#include "project.h"
#include "frame.h"
#include "palqueue.h"
#include "curve.h"
#include "btlpty.h"
#include "b5_context.h"
#include "vram_tab.h"
#include "vram_block.h"

void Runtime_RemapBytesByTable(u8 *buf, s32 cnt)
{
	s32 n;
	u8 *p;
	u8 *tbl;

	p = buf;
	tbl = Runtime_ByteRemapTable;
	n = cnt - 1;
	if (n != -1)
	{
		do
		{
			n -= 1;
			*p = tbl[*p];
			p += 1;
		} while (n != -1);
	}
}

//--------------------------------------------------------------------------

// decode_staged_buffer.c

// マップ：展開したブロックのデータを gMapBlocks にほどく

//--------------------------------------------------------------------------

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
	u32	 cnt = 128 << 7;
	u32	 mask = 0xfff;
	s32	 no = -1;

	do
	{
		u32 value = *p++;
		u32 idx = value & mask;

		if (idx == mask)
		{
			if (no != idx)
			{
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
	u8		 *saved;
	ConvertFn routine;
	u32		  size;

	size = 0x8000;
	saved = Runtime_BumpAllocateAlternatePool(size);
	Iwram_CopyWords(saved, Ram_MapCellBuffer, size);
	RamCode_Load(routine, Tile_ConvertMap, Tile_ConvertMapCodeSize);
	routine(((u8 *)gMapCellBuffer + 0x8000), Ram_MapCellBuffer, saved);
	MemFree(routine);
	MemFree(saved);
}

static __inline__ void CopyBlock(u32 *map, u8 *base, u32 rowmod, u32 colmod, u32 parity)
{
	u16 *colors;
	u8	*destination;
	u32	 index = ((*map << 20) >> 18) + parity;

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
	u8	*dest;
	u32	 index = (*map << 20) >> 19;

	tiles = (u32 *)Ram_MapBlocks;
	tiles += index;
	dest = base + rowmod * 2 + colmod * 2;
	*(u32 *)dest = *tiles;
	tiles = (u32 *)(Ram_MapBlocks + 4);
	tiles += index;
	*(u32 *)(dest + 64) = *tiles;
}

struct SceneEntry
{
	u16 resources[6];
};

extern struct SceneEntry Map_LayeredScenes[];

struct SceneLayerSource
{
	u8 x;
	u8 y;
	s8 scroll_x;
	s8 scroll_y;
	s8 speed_x;
	s8 speed_y;
	u8 period_x;
	u8 period_y;
};

struct SceneHeader
{
	u8						origin[4];
	u8						priority[3];
	u8						char_base[3];
	u16						unknown_0a;
	struct SceneLayerSource layers[3];
	s32						tiles;
	s32						collision;
	s32						tilemap;
	s32						animation;
	s32						blend;
	s32						script;
};

s32 Map_LoadLayeredScene(s32 index)
{
	struct SceneEntry		*entry;
	struct MapScrollWork	*work;
	struct SceneHeader		*header;
	struct MapLayerScroll	*layer;
	struct SceneLayerSource *source;
	s32						*scale_x;
	s32						*scale_y;
	s32						 i;
	u8						*buf;
	s32						 backdrop;
	u8						*src;
	s32						 cnt;

	REG_DISPCNT &=
		~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	Blend_SetDarkenTarget0(0);
	entry = &Map_LayeredScenes[index];
	work = Runtime_AllocateBlock(8, sizeof(struct MapScrollWork));
	Iwram_ClearWords(work, sizeof(struct MapScrollWork));
	header =
		(struct SceneHeader *)Resource_GetTableEntry(entry->resources[0] + (u32)&ResourceId_Map001);
	src = (u8 *)header + header->tiles;
	Tilemap_DecodeStagedBuffer(Resource_DecodeType01(src, Ram_MapCellBuffer + 1));
	src = (u8 *)header + header->collision;
	Resource_DecodeType01(src, Ram_MapCollision);
	src = (u8 *)header + header->tilemap;
	Resource_DecodeType01(src, Ram_MapCellBuffer);
	Tilemap_ConvertBuffer();
	if (header->animation != 0)
	{
		src = (u8 *)header + header->animation;
		Resource_DecodeType01(src, Ram_MapCollision + 0x1000);
		MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
	}
	if (header->blend != 0)
	{
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
	for (i = 0; i < 3; i++)
	{
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
	Io_Write16(work->enabled[1] | BGCNT_CHAR_BASE(header->char_base[1]) | BGCNT_SCREEN_BASE(6),
		&REG_BG2CNT);
	Io_Write16(work->enabled[2] | BGCNT_CHAR_BASE(header->char_base[2]) | BGCNT_SCREEN_BASE(7),
		&REG_BG1CNT);
	if (GameFlag_TestFar(FLAG_KEEP_LAYER_GRAPHICS) != 0)
	{
		GameFlag_ClearBitFar(FLAG_KEEP_LAYER_GRAPHICS);
	}
	else
	{
		buf = MemAlloc(0x4000);
		if (buf != NULL)
		{
			backdrop = *(s16 *)BG_PLTT;
			Resource_DecodeType01(
				Resource_GetTableEntry(entry->resources[1] + (u32)&ResourceId_Map001), buf);
			*(s16 *)buf = backdrop;
			/* FAKEMATCH: needed to match: these four copies call the fixed entry itself; through Iwram_CopyWords the scene header and the work move from r5 and r8 to r8 and sl (237 of 357 instructions differ), with or without the function's other devices. */
			Iwram_CopyWordsEntry((void *)BG_PLTT, buf, 0x1c0);
			Resource_DecodeType2(
				Resource_GetTableEntry(entry->resources[2] + (u32)&ResourceId_Map001), buf);
			Iwram_CopyWordsEntry(BG_CHAR_BLOCK(1), buf, 0x4000);
			Resource_DecodeType2(
				Resource_GetTableEntry(entry->resources[3] + (u32)&ResourceId_Map001), buf);
			Iwram_CopyWordsEntry(BG_CHAR_BLOCK(2), buf, 0x4000);
			Resource_DecodeType2(
				Resource_GetTableEntry(entry->resources[4] + (u32)&ResourceId_Map001), buf);
			Iwram_CopyWordsEntry(BG_CHAR_BLOCK(3), buf, 0x4000);
			Resource_DecodeType2(
				Resource_GetTableEntry(entry->resources[5] + (u32)&ResourceId_Map001),
				gMapLayerData);
			MemFree(buf);
		}
	}
	cnt = 0;
	REG_MOSAIC = cnt;
	REG_BLDCNT = cnt;
	cnt = DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP;
	REG_DISPCNT = cnt;
	/* FAKEMATCH: Scheduler_AddOrUpdateCallback is called as void (*)(void (*)(void), s32); as s32 (*)(SchedulerCallback, s32), its prototype, Map_LoadLayeredScene has 6 of 868 bytes different */
	((void (*)(void (*)(void), s32))Scheduler_AddOrUpdateCallback)(Map_UpdateLayerScroll, 0xc85);
	return 2;
}

void Map_ApplyWorkOriginAndSpan(void)
{
	struct MapScrollWork *work = gMapWork[0];
	s32					  x;
	s32					  y;
	s32					  height;
	s32					 *p;

	p = work->origin;
	x = 0;
	height = 0;
	y = 0;
	if (p != NULL)
	{
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

	for (counter = 0; counter <= 15; counter++)
	{
		u32 *map = (u32 *)Ram_MapCellBuffer;
		u32 *tiles;
		u8	*cell;
		u32	 index;

		map += row + col;
		index = (*map << 20) >> 19;
		tiles = (u32 *)Ram_MapBlocks;
		tiles += index;
		cell = dest + (rowmod + colmod) * 2;
		*(u32 *)cell = *tiles;
		tiles = (u32 *)(Ram_MapBlocks + 4);
		tiles += index;
		*(u32 *)(cell + 64) = *tiles;
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

	for (counter = 0; counter <= 10; counter++)
	{
		u32 *map = (u32 *)Ram_MapCellBuffer;

		map += row + col;
		CopyBlock(map, destination, rowmod, colmod, parity);

		row = (row + 128) & 0x3F80;
		rowmod = (rowmod + 64) & 0x3C0;
	}
}

void Map_UpdateLayerScroll(void)
{
	struct MapScrollWork  *work;
	struct MapLayerScroll *layer;
	s32					  *origin;
	s32					   x, y;
	s32					   min_x, min_y, max_x, max_y;
	s32					   base_x, base_y;
	u32					   i;

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

	if (work->shake_x != 0)
	{
		s32 first = Random16();
		s32 second = Random16();
		s32 amplitude = work->shake_x;
		base_x += Iwram_MulQ16(amplitude, first - second);
		work->shake_x = Iwram_MulQ16(amplitude, work->shake_decay);
	}
	if (work->shake_y != 0)
	{
		s32 first = Random16();
		s32 second = Random16();
		s32 amplitude = work->shake_y;
		base_y += Iwram_MulQ16(amplitude, first - second);
		work->shake_y = Iwram_MulQ16(amplitude, work->shake_decay);
	}
	work->view_x = base_x;
	work->view_y = base_y;
	for (i = 0; i < 3; i++, layer++)
	{
		base_x = Iwram_MulQ16(work->view_x, layer->scale_x);
		base_y = Iwram_MulQ16(work->view_y, layer->scale_y);
		if (layer->speed_x != 0)
		{
			layer->phase_x += layer->speed_x;
			base_x = (base_x + layer->phase_x) & ((layer->mask_x << 19) | 0x7ffff);
		}
		if (layer->speed_y != 0)
		{
			layer->phase_y += layer->speed_y;
			base_y = (base_y + layer->phase_y) & ((layer->mask_y << 19) | 0x7ffff);
		}
		base_x += layer->offset_x;
		base_y += layer->offset_y;
		x = base_x / 0x80000;
		y = base_y / 0x80000;
		if ((layer->x ^ base_x) & 0x80000)
		{
			if (layer->x < base_x)
				Map_RenderPaletteMappedBlock(i, x + 30, y);
			else
				Map_RenderPaletteMappedBlock(i, x, y);
		}
		if ((layer->y ^ base_y) & 0x100000)
		{
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
	u32					   no;
	u8					  *dest;
	struct MapScrollWork  *work = gMapWork[0];
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

	for (no = 0; no < 3; no++)
	{
		if (work->enabled[no])
		{
			u32 rows = 22;
			u32 row, col, rowmod, colmod, i, j;

			x = Iwram_MulQ16(work->view_x, layer->scale_x);
			y = Iwram_MulQ16(work->view_y, layer->scale_y);
			if (layer->speed_x)
			{
				layer->phase_x += layer->speed_x;
				x = (x + layer->phase_x) & ((layer->mask_x << 19) | 0x7ffff);
			}
			if (layer->speed_y)
			{
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
			for (i = 0; i < rows / 2; i++)
			{
				col = (x / 2) & 127;
				colmod = x & 30;
				for (j = 0; j <= 15; j++)
				{
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

// レイヤーの表示範囲の左上（セル）
struct TilePos
{
	s32 x, y;
};

void Map_CopyMetatileIndicesRect(s32 src_x, s32 src_y, u32 dst_x, u32 dst_y, u32 width, u32 height)
{
	/* FAKEMATCH: needed to match: temporary sharing preserves register allocation and scheduling without changing the frame or instructions. Measured 2026-10-11, before the volatile casts on the two word copies were dropped as unneeded: i = 0 after the offset differs 59, at the top of the body 32; a fresh bytes = cell * 8 is hoisted by loop.c, 158; the scale before the first destination, 4; address expressions without scr and blk, 113-153. A plain body differs 214. */
	u32					  *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
	u32					  *dst = (u32 *)Ram_MapCellBuffer + (dst_y * 128 + dst_x);
	struct MapLayerScroll *layer = ((struct MapScrollWork *)gMapWork[0])->layers;
	struct TilePos		   tile[3];
	s32					   i;
	struct TilePos		  *pos = tile;
	u32					   y;
	u32					   x;

	for (i = 2; i >= 0; i--)
	{
		pos->x = layer->x >> 20;
		pos->y = layer->y >> 20;
		pos++;
		layer++;
	}
	for (y = dst_y; y < dst_y + height; y++)
	{
		for (x = dst_x; x < dst_x + width; x++)
		{
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
			for (; i < 3; i++)
			{
				if (pos->x <= (s32)x && 16 + pos->x > (s32)x && pos->y <= (s32)y
					&& pos->y + 12 > (s32)y)
				{
					u32 scr, blk;
					u8 *dest, *tiles;

					scr = 0x06002800;
					dest = (u8 *)scr;
					dest += offset;
					tmp11 = cell * 8;
					blk = (u32)Ram_MapBlocks;
					tiles = (u8 *)blk;
					tiles += tmp11;
					*(u32 *)dest = *(u32 *)tiles;

					blk = (u32)Ram_MapBlocks + 4;
					tiles = (u8 *)blk;
					tiles += tmp11;
					scr = 0x06002840;
					dest = (u8 *)scr;
					dest += offset;
					*(u32 *)dest = *(u32 *)tiles;
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

struct MapCopyCommand
{
	u16 source;
	s16 y, width, height, delay;
} __attribute__((packed, aligned(2)));
typedef char MapCopyCommandSize[(sizeof(struct MapCopyCommand) == 10) ? 1 : -1];

void Map_PlayMetatileCopySequence(u16 *command, s32 destination_x, u32 destination_y)
{
	struct MapCopyCommand *record = (struct MapCopyCommand *)command;
	u32					   source = record->source;
	u32					   end = 0xffff;
	u32					   test = source;

	/* FAKEMATCH: the conditional cancelling updates end CSE before the
     * comparison, preserving its ip copy while source stays in r0. Both
     * updates disappear; removing them loses the copy (53 -> 52 English
     * instructions, five rows differ). */
	if (record)
	{
		source ^= test;
		source ^= test;
	}
	if (test != end)
	{
		u32	 stop = end;
		s16 *args = (s16 *)(command + 1);

		while (1)
		{
			s32 source_y = args[0];
			s32 width = args[1];
			s32 height = args[2];
			s32 delay = args[3];
			args += 5;

			Map_CopyMetatileIndicesRect(
				source, (u16)source_y, destination_x, destination_y, (u16)width, (u16)height);
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

#define MAP_CELLS ((u32 *)gMapCellBuffer)
#define MAP_CELL_TILE_MASK 0x00000fff	   // ブロック番号
#define MAP_CELL_ATTRIBUTE_MASK 0xfffff000 // 属性

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
	struct TilePos		   view[3];
	s32					   i;
	struct TilePos		  *pos = view;
	s32					   x;

	// 3 つのレイヤーの表示範囲（スクロール位置をセル単位に）
	i = 2;
	while (i >= 0)
	{
		s32 tmp5;

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
				u32	 val = *src++;
				u32	 cell;
				u32	 offset;
				u32	 tmp3;
				u32 *dest;
				u32	 n;
				u32 *tiles;
				u32	 tmp6;
				u32	 tmp12;
				u32	 tmp13;

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
						 * folds away, preserving the native 36-byte frame and registers.
						 * The copy chain through tmp2 to tmp17 sets the registers and
						 * stack slots: written as two plain word stores, tbs-en differs. */
						u32 bytes = cell * 8;
						u32 tmp8;
						u32 tmp17;

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
			src += (128 - width);
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
	/* FAKEMATCH: needed to match: word/index temporaries preserve register allocation; the plain body allocates differently and uses a 40-byte frame. Measured 2026-10-11, before the volatile casts on the two word copies were dropped as unneeded: i = 0 after pos = view differs 38; the scale into n, 52; the scale before the first destination, 54. A plain body differs 204. */
	s32					   y;
	u32					  *src = (u32 *)Ram_MapCellBuffer + (src_y * 128 + src_x);
	u32					  *dst = &((u32 *)Ram_MapCellBuffer)[dst_y * 128 + dst_x];
	struct MapLayerScroll *layer = ((struct MapScrollWork *)*gMapWork)->layers;
	struct TilePos		   view[3];
	s32					   i;
	struct TilePos		  *pos = view;
	s32					   x;

	// 3 つのレイヤーの表示範囲（スクロール位置をセル単位に）
	i = 2;
	while (i >= 0)
	{
		s32 tmp5;

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
				u32 val = *src++;
				u32 cell;
				u32 offset;
				u32 tmp3;
				u32 n;
				u32 tmp6;
				u32 tmp12;
				u32 tmp13;

				cell = val & MAP_CELL_TILE_MASK;
				dst[0] = (dst[0] & MAP_CELL_ATTRIBUTE_MASK) | cell;
				tmp12 = x & 15;
				tmp6 = tmp12;
				dst += 1;
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
						*(u32 *)dest = *(u32 *)tiles;

						blk = (u32)Ram_MapBlocks + 4;
						tiles = (u8 *)blk;
						tiles += tmp13;
						scr = 0x06002840;
						dest = (u8 *)scr;
						dest += offset;
						*(u32 *)dest = *(u32 *)tiles;
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

void Map_SetWorkFlagBits9To11(u32 v)
{
	struct MapState *state = gMapWork[0];
	u32				 mask = v & 0xe00;
	u32				 flags = state->flags;

	flags = (flags & 0xf1ff) | mask;
	state->flags = flags;
}
