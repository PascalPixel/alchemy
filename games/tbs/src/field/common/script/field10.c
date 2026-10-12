#include "unit/field/common/fielddat.h"
#include "unit/field/common/object/objdata.h"
#include "unit/graphics/render/facing.h"
#include "unit/graphics/tile/griddata.h"
#include "unit/lib/curvedat.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/field/common/object/update.h"
#include "unit/graphics/tile/vertices.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
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

typedef s32 (*TerrainHeightFn)(u8 *heights, s32 x, s32 y);

extern TerrainHeightFn Map_TerrainHeightFunctions[16];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 y)
{
	struct MapState *work = gMapWork[0];
	struct MapCell	*cells;
	u8				*attributes;

	x >>= 16;
	y >>= 16;
	cells = (struct MapCell *)Ram_MapCellBuffer;
	if (work != NULL)
	{
		cells = work->layers[layer & 3].cells;
	}
	cells += x / 16 + (y / 16 << 7);
	attributes = Ram_MapCollision + cells->attribute_b * 4;
	return Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, y & 15);
}

s32 Map_GetCellAttributeLowNibble(s32 index, s32 x, s32 y)
{
	struct MapState *state = gMapWork[0];
	struct MapCell	*map;
	s32				 col;
	s32				 row;
	u32				 attr;

	x >>= 16;
	y >>= 16;
	map = (struct MapCell *)gMapCellBuffer;
	if (state != 0)
	{
		map = state->layers[index & 3].cells;
	}
	col = x / 16;
	row = y / 16;
	map += col + (row << 7);
	attr = map->attribute_b;
	return *(Ram_MapCollision + attr * 4) & 15;
}

s32 GetMapCellCollision(s32 layer, s32 x, s32 y)
{
	struct MapState *state;
	struct MapCell	*cells;

	state = gMapWork[0];
	x >>= 20;
	y >>= 20;
	cells = (struct MapCell *)gMapCellBuffer;
	if (state != NULL)
	{
		cells = state->layers[layer & 3].cells;
	}
	cells += x + (y << 7);
	return cells->collision_code;
}

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
	struct MapState *state = gMapWork[0];

	x >>= 20;
	y >>= 20;
	if (state != NULL)
	{
		struct MapCell *cells = state->layers[layer & 3].cells;

		cells += x + (y << 7);
		cells->collision_code = collision_code;
	}
}

s32 Map_GetCellHighFlags(s32 x, s32 y)
{
	s32				tile_x = x / 16;
	s32				tile_y = y / 16;
	struct MapCell *cell = (struct MapCell *)gMapCellBuffer + (tile_x + tile_y * 128);

	return ((u8 *)&cell->metatile_and_flags)[1] >> 6;
}

s32 Map_CheckObjectStep(struct ObjectRuntime *object, struct FieldPosition *position)
{
	struct MapState *state;
	s32				 x;
	s32				 z;
	struct MapCell	*cell;
	u8				*attributes;
	s32				 height;
	s32				 delta;

	x = ((union FieldCoordinate *)&position->x)->part.pixel;
	z = ((union FieldCoordinate *)&position->z)->part.pixel;
	state = gMapWork[0];
	if (state == NULL)
		return 0;
	if (object->terrain_id <= 2)
		cell = state->layers[object->terrain_id].cells;
	else
		cell = (struct MapCell *)Ram_MapCellBuffer;
	cell += x / 16 + (z / 16 << 7);
	if (cell->collision_code == 0xff)
		return 2;
	attributes = Ram_MapCollision + cell->attribute_b * 4;
	height = Map_TerrainHeightFunctions[*attributes++ & 15](attributes, x & 15, z & 15);
	delta = height - object->terrain_height;
	if (delta > 0x80000)
		return 1;
	if (delta < -0xc0000)
		return -1;
	return 0;
}

s32 CheckMapPositionCellOccupied(struct WorldPosition *position)
{
	s32				 x;
	s32				 y;
	s32				 tile_x;
	s32				 tile_y;
	struct MapState *work;
	struct MapCell	*cell;

	x = position->x / 65536;
	y = (position->y - position->height) / 65536;
	work = gMapWork[0];
	if (work == NULL)
		return 0;
	cell = work->layers[2].cells;
	tile_x = x / 16;
	tile_y = y / 16;
	cell += tile_x + tile_y * 128;
	return (cell->collision_code != 0xff) - 1;
}

s32 GetWorldMapCollision(struct WorldPosition *position)
{
	s32 x_step = position->x >> 17;
	s32 y = position->y;
	s32 y_step = y >> 17;
	u32 cell;
	u32 pixel_offset;
	u8	tile;
	u8	packed_pixels;
	u32 result;

	cell = (((y_step / 8) & 63) << 6) + ((x_step / 8) & 63);
	pixel_offset = (((y_step / 2) & 3) << 1) + ((x_step / 4) & 1);

	tile = BG_SCREEN_BLOCK(10)[cell];
	packed_pixels = *(Ram_MapCollision + 0x800 + (tile << 3) + pixel_offset);
	if (packed_pixels != 0)
	{
		if (x_step & 2)
			result = packed_pixels >> 4;
		else
			result = packed_pixels & 15;
		if (result != 0)
			return result;
	}

	tile = BG_SCREEN_BLOCK(8)[cell];
	packed_pixels = *(Ram_MapCollision + (tile << 3) + pixel_offset);
	if (packed_pixels != 0)
	{
		if (x_step & 2)
			result = packed_pixels >> 4;
		else
			result = packed_pixels & 15;
		if (result != 0)
			return result;
	}

	return 7;
}

