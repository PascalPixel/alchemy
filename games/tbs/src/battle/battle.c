#include "vram_tab.h"
#include "window.h"
#include "unit/battle/battle.h"
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

s32 BattleParty_PrepareActiveOwners(u16 *owners)
{
	s32 limit;
	s32 count;
	s32 index;

	limit = 4;
	if (BtlWk->two_sided != 0)
		limit = 3;

	count = Party_CountActiveOwnersFar();
	if (count > limit)
		count = limit;

	for (index = 0; index < count; index++)
	{
		s32 owner = gGameState.active_owners[index];

		if (owners != 0)
			*owners++ = owner;
		UnitGetFar(owner)->side = BATTLE_UNIT_PARTY;
	}

	if (owners != 0)
		*owners = BATTLE_UNIT_LIST_END;
	return count;
}

s32 BtlEnmList(s16 *unit_ids)
{
	s16 *output;
	s32	 battle_result;
	s32	 id;
	s32	 entry_limit;
	s32	 entry_count;

	output = unit_ids;
	entry_count = 0;
	entry_limit = 6;
	battle_result = 0;
	if (output != NULL)
	{
		if (GameFlag_TestFar(0x16C) != 0)
		{
			entry_limit = 3;
		}
		id = 0x80;
		entry_limit += 0x80;
		for (; id < entry_limit; id += 1)
		{
			if (UnitGetFar(id)->side != 0)
			{
				*output = id;
				entry_count += 1;
				output += 1;
			}
		}
		*output = BATTLE_UNIT_LIST_END;
		battle_result = entry_count;
	}
	return battle_result;
}

s32 BattleParty_ListLivingUnits(s32 side_mask, u16 *unit_ids)
{
	u16				active_members[8];
	u16			   *base;
	s32				remaining;
	s32				enemy_limit;
	u16			   *output;
	s32				living_count;
	s32				enemy_capacity;
	u16			   *member;
	s32				unit_id;
	s16				hp;
	s32				active_count;
	struct BtlUnit *unit;

	output = unit_ids;
	living_count = 0;
	enemy_capacity = 6;
	if (GameFlag_TestFar(0x16C) != 0)
	{
		enemy_capacity = 3;
	}
	if (side_mask & BATTLE_SIDE_PARTY)
	{
		base = active_members;
		active_count = BattleParty_PrepareActiveOwners(base);
		if (living_count < active_count)
		{
			member = base;
			remaining = active_count;
			do
			{
				unit_id = *member;
				member += 1;
				hp = UnitGetFar(unit_id)->hp;
				if (hp > 0)
				{
					if (output != NULL)
					{
						*output = unit_id;
						output += 1;
					}
					living_count += 1;
				}
				remaining -= 1;
			} while (remaining != 0);
		}
	}
	if (side_mask & BATTLE_SIDE_ENEMIES)
	{
		remaining = 0x80;
		enemy_limit = enemy_capacity + 0x80;
		if (remaining < enemy_limit)
		{
			do
			{
				unit = UnitGetFar(remaining);
				if ((unit->side != 0) && (unit->hp > 0))
				{
					if (output != NULL)
					{
						*output = remaining;
						output += 1;
					}
					living_count += 1;
				}
				remaining += 1;
			} while (remaining < enemy_limit);
		}
	}
	if (output != NULL)
	{
		*output = BATTLE_UNIT_LIST_END;
	}
	return living_count;
}

s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
	struct BtlWork *order;
	s32				count;
	s32				i;

	order = BtlWk;
	count = 0;
	if (groups & BATTLE_SIDE_PARTY)
	{
		for (i = 0; order->party_units[i] != BATTLE_UNIT_LIST_END; i++)
		{
			if (order->party_units[i] != BATTLE_UNIT_REMOVED)
			{
				if (dst != NULL)
					*dst++ = order->party_units[i];
				count++;
			}
		}
	}
	if (groups & BATTLE_SIDE_ENEMIES)
	{
		for (i = 0; order->enm_no[i] != BATTLE_UNIT_LIST_END; i++)
		{
			if (order->enm_no[i] != BATTLE_UNIT_REMOVED)
			{
				if (dst != NULL)
					*dst++ = order->enm_no[i];
				count++;
			}
		}
	}
	if (dst != NULL)
		*dst = BATTLE_UNIT_LIST_END;
	return count;
}

//--------------------------------------------------------------------------
// actor_commit_placement.c
// バトル：全ユニットのオブジェクト配置
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 戦闘中の全ユニットの絵を用意し（いなくなったユニットの絵は解放）、
// 全員の表示モードと動作を設定し直す
//--------------------------------------------------------------------------
void BtlObjPut(void)
{
	s16 list[14];

	BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	BattleActor_SpawnObjectsForList(list, 1);
}

//--------------------------------------------------------------------------
// list_all_units_and_submit.c
// バトル：全ユニットのオブジェクト配置
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 戦闘中の全ユニットの絵を用意する（いなくなったユニットの絵は解放）
//--------------------------------------------------------------------------
void BattleParty_ListAllUnitsAndSubmit(void)
{
	s16 list[14];

	BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	BattleActor_SpawnObjectsForList(list, 0);
}

s32 BattleMotion_GetSlotPalette(s32 id)
{
	return BtlObjGet(id)->palette;
}

s32 EnmChk(s32 summon)
{
	struct BtlWork *ptr;
	s32				retval;
	s32				i;

	retval = Summon_IsEntryFlagged(summon);
	ptr = BtlWk;
	for (i = 0; i <= 5; i++)
	{
		if (ptr->sprite_slots[i] != 0)
			continue;
		if (retval != 0)
			break;
		if (i <= 4 && ptr->sprite_slots[i + 1] == 0)
			break;
	}
	return i != 6;
}

