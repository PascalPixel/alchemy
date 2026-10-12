#include "vram_tab.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/battle/battle2.h"
#include "unit/battle/battle3.h"
#include "unit/battle/battle4.h"
#include "unit/battle/battle5.h"
#include "unit/battle/battle6.h"
#include "unit/battle/battle7.h"
#include "unit/battle/battle8.h"
#include "unit/battle/battle9.h"
#include "unit/battle/battle10.h"
#include "unit/battle/battle11.h"
#include "unit/battle/battle12.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object18.h"
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
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "owner_state.h"
#include "edition.h"
#include "io_reg.h"
#include "battle_unit.h"
#include "game_state.h"
#include "btlsys.h"
#include "text/msg_ids.h"
#include "mem.h"
#include "resource_ids.h"
#include "callback_scheduler.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "btldisp.h"
#include "btlmes.h"
#include "btlpty.h"
#include "battle_intro.h"
#include "inventory.h"
#include "dma.h"
#include "scene.h"
#include "btlwork.h"
#include "btltype.h"
#include "btlsmn.h"
#include "tbs_edition.h"
#include "serial_runtime.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "battle_command.h"
#include "resource.h"
#include "heap_state.h"
#include "flag_ids.h"
#include "menu_list.h"
#include "audio_engine_symbols.h"
#include "ui.h"
#include "btlcalc.h"
#include "objdisp.h"
#include "motobj.h"
#include "battle_status_icon.h"
#include "animspr.h"
#include "field_event.h"
#include "metadata_lookup.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "effect_step.h"
#include "battle_escape.h"
#include "battle_target.h"
#include "btlobj.h"
#include "btlev.h"
#include "project.h"
#include "battle_random.h"
#include "irqlock.h"
#include "transform.h"
#include "battle_efx.h"
#include "character.h"
#include "io_write_queue.h"
#include "item.h"
#include "irq.h"
#include "vram_block.h"
#include "ownerval.h"
#include "sound_ids.h"
#include "ramcode.h"
#include "map_scroll.h"
#include "iowrite.h"
#include "battle_formation.h"
#include "b5_context.h"
#include "frame.h"

s32 BattlePresentation_BuildSortedUnitEntries(struct BattleActionRecord *entries)
{
	struct BattleActionRecord  swap;
	u16						   unit_ids[14];
	s32						   first_count;
	s32						   count = 0;
	s32						   second_count;
	s32						   priority_range;
	s32						   index;
	struct BattleActionRecord *entry;
	struct BtlUnit			  *unit;

	first_count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unit_ids);
	for (index = 0; index != 4; index++)
		UnitGetFar(index);
	for (index = 0; index < first_count; index++)
	{
		s32 unit_id = unit_ids[index];

		unit = UnitGetFar(unit_id);
		entry = &entries[index];
		entry->unit_id = unit_id;
		entry->priority = unit->agi;
		entry->command = 0;
		entry->parameter = 0;
		entry->target = 0x80;
		count++;
	}
	second_count = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unit_ids);
	entry = &entries[first_count];
	priority_range = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, NULL);
	for (index = 0; index < second_count; index++)
	{
		s32 unit_id = unit_ids[index];

		unit = UnitGetFar(unit_id);
		entry->unit_id = unit_id;
		entry->priority = unit->agi >> 1;
		if (entry->priority != 0)
			entry->priority += (Random16() * unit->agi) >> 16;
		entry->command = 0;
		entry->parameter = 0;
		entry->target = (Random16() * priority_range) >> 16;
		count++;
		entry++;
	}
	for (index = count - 2; index > 0; index--)
	{
		s32 swaps = 0;
		s32 pos;

		for (pos = count - 1; pos > 0; pos--)
		{
			if ((s16)entries[pos].priority > (s16)entries[pos - 1].priority)
			{
				Dma_Set(&entries[pos], &swap, 0x84000004, REG_DMA3);
				Dma_Set(&entries[pos - 1], &entries[pos], 0x84000004, REG_DMA3);
				Dma_Set(&swap, &entries[pos - 1], 0x84000004, REG_DMA3);
				swaps++;
			}
		}
		if (swaps == 0)
			break;
	}
	return count;
}

DEFINE_ADJUST_CAMERA(BattlePres_AdjustCameraByShoulderKeys)

