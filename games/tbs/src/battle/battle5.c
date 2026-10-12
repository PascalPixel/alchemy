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
#include "unit/battle/battle11.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object16.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
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

s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request, s32 delay)
{
	struct BattlePresentationTransition *view;
	struct BtlWork						*battle;
	struct BattleCamera					*camera;
	struct BtlUnit						*unit;
	u16									 actor[2];
	u16									 ids[14];
	s32									 kept;
	s32									 result;
	s32									 count;
	s32									 i;

	kept = 0;
	if (request->unit_id == 0xff)
		return 0;
	unit = UnitGetFar(request->unit_id);
	if (unit->hp == 0)
		return -1;
	if (unit->class_index == 0)
		BattleCommand_SelectAutomatic(request, 1);
	view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
	view->frames = 60;
	battle = WORK_SLOT(HEAP_SLOT_BATTLE);
	view->flag = 0;
	battle->brightness = 0x10000;
	camera = WORK_SLOT(HEAP_SLOT_CAMERA);
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
	Camera_StoreSceneParameters(0x01fe0000, Iwram_RatioMulQ14(0x01fe0000, 0xc000), 0x7fff0000);
	if (delay != 0)
	{
		view->target_yaw = 0x2000;
		WaitFrames(delay);
	}
	actor[0] = request->unit_id;
	actor[1] = 0xff;
	BattlePres_SetActorModes(actor, 1);
	result = BattleCommand_BuildPlan(request, &battle->plan);
	if (result == 0)
	{
		switch (battle->plan.outcome)
		{
		case 1:
			BattlePres_RunActorEntries(&battle->plan, 0);
			break;
		case 2:
			RunBattlePresentation(&battle->plan, 0);
			break;
		case 5:
			BattlePresentation_RunUnitTransition(&battle->plan, 1);
			break;
		case 9:
			BattlePresentation_RunUnitTransition(&battle->plan, 0);
			break;
		case 3:
			BattlePres_RunPsynergyCast(&battle->plan, 0);
			break;
		case 6:
			BattlePres_RunPsynergyCast(&battle->plan, 1);
			break;
		case 8:
			BattlePres_RunPsynergyCast(&battle->plan, 2);
			break;
		case 4:
			BattlePres_RunItem(&battle->plan, 0, request);
			break;
		case 7:
			if (BattlePresentation_RunEncounterOrUnitTrigger(&battle->plan) != 0)
				kept = 1;
			if (kept != 0)
				goto finish;
			break;
		}
	}
	else
	{
		if (result == -1)
		{
			BattlePresentation_WaitForAdvance();
			WaitFrames(3);
		}
		BattlePres_SetActorModes(0, 0);
	}
	BattleMotion_DestroyAllSlotObjects();
	BattleUnit_ProcessTurnEnd(&battle->plan);
	BtlObjPut();
	count = BattleParty_ListActorIds(3, ids);
	for (i = 0; i < count; i++)
		ObjMotRst(ids[i]);
	request->unit_id = 0xff;
finish:
	BattlePresentation_ConfigurePaletteFade(2, BtlWk->background, 0);
	return kept;
}

s32 BattlePres_BuildTargetList(struct BtlPlan *plan, struct BattlePresentationWork *work)
{
	s16 *dst;
	u32	 flags;
	s32	 count;
	s32	 i;
	u8	*target;

	count = 0;
	work->flags = 0;
	flags = plan->presentation_flags;
	work->kind = flags & 0xfff;
	work->variant = (flags & 0x3000) >> 12;
	work->actor = plan->aid;
	i = 0;
	if (i < plan->target_count)
	{
		target = plan->tgt_no;
		dst = work->actors;
		do
		{
			if (UnitGetFar(*target)->hp != 0 || (plan->presentation_flags & 0x10000))
			{
				count++;
				*dst++ = *target;
			}
			i++;
			target++;
		} while (i < plan->target_count);
	}
	if (count == 0)
	{
		work->actors[0] = plan->tgt_no[0];
		count = 1;
	}
	work->first_target = plan->tgt_no[0];
	work->count = count;
	work->initial_value = 1;
}

