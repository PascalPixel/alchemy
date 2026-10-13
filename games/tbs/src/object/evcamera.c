#include "vram_tab.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/evtable.h"
#include "unit/object/marker.h"
#include "unit/object/sparkle.h"
#include "unit/object/disptran.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/math.h"
#include "unit/runtime/heap.h"
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

void Object_AttachWorkTargetToObject(s32 id, s32 flag)
{
	struct ObjectRuntime *obj;
	struct ObjectRuntime *target;
	struct EventWork	 *work;
	struct MapScrollWork *p;

	obj = ObjectTable_Get(id);
	work = Runtime_AllocateBlock(0x1B, 0xCCC);
	target = (struct ObjectRuntime *)work->view_center;
	p = gMapWork[0];
	if (obj != 0)
	{
		p->origin = &target->x;
		ObjectDispatch_InitFromTable4WithArgumentFar(target, obj);
		if (flag == 0)
		{
			target->x = obj->x;
			target->y = obj->y;
			target->z = obj->z;
			WaitFrames(1);
			if (((struct EventRuntime *)work)->mode_19e != 3)
			{
				Map_ApplyWorkOriginAndSpanFar();
			}
		}
	}
}

void ObjectTable_AllocateAndSetObjectSpeed(s32 first, s32 second)
{
	struct EventWork *owner = Runtime_AllocateBlock(0x1b, 0xccc);
	((struct ObjectRuntime *)owner->view_center)->speed_limit = first;
	((struct ObjectRuntime *)owner->view_center)->acceleration = second;
}

void Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter)
{
	s32					  should_use_setter;
	struct EventWork	 *runtime_block;
	s32					  minimum_x;
	s32					  minimum_z;
	s32					  maximum_x;
	s32					  object_z_offset;
	s32					  maximum_z;
	s32					  position_x;
	s32					  position_z;
	s32					  position_y;
	struct MapScrollWork *camera_state;
	struct ObjectRuntime *object;

	position_x = requested_x;
	position_y = requested_y;
	should_use_setter = use_setter;
	position_z = requested_z;
	runtime_block = Runtime_AllocateBlock(0x1B, 0xCCC);
	object = (struct ObjectRuntime *)runtime_block->view_center;
	camera_state = gMapWork[0];
	minimum_x = camera_state->min_x + 0x780000;
	object_z_offset = object->y;
	minimum_z = camera_state->min_y + object_z_offset + 0x600000;
	maximum_x = camera_state->max_x + 0xFF880000;
	maximum_z = camera_state->max_y + object_z_offset + 0xFFC00000;
	camera_state->origin = &object->x;
	ObjectDispatch_InitFromTable6Far(object);
	if (position_x == -1)
	{
		position_x = object->x;
	}
	if (position_y == -1)
	{
		position_y = object->y;
	}
	if (position_z == -1)
	{
		position_z = object->z;
	}
	if (position_x < minimum_x)
	{
		position_x = minimum_x;
	}
	if (position_z < minimum_z)
	{
		position_z = minimum_z;
	}
	if (position_x > maximum_x)
	{
		position_x = maximum_x;
	}
	if (position_z > maximum_z)
	{
		position_z = maximum_z;
	}
	if (should_use_setter == 0)
	{
		object->x = position_x;
		object->y = position_y;
		object->z = position_z;
		WaitFrames(1U);
		if (((struct EventRuntime *)runtime_block)->mode_19e != 3)
		{
			Map_ApplyWorkOriginAndSpanFar();
		}
	}
	else
	{
		Object_SetMoveTargetFar(object, position_x, position_y, position_z);
	}
}

void Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1)
{
	struct ObjectRuntime *obj;

	obj = ObjectTable_Get(arg0);
	Runtime_AllocateBlock(0x1B, 0xCCC);
	if (obj != NULL)
	{
		Motion_CamBounds(obj->x, -1, obj->z, arg1);
	}
}

