#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
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

struct FieldActor *Object_GetById(s32 object_id)
{
	struct FieldActor *object = ObjectTable_Get(object_id);

	if (object == NULL)
		return NULL;
	return object;
}

void ObjectMotion_SetSpeedParameters(u32 object_id, s32 speed_limit, s32 acceleration)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->acceleration = acceleration;
		object->speed_limit = speed_limit;
	}
}

void ObjectMotion_EnableActionAndSetCallback(u32 object_id, s32 action)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		s32 value = 1;
		value |= object->action_flags;
		object->action_flags = value;
		ObjectMotion_SetActionCallback(object, action);
	}
}

void ObjectMotion_EnableActionAndResetMotion(u32 object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);
	u8					  enabled;

	if (object != NULL)
	{
		enabled = 1;
		object->action_flags = enabled | object->action_flags;
		ObjectDispatch_InitFromTable6Far(object);
	}
}

void Object_LinkObjectAndSetCallback(u32 object_id, u32 linked_object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->linked_object = (struct ObjectRuntime *)Object_GetById(linked_object_id);
		ObjectMotion_SetActionCallback(object, (s32)ObjectMotion_LinkedActionScript);
	}
}

void Object_RefreshSelectorById(u32 object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
		ObjectDispatch_WaitForValue16Far(object);
}

void Object_SetActionCallbackAndRefreshById(u32 object_id, s32 action)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		s32 value = 1;
		value |= object->action_flags;
		object->action_flags = value;
		ObjectMotion_SetActionCallback(object, action);
		ObjectDispatch_WaitForValue16Far(object);
	}
}

void ObjectMotion_ResetAndSetPosition(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
	}
}

void ObjectMotion_SetPositionAndCommit(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
		Script_WaitForEventTimeoutFar(object);
	}
}

void ObjectMotion_ResetAndSetPositionInMode2(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
	}
}

void ObjectMotion_SetPositionAndReset(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		Object_SetMoveTargetFar(object, x << 16, object->y, z << 16);
		Script_WaitForEventTimeoutFar(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	}
}

void ObjectMotion_CenterOnColumn(u32 object_id, s32 action, s32 z_offset)
{
	struct ObjectRuntime *object;
	s16					  current_angle;
	s32					  snapped_angle;
	s16					  angle_remainder;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		current_angle = ((union FieldCoordinate *)&object->x)->part.pixel;
		snapped_angle = current_angle;
		if (current_angle < 0)
		{
			snapped_angle += 15;
		}
		snapped_angle >>= 4;
		snapped_angle *= 16;
		angle_remainder = current_angle - snapped_angle;
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		Object_SetMoveTargetFar(
			object, object->x + ((8 - angle_remainder) << 16), object->y, object->z);
		Script_WaitForEventTimeoutFar(object);
		ObjectMotion_SetActionVariant(object_id, action);
		Object_SetMoveTargetFar(object, object->x, object->y, object->z + (z_offset << 16));
	}
}

void ObjectMotion_OffsetPositionAndResetMotion(u32 object_id, s32 x_offset, s32 z_offset)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		Object_SetMoveTargetFar(
			object, object->x + (x_offset << 16), object->y, object->z + (z_offset << 16));
	}
}

void ObjectMotion_OffsetPositionAndReset(u32 object_id, s32 x_offset, s32 z_offset)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->movement_state = 0;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		Object_SetMoveTargetFar(
			object, object->x + (x_offset << 16), object->y, object->z + (z_offset << 16));
	}
}

void ObjectMotion_CommitPositionAndActivate(u32 object_id, s32 x_offset, s32 z_offset)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	ObjectMotion_OffsetPositionAndReset(object_id, x_offset, z_offset);
	if (object != NULL)
	{
		Script_WaitForEventTimeoutFar(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	}
}

