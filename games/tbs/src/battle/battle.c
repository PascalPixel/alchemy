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

struct DebugPartyEntry
{
	s8	owner;
	u8	level;
	s8	djinn_counts[4];
	u16 items[4];
	u16 actions[2];
	u16 unknown_12;
};

extern struct DebugPartyEntry Debug_PartyPresets[];

s32 DebugParty_LoadPreset(s32 preset)
{
	s32				next_djinn[4];
	struct BtlUnit *state;
	s32				owner;
	s32				done;
	s32				row;
	s32				j;
	s32				set;
	s32				i;

	done = 0;
	Party_RemoveActiveOwnerFar(0);
	Party_RemoveActiveOwnerFar(1);
	Party_RemoveActiveOwnerFar(2);
	Party_RemoveActiveOwnerFar(3);
	Party_RemoveActiveOwnerFar(5);

	set = 0;
	for (i = 0; i < 4; i++)
		next_djinn[i] = 0;

	for (row = 0;; row++)
	{
		owner = Debug_PartyPresets[row].owner;
		if (row < 0 || row > 332)
		{
			done = 1;
			break;
		}
		if (owner == -1)
		{
			if (set == preset)
				break;
			set++;
			continue;
		}
		if (set != preset)
			continue;

		Party_AddActiveOwnerFar(owner);
		Party_AdvanceOwnerCountToTargetFar(owner, Debug_PartyPresets[row].level);
		state = (struct BtlUnit *)UnitGetFar(owner);
		for (i = 0; i < 4; i++)
		{
			state->djinn_owned_counts[i] = 0;
			state->djinn_active_counts[i] = 0;
			state->djinn_available[i] = 0;
			state->djinn_active[i] = 0;
		}
		for (i = 0; i < 32; i++)
			/* FAKEMATCH: the halfword store through a u16 cast loads its zero from the literal pool, as the reference does. */
			*(u16 *)&((struct BtlUnit *)state)->action_slots[i] = 0;
		for (i = 0; i < 2; i++)
		{
			if (Debug_PartyPresets[row].actions[i] != 0)
				OwnerAction_AddFar(owner, Debug_PartyPresets[row].actions[i]);
		}
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < Debug_PartyPresets[row].djinn_counts[i]; j++)
			{
				Djinn_AddToOwnerFar(owner, i, next_djinn[i]);
				Djinn_ActivateFar(owner, i, next_djinn[i]);
				next_djinn[i]++;
			}
		}
		for (i = 0; i < 16; i++)
			Inventory_RemoveFar(owner, 0);
		for (i = 0; i < 4; i++)
		{
			if (Debug_PartyPresets[row].items[i] != 0)
				Inventory_EquipFar(
					owner, Inventory_AddItemFar(owner, Debug_PartyPresets[row].items[i]));
		}
	}
	return done;
}

#if EDITION_INTERNATIONAL
#define WINDOW_WORK_MESSAGE_BUSY 0x12f8
#else
#define WINDOW_WORK_MESSAGE_BUSY 0x1188
#endif

#if defined(TBS_EDITION_EN)
#define DEBUG_SECOND_FORM ((s32)MsgBecameCursed)
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define DEBUG_SECOND_FORM ((s32)MsgEnd)
#else
#define DEBUG_SECOND_FORM ((s32)MsgDebugSecondForm)
#endif

#define DEBUG_VIEW_LIMIT                                                                         \
	((s32)MsgRariberoHowDidSearchForSheba - (s32)MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely \
		+ 5)