s32 SummonSlot_RegisterActorSprites(s32 unit)
{
	s32				pass;
	struct BtlWork *table = BtlWk;
	struct BtlUnit *actor = UnitGetFar(unit);
	s32				single_slot = Summon_IsEntryFlagged(actor->class_id);
	/* FAKEMATCH: initializing the two halves preserves the word copy into pass, mov r4, r10; one s32 zero folds that copy into movs r4, #0 (one instruction differs). */
	union
	{
		s32 value;
		u16 halves[2];
	} result;
	s32 sprite_value;

	result.halves[0] = 0;
	result.halves[1] = 0;
	sprite_value = Summon_GetEntryValue(actor->class_id);
	pass = result.value;
	do
	{
		s32 slot;

		if (actor->class_index != 0)
			continue;

		for (slot = 0; slot <= 5; slot++)
		{
			if (table->sprite_slots[slot] != 0)
				continue;
			if (single_slot)
				break;
			if (slot <= 4 && table->sprite_slots[slot + 1] == 0)
				break;
		}

		if (slot == 6)
			break;

		{
			s32 flag = Summon_GetEntryFlag1Field(actor->class_id);
			s32 buffer_addr = (slot << 14) + (s32)Ram_ActorSpriteSlots;

			if (ResourceSlot_LoadFar(slot, (u32 *)buffer_addr, sprite_value + pass, flag) == 0)
				return 0;
		}

		/* FAKEMATCH: cancelling updates retain sprite_value's allocation
		 * priority; a self-assignment instead differs in 18 rows. */
		sprite_value ^= 1;
		sprite_value ^= 1;
		if (pass == 0)
			result.value = (slot << 12) | sprite_value;

		table->sprite_slots[slot] = unit;
		if (!single_slot)
			table->sprite_slots[slot + 1] = unit;

		if (sprite_value != 476 && sprite_value != 483)
			break;
	} while (++pass <= 1);

	return result.value;
}

s32 BattleMotion_ReleaseObjectSlotByValue(s32 value)
{
	struct BtlWork *work = BtlWk;
	s32				index;

	for (index = 0; index <= 5; index++)
	{
		if (work->sprite_slots[index] == value)
		{
			ResourceSlot_LoadFar(index, 0, 0, 0);
			work->sprite_slots[index] = 0;
		}
	}
}

s32 Resource_FindFreeSlot(s32 key)
{
	struct SlotAssignmentView
	{
		volatile u16 entries[52];
	};
	s32 index;
	s32 entry;
	u32 value;
	s32 result;

	for (index = 0;; index++)
	{
		entry = Resource_SlotAssignments[index];
		if (key == (entry & 0x1ff))
		{
			/* FAKEMATCH: the volatile array member preserves the native second ldrh [table, index]. A scalar pointer needs an extra address add; a nonvolatile member reuses the first load. */
			value = ((struct SlotAssignmentView *)Resource_SlotAssignments)->entries[index];
			result = value >> 9;
			goto done;
		}
		if ((s16)entry == -1)
			break;
	}
	result = 6;
done:
	return result;
}

s32 BattleUnit_LookupWeaponValueByClass(s32 id)
{
	struct BtlUnit *state;
	s32				entry;
	s32				result;

	state = UnitGetFar(id);
	entry = Inventory_FindEquippedFar(id, 1);
	result = 0;
	if (entry >= 0)
	{
		s32 sel;

		sel = Resource_FindFreeSlot(state->inventory[entry] & 0x1FF);
		switch (state->class_id)
		{
		case ROBIN:
			result = BattleUnit_WeaponAnimsClass0[sel];
			break;
		case GERALD:
			result = BattleUnit_WeaponAnimsClass1[sel];
			break;
		case IWAN:
			result = BattleUnit_WeaponAnimsClass2[sel];
			break;
		case MEARI:
			result = BattleUnit_WeaponAnimsClass3[sel];
			break;
		case GARUSHIA:
			break;
		case JASMINE:
			result = BattleUnit_WeaponAnimsClass5[sel];
			break;
		}
	}
	return result;
}

