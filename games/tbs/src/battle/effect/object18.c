#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/effect/move.h"
#include "unit/graphics/display/scroll_initialize_hblank_dma.h"
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
#include "types.h"
#include "scene.h"
#include "btltype.h"
#include "game_state.h"
#include "fieldrun.h"
#include "btlsys.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "eventwrk.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "fieldobj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "ownerval.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "itemgift.h"
#include "btlcalc.h"
#include "disptran.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "btlpty.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "fieldcoo.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_efx.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"

// ワールドマップの作業領域（gMapCellBuffer に置く）
struct WorldMapWork
{
	u16						   vram_block;	// 印の OBJ キャラクタの VRAM ブロック
	u16						   window_slot; // もうひとつの VRAM ブロック（終わりに返す）
	union FieldCoordinate	   x;			// カーソル（ドット）
	union FieldCoordinate	   z;
	u8						   padding0c[6];
	s16						   shown; // いま地名ウインドウに出している印（-1 はなし）
	u8						   padding14[4];
	s32						   speed; // カーソルの速さ
	s32						   window;
	struct AnimationSpritePart markers[67];
};

void BattleFx_SetupResourcesAndWindow(void)
{
	u32					*oam;
	volatile u32		 fill;
	u32					*pal;
	struct WorldMapWork *work;
	struct FieldActor	*object;
	s32					 attr;
	u32					 i;
	u32					 tile;
	u32					 attr2;

	pal = Runtime_BumpAllocateAlternatePool(32);
	work = (struct WorldMapWork *)gMapCellBuffer;
	oam = (u32 *)work->markers;
	object = Object_GetById(gGameState.selected_actor);
	work->vram_block = Resource_FindFreeEntry();
	fill = 0;
	Dma_Set((const void *)&fill, pal, 0x85000020, REG_DMA3);
	pal[0] = 255;
	pal[1] = 0x1ff;
	pal[2] = 272;
	pal[8] = 68;
	pal[9] = 324;
	pal[10] = 272;
	pal[16] = 119;
	pal[17] = 0x177;
	pal[18] = 272;
	pal[24] = 0xff0;
	pal[25] = 0xffff;
	pal[26] = 0x1ffff;
	pal[27] = 0x11ff0;
	pal[28] = 0x1100;
	attr = VramBlock_LoadCached(work->vram_block, 128, pal) | 0x400;
	for (i = 0; i < 66; i++)
	{
		*oam++ = 0;
		*oam++ = 0;
		*oam++ = attr;
	}
	MemFree(pal);
	if (GameFlag_TestFar(284))
	{
		work->x.fixed = 240 << 15;
		work->z.fixed = 160 << 15;
	}
	else
	{
		work->x.fixed = (((object->x.fixed - 0x10000000) >> 16) * 240 / 4096) << 16;
		work->z.fixed = ((object->z.part.pixel * 160) / 4096) << 16;
	}
	work->window_slot = Resource_LoadFixedBlockBIntoFreeSlotFar();
	tile = gVramBlockCache[work->window_slot].offset;
	work->window = UiWindow_CreateFar(0, 0, 0, 0, 2);
	work->shown = -1;
	work->speed = 0x10000;
	*oam++ = 0;
	*oam++ = 0x40000000;
	attr2 = 0x400;
	attr2 |= tile >> 5;
	*oam = attr2;
}

//--------------------------------------------------------------------------
// cleanup_resources_and_window.c
// ワールドマップ：地名の印・カーソル・地名ウインドウ
//--------------------------------------------------------------------------

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
// キーを離すとカーソルが近くの地名へ寄る
#define WORLD_MAP_CURSOR_SNAPS 1
#define WORLD_MAP_NAME_RANGE 64 // 地名を出す距離の 2 乗
#define WORLD_MAP_NAME_LEFT 3	// 地名ウインドウを印から左へずらす量
#else
#define WORLD_MAP_CURSOR_SNAPS 0
#define WORLD_MAP_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

#define FLAG_NOT_ON_MAP 284 // いまいる場所が地図にのっていない