void DebugBattle_ViewMessages(void)
{
	u16				text[64];
	s32				second = 0;
	struct BtlUnit *unit = UnitGetFar(0);
	s32				i;
	s32				index;
	s32				window;
	s32				message;

	Ui_AdjustValueWithoutLimitFar((s32)MsgDebugSampleName, text);
	for (i = 0; i <= 13; i++)
	{
		unit->name[i] = text[i];
		if (text[i] == 0)
			break;
	}
	unit->name[14] = 0;
	UiWork_InitializeWithResourceCountersFar();
	AudioCommand_PlayFar(71);
	for (;;)
	{
		index = 0;
		{
			s32 control = 0x1341;

			REG_DISPCNT = control;
		}
		gGameState.message_speed = 2;
		for (;;)
		{
			UiWork_ClearValueNameTablesFar();
			UiWork_PushValueSlotFar(999, 5);
			UiWork_PushValueSlotFar(0, 3);
			UiWork_PushValueSlotFar(1, 1);
			UiWork_PushValueSlotFar(1, 2);
			UiWork_PushValueSlotFar(2, 4);
			if (second == 0)
			{
				message = (s32)MsgBabiIriguchiTheyWereTerribleWeWereAbsolutely;
				window = UiText_OpenMessageWindowFar(index + message, 2, 10, 4);
			}
			else
			{
				message = DEBUG_SECOND_FORM;
				window = UiText_OpenMessageWindowFar(index + message, 2, 2, 4);
			}
			WaitFrames(10);
			for (;;)
			{
				if (gKeysRepeat & KEY_B)
				{
					if (second != 0)
					{
						second = 0;
					}
					else
					{
						index++;
						second = 1;
					}
				}
				if (gKeysRepeat & KEY_RIGHT)
					index++;
				if (gKeysRepeat & KEY_LEFT)
					index -= 2;
				if (gKeysRepeat & KEY_UP)
					second = 1;
				if (gKeysRepeat & KEY_DOWN)
					second = 0;
				if (gKeysRepeat & KEY_R)
					index += 10;
				if (gKeysRepeat & KEY_L)
					index -= 10;
				if (index < 0)
					index = 0;
				if (index >= (u32)DEBUG_VIEW_LIMIT)
					index = DEBUG_VIEW_LIMIT;
				if (gKeysRepeat & (KEY_B | KEYS_DPAD | KEYS_SHOULDERS))
					break;
				if (UiWork_IsCompleteFar() != 0 && (gKeysRepeat & KEY_A))
					break;
				WaitFrames(1);
			}
			UiWork_DrainPendingFar(1);
			UiWork_FinalizeFar(window, 1);
			(*(u8 **)&gWindowWork)[WINDOW_WORK_MESSAGE_BUSY] = 0;
		}
	}
}

void DebugBattle_RunMenu(void)
{
	s32 locked = 0;
	s32 encounter;
	s32 preset;
	s32 loaded;

	GameState_InitDefaultsFar();
restart:
	Ui_LoadWindowGraphics();
	Bg0_ClearTilemap();
	Scheduler_ResetTaskTable();
	Runtime_InitializeHeap();
	Resource_InitializeTable();
	GameFlag_SetBitFar(362);
	encounter = 257;
	if (gKeysHeld & KEY_DOWN)
	{
		loaded = -1;
		GameFlag_ClearBitFar(362);
		preset = 0;
		for (;;)
		{
			GameFlag_ClearBitFar(32);
			WaitFrames(1);
			for (;;)
			{
				if (gKeysRepeat & KEY_RIGHT)
					encounter++;
				if (gKeysRepeat & KEY_LEFT)
					encounter--;
				if (gKeysRepeat & KEY_UP)
					encounter -= 10;
				if (gKeysRepeat & KEY_DOWN)
					encounter += 10;
				if (gKeysRepeat & KEY_R)
					preset++;
				if (gKeysRepeat & KEY_L)
					preset--;
				if (gKeysRepeat & KEY_A)
					break;
				if (gKeysRepeat & KEY_START)
					DebugBattle_ViewMessages();
				if (gKeysRepeat & KEY_SELECT)
					Battle_ReservedNoOp2A08();
				if ((gKeysRepeat & KEY_B) || locked)
				{
					locked = 1;
					Battle_SetStart(5);
				}
				if (preset != loaded)
				{
					GameState_InitDefaultsFar();
					DebugParty_LoadPreset(preset);
					loaded = preset;
				}
				WaitFrames(1);
			}
			if (gKeysHeld & KEY_DOWN)
				GameFlag_SetBitFar(364);
			UnitCalcFar(0);
			gGameState.special = (s32)&ResourceId_SuharaGateBackdrop;
			if (encounter == 28)
				GameFlag_SetBitFar(366);
			GameFlag_SetBitFar(354);
			Battle_RunEncounter(encounter);
			Ui_LoadWindowGraphics();
			Bg0_ClearTilemap();
			Scheduler_ResetTaskTable();
			Runtime_InitializeHeap();
			Resource_InitializeTable();
		}
	}
	GameFlag_SetBitFar(354);
	Battle_RunEncounter(257);
	goto restart;
}