s32 BattlePres_RunAction(struct BattleActionRecord *action)
{
	struct BattlePresentationTransition *transition;
	s32									 actor_id;
	s32									 battle_mode;
	struct BtlUnit						*actor;

	actor_id = action->unit_id;
	actor = UnitGetFar(actor_id);
	if (actor->hp == 0)
		return -1;

	action->target = BattleTarget_ReplaceDefeated(action);
	transition = gTransitionWork;
	if (action->unit_id > 4)
		battle_mode = -0x2000;
	else
		battle_mode = 0x2000;
	transition->target_yaw = battle_mode;
	transition->frames = 60;
	UiWork_ClearValueNameTablesFar();

	switch (action->command)
	{
	case 99:
		UiText_ShowMessageAndWaitCoreFar((s32)MsgPartyFlees);
		if (BattleEscape_PlayRun(action) != 0)
			return 1;
		break;
	case 3:
		WaitFrames(45);
		BattlePres_ShowDefendMessage(action);
		break;
	case 2:
		WaitFrames(45);
		BattlePres_RunUnitAction(action);
		break;
	case 0:
	default:
	{
		struct BattlePresentationTransition *tr = gTransitionWork;
		tr->flag = 0;
		BattlePres_RunUnitAction(action);
		tr->flag = 0;
		break;
	}
	case 1:
		BattlePresentation_RunPairedUnitTransition(action);
		break;
	}

	UiWork_ResetFreeChannelFar();
	return 0;
}

s32 BattleObject_IsValidId(u32 object_id)
{
	if (object_id <= 7)
	{
		return 0;
	}
	if (object_id - 128 <= 5)
	{
		return 0;
	}
	return -1;
}

s32 BattleEscape_PlayRun(struct BattleActionRecord *action)
{
	s16 party_members[14];
	s32 party_size;
	s32 animated;
	s32 member_slot;

	(void)action;
	if (((Random16() << 4) >> 16) != 0)
	{
		party_size = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, party_members);
		animated = 0;
		if (party_size != 0)
		{
			member_slot = 0;
			do
			{
				BattleMotion_SetupEscapeObject(party_members[member_slot]);
				animated++;
				WaitFrames(8);
				member_slot++;
			} while (animated != party_size);
		}
		WaitFrames(22);
		return 1;
	}
	UiText_ShowMessageAndWaitCoreFar((s32)MsgCannotEscape);
	return 0;
}

s32 BattlePres_ShowDefendMessage(const struct BattleActionRecord *action)
{
	s32				object_id;
	s32				result;
	struct BtlUnit *unit;

	object_id = action->unit_id;
	unit = UnitGetFar(object_id);
	if (BattleObject_IsValidId(object_id) < 0)
	{
		return -1;
	}
	result = 0;
	if (unit->hp <= 0)
	{
		return result;
	}
	UiWork_ClearValueNameTablesFar();
	UiWork_PushValueSlotFar(object_id, 1);
	UiText_ShowMessageAndWaitCoreFar((s32)MsgActorDefends);
	return 0;
}