//--------------------------------------------------------------------------
// ワールドマップを閉じる：VRAM ブロックを返し、地名ウインドウを消す
//--------------------------------------------------------------------------
void BattleFx_CleanupResourcesAndWindow(void)
{
	struct WorldMapWork *work = (struct WorldMapWork *)gMapCellBuffer;

	Resource_ResetEntry(work->vram_block);
	Resource_ResetEntry(work->window_slot);
	UiWork_FinalizeFar(work->window, 2);
}

//--------------------------------------------------------------------------
// ワールドマップの毎フレーム処理（スケジューラのコールバック）
// フラグの立った場所に印を出し、十字キーでカーソルを動かし、
// カーソルにいちばん近い印の地名をウインドウに出す
// L・R を押している間はカーソルを現在地に合わせる
//--------------------------------------------------------------------------
void Map_UpdateWorldMapMarkers(void)
{
	s32							leader;
	const s32				   *place;
	s32							tile_base;
	s32							blend;
	s32							best;
	s32							best_dist;
	s32							cursor_x;
	s32							best_x;
	s32							best_y;
	s32							mes;
	s32							best_mes;
	s32							height;
	s32							width;
	s32							pos[3];
	struct WorldMapWork		   *work;
	struct FieldActor		   *obj;
	s32							cursor_y;
	s32							tile;
	s32							mode;
	s32							flag;
	s32							i;
	struct AnimationSpritePart *marker;
	s32							x;
	s32							y;
	s32							angle;

#if WORLD_MAP_CURSOR_SNAPS
	leader = gGameState.selected_actor;
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	marker = work->markers;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
#else
	work = (struct WorldMapWork *)Ram_MapCellBuffer;
	leader = gGameState.selected_actor;
	place = WorldMap_PlaceMarkers;
	tile_base = gVramBlockCache[work->vram_block].offset >> 5;
	marker = work->markers;
#endif
	best = -1;
	best_dist = WORLD_MAP_NAME_RANGE;
	blend = WorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

	//------------------------------------------------------------------
	// カーソル
	//------------------------------------------------------------------
	if (!GameFlag_TestFar(FLAG_NOT_ON_MAP) && (gKeysHeld & KEYS_SHOULDERS))
	{
		// L・R：現在地
		obj = ObjectTable_Get(gGameState.selected_actor);
		if (obj == NULL)
			goto markers;
		cursor_x = ((obj->x.fixed - 0x10000000) >> 16) * 240 / 4096;
		cursor_y = obj->z.part.pixel * 160 / 4096;
	}
	else
	{
		// 十字キー：押し続けると速くなる
		angle = WorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
		if (angle != 0xffff)
		{
			pos[0] = work->x.fixed;
			pos[1] = 0;
			pos[2] = work->z.fixed;
			Vector_AddPolarOffset(work->speed, angle, pos);
			if (pos[0] < 0x100000) // x は 16〜239 ドット
				pos[0] = 0x100000;
			if (pos[0] > 0xef0000)
				pos[0] = 0xef0000;
			if (pos[2] < 0) // y は 0〜141 ドット
				pos[2] = 0;
			if (pos[2] > 0x8d0000)
				pos[2] = 0x8d0000;
			work->x.fixed = pos[0];
			work->z.fixed = pos[2];
			if (work->speed < 0x60000)
				work->speed += 0x2000;
		}
		else
		{
			work->speed = 0x10000;
		}
		cursor_x = work->x.part.pixel;
		cursor_y = work->z.part.pixel;
	}

	//------------------------------------------------------------------
	// 印：0 番は現在地、11〜65 番は WorldMap_PlaceMarkers の場所
	// 表は（フラグ, 地名）の組で、フラグの上位 16bit はキャラクタ番号
	//------------------------------------------------------------------
markers:
	for (i = 0; i <= 65; i++)
	{
		if (i != leader && i <= 10)
			continue;
		if (i == 0)
		{
			if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
				continue;
			tile = 0;
			flag = leader;
			mode = 0;
		}
		else
		{
			flag = *place++;
			mes = *place++;
			if (flag == 0)
				continue;
#if WORLD_MAP_CURSOR_SNAPS
			if (flag == -1)
				break;
			tile = flag >> 16;
			mode = 1;
#else
			tile = flag >> 16;
			mode = 1;
			if (flag == -1)
				break;
#endif
		}
		if (!GameFlag_TestFar(flag))
			continue;
		obj = ObjectTable_Get(i);
		if (obj == NULL)
			continue;

		// ワールドの 4096 ドット四方を 240x160 に縮める
		x = ((obj->x.fixed - 0x10000000) >> 16) * 240 / 4096;
		y = obj->z.part.pixel * 160 / 4096;
		marker->object_mode = mode;
		marker->tile = tile_base + tile;
		marker->x = x - 1;
		marker->y = y - 1;
		width = x - cursor_x;
		height = y - cursor_y;
		if (width * width + height * height < best_dist)
		{
			best_mes = mes;
			best = i;
			best_dist = width * width + height * height;
			best_x = x;
			best_y = y;
		}
		if (i != 0 || (gFrameCount & 15) <= 7) // 現在地は 8 フレームごとに点滅
			Runtime_PushSlotEntry(marker++, y);
	}
#if WORLD_MAP_CURSOR_SNAPS
	// キーを離していれば、カーソルをいちばん近い印へ寄せる
	if (gKeysHeld == 0)
	{
		if (best != -1)
		{
			if (best_dist <= 1)
			{
				work->x.fixed = best_x << 16;
				work->z.fixed = best_y << 16;
				work->speed = 0x10000;
			}
			else
			{
				if (work->speed < 0x20000)
					work->speed = 0x20000;
				angle = ArcTan2(best_y - cursor_y, best_x - cursor_x);
				pos[0] = work->x.fixed;
				pos[1] = 0;
				pos[2] = work->z.fixed;
				Vector_AddPolarOffset(work->speed, (u16)angle, pos);
				work->x.fixed = pos[0];
				work->z.fixed = pos[2];
				if (work->speed < 0x60000)
					work->speed += 0x2000;
			}
			cursor_x = work->x.part.pixel;
			cursor_y = work->z.part.pixel;
		}
		else
		{
			work->speed = 0x10000;
		}
	}
#endif

	// いちばん近い印を囲む枠（点滅）
	if (best != -1 && (gFrameCount & 15) <= 7)
	{
		marker->object_mode = 0;
		marker->tile = tile_base + 3;
		marker->x = best_x - 2;
		marker->y = best_y - 2;
		Runtime_PushSlotEntry(marker, best_y);
	}

	// カーソル
	marker = &work->markers[66];
	marker->affine_index = 0;
	marker->x = cursor_x - 17;
	marker->y = cursor_y + 1;
	Runtime_PushSlotEntry(marker, 246);

	//------------------------------------------------------------------
	// 地名ウインドウ：印が変わったときだけ書き直す
	//------------------------------------------------------------------
	if (work->shown != best)
	{
		work->shown = best;
		RenderOutput_PrepareForRedrawFar(work->window);
		if (best != -1)
		{
			if (best == 0)
				best_mes = (s32)MsgPresentLocation;
			else
				best_mes = Scene_GetAreaNameIndex(best_mes, 1) + (s32)MsgDebugEntryName;
			UiText_MeasureResourceEntriesFar(best_mes, &width, &height);
			cursor_x = best_x;
			cursor_y = best_y;
			cursor_x -= WORLD_MAP_NAME_LEFT;
			cursor_y -= 11;
			if (cursor_x + width > 240)
			{
				// 右へはみ出すなら、印の左上へ回す
				cursor_x = 232 - width;
				cursor_y = best_y - 20;
			}
			if (cursor_y < 0)
				cursor_y = 0;
			UiText_DrawResourceFar(best_mes, work->window, cursor_x, cursor_y);
		}
	}

	// 印の半透明を明滅させる
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x3f00, 0x20000);
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}