void BtlObjMake(struct BtlObj *actor, s32 unit, s32 x, s32 y)
{
	s32					 fixed_x;
	s32					 fixed_y;
	s32					 actor_flag;
	s32					 sprite;
	s32					 existing_sprite;
	struct BtlUnit		*unit_record;
	struct MotionObject *object;
	s32					 position;
	s32					 anim;
	u8					 class_id;

	fixed_x = x << 16;
	fixed_y = y << 16;
	object = Object_CreateFar(0xf000, fixed_x, 0, fixed_y);
	unit_record = UnitGetFar(unit);
	actor_flag = 0;
	existing_sprite = SummonSlot_RegisterActorSprites(unit);

	if (unit_record->class_index == 0)
	{
		sprite = Summon_GetEntryValue(unit_record->class_id);
		if (existing_sprite == 0)
			actor_flag = Summon_GetEntryFlag1Field(unit_record->class_id);
		else
			sprite = existing_sprite;
	}
	else
	{
		switch (unit_record->class_id)
		{
		case GERALD:
			sprite = BTL_CHARA(GERALD);
			break;
		case MEARI:
			sprite = BTL_CHARA(MEARI);
			break;
		case IWAN:
			sprite = BTL_CHARA(IWAN);
			break;
		case JASMINE:
			sprite = BTL_CHARA(JASMINE);
			break;
		case ROBIN:
		default:
			sprite = BTL_CHARA(ROBIN);
			break;
		}
		if (unit < 0 || unit > 7)
			actor_flag = 1;
	}

	actor->scale = 0x10000;
	switch (unit_record->class_id)
	{
	case 78:
		actor->scale = 0x19999;
		break;
	case 89:
		actor->scale = 0x18ccc;
		break;
	case 130:
		actor->scale = 0x13333;
		break;
	case 131:
		actor->scale = 0x19999;
		break;
	case 138:
		actor->scale = 0x18000;
		break;
	case 147:
		actor->scale = 0x1cccc;
		break;
	case 149:
		actor->scale = 0x1cccc;
		break;
	case 29:
		actor->scale = 0x10000;
		break;
	case 121:
		actor->scale = 0x1b333;
		break;
	case 148:
		actor->scale = 0x18000;
		break;
	case 150:
		actor->scale = 0x18000;
		break;
	case 151:
		actor->scale = 0x18000;
		break;
	case 152:
		actor->scale = 0x18000;
		break;
	case 153:
		actor->scale = 0x18000;
		break;
	case 154:
		actor->scale = 0x18000;
		break;
	case 155:
		actor->scale = 0x18000;
		break;
	case 156:
		actor->scale = 0x18000;
		break;
	case 157:
		actor->scale = 0x18000;
		break;
	case 47:
		actor->scale = 0x13333;
		break;
	case 48:
		actor->scale = 0x13333;
		break;
	case 49:
		actor->scale = 0x16666;
		break;
	case 84:
		actor->scale = 0x10000;
		break;
	case 85:
		actor->scale = 0x14000;
		break;
	case 128:
		actor->scale = 0x16666;
		break;
	case 129:
		actor->scale = 0x16666;
		break;
	case 94:
		actor->scale = 0x18000;
		break;
	case 98:
		actor->scale = 0x14ccc;
		break;
	case 110:
		actor->scale = 0x13333;
		break;
	case 132:
		actor->scale = 0x10ccc;
		break;
	case 133:
		actor->scale = 0x10ccc;
		break;
	case 134:
		actor->scale = 0x11999;
		break;
	case 135:
		actor->scale = 0x11999;
		break;
	case 136:
		actor->scale = 0x13333;
		break;
	case 137:
		actor->scale = 0x13333;
		break;
	case 141:
		actor->scale = 0x18000;
		break;
	case 144:
		actor->scale = 0x13333;
		break;
	case 145:
		actor->scale = 0x18000;
		break;
	case 146:
		actor->scale = 0x18ccc;
		break;
	case 52:
		actor->scale = 0x14000;
		break;
	case 105:
		actor->scale = 0x14000;
		break;
	case 18:
	case 19:
	case 20:
	case 21:
	case 30:
	case 68:
	case 69:
	case 70:
	case 92:
	case 122:
	case 123:
	case 124:
	case 125:
	case 126:
		actor->scale = 0xe666;
		break;
	}

	actor->object = object;
	actor->anchor_x = fixed_x;
	actor->anchor_z = fixed_y;
	actor->palette = actor_flag;
	actor->resource = sprite;
	anim = BattleUnit_LookupWeaponValueByClass(unit);
	actor->animation = 0;
	actor->animation_entry = 0;
	actor->effect_entry = 0;
	actor->active = 0;
	actor->fading = 0;
	actor->effect = 0x1fe;
	class_id = unit_record->class_id;
	actor->overlay = anim;

	if (class_id <= GERALD && Inventory_GetEquippedItemFar(unit_record, 1) == 15)
	{
		if (unit_record->class_id == ROBIN)
		{
			sprite = 480;
			actor->resource = sprite;
		}
		else
		{
			sprite = 482;
			actor->resource = sprite;
		}
		actor->overlay = 0;
	}

	position = ArcTan2(y / 8, x) + 0x8000;
	object->angle = position;
	object->unknown_59 = 3;
	object->motion_flags = 2;
	if (unit_record->class_index == 0)
	{
		object->scale_x = 0x14ccc;
		object->scale_y = 0x14ccc;
	}
	else
	{
		object->scale_x = 0x10000;
		object->scale_y = 0x10000;
	}
	ObjectDispatch_InitializeFar((struct DispatchObject *)object, BattlePres_ActorObjectScript);
}

void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
	index *= 2;
	*x = BattlePlacement_StepPairs[index];
	*y = BattlePlacement_StepPairs[index + 1];
}

void Summon_LayoutPositions(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions)
{
	s32 total_spacing = count <= 4 ? 30 : 27;
	s32 z = ((count - 1) * total_spacing) / 2;
	s32 index;

	for (index = 0; index != count; index++)
	{
		s32 spacing = 0;

		x_positions[index] = -80;
		if (index != 0)
		{
			struct BtlUnit *actor;

			spacing = 25;
			if ((u16)(actor_ids[index] - 254) > 1)
			{
				actor = UnitGetFar(actor_ids[index]);
				spacing = Summon_IsEntryFlagged(actor->class_id) ? 27 : 38;
				if (actor->class_id == 148 || actor->class_id == 121)
				{
					x_positions[index] = -50;
				}
			}
		}
		z -= spacing / 2;
		z_positions[index] = z;
		spacing = 25;
		if ((u16)(actor_ids[index] - 254) > 1)
		{
			struct BtlUnit *actor = UnitGetFar(actor_ids[index]);

			spacing = Summon_IsEntryFlagged(actor->class_id) ? 27 : 38;
		}
		z -= spacing / 2;
	}
}

s32 EnmSlotGet(void)
{
	s32 i;
	s32 id;

	for (i = 0; i <= 5; i++)
	{
		id = i + 0x80;
		if (UnitGetFar(id)->side == 0)
			break;
	}
	if (i == 6)
		return -1;
	return id;
}