void BattleFx_CommitObjectPositionAndWait(void)
{
	Script_WaitForEventTimeoutFar(
		((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center);
	Battle_WaitMode0(2);
}

struct FieldActor *Battle_GetWorkObject1e0(void)
{
	return (s32)((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center;
}

void BattleFx_LinkObjectToTarget(struct ObjectRuntime *target, s32 keep_current_position)
{
	struct ObjectRuntime *object;

	object = (struct ObjectRuntime *)((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))
				 ->view_center;
	if (target != NULL)
	{
		ObjectDispatch_InitFromTable4WithArgumentFar(object, NULL);
		object->linked_object = target;
		if (keep_current_position == 0)
		{
			object->x = target->x;
			object->y = target->y;
			object->z = target->z;
		}
	}
}

//--------------------------------------------------------------------------
// mapfx.c
// マップの表示範囲とズームの移り変わり、ほかのオブジェクトについて回るオブジェクト
//--------------------------------------------------------------------------

// スプライトの OAM 属性2 の上位バイト (bit 2〜3 が表示優先度)
#define ATTR2_HI(spr) (((u8 *)&((struct AnimationObject *)(spr))->part[0])[9])

//--------------------------------------------------------------------------
// マップの表示範囲を決める
//--------------------------------------------------------------------------
void Map_SetViewBounds(s32 min_x, s32 min_y, s32 max_x, s32 max_y)
{
	struct MapScrollWork *work = gMapWork[0];

	work->min_x = min_x;
	work->min_y = min_y;
	work->max_x = max_x;
	work->max_y = max_y;
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : ズームを前の値から目標へ 1 段すすめる
// 画面中心のオブジェクトが止まっているあいだは待つ
//--------------------------------------------------------------------------
s32 BattleFx_StepRatioTransition(void)
{
	struct PerspectiveWork *work;
	s16					   *frames;
	s32					   *from;
	s16					   *step;
	s32						zoom;
	s32						delta;

	work = gMapWork[0];
	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->view_center->paused
		!= 0)
		return;
	frames = (s16 *)&work->zoom_transition_frames;
	if (*frames == 0)
		return;
	from = &work->previous_zoom;
	delta = work->zoom - *from;
	step = &work->zoom_transition_step;
	(*step)++;
	/* FAKEMATCH: an explicit libgcc call, an ordinary call where the operator makes a libcall block. Plain form measured 2026-10-13: *from + delta * *step / *frames differ 93 (84 instructions instead of 87). */
	zoom = *from + __divsi3(delta * *step, *frames);
	work->distance = Iwram_MulQ16(work->far_plane, zoom);
	gScanlinePitch = work->pitch + 1;
	if (*step == *frames)
	{
		*frames = 0;
		Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StepRatioTransition);
	}
}

//--------------------------------------------------------------------------
// ズームの移り変わりを始める (イベントの mode が 3 のときだけ)
// zoom   : 倍率 (16.16 固定小数)
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void BattleFx_ScheduleRatioTransition(s32 zoom, s32 frames)
{
	struct PerspectiveWork *work = gMapWork[0];
	s32						inv;

	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->mode != 3)
		return;
	{
		inv = Iwram_RatioMulQ14(zoom, 0x10000); // 1 / zoom
	}
	work->previous_zoom = work->zoom;
	work->zoom = inv;
	work->zoom_transition_frames = frames;
	work->zoom_transition_step = 0;
	Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_StepRatioTransition, 0xc94);
}

//--------------------------------------------------------------------------
// ズームの移り変わりが終わるまで待つ (イベントの mode が 3 のとき、最長 300 フレーム)
//--------------------------------------------------------------------------
void Event_WaitForZoomTransition(void)
{
	s32						frames;
	struct PerspectiveWork *work = gMapWork[0];

	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->mode == 3)
	{
		frames = 0;
		if (*(s16 *)&work->zoom_transition_frames != 0)
		{
			do
			{
				WaitFrames(1);
				frames++;
			} while (frames < 300 && *(s16 *)&work->zoom_transition_frames != 0);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 付いていく相手の位置に置き、y には絵の box_x と 8 ドットを足す
//--------------------------------------------------------------------------
s32 BattleFx_CopyLinkedObjectPosition(void *self)
{
	struct FieldActor *obj = self;
	struct FieldActor *link;

	link = obj->linked_object;
	if (link != NULL)
	{
		obj->motion_flags = 0;
		obj->x.fixed = link->x.fixed;
		obj->y.fixed = link->y.fixed
					   + ((s8)Resource_GetMetadataRecordFar(
							  (s16)BattleAction_FindDescriptor(*(s16 *)&obj->work)->sprite)
							   ->box_x
						   << 16)
					   + (8 << 16);
		obj->terrain_height = link->terrain_height;
		obj->z.fixed = link->z.fixed;
	}
	return 0;
}

//--------------------------------------------------------------------------
// id のオブジェクトについて回るオブジェクト (種類 21) を出し、wait フレーム待つ
// mode : 下位 4 ビットはオブジェクトの mode、下位 8 ビットが 6 なら音 110、
//        0x100 で表示優先度 1 (なければ相手と同じ優先度)
//--------------------------------------------------------------------------
void BattleFx_SpawnLinked(s32 id, s32 mode, s32 wait)
{
	struct FieldActor *target;

	if ((mode & 0xff) == 6)
	{
		AudioCommand_PlayFar(110);
	}

	target = ObjectTable_Get(id);
	if (target != 0)
	{
		struct FieldActor *obj =
			Object_CreateFar(21, target->x.fixed, target->y.fixed, target->z.fixed);

		if (obj != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_LinkedObjectScript);
			ObjectDispatch_ApplyArgumentToChildrenFar(obj, mode & 15);
			obj->motion_flags = 0;
			obj->timer = 0;
			obj->work = id;
			obj->update = (void (*)(union FieldObject *))BattleFx_CopyLinkedObjectPosition;
			obj->sprite->flags = 0;
			obj->linked_object = target;

			if ((mode & 0x100) != 0)
			{
				s32 mask = 13;
				u8	attr = ATTR2_HI(obj->sprite);

				mask = -mask;
				mask &= attr;
				mask |= 4;
				ATTR2_HI(obj->sprite) = mask;
			}
			else
			{
				s32 pri = 12;
				u8	src = ATTR2_HI(target->sprite);
				u8	dst;
				s32 mask = 13;

				pri &= src;
				dst = ATTR2_HI(obj->sprite);
				mask = -mask;
				mask &= dst;
				mask |= pri;
				ATTR2_HI(obj->sprite) = mask;
			}
		}
		Battle_WaitMode0(wait);
	}
}

void BattleFx_ConfigureLinkedObject(s32 id, s32 flags)
{
	struct FieldActor  *object = ObjectTable_Get(id);
	struct FieldActor  *child;
	struct FieldSprite *visual;
	s32					mode;

	child = 0;
	visual = 0;

	if (object == 0)
		return;

	if ((flags & 3) != 0)
	{
		if ((flags & 3) == 2 || object->linked_object == 0)
		{
			child = Object_CreateFar(209, object->x.fixed, object->y.fixed, object->z.fixed);
		}
	}
	else
	{
		child = object->linked_object;
		if (child == 0)
			return;
		ObjectDispatch_ReleaseFar(child);
		object->linked_object = 0;
		return;
	}

	if (child == 0)
		return;

	mode = flags & 3;
	switch (mode)
	{
	case 1:
		ObjectDispatch_ApplyArgumentToChildrenFar(child, 1);
		object->linked_object = child;
		child->timer = 1;
		break;
	case 2:
		ObjectDispatch_ApplyArgumentToChildrenFar(child, 2);
		ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)ObjectMotion_WaitScript);
		child->timer = 1;
		break;
	}

	child->work = id;
	child->motion_flags = 0;
	child->update = BattleFx_CopyLinkedObjectPosition;
	visual = child->sprite;
	visual->flags = 0;
	child->linked_object = object;

	if (flags & 0x100)
	{
		s32 mask = 13;
		u8	visual_flags = ((u8 *)visual)[9];

		mask = -mask;
		mask &= visual_flags;
		mask |= 4;
		((u8 *)visual)[9] = mask;
	}
	else
	{
		s32 copied_flags = 12;
		u8	source_flags = ((u8 *)object->sprite)[9];
		u8	destination_flags;
		s32 clear_mask = 13;

		copied_flags &= source_flags;
		destination_flags = ((u8 *)visual)[9];
		clear_mask = -clear_mask;
		clear_mask &= destination_flags;
		clear_mask |= copied_flags;
		((u8 *)visual)[9] = clear_mask;
	}
}