s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BtlPlan *plan)
{
	struct BattlePresentationTransition *presentation = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
	struct BtlWork						*scene = WORK_SLOT(HEAP_SLOT_BATTLE);
	/* FAKEMATCH: the halfword initialization preserves the word copy into party_mode instead of folding it into an immediate zero (one instruction differs). */
	union
	{
		s32 value;
		u16 halves[2];
	} completed;
	s32 party_mode;

	completed.halves[0] = 0;
	completed.halves[1] = 0;
	presentation->target_yaw = 0x2000;
	presentation->active = 1;
	BattlePres_SetActorModes(0, 0);

	if (plan->aid <= 7)
	{
		party_mode = completed.value;

		if (scene->encounter_mode != 2)
		{
			party_mode = 1;
		}

		if (!party_mode)
		{
			UiText_ShowMessageAndWaitCoreFar((s32)MsgNoEscape);
			BattlePresentation_WaitForAdvance();
		}
		else
		{
			s16 unit_ids[14];
			s32 index = BattleParty_ListLivingUnits(1, (u16 *)unit_ids) - 1;

			while (index != -1)
			{
				struct BtlUnit *unit = UnitGetFar(unit_ids[index]);

				if (unit->shibire == 0 && unit->nemuri == 0)
				{
					BattleMotion_SetupEscapeObject(unit_ids[index]);
					WaitFrames(8);
				}
				index--;
			}
			WaitFrames(22);
			completed.value = 1;
		}
	}
	else if (((Random16() * 10) >> 16) <= 6)
	{
		s16 event[2];

		event[0] = plan->aid;
		event[1] = 0xff;
		BattleMotion_SetupEscapeObject(event[0]);
		WaitFrames(8);
		BattleActor_RemoveFromLists(plan->aid);
		ActivateBattleObjectSlot(plan->aid);
	}
	else
	{
		UiText_ShowMessageAndWaitCoreFar((s32)MsgNoEscape);
		BattlePresentation_WaitForAdvance();
	}

	presentation->active = 0;
	return completed.value;
}

s32 BattlePresentation_RunUnitTransition(struct BtlPlan *plan, s32 mode)
{
	u16							  units[14];
	struct BattlePresentationWork work;
	u32							  actor;
	u32							  target;
	u32							  count;
	s32							  i;
	s32							  n;
	u32							  kept;
	struct MotionObject			 *object;
	void						**slots = (void **)&BtlWk;

	BattlePres_BuildTargetList(plan, &work);
	actor = plan->aid;
	target = plan->tgt_no[0];
	if (plan->presentation_flags & 0x8000)
	{
		struct BattlePresentationTransition *transition;
		s32									 yaw;
		transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
		yaw = 0x2000;
		if (actor > 7)
			yaw = 0x5000;
		transition->target_yaw = yaw;
		transition->frames = 60;
	}
	else
	{
		struct BattlePresentationTransition *transition;
		s32									 yaw;
		transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
		yaw = -0x2000;
		if (actor <= 7)
			yaw = 0x2000;
		if (transition->target_yaw != yaw)
			transition->target_yaw = yaw;
	}
	BattlePres_SetActorModes(0, 0);
	PtyStDrawFar(BtlWk->pty_st & ~1);
	object = BtlObjGet(actor)->object;
	REG_BLDCNT = 0x3f40;
	count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
	for (i = 0; i != count; i++)
	{
		if (units[i] == BATTLE_UNIT_REMOVED)
			continue;
		if (units[i] != actor)
		{
			if ((target <= 7) != (units[i] <= 7))
				BattlePres_SetActorRecordMode(units[i], 1);
		}
		else
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
		}
	}
	AudioCommand_PlayFar(0x9a);
	BattleFx_PlayUnitElementEffect(work.actor, plan->elm, 0, 0);
	if (mode & 1)
		BattlePres_SetActorRecordMode(actor, 1);
	for (i = 0; i != 16; i++)
	{
		REG_BLDALPHA = (16 - i) | 0x1000;
		WaitFrames(1);
	}
	if (plan->failure != 0)
	{
		if (plan->failure == 1)
		{
			BtlEvPush(0, plan->aid);
			BtlEvPush(4, (s32)MsgPsynergyBlocked);
		}
		else
		{
			BtlEvPush(4, (s32)MsgCastLacksPp);
		}
		BattleEv_DispatchQueued();
		BattlePres_RunWithZeroArguments();
	}
	else
	{
		kept = 0;
		for (i = 0; i < count; i++)
		{
			u32 unit = units[i];
			if (unit == actor)
			{
				if (!(mode & 1))
					units[kept++] = actor;
			}
			else if ((target <= 7) == (unit <= 7))
			{
				units[kept++] = unit;
			}
		}
		units[kept] = BATTLE_UNIT_LIST_END;
		BattleActor_SpawnObjectsForList(units, 0);
		for (n = 0; n < plan->target_count; n++)
			units[n] = plan->tgt_no[n];
		units[n] = BATTLE_UNIT_LIST_END;
		for (i = 0; i != work.count; i++)
		{
			struct AnimationObject *animation;
			u32						child;

			animation = GetMotionRecord(BtlObjGet(work.actors[i])->object, 0);
			for (child = 0; child != animation->count - 1; child++)
				work.params[i][child] = animation->entries[child]->param;
		}
		if (plan->presentation_flags & 0x8000)
		{
			if (target > 7)
				work.side = 0;
			else
				work.side = 1;
		}
		else
		{
			if (plan->tgt_no[0] <= 7)
				work.side = 1;
			else
				work.side = 0;
		}
		if (plan->presentation_flags & 0x20000)
			work.side ^= 1;
		Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
		if (plan->presentation_flags & 0x8000)
			BattleFx_InitializeModeFar((s32 *)&work);
		else if (plan->presentation_flags & 0x4000)
			BattleFx_DispatchByIdRangeFar((s32 *)&work);
		else
			BattleFx_DispatchModeFar((s32 *)&work);
		BattleEventRuntime_WaitForReady();
	}
	BtlObjPut();
	count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
	REG_BLDCNT = 0x3f40;
	for (i = 0; i != count; i++)
	{
		if (units[i] == BATTLE_UNIT_REMOVED || units[i] == actor)
			continue;
		if ((target <= 7) != (units[i] <= 7))
			BattlePres_SetActorRecordMode(units[i], 1);
	}
	for (i = 0; i != 16; i++)
	{
		REG_BLDALPHA = i | 0x1000;
		WaitFrames(1);
	}
	for (i = 0; i != count; i++)
		BattlePres_SetActorRecordMode(units[i], 0);
	BattlePres_SetupTransitionScene(0, 0, 0, 100);
	WaitFrames(1);
	return 0;
}

