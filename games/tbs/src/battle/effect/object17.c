#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
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

void EffectSlot_Update(struct EffectSlot *effect)
{
	if (effect->active != 0)
	{
		effect->age++;
		if (effect->callback_delay != 0)
			effect->callback_delay--;
		else if (effect->callback != 0)
			effect->callback(effect);
		if (effect->active != 0)
		{
			if (effect->update_motion != 0)
				EffectSlot_UpdateMotion(effect);
			if (effect->render != 0)
				BattleFx_DrawScaledObject(effect);
		}
	}
}

void BattleFx_DrawScaledObject(struct EffectSlot *object)
{
	struct AnimationObject *sprite;
	s32						offset;
	s32						scale[2];
	s32						position[4];

	sprite = object->object;
	offset = 0;
	if (object->flags & 4)
		offset = 0x1fc0000 - object->z;
	scale[0] = Iwram_MulQ16(object->scale_x, sprite->scale);
	scale[1] = Iwram_MulQ16(object->scale_y, sprite->scale);
	position[0] = object->x;
	position[1] = offset;
	position[2] = object->z + offset;
	position[3] = 0;
	if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000
		&& object->z < 0xe00000)
		Object_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

void EffectSlot_UpdateMotion(struct EffectSlot *effect)
{
	s32 dx;
	s32 dz;
	s32 distance;
	s32 ix;
	s32 iz;
	s32 speed;
	s16 angle;
	s32 heading;
	s32 facing;
	s16 turn;

	if (effect->target_x == EFFECT_NO_TARGET)
		return;
	dx = effect->target_x - effect->x;
	dz = effect->target_z - effect->z;
	if (effect->stop_at_target != 0)
	{
		ix = dx / 0x10000;
		iz = dz / 0x10000;
		distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
		if (distance < 0x800000)
			distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
		if (distance <= 0x80000)
		{
			EffectSlot_SetPosition(effect, effect->target_x, effect->target_z);
			return;
		}
	}
	angle = ArcTan2(dz, dx);
	if (effect->flag42 != 0)
	{
		heading = effect->heading;
		turn = angle - heading;
		if ((turn >= 0 ? turn : -turn) >= effect->max_turn_step)
		{
			if (turn < 0)
			{
				if (-turn > effect->max_turn_step)
					turn = -effect->max_turn_step;
			}
			else if (turn > effect->max_turn_step)
			{
				turn = effect->max_turn_step;
			}
			angle = turn + heading;
		}
	}
	facing = (u16)angle;
	effect->heading = facing;
	speed = effect->speed + effect->acceleration;
	if (speed > effect->max_speed)
		speed = effect->max_speed;
	angle = facing;
	effect->speed = speed;
	effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
	effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

u32 BattleFx_HasReachedTarget(struct EffectSlot *effect)
{
	u32 value;

	if (effect->stop_at_target == 0)
	{
		return 0;
	}
	value = effect->target_x ^ 0x80000000;
	return ((0u - value) | value) >> 31;
}

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
	effect->target_x = EFFECT_NO_TARGET;
	effect->target_z = EFFECT_NO_TARGET;
	effect->x = x;
	effect->z = z;
	effect->speed = 0;
}

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
	AnimationObjects_SelectAnimationFar(effect->object, mode);
}

void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
	effect->callback = callback;
	effect->callback_delay = 0;
	effect->age = 0;
	effect->state = 0;
}

void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z)
{
	s32			 unit;
	s8			*object;
	volatile u32 zero;

	zero = 0;
	Dma_Set((const void *)&zero, effect, 0x85000000 | (sizeof *effect / 4), REG_DMA3);
	object = ResourceObject_CreateFar(kind);
	effect->object = object;
	if (object != NULL)
		object[9] &= ~0x0c;
	EffectSlot_SetPosition(effect, x, z);
	/* FAKEMATCH: needed to match: 1.0 is held in a local, set before max_speed, so the object reload is scheduled ahead of the three unit stores */
	unit = 0x10000;
	effect->max_speed = 0x20000;
	effect->acceleration = effect->scale_y = effect->scale_x = unit;
	effect->origin_x = x;
	effect->origin_z = z;
	((s8 *)effect->object)[38] = 0;
	effect->stop_at_target = 1;
	effect->flag42 = 1;
	effect->update_motion = 1;
	effect->render = 1;
	effect->active = 1;
	effect->random_value = Random16();
	effect->flags = 4;
	EffectSlot_SetObjectMode(effect, 1);
}

void BattleFx_ClearOwnedSlot(struct EffectSlot *slot)
{
	volatile u32 zero;
	if (slot->object)
		ResourceObject_ReleaseFar(slot->object);
	zero = 0;
	Dma_Set(&zero, slot, 0x85000000 | (sizeof *slot / 4), REG_DMA3);
}