s32 Object_ClearChildAndCollisionFlags(void *obj)
{
	ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
	((struct ScriptObjectRuntime *)obj)->flags_59 = 0;
	return 0;
}

s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object)
{
	s32					  step;
	struct ObjectRuntime *target;
	s32					  deltaX;
	s32					  deltaY;
	s32					  cellX;
	s32					  cellY;
	s32					  newX;
	s32					  distance;

	target = object->linked_object;
	if (target != 0)
	{
		deltaX = target->x - object->x;
		if (deltaX < 0)
			deltaX += 0xffff;
		cellX = deltaX >> 16;
		deltaY = target->z - object->z;
		if (deltaY < 0)
			deltaY += 0xffff;
		cellY = deltaY >> 16;
		distance = Iwram_Sqrt(cellX * cellX + cellY * cellY);
		step = object->action;
		if (distance >= step)
		{
			newX = object->x + (cellX << 20) / step;
			/* FAKEMATCH: an explicit libgcc call, an ordinary call where the operator makes a libcall block. Plain form measured 2026-10-13: (cellY << 20) / step, as newX divides, differ 54 (70 instructions instead of 66). */
			Object_SetMoveTargetFar(
				object, newX, object->y, object->z + __divsi3(cellY << 20, step));
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		}
		else
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
		}
	}
	return 1;
}