s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *action)
{
	u16							  visible_units[14];
	struct BattlePresentationWork context;
	s32							  actor_id;
	s32							  target_id;
	u32							  side_start;
	u32							  side_count;
	u32							  living_count;
	struct MotionObject			 *actor_record;
	u32							  index;

	living_count = 0;
	actor_id = action->unit_id;
	if (BattleObject_IsValidId(actor_id) < 0)
	{
		return -1;
	}
	target_id = action->target;
	if (BattleObject_IsValidId(target_id) < 0)
	{
		return -1;
	}

	{
		struct BattlePresentationTransition *transition = gTransitionWork;
		transition->target_yaw = action->unit_id > 4 ? 0x5000 : 0x2000;
		transition->frames = 60;
	}
	WaitFrames(10);
	Random16();
	actor_record = BtlObjGet(actor_id)->object;

	if (target_id >= 0 && target_id <= 7)
	{
		side_count = BattleParty_ListLivingUnits(2, visible_units);
		side_start = 0x80;
	}
	else
	{
		side_count = BattleParty_ListLivingUnits(1, visible_units);
		side_start = 0;
	}

	for (index = 0; index != side_count; index++)
	{
		if (index + side_start == actor_id)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(actor_record, 3);
		}
	}

	WaitFrames(30);
	REG_BLDCNT = 0x3f40;
	for (index = 0; index != side_count; index++)
	{
		BattlePres_SetActorRecordMode(index + side_start, 1);
	}
	for (index = 0; index != 16; index++)
	{
		REG_BLDALPHA = (16 - index) | 0x1000;
		WaitFrames(1);
	}
	PtyStDrawFar(9);

	if (target_id > 0x7f)
	{
		u32 count = BattleParty_ListLivingUnits(2, visible_units);
		for (index = 0; living_count != count; index++)
		{
			u32 unit = index + 0x80;
			if (UnitGetFar(unit)->hp > 0)
			{
				visible_units[living_count++] = unit;
			}
		}
	}
	else
	{
		u32 count = BattleParty_ListLivingUnits(1, visible_units);
		for (index = 0; living_count != count; index++)
		{
			if (UnitGetFar(index)->hp > 0)
			{
				visible_units[living_count++] = index;
			}
		}
	}
	visible_units[living_count] = 0xff;
	BattleActor_SpawnObjectsForList(visible_units, 0);

	context.kind = action->parameter;
	context.actor = actor_id;
	for (index = 0; index != living_count; index++)
	{
		context.actors[index] = visible_units[index];
	}
	context.count = living_count;
	if (target_id >= 0 && target_id <= 7)
	{
		context.side = 1;
	}
	else
	{
		context.side = 0;
	}
	BattleFx_InitializeModeFar((s32 *)&context);

	WaitFrames(10);
	BtlObjPut();
	REG_BLDCNT = 0x3f40;
	for (index = 0; index != side_count; index++)
	{
		BattlePres_SetActorRecordMode(index + side_start, 1);
	}
	for (index = 0; index != 16; index++)
	{
		REG_BLDALPHA = index | 0x1000;
		WaitFrames(1);
	}
	for (index = 0; index != side_count; index++)
	{
		BattlePres_SetActorRecordMode(index + side_start, 0);
	}
	BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
	WaitFrames(3);
	return 0;
}

s32 BattlePres_RunApproachAction(struct BattleActionRecord *input)
{
	struct BattlePresentationWork work;

	if (gTransitionWork->target_yaw == 0x2000)
	{
		gTransitionWork->target_yaw = 0x2000;
		WaitFrames(10);
	}
	else
	{
		gTransitionWork->target_yaw = 0x2000;
		WaitFrames(30);
	}

	work.actor = input->unit_id;
	if (BattleObject_IsValidId(work.actor) < 0)
		return -1;

	work.actors[0] = input->target;
	if (BattleObject_IsValidId(work.actors[0]) < 0)
		return -1;

	UnitGetFar(work.actor);
	UnitGetFar(work.actors[0]);
	Random16();
	UiWork_PushValueSlotFar(work.actor, 1);
	UiText_ShowMessageAndWaitCoreFar((s32)MsgActorAttacks);
	BattleMotion_ApproachTarget(work.actor, work.actors[0], 13, 0);
	ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(work.actor)->object, 16);
	BtlObjGet(work.actors[0]);

	work.count = 1;
	if ((u16)work.actors[0] <= 7)
		work.side = 1;
	else
		work.side = 0;
	work.flags = 0;

	WaitFrames(4);
	BattleFx_DispatchByIdRangeFar(&work);
	ObjMotRst(work.actors[0]);
	ObjMotRst(work.actor);
	return 0;
}