s32 CheckWorldMapCollisionRange(s32 unused, struct WorldPosition *position)
{
	if ((GetWorldMapCollision(position) - 5) <= 7U)
	{
		return 0;
	}
	return -1;
}

s32 GetWorldMapTerrainBehavior(struct WorldPosition *position, s32 *terrain_kind)
{
	s32	 selector = GetWorldMapCollision(position);
	s32	 x = position->x;
	s32	 y;
	s32	 flag = 0;
	u32	 tile_x;
	u32	 index;
	u32 *tile;

	if (x < 0)
		x += 0x1fffff;
	tile_x = (x >> 21) & 31;
	y = position->y;
	if (y < 0)
		y += 0x1fffff;

	index = tile_x + (((y >> 21) & 31) << 5);
	tile = gMapBlocks + index;
	if (((u8 *)tile)[3] & 0x80)
		flag = 0x10;

	*terrain_kind = (*tile << 1) >> 25;
	if (*terrain_kind == 21)
		flag = 0x20;

	return WorldMap_TerrainBehaviorTable[flag + selector];
}

void Runtime_SetWorkTripleIfNonNegative(s32 value0, s32 value1, s32 value2)
{
	struct MapScrollWork *work;

	work = gMapWork[0];
	if (value0 >= 0)
	{
		work->shake_x = value0;
	}
	if (value1 >= 0)
	{
		work->shake_y = value1;
	}
	if (value2 >= 0)
	{
		work->shake_decay = value2;
	}
}

void Map_WaitWorkValuesBelow256(void)
{
	struct MapScrollWork *work;
	s32					  cnt;

	work = gMapWork[0];
	cnt = 0;
	if (work->shake_x > 255 || work->shake_y > 255)
	{
		goto body;
	body:
		WaitFrames(1);
		++cnt;
		if (cnt >= 300)
		{
			goto done;
		}
		if (work->shake_x > 255)
		{
			goto body;
		}
		if (work->shake_y > 255)
		{
			goto body;
		}
	}
done:
	work->shake_decay = 0;
}

void Resource_RunCopiedDecoder(s32 a, s32 b)
{
	u8 *base;

	base = gDecodeBuffer;
	RamCode_LoadSlot(49, TileMap_DrawRows, TileMap_DrawRowsCodeSize);
	/* FAKEMATCH: needed to match: the do-while orders the call's argument loads. */
	do
	{
		((void (*)(s32, s32, void *, void *))gWorkSlot[49])(
			a, b, gBgTileBuffer + 0x4000, base + 0x1000);
	} while (0);
	Runtime_ReleaseHeapBlock(49);
}

void WorldMap_BuildScanlineTable(s32 depth, s32 *position, struct MapScanline *row)
{
	s32	 ground[3];
	s32	 viewed[3];
	s32	 horizon;
	s32	 focus;
	s32 *view;
	s32	 line;
	s32	 distance;
	s32	 difference;
	s32	 scale;
	s32	 x;
	s32	 y;
	s32	 diagonal;
	s32	 vertical;
	s32	 zero;
	s32	 horizontal;

	zero = 0;
	ground[0] = position[0];
	ground[1] = zero;
	ground[2] = position[2];
	Iwram_TransformVector(ground, viewed);
	view = viewed;
	horizon = view[1] - Iwram_MulQ16(view[2], depth);
	focus = -gProjection.focal;
	for (line = 0; line < 160; line++)
	{
		distance = Iwram_RatioMulQ14(focus, (gProjection.center_y - line) << 16);
		difference = distance - depth;
		if (difference == 0)
			difference = 1;
		scale = Iwram_RatioMulQ14(difference, horizon);
		if (scale < 0)
		{
			diagonal = Iwram_MulQ16(-scale, 0x8000);
			row->scale = Iwram_RatioMulQ14(gProjection.focal, diagonal);
			diagonal = Iwram_MulQ16(scale, distance);
			horizontal = (view[2] - scale) >> 4;
			vertical = (diagonal - view[1]) >> 4;
			x = Iwram_MulQ16(horizontal, horizontal);
			y = Iwram_MulQ16(vertical, vertical);
			y = Iwram_Sqrt(x + y) << 12;
			if (vertical < 0)
				y = -y;
			row->depth = Iwram_MulQ16(y, 0x8000);
		}
		else
		{
			row->scale = 0;
			row->depth = 0;
		}
		row->offset_x = 0;
		row->offset_y = 0;
		row++;
	}
}