s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object)
{
	s16					  turn_step;
	u16					  current_angle;
	s32					  target_angle;
	struct ObjectRuntime *target;

	target = object->linked_object;
	if (target != NULL)
	{
		object->action_flags &= 0xfe;
		target_angle = (u16)ArcTan2(target->z - object->z, target->x - object->x);
		current_angle = object->angle;
		turn_step = target_angle - current_angle;
		if (turn_step != 0)
		{
			if (turn_step > 0x1000)
				turn_step = 0x1000;
			if (turn_step < -0x1000)
				turn_step = -0x1000;
			object->angle = (current_angle + turn_step);
		}
	}
	return 1;
}

void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 kind)
{
	switch ((kind - 1))
	{
	case 0:
		kind = (s32)ObjectMotion_ActionKind1Script;
		break;
	case 1:
		kind = (s32)ObjectMotion_ActionKind2Script;
		break;
	case 2:
		kind = (s32)ObjectMotion_ActionKind3Script;
		break;
	case 3:
		kind = (s32)ObjectMotion_ActionKind4Script;
		break;
	case 4:
		kind = (s32)ObjectMotion_MoveTowardTargetScript;
		break;
	case 5:
		object->linked_object = Object_GetById(gGameState.selected_actor);
		kind = (s32)ObjectMotion_TurnTowardLinkedScript;
		break;
	case 6:
		kind = (s32)ObjectMotion_ResetActionScript;
		break;
	default:
		break;
	}
	ObjectDispatch_InitializeFar((struct DispatchObject *)object, kind);
}

struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self, s32 id)
{
	struct ObjectRuntime *entry;
	struct ObjectRuntime *found;
	s32					  cnt;
	s32					  best;
	s32					  dy;
	s32					  dx;
	s32					  dz;
	s32					  dist;
	s32					  angle;
	s32					  turn;

	found = NULL;
	best = 40;
	entry = gObjectSlots;
	for (cnt = 0; cnt < 64; cnt++, entry++)
	{
		if (entry->script == NULL)
			continue;
		if (entry == self)
			continue;
		if (entry->animation_kind != 1)
			continue;
		dy = entry->y - self->y;
		if (dy >= 0)
		{
			if (dy > 0x2fffff)
				continue;
		}
		else
		{
			if (self->y - entry->y > 0x2fffff)
				continue;
		}
		dx = (entry->x - self->x) / 0x10000;
		dz = (entry->z - self->z) / 0x10000;
		dist = Iwram_Sqrt(dx * dx + dz * dz);
		if (dist >= best)
			continue;
		angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
		if (dist > 23)
		{
			turn = (s16)(angle - self->angle);
			if (turn < -0x2fff)
				continue;
			if (turn > 0x2fff)
				continue;
		}
		found = entry;
		best = dist;
	}
	if (found == NULL)
		return NULL;
	if (((struct AnimationObject *)found->animation)->entries[0]->anim_id != id)
		return NULL;
	return found;
}

#define TILE_CELLS ((struct MapCell *)Ram_MapCellBuffer)
#define TILE_CELLS_TARGET ((struct MapCell *)(Ram_MapCellBuffer + 0x200))
#define TILE_CELLS_ABOVE ((struct MapCell *)(Ram_MapCellBuffer - 0x200))