//--------------------------------------------------------------------------
// act_run_entries.c
// バトル表示：行動の効果を対象ごとに出す
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ハイライトを消し、対象ごとに効果をかけてイベントを流す
// 対象がいなければ、メッセージの送りを待つだけ
// plan : 行動と対象の情報
//--------------------------------------------------------------------------
s32 BattlePres_RunActorEntries(struct BtlPlan *plan, s32 unused)
{
	s32 i;

	BattlePres_SetActorModes(0, 0);
	if (plan->target_count == 0)
	{
		BattlePresentation_WaitForAdvance();
	}
	else
	{
		for (i = 0; i < plan->target_count; i++)
		{
			BtlTgtExec(plan, i);
			BattleEv_DispatchQueued();
		}
	}
	WaitFrames(1);
}

s32 RunBattlePresentation(struct BtlPlan *plan, s32 mode)
{
	struct BattlePresentationWork		 work;
	struct MotionObject					*object;
	struct AnimationObject				*animation;
	struct BtlUnit						*unit;
	s16									 position[3];
	s32									 scripted;
	struct BattlePresentationTransition *transition;
	s32									 angle;
	s32									 adjusted;
	s32									 facing_angle;
	s32									 x;
	s32									 z;
	s32									 divisor;
	s32									 direct;
	s32									 index;
	s32									 phase;

	transition = gTransitionWork;
	{
		struct BtlPlan *saved_plan = plan;

		/* FAKEMATCH: the temporary null assignment and restoration end the
		 * native first CSE block, so the first plan read reloads its stack
		 * carrier. Both conditional arms disappear from the generated code.
		 * Without them, 288 of 289 instructions remain and 17 rows differ. */
		if (transition)
			plan = NULL;
		if (transition)
			plan = saved_plan;
	}
	object = BtlObjGet(plan->aid)->object;
	z = object->z;
	x = object->x;
	angle = (u16)ArcTan2(x, z);
	adjusted = angle - 0x2000;
	if (plan->aid > 7)
		adjusted = angle + 0x6000;
	adjusted &= 0x7fff;
	facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

	if (transition->target_yaw == facing_angle)
	{
		transition->target_yaw = facing_angle;
		WaitFrames(5);
	}
	else
	{
		transition->target_yaw = facing_angle;
		WaitFrames(10);
	}

	BattlePres_SetActorModes(0, 0);
	BattlePres_BuildTargetList(plan, &work);
	if (work.kind == 0x87)
		PtyStDrawFar(BtlWk->pty_st & ~1);

	unit = UnitGetFar(work.actor);
	UnitGetFar(work.actors[0]);
	scripted = plan->tgt_crit[0];
	direct = plan->tgt_adj[0] == 0;

	animation = GetMotionRecord(BtlObjGet(plan->aid)->object, 0);
	divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->anim_id, 2, 1);
	BattleMotion_ApproachTarget(
		work.actor, work.actors[0], divisor, Battle_GetSpriteFlagHighBits(unit->class_id) << 16);
	ObjectDispatch_ApplyValueToChildrenFar(
		(struct DispatchObject *)BtlObjGet(work.actor)->object, 16);
	BtlObjGet(work.actors[0]);

	if ((u16)work.actors[0] <= 7)
		work.side = 1;
	else
		work.side = 0;

	REG_WIN0H = 0x00f0;
	REG_WIN0V = 0x1088;
	REG_WIN1H = 0x00f0;
	REG_WIN1V = 0x1088;
	REG_WININ = 0x3537;
	REG_WINOUT = 0x3f21;
	REG_DISPCNT |= 0x6000;

	if (direct != 0)
	{
		WaitFrames(10);
		BattleMotion_ResetObjectAtScaledAnchor(work.actors[0]);
		WaitFrames(2);
		WaitFrames(4);
		WaitFrames(10);
		BtlEvPush(0, plan->tgt_no[0]);
		BtlEvPush(4, (s32)MsgNimblyDodgesBlow);
		BattleEv_DispatchQueued();
		ObjMotRst(work.actors[0]);
	}
	else
	{
		phase = 0;
		work.flags = 0;
		if (plan->presentation_flags != 0)
			work.flags = 1;

		if (scripted != 0)
		{
			work.kind += 200;
			phase = 1;
			transition->flag = 1;
			position[0] = work.actor;
			position[1] = work.first_target;
			position[2] = 0xff;
			BattleActor_SpawnObjectsForList(position, 0);
		}

		divisor -= 8;
		if (divisor <= 0)
			divisor = 1;
		{
			s32 first;
			s32 second;

			for (index = 0; index != divisor; index++)
			{
				if (phase != 0)
				{
					first = work.actor;
					second = work.first_target;
					BattlePres_SetupTransitionAtPairMidpoint(
						first, second, index * 30 / divisor + 100);
				}
				WaitFrames(1);
			}
		}

		Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
		if (work.kind != 0)
		{
			if (plan->presentation_flags & 0x4000)
				BattleFx_DispatchByIdRangeFar((s32 *)&work);
			else
				BattleFx_DispatchModeFar((s32 *)&work);
		}
		BattleEventRuntime_WaitForReady();
		if (scripted != 0)
		{
			transition->flag = 0;
			BattleParty_ListAllUnitsAndSubmit();
			BattlePres_SetupTransitionScene(0, 0, 0, 100);
		}
		ObjMotRst(work.actors[0]);
	}
	ObjMotRst(work.actor);
	return 0;
}

