#include "unit/field/common/script/field.h"
#include "unit/field/common/script/script.h"
#include "unit/field/common/script/operands.h"
#include "unit/field/common/script/mapview.h"
#include "unit/field/common/script/mapcell.h"
#include "unit/graphics/tile/vertices.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/sched.h"
#include "unit/system/scheduler/math.h"
#include "unit/system/scheduler/heap.h"
#include "unit/system/scheduler/matrix.h"
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

static __inline__ void Io_Set16(s32 value, volatile u16 *reg)
{
	*reg = value;
}

static __inline__ void Io_Put16(volatile u16 *reg, s32 value)
{
	*reg = value;
}

typedef s32 (*PlaneFn)(void *camera, s32 *position, void *lines, void *out);

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct WorldCell
{
	u32 tile : 24;
	u32 kind : 7;
	u32 flag : 1;
};

#define BG_PALETTE ((s16 *)BG_PLTT)

#define WORLD_CELLS ((struct WorldCell *)Ram_MapBlocks)

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update)
{
	struct PerspectiveWork *work = gMapWork[0];
	u32					   *graphics = work->tiles;
	u16					   *shown;
	u16					   *buffer;
	u16					   *source;
	u8					   *blocks;
	u16					   *screen;
	u32						i;
	u32						j;
	u32						id;

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
	for (i = 0; i < 16; i++)
	{
		Dma_Set(source, blocks, DMA_ENABLE32 | DMA_32BIT | 0x10, REG_DMA3);
		source += 32;
		blocks += 128;
	}

	if (update != 0)
	{
		screen = (u16 *)(BG_SCREEN_BLOCK(8) + ((((layer * 2 + y) << 6) + x) << 5));
		source = buffer;
		for (i = 0; i < 16; i++)
		{
			for (j = 0; j < 16; j++)
			{
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
	struct BattleCamera	   *camera;
	struct MapFrameWork	   *tiles;
	struct MapAffinePair(*lines)[160];
	s32						 *position;
	s32						 *distance;
	u16						 *yaw;
	u16						 *pitch;
	u16						 *turn;
	volatile u32			  fill;
	struct FixedPointPosition vector;
	s32						  far_plane;
	s32						  i;

	REG_DISPCNT &=
		~(DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON);
	Blend_SetDarkenTarget0(0);
	work = Runtime_AllocateHeapBlock(8, sizeof(struct PerspectiveWork));
	fill = 0;
	Dma_Set(&fill, work,
		DMA_ENABLE32 | DMA_SRC_FIXED32 | DMA_32BIT | (sizeof(struct PerspectiveWork) / 4),
		REG_DMA3);
	work->view_x = 0;
	work->view_y = 0;
	work->scale_x = 0x200000;
	work->scale_y = 0x400000;
	work->limit_x = 0x1fe00000;
	work->limit_y = 0x1fe00000;
	work->script = NULL;
	work->tiles = Resource_GetTableEntry((s32)&ResourceId_PerspectiveDataA);
	Resource_DecodeType01(
		Resource_GetTableEntry((s32)&ResourceId_DefaultMapAnimation), Ram_MapCollision + 0x1000);
	MapAnimation_StartChannels((const u16 *)(Ram_MapCollision + 0x1000));
	Io_Set16(0x3f9e, &REG_BLDCNT);
	Io_Set16(0x1010, &REG_BLDALPHA);
	REG_BLDY = 0;
	Resource_DecodeType01(
		Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), Ram_MapCellBuffer);
	Resource_DecodeType01(
		Resource_GetTableEntry((s32)&ResourceId_DefaultMetatileAttributes), Ram_MapCollision);
	work->fade = 0x1f00;
	work->fade_step = 0x80;
	Io_Set16(0xa80a, &REG_BG3CNT);
	Io_Set16(0xaa0e, &REG_BG2CNT);
	Io_Set16(0x0501, &REG_BG1CNT);
	Io_Write16(0x100, &REG_BG2PA);
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	/* FAKEMATCH: needed to match: loop notes keep BG2PD's constant late and r0 first below */
	do
	{
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
	WorldMap_BuildScanlineTable(
		Iwram_RatioMulQ14(Trig_Cos(*pitch), Trig_Sin(*pitch)), position, tiles->scanlines);
	gScanlineAngle = 0;
	gScanlinePitch = work->pitch;
	((PlaneFn)gWorkSlot[46])(camera, position, tiles->scanlines, lines[gFrameCount & 1]);
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
	Io_Write16(0x42, &REG_DISPCNT);
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
	s32					   *position;
	s32						origin_x;
	s32						origin_y;
	s32						tile;

	window = gMapWork[0];
	origin_x = 0;
	origin_y = 0;
	position = window->origin;
	if (position != 0)
	{
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

	if (ABS(origin_x - px) <= 1 && ABS(origin_y - py) <= 1)
	{
		Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
		Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x140, 1);
	}
}

void WorldMap_LoadGraphics(s32 x, s32 z)
{
	struct PerspectiveWork *state;
	u32					   *resources;
	u8					   *buffer;
	u8					   *tiles;
	u32					   *cursor;
	u32						tile;
	s32						variant;
	s32						row;
	s32						col;
	s16						value;
	volatile u32			fill;

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
	Dma_Set(Ram_BgTileBuffer + 0x2000, BG_CHAR_BLOCK(2) + 0x2000, DMA_ENABLE32 | DMA_32BIT | 0x800,
		REG_DMA3);
	Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Ram_BgTileBuffer + 0x4000);
	Dma_Set(
		Ram_BgTileBuffer + 0x4000, BG_CHAR_BLOCK(3), DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
	Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Ram_BgTileBuffer + 0x6000);
	Dma_Set(Ram_BgTileBuffer + 0x6000, BG_CHAR_BLOCK(3) + 0x2000, DMA_ENABLE32 | DMA_32BIT | 0x800,
		REG_DMA3);
	Resource_DecodeType01(Resource_GetTableEntry(resources[5]), gMapLayerData);
	fill = 0xf07ff07f;
	Dma_Set((const void *)&fill, BG_SCREEN_BLOCK(5),
		DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x180, REG_DMA3);
	cursor = (u32 *)BG_SCREEN_BLOCK(6);
	tile = 0x01a901a8;
	for (row = 0; row < 20; row++)
	{
		for (col = 0; col < 15; col++)
		{
			*cursor++ = tile;
			tile += 0x00020002;
		}
		cursor++;
	}
	if (variant == 1)
	{
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
	struct MapAffinePair(*lines)[160] = ((struct MapFrameWork *)WORK_SLOT(7))->pages;
	s32						dispcnt = (s16)(REG_DISPCNT & 0xfff8);
	u32					   *dst = (u32 *)&REG_BG2PA;
	struct PerspectiveWork *work = WORK_SLOT(8);
	volatile u16		   *channel = REG_DMA0;
	u32					   *src;
	u32						mode;
	u32						top;
	u32						control;

	channel[5] &= 0xc5ff;
	channel[5] &= 0x7fff;
	(void)channel[5];
	if (lines != NULL)
	{
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
	if (top < 200)
	{
		u16 bottom = work->shown_bottom;

		mode = (bottom != 0) << 1;
		if (top <= bottom)
		{
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

static __inline__ void CopyEntry(u8 *map, u8 *destination)
{
	u32	 palette = *(u16 *)map;
	u16 *colors = (u16 *)Ram_MapCellBuffer;

	colors += palette * 2;
	*(u16 *)destination = *colors++;
	*(u16 *)(destination + 64) = *colors;
}

void Map_RenderPaletteMappedRow(u32 value)
{
	u8 *map;
	u8 *destination;
	u32 counter;

	map = Ram_MapBlocks + ((((s32)value / 2) & 31) << 7);
	destination = (u8 *)(0x06004000 + ((value & 62) << 6));
	counter = 0;
	do
	{
		CopyEntry(map, destination);
		counter++;
		destination += 2;
		map += 4;
	} while (counter <= 31);

	destination += 0xfc0;
	map += 0xf80;
	counter = 0;
	do
	{
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
	do
	{
		CopyEntry(map, destination);
		counter++;
		destination += 128;
		map += 128;
	} while (counter <= 63);
}

void WorldMap_UpdateView(void)
{
	struct BattleCamera	   *cam = WORK_SLOT(HEAP_SLOT_CAMERA);
	struct MapFrameWork	   *map = WORK_SLOT(7);
	struct PerspectiveWork *view = WORK_SLOT(8);
	s32					   *pos = cam->pos;
	s32					   *target = view->origin;
	struct MapAffinePair(*buffer)[160] = map->pages;
	s32						  distance = view->far_plane;
	s32						  height = view->distance;
	struct FixedPointPosition local;

	Map_UpdateCurrentTileBlockUntilBlocked();
	if (target != NULL)
	{
		s32 x;
		s32 y;
		s32 col;
		s32 row;

		y = target[2];
		x = target[0];
		if (view->shake_x != 0)
		{
			s32 r = Random16();
			s32 amp;

			r -= Random16();
			amp = view->shake_x;
			x += Iwram_MulQ16(amp, r);
			view->shake_x = Iwram_MulQ16(amp, view->shake_decay);
		}
		if (view->shake_y != 0)
		{
			s32 r = Random16();
			s32 amp;

			r -= Random16();
			amp = view->shake_y;
			y += Iwram_MulQ16(amp, r);
			view->shake_y = Iwram_MulQ16(amp, view->shake_decay);
		}
		col = x / 0x100000;
		row = y / 0x100000;
		if ((view->view_x ^ x) & 0x100000)
		{
			if (view->view_x < x)
				Map_RenderPaletteMappedColumn(col + 16);
			else
				Map_RenderPaletteMappedColumn(col - 16);
		}
		if ((view->view_y ^ y) & 0x100000)
		{
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
	Graphics_PrepareTransferInIwramWork(cam, pos);
	if (gScanlinePitch != view->pitch)
	{
		s32 c = Trig_Cos(view->pitch);
		s32 s = Trig_Sin(view->pitch);

		WorldMap_BuildScanlineTable(Iwram_RatioMulQ14(c, s), pos, map->scanlines);
		gScanlineAngle = 0;
		gScanlinePitch = view->pitch;
	}
	((s32 (*)(void *, s32 *, void *, void *))gWorkSlot[46])(
		cam, pos, map->scanlines, buffer[gFrameCount & 1]);
}

void Map_UpdateCurrentTileBlock(void)
{
	struct PerspectiveWork *window = gMapWork[0];
	s32						x0 = 0;
	s32						y0 = 0;
	u32						layer;
	u32						row;
	u32						col;
	s32						bias;
	s32						tile;

	if (window->origin != NULL)
	{
		s32 *p = window->origin;

		x0 = *p++;
		y0 = p[1];
	}
	x0 = (x0 - 0x1000000) >> 25;
	y0 = (y0 - 0x1400000) >> 25;
	layer = 0;
	bias = 0;
	for (; layer < 2; layer++)
	{
		for (row = 0; row < 2; row++)
		{
			for (col = 0; col < 2; col++)
			{
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
	s32						x0 = 0;
	s32						y0 = 0;
	u32						layer;
	u32						row;
	u32						col;
	s32						tile;

	if (window->origin != NULL)
	{
		s32 *p = window->origin;

		x0 = *p++;
		y0 = p[1];
	}
	x0 = (x0 - 0x1000000) >> 25;
	y0 = (y0 - 0x1400000) >> 25;
	for (layer = 0; layer < 2; layer++)
	{
		for (row = 0; row < 2; row++)
		{
			for (col = 0; col < 2; col++)
			{
				tile = (((y0 + row) & 15) << 4) + ((x0 + col) & 15);
				tile = window->tile_ids[tile];
				tile += layer * 320;
				if (Map_WriteLayerCellTile(layer, x0 + col, y0 + row, tile, 0) != 0)
					return;
			}
		}
	}
}

void MapAnimation_PresentFrame(void)
{
	Io_Write16(0x682, &REG_BG1CNT);
	Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | 0x2580, REG_DMA3);
}

void MapAnimation_Start(void)
{
	struct MapFrameWork	   *pages = WORK_SLOT(7);
	struct PerspectiveWork *work = WORK_SLOT(8);
	s32						one = 1;

	work->animation_active = one;
	Scheduler_EnableCallbacks(MapAnimation_Update);
	Dma_Set(BG_CHAR_BLOCK(1), gDecodeBuffer, DMA_ENABLE32 | DMA_32BIT | 0x800, REG_DMA3);
	WaitFrames(1);
	Resource_RunCopiedDecoder((s32)pages->pages[gFrameCount & one], (s32)gMapCellBuffer);
	work->window_top = 200;
	work->window_bottom = 255;
	gVBlankHook = MapAnimation_PresentFrame;
}

void Map_ShowBg1FromBuffer(void)
{
	Io_Write16(0x501, &REG_BG1CNT);
	Dma_Set(gBgTileBuffer, BG_CHAR_BLOCK(2), 0x84002000, REG_DMA3);
}

void Map_LoadAreaGraphics(void)
{
	struct PerspectiveWork *state;
	u32					   *resources;
	u8					   *buffer;
	s16						value;

	state = gMapWork[0];
	buffer = (u8 *)gMapCellBuffer;
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
	u32				 i;

	for (i = 0; i <= 15; i++)
	{
		const u16 *script;
		u32		   op;
		u32		   count;
		u32		   dst;

		if ((state->anim + i)->start == NULL || (state->anim + i)->paused != 0)
			continue;
	next:
		if ((state->anim + i)->timer == 0)
		{
			script = (state->anim + i)->cursor;
			op = *script++;
			if (op == 0xffff)
			{
				(state->anim + i)->cursor = (state->anim + i)->start;
				goto next;
			}
			if ((op & 0xff00) == 0xfe00)
			{
				if ((op & 0xff) == 0xff)
					continue;
				(state->anim + i)->cursor = (state->anim + i)->start + (op & 0xff) * 2;
				goto next;
			}
			count = *script++;
			dst = script[0];
			(state->anim + i)->timer = script[1];
			if (state->wide_tiles == 0)
			{
				if (op >= 0x600)
					Dma_Set(Ram_DecodeBuffer + op * 32, BG_CHAR_BLOCK(1) + dst * 32,
						DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
				else
					Dma_Set(BG_CHAR_BLOCK(1) + op * 32, BG_CHAR_BLOCK(1) + dst * 32,
						DMA_ENABLE32 | DMA_32BIT | count * 8, REG_DMA3);
			}
			else
			{
				if (op >= 0x200)
					Dma_Set(Ram_MapBlocks + op * 64, BG_CHAR_BLOCK(2) + dst * 64,
						DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
				else
					Dma_Set(BG_CHAR_BLOCK(2) + op * 64, BG_CHAR_BLOCK(2) + dst * 64,
						DMA_ENABLE32 | DMA_32BIT | count * 16, REG_DMA3);
			}
			(state->anim + i)->cursor += 4;
			goto next;
		}
		else
		{
			(state->anim + i)->timer--;
		}
	}
}

void Map_ClearLayerEntryFlag(u32 no)
{
	struct MapState		*state = gMapWork[0];
	struct MapAnimation *anim = &state->anim[no];

	anim->paused = 0;
}

void Map_SetLayerEntryFlag(u32 no)
{
	struct MapState		*state = gMapWork[0];
	struct MapAnimation *anim = &state->anim[no];

	anim->paused = 1;
}

void MapAnimation_StartChannels(const u16 *script)
{
	struct MapState *state;
	s32				 count;
	u32				 command;
	volatile u32	 zero;

	state = gMapWork[0];
	count = 0;
	zero = 0;
	Dma_Set((const void *)&zero, state->anim, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x30,
		REG_DMA3);
	command = *script++;
	while (command != 0xffff)
	{
		if ((command & 0xff00) == 0xfd00)
		{
			struct MapAnimation *anim;
			u32					 idx = command & 15;
			u32					 flag = 0;
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
	struct MapState			*work = gMapWork[0];
	struct BlendScriptState *state;
	u16						*cursor;
	u32						 command;
	u32						 value;
	u16						 test;

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
	if (command == 0xffff)
	{
		state->cursor = state->script;
		goto again;
	}

	if ((command & 0xff00) == 0xfe00)
	{
		value = command & 0xff;
		if (value == 0xff)
			return;
		state->cursor = state->script + value * 2;
		goto again;
	}

	if ((command & 0xf000) == 0x3000)
	{
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
	s32						 started;
	volatile u32			 zero;

	started = 0;
	zero = 0;
	state = &((struct MapState *)gMapWork[0])->blend;
	Dma_Set((const void *)&zero, state, 0x85000003, REG_DMA3);
	if (*script != 0xffff)
	{
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