s32 Field_TryJumpForward(void)
{
	struct ObjectRuntime *leader = Object_GetById(gGameState.selected_actor);
	s32					  result = -1;
	s32					  angle = (leader->angle + 0x2000) & 0xc000;
	u8					 *flags = &leader->flags;
	u8					  saved = *flags;
	struct FieldStepWork *work = gEventWork;
	s32					  child_mode = 1;
	struct FieldPosition  position;
	struct FieldPosition *pos;
	struct ObjectRuntime *target;

	pos = &position;
again:
	pos->x = (leader->x & 0xfff00000) + 0x80000;
	pos->y = leader->y;
	pos->z = (leader->z & 0xfff00000) + 0x80000;
	Vector_AddPolarOffset(0x100000, angle, pos);
	if (Map_CheckObjectStepFar(leader, pos) == 1)
		return -1;
	pos->x = (leader->x & 0xfff00000) + 0x80000;
	pos->y = leader->y;
	pos->z = (leader->z & 0xfff00000) + 0x80000;
	Vector_AddPolarOffset(0x200000, angle, pos);
	if (Map_CheckObjectStepFar(leader, pos) == 0)
	{
		if (leader->animation_kind == 1)
			child_mode = ((struct FieldSprite *)leader->animation)->flags;
		Battle_Reset();
		ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
		WaitFrames(6);
		AudioCommand_PlayFar(152);
		ObjectDispatch_ApplyArgumentToChildrenFar(leader, 7);
		leader->speed_limit = 0x30000;
		leader->acceleration = 0x20000;
		leader->velocity_y = 0x40000;
		*flags &= 0x7e;
		ObjectDispatch_SetSingleChildFlagsFar(leader, child_mode & 0xfe);
		ObjectMotion_SetPositionAndCommit(gGameState.selected_actor,
			((union FieldCoordinate *)&pos->x)->part.pixel,
			((union FieldCoordinate *)&pos->z)->part.pixel);
		ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
		ObjectDispatch_SetSingleChildFlagsFar(leader, child_mode);
		if ((target = Object_FindNearestFacingTarget(leader, 207)) != NULL
			|| (target = Object_FindNearestFacingTarget(leader, 205)) != NULL)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(target, 7);
			leader->y -= 0x10000;
			leader->terrain_height -= 0x10000;
			WaitFrames(2);
			leader->y -= 0x10000;
			leader->terrain_height -= 0x10000;
			WaitFrames(10);
			leader->y += 0x10000;
			leader->terrain_height += 0x10000;
			WaitFrames(4);
			leader->y += 0x10000;
			leader->terrain_height += 0x10000;
		}
		else
		{
			WaitFrames(6);
		}
		*flags = saved;
		BattleFx_FinishAction();
		if (work != NULL)
			work->encounter_steps += Iwram_MulQ16(work->encounter_rate, 0x200000);
		if (GetMapCellCollisionFar(leader->terrain_id, pos->x, pos->z) == 0xf9)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(leader, 1);
			WaitFrames(6);
			goto again;
		}
		result = 0;
	}
	return result;
}

s32 FieldEffect_UpdateGridPlacement(void)
{
	struct ObjectRuntime *object = Object_GetById(gGameState.selected_actor);
	s32 grid_x = 8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
	s32 grid_z = 8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
	s32 result;

	Battle_Reset();

	if (gGameState.movement_mode == 0)
	{
		s32 index = grid_x / 16 + (grid_z / 16) * 128;

		if (TILE_CELLS[index].collision_code == TILE_CELLS_TARGET[index].collision_code)
		{
			/* FAKEMATCH: needed to match: the six-word probe preserves the 24-byte frame; a three-word position halves it. */
			s32					  vector[6];
			struct FieldPosition *position = (struct FieldPosition *)vector;

			position->x = object->x;
			position->y = object->y - 0x100000;
			position->z = object->z;
			result = CheckMapPositionCellOccupiedFar((const s32 *)position);
			if (result != 0)
				goto failure;

			ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, grid_x, grid_z);
			object->speed_limit = 0x10000;
			ObjectMotion_ArmCallback(gGameState.selected_actor, 0xc000, 0);
			Object_RefreshSelectorById(gGameState.selected_actor);
			object->action_flags = 1;
			object->flags = 0;
			ObjectDispatch_SetSingleChildFlagsFar(object, 0);
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 13);
			Object_SetMoveTargetFar(
				object, grid_x << 16, object->y - 0x100000, (grid_z << 16) + 0x100000);
			ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selected_actor);
			gGameState.movement_mode = 1;
		}
		else
		{
			goto failure;
		}
	}
	else
	{
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 10);
		object->flags = 3;
		object->velocity_y = 0x40000;
		object->terrain_height = object->y;
		ObjectDispatch_SetSingleChildFlagsFar(object, 1);
		Battle_WaitMode0(6);
		gGameState.movement_mode = 0;
		object->action_flags = 1;
		object->angle = 0xc000;
	}

	BattleFx_FinishAction();
	return 0;