void Motion_LaunchFromFocusedObject(u32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
	struct ObjectRuntime *object = ObjectTable_Get(arg0);

	if (object != NULL)
	{
		struct ObjectRuntime *other;

		ObjectMotion_SetSpeedParameters(arg0, 0x9999, 0x4CCC);
		other = (struct ObjectRuntime *)Object_GetById(gGameState.selected_actor);
		if (other != NULL)
			ObjectMotion_SetHorizontalPositionWithTerrain(arg0, other->x, other->z);
		object->movement_state = 0;
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
		ObjectMotion_OffsetPositionAndResetMotion(arg0, arg1, arg2);
		ObjectDispatch_InitializeFar(
			(struct DispatchObject *)object, (const s32 *)&ObjectMotion_LaunchScript);
		object->action = arg3;
	}
}

void ObjectMotion_CommitCurrentPositionAndActivate(u32 object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		Script_WaitForEventTimeoutFar(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	}
}

void ObjectMotion_SetHorizontalPositionWithTerrain(u32 object_id, s32 x, s32 z)
{
	struct ObjectRuntime *object;
	s32					  terrain_height;
	s32					  tile_x;
	s32					  tile_z;
	s32					  terrain_id;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		ObjectDispatch_InitFromTable6Far(object);
		object->velocity_x = 0;
		object->velocity_y = 0;
		object->velocity_z = 0;
		object->target_y = ACTOR_NO_TARGET;
		object->target_x = ACTOR_NO_TARGET;
		object->x = x;
		object->z = z;
		if (1 & object->flags)
		{
			terrain_id = object->terrain_id;
			tile_x = x;
			if (tile_x < 0)
			{
				tile_x += 0xFFFF;
			}
			tile_x >>= 0x10;
			tile_z = z;
			if (tile_z < 0)
			{
				tile_z += 0xFFFF;
			}
			tile_z >>= 0x10;
			terrain_height = Map_GetTerrainHeightFar(terrain_id, tile_x, tile_z) << 0x10;
			object->y = (object->y - object->terrain_height) + terrain_height;
			object->terrain_height = terrain_height;
		}
	}
}

void ObjectMotion_SetPositionWithTerrain(u32 object_id, s32 x, s32 y, s32 z)
{
	struct ObjectRuntime *object;
	s32					  terrain_height;
	s32					  tile_x;
	s32					  tile_z;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		ObjectDispatch_InitFromTable6Far(object);
		object->velocity_x = 0;
		object->velocity_y = 0;
		object->velocity_z = 0;
		object->target_y = ACTOR_NO_TARGET;
		object->target_x = ACTOR_NO_TARGET;
		object->x = x;
		object->y = y;
		object->z = z;
		if (1 & object->flags)
		{
			s32 terrain_id = object->terrain_id;
			tile_x = x;
			if (tile_x < 0)
			{
				tile_x += 0xFFFF;
			}
			tile_x >>= 0x10;
			tile_z = z;
			if (tile_z < 0)
			{
				tile_z += 0xFFFF;
			}
			terrain_height = Map_GetTerrainHeightFar(terrain_id, tile_x, tile_z >> 0x10) << 0x10;
			object->y = (object->y - object->terrain_height) + terrain_height;
			object->terrain_height = terrain_height;
		}
	}
}

void Object_SetModeById(u32 object_id, s32 action)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
		ObjectDispatch_ApplyArgumentToChildrenFar(object, action);
}

void Object_SetActionById(u32 object_id, s32 action)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
		ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, action);
}

void ObjectMotion_WaitForAnimationChange(u32 object_id)
{
	struct ObjectRuntime   *object;
	struct AnimationObject *sprite;
	volatile s32			saved;
	s32						i;

	object = ObjectTable_Get(object_id);
	if (object != NULL && object->animation_kind == 1)
	{
		sprite = object->animation;
		saved = sprite->last_no;
		for (i = 0; i <= 89; i++)
		{
			WaitFrames(1);
			if (saved != sprite->last_no)
			{
				break;
			}
		}
	}
}

void Motion_SetModeAndWaitAnimation(u32 object_id, s32 action)
{
	Object_SetModeById(object_id, action);
	ObjectMotion_WaitForAnimationChange(object_id);
}

void ObjectMotion_NoOp(void)
{
}

