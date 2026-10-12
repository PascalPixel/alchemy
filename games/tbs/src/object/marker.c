#include "vram_tab.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/marker.h"
#include "unit/object/sparkle.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/runtime/runtime.h"
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

//--------------------------------------------------------------------------
// 物体を地面の高さに合わせる
//--------------------------------------------------------------------------
void ObjectMotion_SnapToTerrain(void *object)
{
	struct ObjectRuntime *obj = object;
	s32					  height;

	height = Map_GetTerrainHeightFar(0, obj->x, obj->z);
	obj->y = height;
	obj->terrain_height = height;
}

// マップデータの先頭
struct MarkerMap
{
	u8	unknown_00[0x10];
	u8 *markers; // (列, 行, 番号) の並び、255, 255 で終わる
};

struct MarkerSlot
{
	struct ScriptObjectRuntime *object;
	u8							id; // イベントのトリガー番号
	u8							unknown_05;
	u8							column; // 16 ドットのマス
	u8							row;
};

// イベントワークのうち、マーカーの記録
struct MarkerWork
{
	u8				  unknown_000[0x11c];
	struct MarkerSlot slots[MARKER_MAX];
};

//--------------------------------------------------------------------------
// マーカーの位置に、対応するイベントの物体を置く
// 番号 100-239 のマーカーだけを扱い、最大 10 個まで記録する
//--------------------------------------------------------------------------
void Battle_PlaceMapMarkers(void)
{
	u32							id;
	struct MarkerMap		   *map = gMapWork[0];
	struct MarkerWork		   *work = gMapWork[HEAP_SLOT_EVENT - 8];
	s32							count = 0;
	u8						   *list = map->markers;
	struct MarkerSlot		   *slot = work->slots;
	volatile u32				fill;
	const struct SceneEvent	   *event;
	struct ScriptObjectRuntime *obj;
	u32							column;
	u32							row;

	fill = 0;
	Dma_Set((void *)&fill, slot,
		DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof(struct MarkerSlot) * MARKER_MAX / 4),
		REG_DMA3);
	if (list == NULL)
		return;

	column = *list++;
	row = *list++;
	while (column != 255 || row != 255)
	{
		id = *list++;
		if (id >= 100 && id <= 239)
		{
			event = gOverlayArea.events();
			for (; event->control != -1; event++)
			{
				if (event->trigger != id)
					continue;
				if ((event->control & 0x1ff) == 19)
				{
					// マスの中心に物体 20 を置く
					obj = (struct ScriptObjectRuntime *)Object_CreateFar(
						20, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
					if (obj == NULL)
						continue;
					ObjectMotion_SnapToTerrain(obj);
					ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
					if (GameFlag_TestFar(event->condition))
					{
						if ((event->value & 0xfff00000) == 0x500000)
						{
							ObjectDispatch_ReleaseFar(obj);
							continue;
						}
						ObjectDispatch_ApplyArgumentToChildrenFar(obj, 2);
					}
					ObjectDispatch_InitFromTable6Far(obj);
					obj->home_x = obj->x / 0x10000;
					obj->home_z = obj->z / 0x10000;
					obj->unknown_23 = 1;
					obj->flags_59 = 1;
					slot->id = event->trigger;
					slot->object = obj;
					slot->column = obj->x / 0x100000;
					slot->row = obj->z / 0x100000;
					slot++;
					count++;
					if (count >= MARKER_MAX)
						return;
				}
				else if ((event->control & 0x1ff) == 3)
				{
					// フラグが立っていなければ、物体 28 を置く
					if ((event->value & 0xfff00000) != 0x300000)
						continue;
					if (GameFlag_TestFar(event->condition))
						continue;
					obj = (struct ScriptObjectRuntime *)Object_CreateFar(
						28, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
					if (obj == NULL)
						continue;
					ObjectMotion_SnapToTerrain(obj);
					ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
					ObjectDispatch_InitFromTable6Far(obj);
					ObjectDispatch_ApplyArgumentToChildrenFar(obj, 1);
					obj->home_x = obj->x / 0x10000;
					obj->home_z = obj->z / 0x10000;
					obj->flags_59 = 1;
					obj->unknown_23 = 1;
					slot->object = obj;
					slot->id = event->trigger;
					slot->column = obj->x / 0x100000;
					slot->row = obj->z / 0x100000;
					slot++;
					count++;
					if (count >= MARKER_MAX)
						return;
				}
				else
				{
					continue;
				}
				break;
			}
		}
		column = *list++;
		row = *list++;
	}
}

s32 EffectRuntime_FindSlotByObject(s32 id)
{
	s32						  index;
	s32						  found;
	struct EffectRuntimeSlot *slot;

	slot = ((struct EffectRuntime *)gEventWork)->slots;
	found = -1;
	for (index = 0; index <= 9; index++, slot++)
	{
		if (slot->active == id)
		{
			found = index;
			break;
		}
	}
	return found;
}

void EffectRuntime_ReservedNoOp(void)
{
}

// id のオブジェクトの絵のアニメを mode にし、音 cue を鳴らして 12 フレーム待つ
static __inline__ void SetModeAndPlayCue(s32 id, s32 mode, s32 cue)
{
	s32						  slot;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		if (object != NULL)
			ObjectDispatch_ApplyArgumentToChildrenFar(object, mode);
		AudioCommand_PlayFar(cue);
		WaitFrames(0xc);
	}
}