s32 BattlePres_RunSimple(struct BtlPlan *input, s32 flags)
{
	struct BattlePresentationWork		 work;
	struct BtlPlan						*saved_input;
	struct MotionObject					*object;
	struct AnimationObject				*animation;
	struct BattlePresentationTransition *transition;
	s32									 angle;
	s32									 adjusted;
	s32									 facing_angle;
	s32									 x;
	s32									 z;
	s32									 divisor;
	s32									 scripted;

	transition = gTransitionWork;
	saved_input = input;
	object = BtlObjGet(saved_input->aid)->object;
	z = object->z;
	x = object->x;
	angle = (u16)ArcTan2(x, z);
	adjusted = angle - 0x2000;
	if (saved_input->aid > 7)
		adjusted = angle + 0x6000;
	adjusted &= 0x7fff;
	facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

	if (transition->target_yaw == facing_angle)
	{
		transition->target_yaw = facing_angle;
		WaitFrames(5);
	}
	else
	{
		transition->target_yaw = facing_angle;
		WaitFrames(20);
	}

	BattlePres_SetActorModes(0, 0);
	BattlePres_BuildTargetList(saved_input, &work);
	UnitGetFar(work.actor);
	UnitGetFar(saved_input->tgt_no[0]);

	scripted = flags & 2;
	animation = GetMotionRecord(BtlObjGet(saved_input->aid)->object, 0);
	divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->anim_id, 2, 1);
	BattleMotion_ApproachTarget(work.actor, saved_input->tgt_no[0], divisor, 0);
	ObjectDispatch_ApplyValueToChildrenFar(
		(struct DispatchObject *)BtlObjGet(work.actor)->object, 16);
	BtlObjGet(saved_input->tgt_no[0]);

	if (saved_input->tgt_no[0] <= 7)
		work.side = 1;
	else
		work.side = 0;
	if (scripted != 0)
	{
		WaitFrames(10);
		BattleMotion_ResetObjectAtScaledAnchor(saved_input->tgt_no[0]);
		WaitFrames(2);
		WaitFrames(4);
		WaitFrames(10);
		ObjMotRst(saved_input->tgt_no[0]);
	}
	else
	{
		BattleFx_DispatchByIdRangeFar((s32 *)&work);
		BattleEv_DispatchQueued();
		ObjMotRst(saved_input->tgt_no[0]);
	}
	ObjMotRst(work.actor);
	return 0;
}