void ObjectMotion_Launch(u32 object_id, s32 speed, s32 event_id)
{
	struct ObjectRuntime *object;
	u8					 *object_flags;
	s32					  updated_flags;
	s32					  vertical_velocity;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		object_flags = &object->flags;
		updated_flags = *object_flags | 2;
		vertical_velocity = speed << 16;
		*object_flags = updated_flags;
		object->velocity_y = vertical_velocity;
		if (speed > 5)
		{
			AudioCommand_PlayFar(0x99);
		}
		else
		{
			AudioCommand_PlayFar(0x98);
		}
		Battle_WaitMode0(event_id);
	}
}

void ObjectMotion_SetVariantCallback(u32 object_id, s32 variant)
{
	struct ObjectRuntime *object;

	object = ObjectTable_Get(object_id);
	if (object != NULL && variant > 0)
	{
		if (variant > 3)
		{
			variant = 3;
		}
		ObjectDispatch_InitializeFar((struct DispatchObject *)object,
			(const s32 *)(ObjectMotion_VariantScripts + ((3 - variant) << 7)));
	}
}

void Motion_SetVarCbAndRefresh(u32 object_id, s32 variant)
{
	ObjectMotion_SetVariantCallback(object_id, variant);
	Object_RefreshSelectorById(object_id);
}

void BattleFx_UpdateParticleLinearMotion(struct FieldActor *particle)
{
	s32 velocity_x;
	s32 x;
	s32 velocity_z;
	s32 z;
	s32 velocity_y;
	s32 vz;
	velocity_x = particle->speed;
	x = particle->x.fixed + velocity_x;
	particle->x.fixed = x;
	particle->target_x = x;
	velocity_z = particle->acceleration;
	z = particle->z.fixed + velocity_z;
	particle->z.fixed = z;
	particle->target_z = z;
	velocity_y = particle->y.fixed + 0x400;
	particle->y.fixed = velocity_y;
	particle->target_y = velocity_y;
	particle->speed = (velocity_x - velocity_x / 0x12);

	vz = velocity_z;
	if (velocity_z < 0)
	{
		vz += 0xF;
	}
	particle->acceleration = (velocity_z - (vz >> 4));
}

void BattleFx_SpawnBurstParticle(struct FieldActor *source, s32 optional)
{
	struct FieldActor  *object;
	struct FieldSprite *child;
	s32					value;

	object = (struct FieldActor *)Object_CreateFar(
		222, source->x.fixed, source->y.fixed, source->z.fixed);
	if (object != 0)
	{
		child = object->sprite;
		switch (Random16() & 1)
		{
		case 1:
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)object, (const s32 *)BattleFx_BurstParticleScriptA);
			break;
		default:
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
			ObjectDispatch_InitializeFar(
				(struct DispatchObject *)object, (const s32 *)BattleFx_BurstParticleScriptB);
			break;
		}

		if (optional != 0)
			ObjectGroup_SetChildValue(object, optional);

		object->motion_flags = 0;
		value = Random16() % 10 + 5;
		object->acceleration = -0x1999 * value;
		value = Random16() % 15 - 7;
		value <<= 1;
		object->speed = 0x1999 * value;
		object->timer = 0;
		object->update = BattleFx_UpdateParticleLinearMotion;
		child->flags = 0;
		child->priority = source->sprite->priority;
	}
}