void EffectRuntime_SetMode5AndPlayCue(s32 id)
{
	SetModeAndPlayCue(id, 5, 0x7d);
}

void EffectRuntime_SetMode7AndLaunch(s32 id)
{
	s32						  slot;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		WaitFrames(0x12);
		if (object != NULL)
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 7);
		AudioCommand_PlayFar(0x92);
		if (object != NULL)
		{
			object->velocity_y = 0x80000;
			ObjectDispatch_SetSingleChildFlagsFar(object, 1);
		}
	}
}

void EffectRuntime_SetMode4AndPlayCue(s32 id)
{
	SetModeAndPlayCue(id, 4, 0x7c);
}

void EffectRuntime_SetMode2(s32 id)
{
	s32						  slot;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		if (object != NULL)
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
	}
}

struct ObjectRuntime *EffectRuntime_GetCurrentObject(s32 id)
{
	s32						  slot = EffectRuntime_FindSlotByObject(id);
	struct EffectRuntimeSlot *slot_data;

	if (slot == -1)
		return NULL;
	slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
	return slot_data->object;
}

void EffectRuntime_ClearCurrentFlags(s32 id)
{
	s32						  slot;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		if (object != NULL)
			object->animation_kind = 0;
	}
}

void EffectRuntime_SetCurrentPosition(s32 id, s32 x, s32 y)
{
	s32						  slot;
	s32						  height;
	s32						  object_x;
	s32						  object_y;
	struct EffectRuntimeSlot *slot_data;
	struct ObjectRuntime	 *object;

	object_y = y;
	object_x = x;
	slot = EffectRuntime_FindSlotByObject(id);
	if (slot != -1)
	{
		slot_data = &((struct EffectRuntime *)gEventWork)->slots[slot];
		object = slot_data->object;
		if (object != NULL)
		{
			if (object_x == -1)
				object_x = (slot_data->tileX << 20) + 0x80000;
			if (object_y == -1)
				object_y = (slot_data->tileY << 20) + 0x80000;
			object->x = object_x;
			object->z = object_y;
			height = Map_GetTerrainHeightFar(0, object_x, object_y);
			object->terrain_height = height;
			object->y = height;
		}
	}
}

//--------------------------------------------------------------------------
// emitter.c
// フィールド効果：手に入れたアイテムの表示ときらめき、近くの印、ウィンドウの HBlank DMA
//--------------------------------------------------------------------------

LAYOUT_OFFSET_GUARD(ParticleEmitter_travel_offset_offset, struct ObjectRuntime, velocity_y, 0x28);
LAYOUT_OFFSET_GUARD(ParticleEmitter_active_offset, struct ObjectRuntime, flags, 0x55);
LAYOUT_OFFSET_GUARD(ParticleEffectObject_child_offset, struct ObjectRuntime, animation, 0x50);