failure:
	BattleFx_FinishAction();
	return -1;
}

s32 battle_owner_69(void)
{
	struct ObjectRuntime *object = Object_GetById(gGameState.selected_actor);
	s16					  child_mode = 1;
	s32 grid_x = 8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
	s32 grid_z = 8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
	s32 result;

	Battle_Reset();

	if (object->animation_kind == 1)
		child_mode = ((struct FieldSprite *)object->animation)->flags;

	if (gGameState.movement_mode == 0)
	{
		s32 index = grid_x / 16 + (grid_z / 16) * 128;

		if (TILE_CELLS[index].collision_code == TILE_CELLS_ABOVE[index].collision_code)
		{
			/* FAKEMATCH: needed to match: the six-word probe preserves the 24-byte frame; a three-word position halves it. */
			s32					  vector[6];
			struct FieldPosition *position = (struct FieldPosition *)vector;

			position->x = object->x;
			position->y = object->y;
			position->z = object->z;
			result = CheckMapPositionCellOccupiedFar((const s32 *)position);
			if (result != 0)
				goto failure;

			object->action_flags = 0;
			ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, grid_x, grid_z);
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
			WaitFrames(4);
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 7);
			object->velocity_y = 0x40000;
			WaitFrames(4);
			object->flags = 0;
			child_mode &= 0xfe;
			ObjectDispatch_SetSingleChildFlagsFar(object, child_mode);
			object->speed_limit = 0x10000;
			object->velocity_y = 0;
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 12);
			WaitFrames(4);
			gGameState.movement_mode = 1;
			object->action_flags = 1;
			WaitFrames(8);
		}
		else
		{
			goto failure;
		}
	}
	else
	{
		object->flags = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 11);
		Object_SetMoveTargetFar(
			object, grid_x << 16, object->y + 0x80000, (grid_z << 16) - 0x100000);
		ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selected_actor);
		object->flags = 3;
		child_mode |= 1;
		object->terrain_height = object->y;
		ObjectDispatch_SetSingleChildFlagsFar(object, child_mode);
		Battle_WaitMode0(4);
		gGameState.movement_mode = 0;
		object->action_flags = 1;
	}

	BattleFx_FinishAction();
	return 0;

failure:
	BattleFx_FinishAction();
	return -1;
}

s32 Object_GetScreenPosition(s32 object_id, s32 *position)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);
	s32					 *camera;
	s32					  camera_x;
	s32					  camera_z;
	s32					  x;
	s32					  z;

	if (object == 0)
		return -1;
	/* FAKEMATCH: needed to match: the adjacent coordinate-word read avoids four bytes added by separate named bases. */
	camera = &((struct MapScrollWork *)gMapWork[0])->view_x;
	camera_x = camera[0] & 0xffff0000;
	camera_z = camera[1] & 0xffff0000;
	x = object->x - camera_x;
	z = object->z - camera_z - object->y;
	*position++ = x / 0x10000;
	*position = z / 0x10000;
	if ((object->animation_kind & 15) == 1)
		*position -= (s8)Resource_GetMetadataRecordFar(
			((struct AnimationObject *)object->animation)->entries[0]->anim_id)
						 ->box_x;
	return 0;
}

void ObjectEffect_ReservedNoOp941DC(void)
{
}