void BattleFx_RunRisingObjectSequence(s32 sequence_arg, s32 mode_or_frame, s32 optional_action)
{
	s32					  next_y;
	s32					  base_z;
	struct ObjectRuntime *object;
	u8					 *object_flags;

	object = (struct ObjectRuntime *)Object_GetById(sequence_arg);
	base_z = object->z;
	if (object != 0)
	{
		AudioCommand_PlayFar(0x121);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, mode_or_frame);
		WaitFrames(10);
		object_flags = &object->flags;
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
		{
			u8 flags = 2;
			flags |= *object_flags;
			*object_flags = flags;
		}
		object->velocity_y = 0x40000;
		Object_SetMoveTargetFar(object, object->x, object->y, base_z + 0xC0000);
		WaitFrames(6);
		AudioCommand_PlayFar(0xD9);
		mode_or_frame = 0;
		ObjectMotion_ArmCallback(sequence_arg, 0x5000, 0);
		*object_flags = 0;
		do
		{
			next_y = object->y + 0xFFFE0000;
			object->y = next_y;
			object->target_y = next_y;
			WaitFrames(1);
			if ((optional_action != -1) && (mode_or_frame & 1))
			{
				BattleFx_SpawnBurstParticle((struct FieldActor *)object, optional_action);
			}
			mode_or_frame++;
		} while ((u32)mode_or_frame <= 0xD);
		*object_flags = 3;
		object->velocity_y = 0x30000;
		Object_SetMoveTargetFar(object, object->x, object->y, base_z + 0x100000);
		Script_WaitForEventTimeoutFar(object);
		mode_or_frame = 0;
		if (object->y > object->terrain_height)
		{
		wait_for_target_y:
			WaitFrames(1);
			mode_or_frame++;
			if ((u32)mode_or_frame <= 0xB3)
			{
				if (object->y > object->terrain_height)
				{
					goto wait_for_target_y;
				}
			}
		}
		WaitFrames(2);
		BattleFx_PlayQueuedSound();
	}
}

void ObjectMotion_SetAngleToward(s32 first_id, s32 second_id, s32 wait)
{
	struct ObjectRuntime *first;
	struct ObjectRuntime *second;

	first = ObjectTable_Get(first_id);
	second = ObjectTable_Get(second_id);
	if (first != 0 && second != 0)
	{
		first->angle = ArcTan2(second->z - first->z, second->x - first->x);
		Battle_WaitMode0(wait);
	}
}

void Object_LinkPair(s32 first_id, s32 second_id, s32 wait)
{
	void *first = ObjectTable_Get(first_id);
	void *second = ObjectTable_Get(second_id);

	if (first != NULL && second != NULL)
	{
		FacingObject_TurnPairToFaceEachOther(first, second);
		Battle_WaitMode0(wait);
	}
}

void FacingObject_TurnPairToFaceEachOther(
	struct FacingObject *raw_first, struct FacingObject *raw_second)
{
	struct ObjectRuntime *first = (struct ObjectRuntime *)raw_first;
	struct ObjectRuntime *second = (struct ObjectRuntime *)raw_second;
	s32					  toward;
	s32					  away;
	s32					  frame;
	s32					  turning;
	s32					  delta;

	if (first == NULL || second == NULL)
		return;
	toward = (u16)ArcTan2(second->z - first->z, second->x - first->x);
	away = toward + 0x8000;
	for (frame = 0; frame < 60; frame++)
	{
		turning = 2;
		delta = (s16)(toward - first->angle);
		if (delta != 0)
		{
			if (delta > 0x1000)
				delta = 0x1000;
			if (delta < -0x1000)
				delta = -0x1000;
			first->angle += delta;
		}
		else
		{
			turning = 1;
		}
		delta = (s16)(away - second->angle);
		if (delta != 0)
		{
			if (delta > 0x1000)
				delta = 0x1000;
			if (delta < -0x1000)
				delta = -0x1000;
			second->angle += delta;
		}
		else
		{
			turning--;
		}
		if (turning == 0)
			break;
		WaitFrames(1);
	}
}

void ObjectTable_DestroyById(s32 index)
{
	void				   *object = ObjectTable_Get(index);
	struct ObjectSlotTable *table = (struct ObjectSlotTable *)gEventWork;

	if (object != 0)
	{
		ObjectDispatch_ReleaseFar(object);
		table->slots[index] = NULL;
	}
}

void ObjectTable_DestroyNoOp(void)
{
}

void ObjectGroup_ConfigureChildValue(s32 object_id, s32 value)
{
	s32					   flags;
	struct DispatchObject *object;

	object = ObjectTable_Get(object_id);
	if (object != NULL)
	{
		flags = 0x100 & value;
		if (flags != 0)
		{
			((struct ScriptMotionObject *)object)->hook =
				(void (*)(struct ScriptMotionObject *))ObjectGroup_ApplyIndexedChildValue;
			return;
		}
		((struct ScriptMotionObject *)object)->hook = NULL;
		ObjectGroup_SetChildValue((struct FieldActor *)object, value);
	}
}