void Map_ShowWorldMap(s32 picture)
{
	struct MapState			*map = WORK_SLOT(8);
	struct EventWork		*field = Runtime_AllocateBlock(27, 0xccc);
	struct ObjectSystemWork *menu = WORK_SLOT(HEAP_SLOT_MENU_CONTROL);
	s32						 resource = (s32)&ResourceId_WorldMapPicture;
	struct MapAnimation		*layer;
	u8						 saved_flags[16];
	s32						 i;
	u8						*graphics;
	s32						 saved_status;
	s16						 blend;

	if (field->mode != 3)
		return;
	field->view_center->paused = 1;
	saved_status = field->transition_frames;
	field->transition_frames = 6;
	Event_FinishScreenTransition();
	Event_WaitTransitionFrames();
	layer = map->anim;
	for (i = 0; i < 16; i++)
	{
		saved_flags[i] = layer->paused;
		layer->paused = 1;
		layer++;
	}
	menu->suspended = 1;
	WaitFrames(1);
	blend = REG_BLDALPHA;
	graphics = Resource_GetTableEntry(resource);
	Dma_Set(graphics, (void *)BG_PLTT, DMA_ENABLE32 | DMA_32BIT | (448 / 4), REG_DMA3);
	BG_PLTT[0] = 0;
	Resource_DecodeType01(graphics + 448, gMapCellBuffer);
	Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00, DMA_ENABLE32 | DMA_32BIT | (240 * 160 / 4),
		REG_DMA3);
	Io_Write16(BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2, &REG_BG1CNT);
	Io_Write16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	BattleFx_SetupResourcesAndWindow();
	Scheduler_AddOrUpdateCallback(Map_UpdateWorldMapMarkers, 0xc80);
	if (GameFlag_TestFar(284))
		UiText_ShowPositionedMessageAndWaitFar((s32)MsgNotOnMap, 1);
	do
	{
		WaitFrames(1);
	} while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
	Scheduler_RemoveCallback(Map_UpdateWorldMapMarkers);
	BattleFx_CleanupResourcesAndWindow();
	Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
	Map_LoadAreaGraphicsFar();
	IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, 0x20000);
	layer = map->anim;
	for (i = 0; i < 16; i++)
	{
		layer->paused = saved_flags[i];
		layer++;
	}
	menu->suspended = 0;
	Event_StartScreenTransition();
	Event_WaitTransitionFrames();
	field->transition_frames = saved_status;
	field->view_center->paused = 0;
}