void BattlePresentation_UpdateCamera(void)
{
	void							   **slot = (void **)&gCameraWork;
	struct BattleCamera					*state = slot[0];
	struct BattlePresentationTransition *transition =
		slot[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
	struct BtlWork			 *work = slot[HEAP_SLOT_BATTLE - HEAP_SLOT_CAMERA];
	struct FixedPointPosition local;
	s32						 *pos;
	s16						  delta;
	u32						  id;

	if (work->two_sided != 0)
	{
		if ((gLinkStatus & 3) != 3)
		{
			work->link_misses++;
			if (work->link_misses > 24)
			{
				work->link_paused = 1;
			}
		}
		else
		{
			id = (REG_SIOCNT << 0x1A) >> 0x1E;
			if (work->link_side != id)
			{
				work->link_paused = 1;
			}
			work->link_misses = 0;
		}
	}

	if (transition->frames != 0)
	{
		delta = transition->target_yaw - state->yaw;
		delta /= 16;
		state->yaw += delta;
		transition->frames--;
	}

	pos = state->pos;
	if (state->follow_pos != 0)
	{
		pos = state->follow_pos;
	}

	Render_ResetTransformState();
	SceneTransform_ApplyPosition(pos);
	SceneTransform_ApplyYaw((s16)state->yaw);
	SceneTransform_ApplyPitch((s16)state->pitch);

	local.x = 0;
	local.y = 0;
	local.z = state->distance;
	Iwram_TransformVector(&local.x, (s32 *)state);

	if (transition->flag == 0)
	{
		BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
	}
}

void BattleIntro_AnnounceEncounter(s32 enemy_count)
{
	s16				enemies[8];
	struct BtlWork *battle;
	s16			   *enemy;
	s32				announced;

	battle = BtlWk;
	UiWork_ClearValueNameTablesFar();
	BtlEnmList(enemies);

	announced = 0;
	if (enemy_count != 0)
	{
		enemy = enemies;
		do
		{
			UiWork_PushValueSlotFar((u16)*enemy++, 1);
			if (announced == enemy_count - 1)
				UiText_ShowMessageAndWaitCoreFar((s32)MsgLastEnemyAppeared);
			else
				UiText_ShowMessageAndWaitCoreFar((s32)MsgEnemyAppeared);
			announced++;
			BattlePresentation_WaitForAdvance();
		} while (announced != enemy_count);
	}

	UiWork_FinalizeSharedSlotFar();
	if (battle->encounter_mode == BATTLE_ENCOUNTER_PARTY_FIRST)
	{
		UiWork_ClearValueNameTablesFar();
		UiWork_PushValueSlotFar(0, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgPartyStrikesFirst);
		BattlePresentation_WaitForAdvance();
	}
	else if (battle->encounter_mode == BATTLE_ENCOUNTER_ENEMIES_FIRST)
	{
		UiWork_ClearValueNameTablesFar();
		UiWork_PushValueSlotFar(0, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgPartySurprised);
		BattlePresentation_WaitForAdvance();
	}
}

void BattleParty_CollectUnitList(void)
{
	u16				buf[14];
	struct BtlWork *battle;
	s32				count;
	s32				i;
	s32				index;
	s32				kind;

	battle = BtlWk;
	count = BattleParty_PrepareActiveOwners(buf);
	for (i = 0; i < count; i++)
		battle->party_units[i] = buf[i];
	battle->party_units[count] = 0xff;

	count = BtlEnmList(buf);
	kind = battle->battle_type;
	if (kind >= 0)
	{
		if (kind <= 1)
		{
			for (i = 0; i < count; i++)
				battle->enm_no[i] = buf[i];
			goto done;
		}
	}
	for (i = 0; i < count; i++)
	{
		index = BattleParty_CenterOrderOffsets[i] + count / 2;
		battle->enm_no[index] = buf[i];
	}
done:
	battle->enm_no[count] = 0xff;
}

//--------------------------------------------------------------------------
// tile_pattern.c
// バトル表示：VRAM のパターン詰めと BG0 の縦スクロール
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// VRAM 0x06000290 からの 16 バイトを 0x06000280 へ詰め、
// 0x0600028c からの 20 バイトを 0 で消す
//--------------------------------------------------------------------------
s32 BattlePresentation_InitializeTilePattern(void)
{
	Dma_Set((const void *)0x06000290, (void *)0x06000280, 0x80000008, REG_DMA3); // 16 ビット × 8
	return Iwram_ClearWords((void *)0x0600028c, 20);
}

//--------------------------------------------------------------------------
// BG0 の縦スクロールを 0 にもどす
//--------------------------------------------------------------------------
void BattlePresentation_ClearBg0VerticalOffset(void)
{
	u32 zero = 0;

	REG_BG0VOFS = zero;
}

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void Party_ReservedNoOp5B14(s32 unused)
{
}

s32 BattleParty_ResetActiveRuntimeFields(void)
{
	u16 owners[10];
	s32 count;
	s32 i;

	count = BattleParty_PrepareActiveOwners(owners);

	i = 0;
	if (i < count)
	{
		struct BtlUnit *unit;
		u8			   *cursor;
		s32				remaining;

		do
		{
			unit = UnitGetFar(owners[i]);
			cursor = (u8 *)&unit->elm_lv[3];
			remaining = 3;

			do
			{
				remaining--;
				*cursor-- = 0;
			} while (remaining >= 0);

			unit->atk_cnt = 0;
			unit->atk_lv = 0;
			unit->def_cnt = 0;
			unit->def_lv = 0;
			unit->res_cnt = 0;
			unit->res_lv = 0;
			unit->maboroshi = 0;
			unit->konran = 0;
			unit->charm = 0;
			unit->shibire = 0;
			unit->nemuri = 0;
			unit->fuuji = 0;
			unit->refrain = 0;
			unit->reflect = 0;
			unit->shiryo = 0;
			unit->unknown_142[0] = 0;
			unit->unknown_142[1] = 0;
			unit->sonae = 0;
			unit->miugoki = 0;
			unit->agi_cnt = 0;
			unit->agi_lv = 0;
			unit->chouhatsu = 0;

			UnitCalcFar(owners[i]);
			i++;
		} while (i < count);
	}
}

s32 BattleParty_ReconcileDjinnRecovery(void)
{
	u16 owners[10];
	s32 count;
	s32 i;
	s32 owner;
	s32 element;
	s32 index;

	count = BattleParty_PrepareActiveOwners(owners);

	for (i = 0; i < count; i++)
	{
		owner = owners[i];
		for (element = 0; element <= 3; element++)
		{
			for (index = 0; index <= 19; index++)
			{
				if (Trade_CanOfferDjinnFar(owner, element, index) != 0)
				{
					struct DjinnRecoveryList *list =
						&((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(
							  (owner < 0 || owner > 7) ? 1 : 0))
							 ->list;
					s32 j;

					for (j = 0; j < list->count; j++)
					{
						if (element == list->entries[j].element && index == list->entries[j].index)
							break;
					}
					if (j == list->count)
						Trade_AddOfferFar(owner, element, index);
				}
			}
		}
	}

	if (GameFlag_TestFar(364) != 0)
		return;

	{
		struct DjinnRecoveryList *list =
			&((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(0))->list;
		struct DjinnRecoveryEntry *entry;

		i = 0;
		if (i < list->count)
		{
			s32 permanent_timer = -1;

			entry = list->entries;
			do
			{
				if (entry->turns == permanent_timer && BtlObjGet(entry->unit_id) == 0)
				{
					u8 id = entry->unit_id;
					u8 element = entry->element;
					u8 index = entry->index;

					Djinn_ActivateFar(id, element, index);
					Trade_RemoveOfferFar(id, element, index);
				}
				i++;
				entry++;
			} while (i < list->count);
		}
	}
}

struct BattleSummonState *BattleSummon_UpdateAvailability(void)
{
	u8	totals[4];
	u16 party_members[10];
	s32 party_size;
	s32 element;
	u32 available_mask;

	party_size = BattleParty_PrepareActiveOwners(party_members);
	available_mask = 0;

	element = 0;
	do
	{
		totals[element] = 0;
		{
			s32 party_slot;

			for (party_slot = 0; party_slot < party_size; party_slot++)
			{
				struct BtlUnit *member = UnitGetFar(party_members[party_slot]);

				totals[element] += member->djinn_owned_counts[element];
			}
		}
		element++;
	} while (element <= 3);

	element = 0;
	do
	{
		const struct SummonDefinition *summon = SummonDefinition_Get(element);
		s32							   elements_met;

		if (summon != 0)
		{
			const u8 *required = summon->djinn_required;

			elements_met = 0;
			if (totals[0] >= required[0])
			{
				u8 *total = totals;

				do
				{
					elements_met++;
					if (elements_met > 3)
						break;
					total++;
					required++;
				} while (*total >= *required);
			}

			if (elements_met == 4)
				available_mask |= 1u << element;
		}
		element++;
	} while (element <= 31);

	{
		struct BattleSummonState *state = Trade_GetOfferStateFar(0);
		state->available_mask = available_mask;
		return state;
	}
}

void BattleSummon_ReservedNoOp(void)
{
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
#define NAME_LIMIT 7
#define APPEND_NAME_SPACE 1
#else
#define NAME_LIMIT 4
#define APPEND_NAME_SPACE 0
#endif

enum LinkPartyPacketSize
{
	LINK_PARTY_UNIT_BYTES = 340,
	LINK_PARTY_OFFER_BYTES = 320
};

s32 UpdateNameEntries(void)
{
	u16				name_text[24];
	void		   *buffer;
	struct BtlUnit *name_entry;
	s32				named_count;
	s32				index;
	s32				len;
	s32				i;

	buffer = Runtime_BumpAllocateAlternatePool(LINK_PARTY_UNIT_BYTES);
	named_count = 0;
	index = 0;
	while (index <= 2)
	{
		name_entry = UnitGetFar(index + 128);
		if (SerialRuntime_BeginTransferB(name_entry) == -1)
		{
			break;
		}
		SerialRuntime_WaitForTransferB();
		if (name_entry->side != 0)
		{
			named_count += 1;
		}
		WaitFrames(2);
		Ui_AdjustValueWithoutLimitFar((s32)MsgEnemyLabel, name_text);
		i = 0;
		if (name_text[i] != 0)
		{
			do
			{
				i += 1;
				if (i > NAME_LIMIT)
				{
					break;
				}
			} while (name_text[i] != 0);
		}
#if APPEND_NAME_SPACE
		name_text[i] = ' ';
		i += 1;
#endif
		len = i;
		for (i = 14; i >= len; i--)
		{
			name_entry->name[i] = name_entry->name[i - len];
		}
		for (i = 0; i < len; i++)
		{
			name_entry->name[i] = name_text[i];
		}
		name_entry->name[14] = 0;
		index += 1;
	}
	MemFree(buffer);
	buffer = Runtime_BumpAllocateAlternatePool(LINK_PARTY_OFFER_BYTES);
	if (SerialRuntime_BeginTransferB(Trade_GetOfferStateFar(1)) != -1)
	{
		SerialRuntime_WaitForTransferB();
		WaitFrames(2);
	}
	MemFree(buffer);
	return named_count;
}

s32 LinkBattle_SendParty(void)
{
	u8						 *buffer;
	struct BtlWork			 *work;
	s32						  j;
	struct DjinnRecoveryList *list;
	s32						  size;
	s32						  count;
	s32						  i;
	s32						  mark;
	u16						  owners[8];

	size = LINK_PARTY_UNIT_BYTES;
	buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
	mark = 0xff;
	work = BtlWk;
	for (i = 7; i >= 0; i--)
		work->owner_slots[i] = mark;
	count = BattleParty_PrepareActiveOwners(owners);
	for (i = 0; i < count; i++)
	{
		Iwram_CopyWords(buffer, UnitGetFar(owners[i]), size);
		((struct BtlUnit *)buffer)->side = 2;
		work->owner_slots[owners[i]] = i - 128;
		if (SerialRuntime_BeginTransferA(buffer, LINK_PARTY_UNIT_BYTES) == -1)
			break;
		SerialRuntime_WaitForTransferA();
		WaitFrames(2);
	}
	for (; i <= 2; i++)
	{
		((struct BtlUnit *)buffer)->side = 0;
		if (SerialRuntime_BeginTransferA(buffer, LINK_PARTY_UNIT_BYTES) == -1)
			break;
		SerialRuntime_WaitForTransferA();
		WaitFrames(2);
	}
	size = LINK_PARTY_OFFER_BYTES;
	MemFree(buffer);
	buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
	/* FAKEMATCH: a one-pass loop is a sched2 barrier, so the list pointer's copy is emitted before the count's address as in the reference */
	do
	{
		Iwram_CopyWords(buffer, Trade_GetOfferStateFar(0), size);
	} while (0);
	list = &((struct DjinnRecoveryTable *)buffer)->list;
	for (j = 0; j < list->count; j++)
		list->entries[j].unit_id = work->owner_slots[list->entries[j].unit_id];
	if (SerialRuntime_BeginTransferA(buffer, LINK_PARTY_OFFER_BYTES) != -1)
	{
		SerialRuntime_WaitForTransferA();
		WaitFrames(1);
		WaitFrames(2);
	}
	MemFree(buffer);
}

u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source)
{
	u8	buffer[5];
	u8 *base = buffer;
	u32 fill = '_';
	u8 *p = base;
	u8 *dst = p;
	s32 count = 3;

	do
	{
		u32 value = *source;

		*dst = value;
		source++;
		dst++;
		if ((u8)value == 0)
			*p = fill;
		p++;
		count--;
	} while (count >= 0);
	base[4] = 0;
	return dst;
}

//--------------------------------------------------------------------------
// wait_sync.c
// バトル：通信対戦の合図あわせ
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 通信対戦なら、相手と "EXEC" の合図をそろえる
// 30 フレームのうちにそろえば 0、通信が切れたりそろわなければ -1
// 通信対戦でなければ 0
//--------------------------------------------------------------------------
s32 BattlePres_WaitSync(void)
{
	struct BtlWork *work = BtlWk;
	u16			   *peer;
	u16			   *sig;
	s32				lost = 0;
	s32				i;

	if (work->two_sided != 0)
	{
		u32 other = work->link_side ^ 1;

		peer = (u16 *)(gSerialPeerPayloads[other] + 4);
		sig = (u16 *)gSerialTransfer.payload;
		if (work->link_paused == 0)
		{
			sig[0] = 'E';
			sig[1] = 'X';
			sig[2] = 'E';
			sig[3] = 'C';

			for (i = 0; i <= 29; i++)
			{
				if ((gLinkStatus & 3) != 3) // 2 台の通信がとぎれた
				{
					lost++;
					if (lost > 24)
						return -1;
				}
				else
				{
					lost = 0;
					if (sig[2] == peer[2] && sig[3] == peer[3])
						return 0;
				}
				WaitFrames(1);
			}
		}
		return -1;
	}
	return 0;
}

//--------------------------------------------------------------------------
// 通信対戦なら、行動のたびとラウンド終了処理のあとに相手と合図を順にそろえる
// "ex" "TU" → "rn" → "EXEC" → "tu" → "RN"
// 通信が切れているか、前にそろえた合図が相手と食いちがうか、
// 通信が 25 フレーム続けてとぎれたら -1。そろえば（通信対戦でなくても）0
//--------------------------------------------------------------------------
s32 BattlePres_SyncTurn(void)
{
	struct BtlWork *work = BtlWk;
	u16			   *peer;
	u16			   *sig;
	s32				lost = 0;
	/* FAKEMATCH: needed to match: never used: the reference's frame keeps twenty bytes that no instruction touches */
	u16 spare[10];

	if (work->two_sided != 0)
	{
		peer = gLinkPeerSignatures[work->link_side ^ 1];
		sig = (u16 *)gSerialTransfer.payload;
		if (work->link_paused != 0)
			return -1;

		// "ex" "TU"
		sig[0] = 'e';
		sig[1] = 'x';
		sig[4] = 'T';
		sig[5] = 'U';
		WaitFrames(1);
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (sig[2] != peer[2] || sig[3] != peer[3])
					return -1;
				if (sig[0] == peer[0] && sig[1] == peer[1] && sig[4] == peer[4]
					&& sig[5] == peer[5])
					break;
			}
			WaitFrames(1);
		}

		// "rn"
		sig[6] = 'r';
		sig[7] = 'n';
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (sig[4] != peer[4] || sig[5] != peer[5])
					return -1;
				if (sig[6] == peer[6] && sig[7] == peer[7])
					break;
			}
			WaitFrames(1);
		}

		// "EXEC"
		sig[0] = 'E';
		sig[1] = 'X';
		sig[2] = 'E';
		sig[3] = 'C';
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (sig[6] != peer[6] || sig[7] != peer[7])
					return -1;
				if (sig[0] == peer[0] && sig[1] == peer[1] && sig[2] == peer[2]
					&& sig[3] == peer[3])
					break;
			}
			WaitFrames(1);
		}

		// "tu"
		sig[4] = 't';
		sig[5] = 'u';
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (sig[0] != peer[0] || sig[1] != peer[1] || sig[2] != peer[2]
					|| sig[3] != peer[3])
					return -1;
				if (sig[4] == peer[4] && sig[5] == peer[5])
					break;
			}
			WaitFrames(1);
		}

		// "RN"：相手が "rn" から変わるのを待つ
		sig[6] = 'R';
		sig[7] = 'N';
		for (;;)
		{
			if ((gLinkStatus & 3) != 3)
			{
				if (++lost > 24)
					return -1;
			}
			else
			{
				lost = 0;
				if (peer[6] != 'r' || peer[7] != 'n')
					break;
			}
			WaitFrames(1);
		}
	}
	return 0;
}