void ObjectGroup_ApplyIndexedChildValue(struct DispatchObject *object)
{
	if ((object->kind & 0xf) == 1)
	{
		u8						child_value;
		struct AnimationObject *container;
		u8						child_count;

		child_value = ObjectGroup_BlinkChildValues[(gFrameCount >> 1) & 3];
		container = object->target.child;
		child_count = container->count;
		if (child_count != 0)
		{
			struct AnimationEntry **entries = container->entries;
			s32						remaining = child_count;
			do
			{
				struct AnimationEntry *entry = *entries++;
				if (entry != 0 && entry->script != 0)
				{
					entry->param = child_value;
				}
				remaining--;
			} while (remaining != 0);
		}
		container->dirty = 1;
	}
}

void ObjectGroup_SetChildValue(struct FieldActor *actor, s32 value)
{
	struct DispatchObject *object = (struct DispatchObject *)actor;

	if ((object->kind & 0xf) == 1)
	{
		struct AnimationObject *container = object->target.child;
		u8						raw_count = container->count;

		if (raw_count != 0)
		{
			struct AnimationEntry **entry = container->entries;
			u32						count = raw_count;
			do
			{
				struct AnimationEntry *item = *entry++;
				if (item != NULL && item->script != 0)
				{
					item->param = value;
				}
				count--;
			} while (count != 0);
		}
		container->dirty = 1;
	}
}

void Object_SetTargetAndCallback(u32 object_id, s32 target_id, const void *callback)
{
	struct ObjectRuntime *first = ObjectTable_Get(object_id);
	struct ObjectRuntime *second = ObjectTable_Get(target_id & 0xff);

	if (first != NULL && second != NULL)
	{
		first->linked_object = second;
		if (!(target_id & 0x10000))
		{
			first->action = 40;
			first->acceleration = second->acceleration * 2;
			first->speed_limit = second->speed_limit;
			first->unknown_59 = 0;
		}
		ObjectDispatch_InitializeFar((struct DispatchObject *)first, callback);
	}
}

s32 ObjectMotion_StepAngle(struct ObjectRuntime *object)
{
	s32 delta = 0;

	if (object != NULL)
	{
		s32 target_angle = (u16)object->action;
		s32 current_angle = object->angle;
		delta = (s16)(target_angle - current_angle);
		if (delta != 0)
		{
			if (delta > 4096)
				delta = 2048;
			if (delta < -4096)
				delta = -2048;
			object->angle = current_angle + delta;
		}
	}
	return delta;
}

void Object_ResetTargetAndSetMode1(u32 object_id)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->target_x = ACTOR_NO_TARGET;
		object->target_y = ACTOR_NO_TARGET;
		object->target_z = ACTOR_NO_TARGET;
		ObjectDispatch_InitFromTable6Far(object);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	}
}

void ObjectMotion_ArmCallback(s32 object_id, s32 angle, s32 wait)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL)
	{
		object->action = angle;
		ObjectDispatch_InitializeFar(
			(struct DispatchObject *)object, (const s32 *)ObjectMotion_StepAngleScript);
		Battle_WaitMode0(wait);
	}
}

void ObjectMotion_SetActionVariant(s32 object_id, s32 priority)
{
	struct ObjectRuntime *object = ObjectTable_Get(object_id);

	if (object != NULL && (object->animation_kind & 0xF) == 1)
	{
		struct FieldSprite *sprite = object->animation;

		sprite->priority = priority;
		sprite->second_priority = priority;
		object->unknown_23 &= ~ACTOR_PRIORITY_AUTOMATIC;
	}
}

void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id)
{
	void *p;
	u8	  flags;
	u32	  shape;

	p = Object_GetById(source_id);
	p = ((struct ObjectRuntime *)p)->animation;
	flags = ((struct FieldSprite *)p)->vram_block;
	shape = ((struct FieldSprite *)p)->tile;

	p = Object_GetById(target_id);
	p = ((struct ObjectRuntime *)p)->animation;
	((struct FieldSprite *)p)->vram_block = flags;
	((struct FieldSprite *)p)->tile = shape;
}

void ObjectVisual_ReservedNoOp(void)
{
}