s32 EnmUpd(void)
{
	struct BtlWork *work = BtlWk;
	u16				unit_ids[14];
	s32				x[6];
	s32				z[6];
	s32				count;
	s32				i;

	for (i = 0; i < 6 && work->enm_no[i] != 0xff; i++)
		unit_ids[i] = work->enm_no[i];
	count = i;
	Summon_LayoutPositions(unit_ids, count, x, z);
	for (i = 0; i < count; i++)
	{
		s32 id = work->enm_no[i];

		if (id != 0xfe)
		{
			struct BtlObj *object = BtlObjGet(id);

			object->anchor_x = x[i] << 16;
			object->anchor_z = z[i] << 16;
		}
	}
}

void BattleUnit_RefreshPlacement(void)
{
	struct BtlWork *work;
	u16				ids[14];
	s32				x[6];
	s32				z[6];
	s32				count;
	s32				i;
	s32				place;
	s32				stand;
	s32				n;
	s32				value;
	u8			   *p;

	place = 0;
	stand = 0;
	work = BtlWk;
	count = BattleParty_PrepareActiveOwners(ids);
	value = 0xff;
	for (i = 13; i >= 0; i--)
		work->placement[i] = value;
	p = &work->placement[13];
	for (n = 5, value = 13; n >= 0; n--, value--)
		*p-- = value;
	for (i = 0; i < count; place++, i++)
	{
		s32 id = ids[i];

		work->placement[id] = place;
		BtlObjMake(BtlObjGet(id), id, BattlePlacement_StepPairs[place * 2],
			BattlePlacement_StepPairs[place * 2 + 1]);
	}
	for (i = 0; i < 6 && work->enm_no[i] != 0xff; i++)
		ids[i] = work->enm_no[i];
	count = i;
	Summon_LayoutPositions(ids, count, x, z);
	for (i = 0; i < count; i++, stand++)
	{
		s32 id = work->enm_no[i];

		if (id != 0xfe)
			BtlObjMake(BtlObjGet(id), id, x[stand], z[stand]);
	}
}

//--------------------------------------------------------------------------
// contains_id.c
// バトル：ユニット番号リストの検索
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 並び番号のユニットがリストにいれば 1 を返す
// list : ユニット番号のリスト（0xff で終わる、最大 14）
// no   : 並び番号（0-7 味方、8-13 敵）
//--------------------------------------------------------------------------
s32 BattlePlacement_ContainsId(s16 *list, s32 no)
{
	u32 i;

	if (no > 7)
		no += 120; // 敵のユニット番号は 128 から
	i = 0;
	do
	{
		u16 id = *list++;

		if (id == 0xff)
			break;
		if (id == no)
			return 1;
	} while (++i < 14);
	return 0;
}

//--------------------------------------------------------------------------
// battle.c
// バトル：キャラの表示の優先度、状態アイコン、ポーズ
//--------------------------------------------------------------------------

#define UNIT_MAX 14 // 敵味方を合わせた人数