s32 BattleParty_AssignMemberSlots(void)
{
	u16				active_members[8];
	struct BtlWork *battle_state = BtlWk;
	s32				party_size = BattleParty_PrepareActiveOwners(active_members);
	s32				member_slot;
	s32				unit_id;

	for (member_slot = 0; member_slot < party_size; member_slot++)
	{
		unit_id = active_members[member_slot];
		battle_state->owner_slots[unit_id] = member_slot - 128;
	}
}

s32 Battle_ClearSerialTransferPayload(void)
{
	return Iwram_ClearWords(gSerialTransfer.payload, 16);
}

#define DMA_FILL32 0x85000000

#define Dma3Fill(cell, dest, bytes)                                              \
	{                                                                            \
		(cell) = 0;                                                              \
		Dma_Set((void *)&(cell), (dest), DMA_FILL32 | ((bytes) >> 2), REG_DMA3); \
	}

#define BattleRandom_ClearSeed() \
	do                           \
	{                            \
		gBattleRandomSeed = 0;   \
	} while (0)

s32 Battle_RunEncounter(s32 enemy_group)
{
	struct BattleCamera					*camera;
	s32									*target;
	struct BtlWork						*battle;
	u8									*owner_states;
	struct BattlePresentationTransition *transition;
	struct BattleDisplayWork			*display;
	s32									 object;
	u8									*dest;
	s32									 count;
	s32									 i;
	s32									 result;
	s32									 handle;
	s32									 wait;
	u32									 pos;
	u32									 actor;
	s16									 cue;
	u8									 unused[48];
	volatile u32						 fill;

	camera = Runtime_AllocateBlock(HEAP_SLOT_CAMERA, sizeof(struct BattleCamera));
	battle = Runtime_AllocateBlock(HEAP_SLOT_BATTLE, sizeof(struct BtlWork));
	owner_states = Runtime_AllocateBlock(54, 0x7c8);
	transition = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_BACKGROUND, 32);
	Runtime_AllocateBlock(11, 0x280);
	target = camera->pos;
	/* FAKEMATCH: needed to match: result starts at 0 and is never read before each exit sets it; CSE takes the setup zeros from its register, r7 as listed (without it 262) */
	result = 0;
	Iwram_ClearWords(owner_states, 0x7c8);
	Scheduler_ResetTaskTable();
	transition->frames = 0;
	transition->target_yaw = 0x2000;
	transition->flag = 1;
	transition->unknown_18 = 0;
	transition->unknown_1c = 0;
	REG_DISPCNT = 1;
	GameFlag_SetBitFar(0x103);
	GameFlag_SetBitFar(0x169);
	Render_ResetTransformState();

	Dma3Fill(fill, camera, sizeof(struct BattleCamera));
	Dma3Fill(fill, battle, sizeof(struct BtlWork));
	battle->prompt_slot = -1;
	battle->enm_grp = enemy_group;
	display = Runtime_AllocateBlock(37, sizeof(struct BattleDisplayWork));
	Dma3Fill(fill, display, sizeof(struct BattleDisplayWork));
	battle->background = Event_GetSpecialValueFar();
	Runtime_AllocateBlock(4, 0xe00);
	Runtime_AllocateBlock(3, 0x600);
	ObjectSystem_InitializeFar(4);
	if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0)
		UiWork_InitializeFar(1);
	else
		UiWork_InitializeFar(0);

	target[0] = 0;
	target[1] = 0x400000;
	target[2] = 0;
	camera->eye[0] = 0;
	camera->eye[1] = 0xb40000;
	camera->eye[2] = 0x400000;
	camera->yaw = 0x2800;
	camera->pitch = 0x5000;
	camera->distance = 0x1000000;
	object = BattleFormation_BuildEnemyList(battle->enm_grp);

	if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0)
	{
		battle->two_sided = 1;
		Battle_SetStart(4);
	}
	if (battle->two_sided != 0)
	{
		BattleRandom_ClearSeed();
		for (wait = 0;;)
		{
			if ((gLinkStatus & 3) == 3)
				break;
			WaitFrames(1);
			wait++;
			if (wait > 24)
			{
				battle->link_paused = 1;
				break;
			}
		}
		battle->link_side = (REG_SIOCNT << 26) >> 30;
		dest = gBattleOwnerStates;
		for (pos = 0; pos <= 0x7c7; pos++)
			*dest++ = gActorSpriteSlots[pos];
		object = GameFlag_GetByteFar(0x3f0);
		BattleParty_AssignMemberSlots();
		battle->battle_type = 0;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_UpdateCamera, 0xc7f);

	cue = gGameState.scene_cue;
	if (cue != 0)
	{
		AudioCommand_PlayFar(cue);
		if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0)
		{
			AudioCommand_PlayFar(55);
			Sound_LoadPresetParameters(4);
		}
	}
	else
	{
		AudioCommand_PlayFar(51);
		AudioCommand_PlayFar(76);
	}

	BattleParty_CollectUnitList();
	BattleUnit_RefreshPlacement();
	BattleParty_ReconcileDjinnRecovery();
	BattleSummon_UpdateAvailability();
	if (((struct BattleSummonState *)Trade_GetOfferStateFar(0))->available_mask != 0)
		battle->pty_st = 3;
	else
		battle->pty_st = 1;
	UiWindow_CreateWithLayoutBoundsFar(9);
	Camera_InitDefaultTransform();
	BtlObjPut();
	BattlePresentation_InitializeWorkAndResetState();
	BattleBackground_Load(1, battle->background, 0);
	BattleCamera_SetRange(0xa00000, 0x500000, 0, 0, 0x20000);
	BattlePres_SetupTransitionScene(0, 0, 0, 190);
	Party_ReservedNoOp5B14(1);
	REG_BLDCNT = 0;
	Summon_ClearWorkFields();
	battle->prompt_slot = Resource_LoadIntoFreeSlot(128);
	battle->encounter_mode = 0;
	if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0)
	{
		battle->encounter_mode = 1;
	}
	else if (gGameState.battle_start == 0)
	{
		if ((BtlRndFar() & 15) == 0)
			battle->encounter_mode = 1;
		else if ((BtlRndFar() & 31) == 0)
			battle->encounter_mode = 2;
	}
	BattlePres_RunEntryTransition(object, enemy_group);
	transition->flag = 0;
	gCpuLoadDisplayEnabled = 0;
	Scheduler_AddOrUpdateCallback(BattleActor_UpdateSpritePriorities, 0xc80);

	for (;;)
	{
		Battle_ReservedNoOp9B2C();
		BattleSummon_UpdateAvailability();
		if (((struct BattleSummonState *)Trade_GetOfferStateFar(0))->available_mask != 0)
			battle->pty_st = 3;
		else
			battle->pty_st = 1;
		transition->target_yaw = 0x2800;
		transition->frames = 60;
		PtyStDrawFar(battle->pty_st);
		Iwram_ClearWords(battle->actions, 0x140);
		Resource_ResetEntry(battle->prompt_slot);
		if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0)
		{
			Runtime_GetRemainingIwram();
			Runtime_GetRemainingEwram();
			count = BattlePresentation_BuildActions(battle->actions);
			Runtime_GetRemainingIwram();
			Runtime_GetRemainingEwram();
		}
		else
		{
			count = BattlePresentation_BuildSortedUnitEntries(battle->actions);
		}
		battle->prompt_slot = Resource_LoadIntoFreeSlot(128);
		PtyStDrawFar(battle->pty_st);
		if (count < 0)
			goto aborted;

		for (i = 0; i < count; i++)
		{
			actor = battle->actions[i].unit_id;
			Runtime_GetRemainingIwram();
			Runtime_GetRemainingEwram();
			if (GameFlag_TestFar(FLAG_AUTO_BATTLE) == 0)
			{
				if (BattlePresentation_DispatchAction(&battle->actions[i], i == 0 ? 10 : 0) == 1)
					goto interrupted;
			}
			else
			{
				if (BattlePres_RunAction(&battle->actions[i]) == 1)
					goto interrupted;
			}
			Runtime_GetRemainingIwram();
			Runtime_GetRemainingEwram();
			if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) == 0)
				goto party_lost;
			if (BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) == 0)
			{
				if (actor <= 7 && battle->spoils.defeated == 1)
					battle->defeat_state = 3;
				goto resolved;
			}
			if (BattlePres_SyncTurn() < 0)
				goto aborted;
		}

		battle->encounter_mode = 0;
		Battle_ReservedNoOpF674();
		Battle_ProcessRoundEnd();
		BattleMotion_DestroyAllSlotObjects();
		if (battle->two_sided != 0)
		{
			if (BattlePres_SyncTurn() < 0)
				goto aborted;
		}
		else
		{
			WaitFrames(20);
		}
		if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) == 0)
			continue;

		handle = UiText_OpenMessageWindowFar((s32)MsgTutorialUnleashed, 0, 4, 1);
		while (UiWork_IsCompleteFar() == 0)
			WaitFrames(1);
		UiWork_FinalizeFar(handle, 1);
		WaitFrames(1);
		handle = UiText_OpenMessageWindowFar((s32)MsgTutorialStandby, 10, 4, 1);
		BattlePresentation_WaitForPromptAt(92, 24);
		UiWork_FinalizeFar(handle, 1);
		WaitFrames(1);
	}