s32 BattlePres_RunItem(struct BtlPlan *plan, s32 unused, struct BattleActionRecord *selection)
{
	struct BattlePresentationWork		 work;
	struct MotionObject					*object;
	s32									 i;
	s32									 item;
	struct BtlUnit						*unit;
	struct ItemDefinition				*def;
	struct BattlePresentationTransition *transition = gTransitionWork;
	s32									 facing;

	if (plan->aid > 4)
		facing = -0x2000;
	else
		facing = 0x2000;
	if (transition->target_yaw != facing)
		transition->target_yaw = facing;
	BattlePres_BuildTargetList(plan, &work);
	BattlePres_SetActorModes(0, 0);
	object = BtlObjGet(work.actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
	ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
	if (plan->tgt_no[0] <= 7)
		work.side = 1;
	else
		work.side = 0;

	for (i = 0; i != work.count; i++)
	{
		struct AnimationObject *animation = GetMotionRecord(BtlObjGet(work.actors[i])->object, 0);
		s32						j;

		for (j = 0; j != animation->count - 1; j++)
			work.params[i][j] = animation->entries[j]->param;
	}

	Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
	if (work.kind != 0)
	{
		s32 fade;

		i = 0;
		fade = 0;
		do
		{
			struct BtlWork *battle = BtlWk;

			if (i < 20)
			{
				u16 *palette = battle->palette;
				s32	 value = 0x10000 - fade;

				battle->brightness = value;
				Graphics_ScaleRgb555Clamped(palette, (u16 *)(BG_PLTT + 6 * 16), value, 128);
			}
			WaitFrames(1);
			i++;
			fade += 0x444;
		} while (i < 20);
		if (plan->presentation_flags & 0x4000)
			BattleFx_DispatchByIdRangeFar((s32 *)&work);
		else
			BattleFx_DispatchModeFar((s32 *)&work);
	}
	else
	{
		WaitFrames(60);
	}
	BattleEventRuntime_WaitForReady();
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
	for (i = 0; i != work.count; i++)
		ObjMotRst(work.actors[i]);

	unit = UnitGetFar(selection->unit_id);
	item = unit->inventory[selection->parameter];
	def = Item_GetDirectFar(item);
	if (def->use_type == 1)
	{
		s32 result = Inventory_RemoveFar(selection->unit_id, selection->parameter);
		s32 index = selection->parameter;
		if (result == 2)
		{
			struct BtlWork *battle = BtlWk;
			u32				row;

			for (row = 0; row < 20; row++)
			{
				if (battle->actions[row].command == 2
					&& battle->actions[row].unit_id == selection->unit_id)
				{
					if (battle->actions[row].parameter == index)
						battle->actions[row].parameter = -1;
					else if (battle->actions[row].parameter > index)
						battle->actions[row].parameter--;
				}
			}
		}
	}
	else if (def->use_type == 2)
	{
		if ((BtlRndFar() & 7) == 0)
		{
			BtlEvPush(BEV_ITEM, unit->inventory[selection->parameter]);
			BtlEvPush(BEV_MES, (s32)MsgUsedItemBroke);
			Inventory_BreakFar(selection->unit_id, selection->parameter);
			BattleEv_DispatchQueued();
		}
	}
	else if (def->use_type == 4)
	{
		if ((item & ITEM_ID_MASK) == 0xb8)
			item = 0xb9;
		unit->inventory[selection->parameter] = item;
	}
	return 0;
}

s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value)
{
	s32						index = 0;
	s32						child_index;
	s32						count;
	struct AnimationObject *record;
	struct AnimationEntry  *child;
	struct AnimationEntry **children;
	/* FAKEMATCH: needed to match: cached compound assignment keeps 94 bytes but changes registers and store order */
	u8 mask = 0xff;
	u8 frame;

	while ((record = GetMotionRecord(object, index)) != NULL)
	{
		children = &record->entries[1];
		child = record->entries[0];
		frame = child->frame;
		child->frame = frame | mask;
		count = record->count;
		child->param = value;
		for (child_index = 1; child_index < count; child_index++)
		{
			child = *children++;
			child->param = 0;
			child->frame = 0xff;
		}
		index++;
	}
}