//--------------------------------------------------------------------------
// 状態アイコンを進め、カメラの向きで味方と敵の表示の優先度を決める
//--------------------------------------------------------------------------
void BattleActor_UpdateSpritePriorities(void)
{
	u16						 ids[UNIT_MAX];
	s32						 pri[2]; // 味方, 敵
	s32						 i;
	s32						 j;
	s32						 count;
	struct MotionObject		*obj;
	struct AnimationObject	*rec;
	struct AnimationObject **list;

	// 全員の状態アイコン
	BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
	i = 0;
	if (ids[i] != BATTLE_UNIT_LIST_END)
	{
		for (;;)
		{
			struct BtlObj *slot = BtlObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				BattleStatusIcon_Cycle(slot);
				if (slot->effect_entry != 0)
				{
					rec = GetMotionRecord(obj, 0);

					if (rec != 0)
					{
						struct SpriteEntry *efx;
						s32					p = 0;

						if (obj->y != 0) // 高さがあれば
							p = 9;
						efx = slot->effect_entry;
						if (efx->priority != p)
						{
							efx->priority = p;
							rec->dirty = 1;
						}
					}
				}
			}
			i++;
			if (i >= UNIT_MAX || ids[i] == BATTLE_UNIT_LIST_END)
				break;
		}
	}

	// カメラの向きで、味方と敵の優先度を入れ替える
	if ((s16)gCameraWork->yaw >= 0)
	{
		pri[0] = 1;
		pri[1] = 2;
	}
	else
	{
		pri[0] = 2;
		pri[1] = 1;
	}

	// 味方
	{
		count = BattleParty_ListActorIds(BATTLE_SIDE_PARTY, ids);
		for (i = 0; i < count; i++)
		{
			struct BtlObj *slot = BtlObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				switch (obj->record_storage_kind & 15)
				{
				case 1: // 一枚
				{
					rec = obj->records;
					rec->part[0].priority = pri[0];
					break;
				}
				case 2: // 四枚までの表
				{
					list = (struct AnimationObject **)obj->records;
					for (j = 0; j < 4; j++)
					{
						rec = *list++;
						if (rec != 0)
							rec->part[0].priority = pri[0];
					}
					break;
				}
				}
			}
		}
	}

	// 敵
	{
		count = BattleParty_ListActorIds(BATTLE_SIDE_ENEMIES, ids);
		for (i = 0; i < count; i++)
		{
			struct BtlObj *slot = BtlObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				switch (obj->record_storage_kind & 15)
				{
				case 1:
				{
					rec = obj->records;
					rec->part[0].priority = pri[1];
					break;
				}
				case 2:
				{
					list = (struct AnimationObject **)obj->records;
					for (j = 0; j < 4; j++)
					{
						rec = *list++;
						if (rec != 0)
							rec->part[0].priority = pri[1];
					}
					break;
				}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------
// 状態アイコンのビット
//--------------------------------------------------------------------------
#define ICON_DOKU 0x01		// 毒
#define ICON_MOUDOKU 0x02	// もう毒
#define ICON_SHIBIRE 0x04	// しびれ
#define ICON_FUUJI 0x08		// エナジーふうじ
#define ICON_NEMURI 0x10	// ねむり
#define ICON_MABOROSHI 0x20 // まぼろし
#define ICON_AKURYO 0x40	// あくりょう
#define ICON_SHIRYO 6		// しりょう：残りの数 1-7 が bit 7-13

#define ICON_MAX 14 // アイコンの種類

//--------------------------------------------------------------------------
// 状態異常から状態アイコンのビットを作る
//--------------------------------------------------------------------------
s32 BattleUnit_BuildStatusFlags(s32 id, struct BtlObj *slot)
{
	struct BtlUnit *unit = UnitGetFar(id);
	s8				doku = unit->doku;
	u32				flags = 0;

	if (doku == 1)
		flags = ICON_DOKU;
	if (doku == 2)
		flags |= doku; // もう毒 (ICON_MOUDOKU)
	if (unit->maboroshi != 0)
		flags |= ICON_MABOROSHI;
	if (unit->shibire != 0)
	{
		s32 cls;
		flags |= ICON_SHIBIRE;
		cls = unit->class_id;
		if (cls == 121 || cls == 148) // この二体には出さない
			flags &= ~ICON_SHIBIRE;
	}
	if (unit->fuuji != 0)
		flags |= ICON_FUUJI;
	if (unit->akuryo != 0)
		flags |= ICON_AKURYO;
	if (unit->nemuri != 0)
		flags |= ICON_NEMURI;
	if (unit->shiryo != 0)
		flags |= 1 << (unit->shiryo + ICON_SHIRYO);
	slot->active_conditions = flags;
}

//--------------------------------------------------------------------------
// 状態アイコンを 80 フレームごとに次の状態へ切り替える
//--------------------------------------------------------------------------
s32 BattleStatusIcon_Cycle(struct BtlObj *slot)
{
	struct AnimationEntry  *old;
	struct AnimationEntry  *icon;
	struct AnimationObject *rec;
	struct MotionObject	   *owner;
	s32						anim;
	s32						handle;
	s32						prev;
	s32						changed = 0;

	if (slot->cycle_timer >= 0)
		slot->cycle_timer--;

	// 表示中の状態が消えたら、すぐに切り替える
	old = (struct AnimationEntry *)slot->animation_entry;
	if (old == 0)
	{
		if ((s16)slot->active_conditions == 0)
			goto wait;
		goto update;
	}
	if ((((s16)slot->active_conditions >> slot->selected_condition) & 1) == 0)
		goto update;
wait:
	if (slot->cycle_timer != 0)
		goto done;

update:
	anim = -1;
	owner = slot->object;
	if ((s16)slot->active_conditions != 0)
	{
		prev = slot->selected_condition;
		for (anim = prev + 1;; anim++)
		{
			if (anim > ICON_MAX - 1)
				anim = 0;
			if ((((s16)slot->active_conditions >> anim) & 1) != 0)
				break;
		}

		if (prev != anim || old == 0)
		{
			slot->selected_condition = anim;
			changed = 1;
		}
		slot->cycle_timer = 80;
	}
	else
	{
		changed = 1;
	}

	rec = GetMotionRecord(owner, 0);
	if (rec == 0)
		goto done;

	// アイコンのアニメ番号は、幅 32 のキャラと他とで別の並び
	if (anim >= 0)
	{
		if (rec->width == 32)
			anim += 340;
		else
			anim += 355;
	}

	if (slot->animation_entry != 0 && changed != 0)
	{
		ResourceMetadata_UnregisterFar(rec, (s32)slot->animation_entry);
		slot->animation_entry = 0;
	}

	if (anim >= 0 && changed != 0)
	{
		// The handle is stored first and cleared again when registration fails.
		handle = ResourceMetadata_RegisterFar(rec, anim);
		slot->animation_entry = (struct SpriteEntry *)handle;
		if (handle == -1)
			slot->animation_entry = 0;
		icon = (struct AnimationEntry *)slot->animation_entry;
		if (icon != 0)
		{
			icon->priority = 3;
			Animation_SetWorkEntryFar(icon, 0);
		}
	}

	rec->dirty = 1;
	if (anim >= 0)
		slot->animation = anim;
	else
		slot->animation = 0;

done:;
}

//--------------------------------------------------------------------------
// 状態に合わせてキャラのポーズを決める
// 動けなければ味方 4・敵 0、倒れていれば味方 4・敵 5、ほかは 1
//--------------------------------------------------------------------------
s32 BattlePres_SetActorModeAndAction(s32 id)
{
	struct BtlUnit *unit;
	s32				mode;

	unit = UnitGetFar(id);
	mode = 1;
	if (unit->hp != 0)
	{
		if (unit->nemuri != 0 || unit->shibire != 0 || unit->miugoki != 0)
			mode = (unit->side != BATTLE_UNIT_ENEMY) * 4;
	}
	else
	{
		s32 mikata = unit->side ^ mode;
		mode = (u32)(-mikata | mikata) >> 31;
		mode = 5 - mode;
	}

	ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(id)->object, mode);
	ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(id)->object, (id & 3) + 14);
}

//--------------------------------------------------------------------------
// release_battle_object_records.c
// バトル：オブジェクトの絵の解放
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// オブジェクトが持つ絵をすべて解放し、絵なしにもどす
// no : オブジェクト番号
//--------------------------------------------------------------------------
void ReleaseBattleObjectRecords(s32 no)
{
	struct BtlObj		*slot;
	struct MotionObject *obj;
	s32					 i;
	void				*rec;

	slot = BtlObjGet(no);
	if (slot != NULL)
	{
		obj = slot->object;
		if (obj != NULL)
		{
			slot->animation_entry = 0;
			slot->effect_entry = 0;
			i = 0;
			while ((rec = GetMotionRecord(obj, i)) != NULL)
			{
				ResourceObject_ReleaseFar(rec);
				i += 1;
			}
			obj->record_storage_kind = 0; // 絵なし
			obj->records = NULL;
		}
	}
}

void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh)
{
	s32						 i;
	s32						 id;
	struct BtlObj			*slot;
	struct MotionObject		*object;
	struct AnimationObject	*res;
	struct SpriteEntry		*entry;
	struct AnimationObject **objects;
	s32						 resource;
	s32						 object_id;

	for (i = 0; i <= 13; i++)
	{
		if (!BattlePlacement_ContainsId(list, i))
		{
			object_id = i + 120;
			if (i <= 7)
				object_id = i;
			ReleaseBattleObjectRecords(object_id);
		}
	}
	if (gSchedulerStatus == 0)
		WaitFrames(1);
	for (i = 0; i <= 13 && (id = list[i]) != 255; i++)
	{
		if (id == 254)
			continue;
		slot = BtlObjGet(id);
		if (slot == 0)
			continue;
		BattleUnit_BuildStatusFlags(id, slot);
		object = slot->object;
		if (object == 0)
			continue;
		if (object->record_storage_kind != 0)
			continue;
		if ((slot->resource & 0xfff) == 476 || (slot->resource & 0xfff) == 483)
		{
			objects = (struct AnimationObject **)gMenuCtrlWork->unknown_08 + gMenuCtrlWork->count;
			resource = slot->resource;
			object->record_storage_kind = 2;
			object->records = objects;
			Iwram_ClearWords(objects, 16);
			res = ResourceObject_CreateFar(resource);
			if (res != 0)
			{
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				object->height = Resource_GetMetadataRecordFar(resource)->box_y >> 1;
				*objects = res;
				objects++;
			}
			res->flags = 0;
			res = ResourceObject_CreateFar(resource + 0x2001);
			if (res != 0)
			{
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				*objects = res;
			}
			res->flags = 0;
		}
		else
		{
			res = ResourceObject_CreateFar(slot->resource);
			if (res != 0)
			{
				object->record_storage_kind = 1;
				object->records = res;
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				entry = (struct SpriteEntry *)res->entries[0];
				entry->priority = 1;
				entry->palette = slot->palette;
				if ((resource = slot->overlay) != 0)
				{
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					entry->priority = 1;
				}
				if ((resource = slot->animation) != 0)
				{
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					slot->animation_entry = entry;
					Animation_SetWorkEntryFar((struct AnimationEntry *)entry, 0);
					entry->priority = 3;
				}
				if ((resource = slot->effect) != 0)
				{
					if (res->width == 32)
						resource = 0x1ff;
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					slot->effect_entry = entry;
					entry->priority = 0;
					res->flags = 0;
				}
			}
		}
		BattlePres_SetActorModeAndAction(id);
	}
	if (refresh)
	{
		i = 0;
		if ((resource = list[0]) != 255)
		{
			for (;;)
			{
				if (list[i] != 254 && (slot = BtlObjGet(resource)) != 0
					&& (object = slot->object) != 0)
					BattlePres_SetActorModeAndAction(resource);
				i++;
				if (i >= 14 || (resource = list[i]) == 255)
					break;
			}
		}
	}
}