void BattleFx_UpdateObjectVisibilityBounds(void)
{
	struct ObjectRuntime *object;
	s32					  x;
	s32					  y;
	s32					  left;
	s32					  right;
	s32					  top;
	s32					  bottom;
	u32					  id;

	// the selected actor is looked up and the result discarded, as the ROM does
	Object_GetById(gGameState.selected_actor);
	object = (struct ObjectRuntime *)gEventWork->view_center;
	x = object->x;
	left = x - (320 << 16);
	right = x + (320 << 16);
	y = object->z;
	top = y - (600 << 16);
	bottom = y + (400 << 16);

	id = 8;
	do
	{
		object = ObjectTable_Get(id);

		if (object != 0)
		{
			s32 ox = object->x;
			s32 oy = object->z;

			if (ox < left || ox > right || oy < top || oy > bottom)
			{
				object->animation_kind = 0;
			}
			else
			{
				object->animation_kind = 1;
			}
		}
		id++;
	} while (id <= 65);
}

void BattleFx_RunVisibilityTransition(void)
{
	struct EventWork *field = Runtime_AllocateBlock(27, 0xccc);

	if (field->mode == 3)
	{
		Scheduler_EnableUnmaskedOverlayCallbacks();
		BattleFx_UpdateObjectVisibilityBounds();
		Battle_InitializeRenderObject();
		BattleFx_ScheduleRatioTransition(0x9d89, 6);
		if (gKeysHeld & KEY_L)
		{
			do
			{
				WaitFrames(1);
			} while (gKeysHeld & KEY_L);
		}
		BattleFx_ScheduleRatioTransition(0x10000, 6);
		Scheduler_DisableOverlayCallbacks();
	}
}