s32 BattlePres_RunUnitAction(struct BattleActionRecord *action)
{
	struct BattlePresentationWork		 work;
	struct BattlePresentationTransition *transition = gTransitionWork;
	struct MotionObject					*object;
	s32									 facing;
	s32									 i;

	if (action->unit_id > 4)
		facing = -0x2000;
	else
		facing = 0x2000;
	if (transition->target_yaw == facing)
	{
		transition->frames = 40;
		WaitFrames(40);
	}
	else
	{
		transition->target_yaw = facing;
		transition->frames = 40;
		WaitFrames(40);
	}
	work.kind = action->parameter;
	work.initial_value = action->rng;
	work.actor = action->unit_id;
	work.first_target = action->target;
	if (BattleObject_IsValidId(work.actor) < 0)
		return -1;
	if (work.first_target > 127)
		work.count = BattleParty_ListLivingUnits(2, work.actors);
	else
		work.count = BattleParty_ListLivingUnits(1, work.actors);
	PtyStDrawFar(BtlWk->pty_st & ~1);
	object = BtlObjGet(work.actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
	ObjectDispatch_ApplyValueToChildrenFar(object, 16);
	if ((u16)action->target <= 7)
	{
		work.side = 1;
		BattleParty_ListLivingUnits(1, work.actors);
		work.count = 1;
	}
	else
	{
		work.side = 0;
		BattleParty_ListLivingUnits(2, work.actors);
		work.count = 1;
	}
	for (i = 0; i != work.count; i++)
	{
		struct AnimationObject *animation = BtlObjGet(work.actors[i])->object->records;
		s32						j;

		for (j = 0; j != animation->count - 1; j++)
			work.params[i][j] = animation->entries[j]->param;
	}
	/* FAKEMATCH: needed to match: the one-pass stages force fresh work pointers in the reset loop and place the dispatch argument before the kind store */
	do
	{
		work.kind = 0;
		work.variant = 0;
		BattleFx_RunIceFleckBurstFar(&work);
		work.kind = 1;
		BattleFx_RunIceFleckBurstFar(&work);
		work.kind = 2;
		BattleFx_RunIceFleckBurstFar(&work);
		work.kind = 3;
		BattleFx_RunIceFleckBurstFar(&work);
		work.kind = 0;
		BattleFx_DispatchModeFar(&work);
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	} while (0);
	for (i = 0; i != work.count; i++)
		ObjMotRst(work.actors[i]);
	ObjMotRst(work.actor);
	return 0;
}

void BattlePresentation_ApplyUnitDamage(u32 unit_id, s32 damage, s32 show_message, u8 *context)
{
	u8				local_context[4];
	struct BtlUnit *character;
	struct BtlObj  *slot;

	if (context == 0)
	{
		context = local_context;
		context[0] = 0;
		context[1] = 0;
		context[2] = 0;
		context[3] = 0;
	}

	character = UnitGetFar(unit_id);
	character->hp -= damage;
	if (character->hp < 0)
		character->hp = 0;

	slot = BtlObjGet(unit_id);
	ObjectDispatch_ApplyArgumentToChildrenFar(slot->object, 5);
	PtyStDrawFar(0);
	UiWork_ClearValueNameTablesFar();

	if (unit_id <= 7)
	{
		if (show_message != 0)
			UiText_ShowMessageAndWaitCoreFar((s32)MsgBitterBlow);
		UiWork_PushValueSlotFar(damage, 5);
		UiWork_PushValueSlotFar(unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgDmgP);
	}
	else
	{
		if (show_message != 0)
			UiText_ShowMessageAndWaitCoreFar((s32)MsgCritical);
		UiWork_PushValueSlotFar(damage, 5);
		UiWork_PushValueSlotFar(unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgDmgE);
		UiWork_PushValueSlotFar(unit_id, 1);
	}

	BattleMotion_SetMode5AndActivateSlot(unit_id);
	if (unit_id <= 7)
	{
		if (character->hp <= 0)
		{
			UiWork_PushValueSlotFar(unit_id, 1);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgGoesDown);
		}
	}
	else if (character->hp <= 0)
	{
		UiWork_PushValueSlotFar(unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgDowned);
	}

	slot = BtlObjGet(unit_id);
	ObjectDispatch_ApplyArgumentToChildrenFar(slot->object, 1);
}

void BattleMotion_SetMode5AndActivateSlot(s32 unit_id)
{
	struct AnimationEntry  *actor_data;
	struct AnimationObject *object;
	if (UnitGetFar(unit_id)->hp <= 0)
	{
		object = BtlObjGet(unit_id)->object->records;
		AnimationObjects_SelectAnimationFar(object, 5);
		actor_data = object->entries[0];
		actor_data->param = 6;
		actor_data->frame = 0xFF;
		WaitFrames(4);
		Map_RenderAnimatedTileFramesForObjectFar(object);
		ActivateBattleObjectSlot(unit_id);
	}
}

s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action)
{
	u16 living_units[14];
	s32 target_id;
	s32 living_count;

	target_id = action->target;
	if (UnitGetFar(target_id)->hp != 0)
	{
		return target_id;
	}

	if (target_id > 0x7F)
	{
		living_count = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, living_units);
	}
	else
	{
		living_count = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, living_units);
	}

	if (living_count == 0)
	{
		return 0x100;
	}

	return (s16)living_units[(Random16() * living_count) >> 0x10];
}