struct BtlObj *BtlObjGet(s32 object_id)
{
	struct BtlWork *work = BtlWk;
	struct BtlObj  *slots = work->slots;
	s32				index;
	if (object_id > 7)
	{
		object_id -= 0x78;
	}
	index = object_id;
	if (work->placement[index] == 0xFF)
	{
		return NULL;
	}
	return &slots[work->placement[index]];
}

void ResetMotionRecordGroup(void *owner)
{
	struct AnimationObject *object = owner;
	s32						i;

	if (object != NULL)
	{
		for (i = 0; i < 4; i++)
		{
			if (object->entries[i] != NULL)
				object->entries[i]->script = NULL;
		}
	}
}

void ResetBattleObjectRecordGroups(struct MotionObject *object)
{
	s32	   storage_kind;
	s32	   remaining;
	void **record_groups;
	void  *record_group;

	if (object != NULL)
	{
		storage_kind = object->record_storage_kind & 0xF;
		switch (storage_kind)
		{
		case 1:
			ResetMotionRecordGroup(object->records);
			return;
		case 2:
			record_groups = object->records;
			remaining = 3;
			do
			{
				record_group = *record_groups++;
				if (record_group != NULL)
				{
					ResetMotionRecordGroup(record_group);
				}
				remaining -= 1;
			} while (remaining >= 0);
			break;
		}
	}
}

s32 ActivateBattleObjectSlot(s32 object_id)
{
	struct BtlObj *slot;
	BattleMotion_ReleaseObjectSlotByValue(object_id);
	slot = BtlObjGet(object_id);
	slot->active = 1;
	return 0;
}

void *BattleMotion_DestroyAllSlotObjects(void)
{
	// 戻り値の型はポインタだが return 文は無い（pop {r1}; bx r1）
	s32			   no;
	s32			   i;
	struct BtlObj *slot;

	i = 0;
	do
	{
		no = i + 0x78;
		if (i <= 7)
		{
			no = i;
		}
		slot = BtlObjGet(no);
		if ((slot != NULL) && (slot->active != 0))
		{
			ObjectDispatch_ReleaseFar((s32)slot->object);
			slot->object = NULL;
			slot->active = 0;
		}
		i += 1;
	} while (i <= 0xD);
}