resolved:
	Battle_ClearSerialTransferPayload();
	if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) == 0)
	{
		if (battle->two_sided != 0)
			AudioCommand_PlayFar(58);
		if (battle->spoils.defeated != 0)
		{
			AudioCommand_PlayFar(58);
			if (battle->defeat_state <= 1)
			{
				BtlUnitSetFar(128, battle->enemy_classes[battle->first_defeated], 26);
				UiWork_ClearValueNameTablesFar();
				UiWork_PushValueSlotFar(128, 1);
				UiText_ShowMessageAndWaitCoreFar(battle->defeat_state + (s32)MsgDowned);
				BattlePresentation_WaitForAdvance();
			}
		}
		Battle_AwardSpoils();
	}
	AudioCommand_PlayFar(17);
	Blend_SetDarkenTarget16(30);
	Blend_WaitForTransition();
	result = battle->spoils.defeated;
	goto finished;

aborted:
	Battle_ClearSerialTransferPayload();
	Scheduler_EnableCallbacks(0);
	REG_DISPCNT = 1;
	result = battle->spoils.defeated;
	GameFlag_SetBitFar(0x3e8);
	goto finished;

party_lost:
	Battle_ClearSerialTransferPayload();
	AudioCommand_PlayFar(59);
	UiWork_ClearValueNameTablesFar();
	UiWork_PushValueSlotFar(gGameState.active_owners[0], 1);
	if (BattleParty_PrepareActiveOwners(0) == 1)
		UiText_ShowMessageAndWaitCoreFar((s32)MsgGoesDownInDefeat);
	else
		UiText_ShowMessageAndWaitCoreFar((s32)MsgPartyDefeated);
	BattlePresentation_WaitForAdvance();
	AudioCommand_PlayFar(17);
	Blend_SetDarkenTarget16(30);
	result = -1;
	Blend_WaitForTransition();
	goto finished;

interrupted:
	AudioCommand_PlayFar(17);
	Blend_SetDarkenTarget16(30);
	Blend_WaitForTransition();
	result = 0x3e7;

finished:
	BattleParty_ResetActiveRuntimeFields();
	Battle_ReservedNoOpF674();
	BattlePlacement_UpdateTimedEntries();
	Battle_SetStart(0);
	Scheduler_RemoveCallback(BattleActor_UpdateSpritePriorities);
	Runtime_ReleaseHeapBlock10();
	return result;
}