s32 Scene_FadeColorFromWhite(s32 unused)
{
	struct EventRuntime *work = gEventWork;
	s32					 i;
	s32					 c;

	AudioCommand_PlayFar(gGameState.scene_cue);
	AudioCommand_PlayFar(288);
	AudioCommand_PlayFar(147);
	if (work->mode_19e == 3)
	{
		Io_Write16(0x7fff, (u16 *)0x050001e6);
		DisplayTransition_Finish(0x401, 16);
		work->status_1c6 = 0;
		WaitFrames(16);
		for (i = 0; i < 16; i++)
		{
			c = 30 - i * 2;
			Io_Write16((c << 10) | (c << 5) | c, (u16 *)0x050001e6);
			WaitFrames(1);
		}
	}
	else
	{
		Io_Write16(0x7fff, &PLTT_BACKDROP);
		DisplayTransition_Finish(0x207, 16);
		work->status_1c6 = 0;
		WaitFrames(16);
		for (i = 0; i < 16; i++)
		{
			c = 30 - i * 2;
			Io_Write16((c << 10) | (c << 5) | c, &PLTT_BACKDROP);
			WaitFrames(1);
		}
	}
}

void ObjectEffect_PrepareContextEffect(s32 value)
{
	u32						zero;
	u8						kind;
	struct ObjectRuntime   *object;
	struct AnimationObject *context;
	struct AnimationEntry  *effect;

	object = ObjectTable_Get(gGameState.selected_actor);
	context = object->animation;
	effect = (struct AnimationEntry *)ResourceMetadata_RegisterFar(context, 27);
	zero = 0;
	kind = 15;

	context->flags = zero;
	effect->param = kind;
	object->x = (object->x & 0xFFF00000) + 0x80000;
	object->z = (object->z & 0xFFF00000) + 0x100000;
	object->velocity_x = zero;
	object->velocity_z = zero;
	object->target_x = ACTOR_NO_TARGET;
	object->target_z = ACTOR_NO_TARGET;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, value);
	WaitFrames(18);
}

void ObjectEffect_BeginContextEffect26(void)
{
	ObjectEffect_PrepareContextEffect(0x1A);
	GameFlag_SetBitFar(0x120);
}

void ObjectEffect_BeginContextEffect25(void)
{
	ObjectEffect_PrepareContextEffect(0x19);
	GameFlag_SetBitFar(0x121);
}

void ObjectEffect_EndContextEffect(s32 arg0)
{
	s32						zero;
	s32						mask;
	struct ObjectRuntime   *obj = ObjectTable_Get(gGameState.selected_actor);
	struct AnimationObject *ctx = obj->animation;
	struct AnimationEntry  *eff = (struct AnimationEntry *)ResourceMetadata_RegisterFar(ctx, 27);

	zero = 0;
	mask = 0xfff00000;
	ctx->flags = zero;
	eff->param = 15;
	obj->x = (obj->x & mask) + 0x80000;
	obj->z &= mask;
	ObjectDispatch_ApplyArgumentToChildrenFar(obj, arg0);
	WaitFrames(30);
	ctx->count = 1;
	ResourceMetadata_ClearRecordFar(ctx->entries[1]);
	ctx->entries[1] = (void *)zero;
	ctx->flags = 1;
	obj->acceleration = 0x10000;
	obj->speed_limit = 0x10000;
	Object_SetMoveTargetFar(obj, obj->x, obj->y, obj->z + 0x80000);
	Script_WaitForEventTimeoutFar(obj);
}

s32 ObjectEffect_RunPendingFlagEvent(void)
{
	s32 result = 0;
	s32 flag = 0x120;

	if (GameFlag_TestFar(flag) != 0)
	{
		ObjectEffect_EndContextEffect(24);
		GameFlag_ClearBitFar(flag);
		result = 1;
	}
	else
	{
		flag = 0x121;
		if (GameFlag_TestFar(flag) != 0)
		{
			ObjectEffect_EndContextEffect(23);
			GameFlag_ClearBitFar(flag);
			result = 2;
		}
		else
		{
			flag = 0x122;
			if (GameFlag_TestFar(flag) != 0)
			{
				s32					  id;
				struct ObjectRuntime *obj;

				GameFlag_ClearBitFar(flag);
				id = gGameState.selected_actor;
				obj = ObjectTable_Get(id);
				obj->y += 0x00a00000;
				Motion_CamBounds(-1, -1, -1, 0);
				while (obj->y + obj->velocity_y > obj->terrain_height)
				{
					WaitFrames(1);
				}
				AudioCommand_PlayFar(159);
				obj->y = obj->terrain_height;
				ObjectDispatch_ApplyArgumentToChildrenFar(obj, 22);
				Battle_WaitMode0(15);
				Object_AttachWorkTargetToObject(id, 1);
				result = 3;
			}
		}
	}
	return result;
}