s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out)
{
	struct BtlWork *state = BtlWk;

	out->x = state->slots[no].anchor_x;
	out->y = 0;
	out->z = state->slots[no].anchor_z;
	return 0;
}

s32 Camera_ApplyTransformByFlag(void)
{
	struct BattleCamera *state = gCameraWork;
	Render_ResetTransformState();
	if (GameFlag_TestFar(0x16B) != 0)
	{
		Iwram_TransformMatrix((s32 *)Camera_FlagTransformWork);
		Graphics_PrepareTransferAndRun(state->eye, state->pos);
	}
	else
	{
		Graphics_PrepareTransferInIwramWork(state->eye, state->pos);
	}
}

s32 BattleMotion_ProjectPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						position[3];
	s32						scaled;

	Camera_ApplyTransformByFlag();
	position[0] = object->x;
	position[1] = object->y;
	position[2] = object->z;
	scaled = Render_ProjectPoint(position, projected);
	(void)Iwram_MulQ16(scaled, record->scale);
	return 0;
}

void *GetMotionRecord(struct MotionObject *object, s32 record_index)
{
	s32 storage_kind = object->record_storage_kind & 0xF;
	if (storage_kind == 1)
	{
		if (record_index == 0)
		{
			return object->records;
		}
	}
	else if (storage_kind == 2)
	{
		return ((void **)object->records)[record_index];
	}
	return NULL;
}

void Camera_InitDefaultTransform(void)
{
	struct BattleCamera		 *state = gCameraWork;
	struct FixedPointPosition transfer;

	state->yaw = 192 << 6;
	state->pitch = 254 << 8;
	state->distance = 255 << 17;
	state->pos[0] = 0;
	state->pos[1] = 0;
	state->pos[2] = 0;
	state->follow_pos = 0;
	state->unknown_18 = 0;

	Render_ResetTransformState();
	SceneTransform_ApplyPosition(&state->pos[0]);
	SceneTransform_ApplyYaw((s16)state->yaw);
	SceneTransform_ApplyPitch((s16)state->pitch);

	transfer.x = 0;
	transfer.y = 0;
	transfer.z = state->distance;
	Iwram_TransformVector((s32 *)&transfer, (s32 *)state);
}

void ObjMotRst(s32 slot_id)
{
	s32					 z;
	s32					 zero;
	struct BtlObj		*slot;
	struct MotionObject *object;

	slot = BtlObjGet(slot_id);
	object = slot->object;
	zero = 0;
	object->acceleration = 0x20000;
	object->speed_limit = 0x80000;
	object->vertical_motion_strength = 0xab85;
	object->velocity_y = zero;
	object->vertical_motion_phase = zero;
	object->auto_face_motion = zero;
	object->snap_to_target = 1;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, slot->anchor_x, 0, slot->anchor_z);
	z = slot->anchor_z;
	if (z < 0)
		z += 7;
	object->angle = ArcTan2(z >> 3, slot->anchor_x) - 0x8000;
}