s32 BattlePres_RunPsynergyCast(struct BtlPlan *plan, s32 mode)
{
	struct BattlePresentationWork		 work;
	struct BattlePresentationTransition *transition = gTransitionWork;
	struct MotionObject					*object;
	s32									 scripted;
	s32									 i;

	if (plan->presentation_flags & 0x40000)
	{
		transition->target_yaw = plan->aid <= 7 ? -0x2000 : 0x5000;
		transition->frames = 60;
	}
	else
	{
		struct MotionObject *actor = BtlObjGet(plan->aid)->object;
		s32					 angle = (u16)ArcTan2(actor->x, actor->z);
		s32					 current;

		if (plan->aid <= 7)
			current = angle - 0x1800;
		else
			current = angle + 0x1800;
		current = (s16)current;
		current += ((plan->aid <= 7 ? 0x2000 : -0x2000) - current) * 3 / 4;
		if (!((plan->aid <= 7) ^ (plan->tgt_no[0] <= 7)))
			current = plan->aid <= 7 ? 0x2400 : -0x2400;
		if (transition->target_yaw != current)
			transition->target_yaw = current;
	}
	if (plan->presentation_flags & 0x80000)
	{
		transition->target_yaw = plan->aid <= 7 ? -0x2000 : 0x2000;
		transition->frames = 60;
	}

	BattlePres_BuildTargetList(plan, &work);
	scripted = mode & 1;
	if (scripted)
		work.flags = 1;
	BattlePres_SetActorModes(0, 0);
	PtyStDrawFar(BtlWk->pty_st & ~1);
	object = BtlObjGet(work.actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
	ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
	AudioCommand_PlayFar(0x9a);
	if (mode & 2)
		BattleFx_PlayUnitElementEffect(work.actor, plan->elm, 1, 0);
	else if (!scripted)
		BattleFx_PlayUnitElementEffect(work.actor, plan->elm, 0, 0);
	if (plan->tgt_no[0] <= 7)
		work.side = 1;
	else
		work.side = 0;

	for (i = 0; i != work.count; i++)
	{
		struct AnimationObject *animation = GetMotionRecord(BtlObjGet(work.actors[i])->object, 0);
		s32						j;

		for (j = 0; j != animation->count - 1; j++)
			work.params[i][j] = animation->entries[j]->param;
	}
	if (plan->failure != 0)
	{
		if (plan->failure == 1)
		{
			BtlEvPush(0, plan->aid);
			BtlEvPush(4, (s32)MsgPsynergyBlocked);
		}
		else
		{
			BtlEvPush(4, (s32)MsgCastLacksPp);
		}
		BattleEv_DispatchQueued();
		BattlePres_RunWithZeroArguments();
	}
	else
	{
		Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
		if (work.kind)
		{
			if (plan->presentation_flags & 0x4000)
				BattleFx_DispatchByIdRangeFar((s32 *)&work);
			else
				BattleFx_DispatchModeFar((s32 *)&work);
		}
		else
		{
			BattlePres_RunWithZeroArguments();
		}
		BattleEventRuntime_WaitForReady();
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
		for (i = 0; i != work.count; i++)
			ObjMotRst(work.actors[i]);
	}
	return 0;
}