void BattleMotion_SetupEscapeObject(s32 object_id)
{
	struct BtlObj		*slot;
	struct MotionObject *object;

	slot = BtlObjGet(object_id);
	object = slot->object;
	object->acceleration = 0x20000;
	object->speed_limit = 0x80000;
	object->velocity_y = 0x50000;
	object->vertical_motion_strength = 0x7851;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, slot->anchor_x * 3, 0, slot->anchor_z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

void BattleMotion_InterpolatePosition(
	struct BtlObj *start_slot, struct BtlObj *end_slot, s32 progress)
{
	s32					 z_step;
	s32					 start_x;
	s32					 x;
	s32					 start_z;
	s32					 end_z;
	struct MotionObject *start;
	struct MotionObject *end;

	start = start_slot->object;
	end = end_slot->object;
	start_x = start->x;
	x = start_x + progress * (end->x - start_x) / 100;
	end_z = end->z;
	start_z = start->z;
	z_step = progress * (end_z - start_z) / 100;
	REG_BLDCNT = 0;
	start->acceleration = 0x20000;
	start->speed_limit = 0x80000;
	start->velocity_y = 0x40000;
	start->vertical_motion_strength = 0xab85;
	start->vertical_motion_phase = 0;
	start->auto_face_motion = 1;
	Object_SetMoveTargetFar(start, x, 0, start_z + z_step);
	ObjectDispatch_ApplyArgumentToChildrenFar(start, 2);
}

void BattleMotion_SetObjectPosition(struct BtlObj *slot)
{
	struct MotionObject *object = slot->object;

	object->acceleration = 0x20000;
	object->speed_limit = 0x80000;
	object->vertical_motion_strength = 0xab85;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	Object_SetMoveTargetFar(object, slot->anchor_x, 0, slot->anchor_z);
}

void BattleMotion_ResetObjectAtScaledAnchor(s32 object_id)
{
	u32					 scaled_x;
	struct BtlObj		*slot;
	struct MotionObject *object;

	slot = BtlObjGet(object_id);
	object = slot->object;
	object->acceleration = 0x10000;
	object->speed_limit = 0x40000;
	object->velocity_y = 0x30000;
	object->vertical_motion_strength = 0x9999;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	scaled_x = slot->anchor_x * 3;
	Object_SetMoveTargetFar(object, (s32)(scaled_x + (scaled_x >> 0x1F)) >> 1, 0, slot->anchor_z);
}

void BattleMotion_InitializeObject(s32 id)
{
	struct BtlObj		*slot = BtlObjGet(id);
	struct MotionObject *object = slot->object;

	object->acceleration = 128 << 9;
	object->speed_limit = 128 << 11;
	object->velocity_y = 128 << 11;
	object->vertical_motion_strength = 0x9999;
	object->vertical_motion_phase = 0;
	object->auto_face_motion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, Iwram_MulQ16(slot->anchor_x, 0x14ccc), 0, slot->anchor_z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant)
{
	struct BtlObj		*slot;
	struct MotionObject *object;
	s32					 x;
	s32					 scale;

	slot = BtlObjGet(id);
	object = slot->object;
	if (UnitGetFar(id)->class_id != 0x94)
	{
		object->acceleration = BattleMotion_VariantAcceleration[variant];
		object->speed_limit = BattleMotion_VariantSpeedLimit[variant];
		if (object->y == 0 || variant > 4)
			object->velocity_y = BattleMotion_VariantVelocityY[variant];
		object->vertical_motion_strength = 0x9999;
		object->vertical_motion_phase = 0;
		object->auto_face_motion = 0;
		ObjectDispatch_InitFromTable6Far(object);
		scale = slot->anchor_x;
		x = (scale * BattleMotion_VariantDistancePercent[variant]) / 100;
		Object_SetMoveTargetFar(object, x, 0, slot->anchor_z);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

void BattleMotion_ApproachTarget(
	s32 actor_id, s32 target_id, s32 travel_divisor, s32 initial_velocity_y)
{
	struct BtlObj		*actor_slot = BtlObjGet(actor_id);
	struct BtlObj		*target_slot = BtlObjGet(target_id);
	struct MotionObject *object = actor_slot->object;
	struct MotionObject *target = target_slot->object;
	s32					 scale = 75;
	s32					 dx = target->x - object->x;
	s32					 start_x = object->x;
	s32					 step_x = scale * dx / 100;
	s32					 dz = target->z - object->z;
	s32					 start_z = object->z;
	s32					 step_z = scale * dz / 100;
	s32					 x = start_x + step_x;
	s32					 z = start_z + step_z;
	s32					 cell_x = step_x >> 8;
	s32					 cell_z = step_z >> 8;
	s32					 dist;

	dist = Iwram_Sqrt(cell_x * cell_x + cell_z * cell_z);
	dist = (dist << 8) / travel_divisor;
	object->acceleration = dist;
	object->speed_limit = dist;
	object->snap_to_target = 1;

	if (object->motion_flags & 4)
		object->velocity_y = initial_velocity_y;
	object->velocity_y = initial_velocity_y;
	object->vertical_motion_strength = 0xab85;
	object->auto_face_motion = 1;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, x, 0, z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
	struct MotionObject *object;

	object = BtlObjGet(id)->object;
	ObjectDispatch_InitFromTable6Far(object);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

void BattleMotion_ReservedNoOp83B0()
{
}

void BattleMotion_ApplyPairMidpoint(s32 first_id, s32 second_id)
{
	struct BtlObj			 *second_slot;
	struct MotionObject		 *first;
	struct MotionObject		 *second;
	s32						  x1;
	s32						  x2;
	s32						  z1;
	s32						  z2;
	struct FixedPointPosition pos;

	first = BtlObjGet(first_id)->object;
	second_slot = BtlObjGet(second_id);
	second = second_slot->object;

	x1 = first->target_x;
	if (x1 == 0x80000000)
	{
		x1 = first->x;
	}
	z1 = first->target_z;
	if (z1 == 0x80000000)
	{
		z1 = first->z;
	}
	x2 = second->target_x;
	if (x2 == 0x80000000)
	{
		x2 = second->x;
	}
	z2 = second->target_z;
	if (z2 == 0x80000000)
	{
		z2 = second->z;
	}

	pos.x = (x1 + x2) / 2;
	pos.y = 0;
	pos.z = (z1 + z2) / 2;

	BattleMotion_ReservedNoOp83B0(&pos, 0x1000);
}

void BattleMotion_SpawnSlotEffectAndWait(s32 id)
{
	struct MotionObject	  *object;
	struct AnimationEntry *effect;
	s32					   entry;

	object = BtlObjGet(id)->object;
	if ((object != NULL) && ((0xF & object->record_storage_kind) == 1))
	{
		entry = ResourceMetadata_RegisterFar(object->records, 0x11b);
		if (entry != 0)
		{
			effect = (struct AnimationEntry *)entry;
			Animation_SetWorkEntryFar(effect, 1);
			effect->priority = 3;
		}
		WaitFrames(0xA);
	}
}

s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						scaled;
	s32						factor;
	/* FAKEMATCH: needed to match: the input position BattleMotion_ProjectPosition fills, unused here */
	struct EffectPosition input;

	Camera_ApplyTransformByFlag();
	scaled = Render_ProjectPoint(&object->x, projected);
	factor = Iwram_MulQ16(scaled, record->scale);
	scaled = Iwram_MulQ16(factor, (s32)Battle_GetObjectTableValue(id) >> 16);
	projected[1] -= scaled;
	return 0;
}

s32 BattleMotion_ProjectConditionalPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BtlObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						scaled;
	s32						factor;

	Camera_ApplyTransformByFlag();
	scaled = Render_ProjectPoint(&object->x, projected);
	factor = Iwram_MulQ16(scaled, record->scale);
	if (Summon_IsEntryFlagged(UnitGetFar(id)->class_id) != 0)
		scaled = Iwram_MulQ16(factor, 24);
	else
		scaled = Iwram_MulQ16(factor, 48);
	projected[1] -= scaled;
	return 0;
}

s32 Battle_GetObjectTableValue(s32 id)
{
	u32 value;
	u8	no;

	no = UnitGetFar(id)->class_id;
	value = (u8)Summon_GetEntryByte4(no) << 16;
	if (value == 0)
	{
		no = UnitGetFar(id)->class_id;
		if (Summon_IsEntryFlagged(no) != 0)
		{
			value = 0x180000;
		}
		else
		{
			value = 0x300000;
		}
	}
	return value;
}

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
