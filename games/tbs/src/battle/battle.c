#include "vram_tab.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/object/object.h"
#include "unit/runtime/runtime.h"
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
#include "battle_system.h"
#include "text/msg_ids.h"
#include "mem.h"
#include "resource_ids.h"
#include "callback_scheduler.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "battle_display.h"
#include "battle_message.h"
#include "battle_party.h"
#include "battle_intro.h"
#include "inventory.h"
#include "dma.h"
#include "scene.h"
#include "battle_work.h"
#include "battle_type.h"
#include "battle_summon.h"
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
#include "battle_calc.h"
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
#include "battle_obj.h"
#include "battle_event.h"
#include "project.h"
#include "battle_random.h"
#include "irqlock.h"
#include "transform.h"
#include "battle_effect.h"
#include "character.h"
#include "io_write_queue.h"
#include "item.h"
#include "irq.h"
#include "vram_block.h"
#include "owner_value.h"
#include "sound_ids.h"
#include "ramcode.h"
#include "map_scroll.h"
#include "iowrite.h"
#include "battle_formation.h"
#include "b5_context.h"
#include "frame.h"
#include "unit/object/psynergy_effects.h"

struct DebugPartyEntry
{
	s8	owner;
	u8	level;
	s8	djinnCounts[4];
	u16 items[4];
	u16 actions[2];
	u16 unk12;
};

extern struct DebugPartyEntry gDebug_PartyPresets[];

s32 DebugParty_LoadPreset(s32 preset)
{
	s32				nextDjinn[4];
	struct BattleUnit *state;
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
		nextDjinn[i] = 0;

	for (row = 0;; row++)
	{
		owner = gDebug_PartyPresets[row].owner;
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
		Party_AdvanceOwnerCountToTargetFar(owner, gDebug_PartyPresets[row].level);
		state = (struct BattleUnit *)UnitGetFar(owner);
		for (i = 0; i < 4; i++)
		{
			state->djinnOwnedCounts[i] = 0;
			state->djinnActiveCounts[i] = 0;
			state->djinnAvailable[i] = 0;
			state->djinnActive[i] = 0;
		}
		for (i = 0; i < 32; i++)
			/* FAKEMATCH: the halfword store through a u16 cast loads its zero from the literal pool, as the reference does. 2026-10-13: the field store state->actionSlots[i].encodedAction = 0, with or without a (u16) cast on the zero, differs 23 of 210: movs r3, #0 takes the place of the ldr of the zero and the literal pool moves to the function end; any store through a dereference (this cast, *&...encodedAction, ((u16 *)state->actionSlots)[i * 2]) matches. */
			*(u16 *)&state->actionSlots[i] = 0;
		for (i = 0; i < 2; i++)
		{
			if (gDebug_PartyPresets[row].actions[i] != 0)
				OwnerAction_AddFar(owner, gDebug_PartyPresets[row].actions[i]);
		}
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < gDebug_PartyPresets[row].djinnCounts[i]; j++)
			{
				Djinn_AddToOwnerFar(owner, i, nextDjinn[i]);
				Djinn_ActivateFar(owner, i, nextDjinn[i]);
				nextDjinn[i]++;
			}
		}
		for (i = 0; i < 16; i++)
			Inventory_RemoveFar(owner, 0);
		for (i = 0; i < 4; i++)
		{
			if (gDebug_PartyPresets[row].items[i] != 0)
				Inventory_EquipFar(
					owner, Inventory_AddItemFar(owner, gDebug_PartyPresets[row].items[i]));
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
#define DEBUG_SECOND_FORM MsgBecameCursed
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define DEBUG_SECOND_FORM MsgEnd
#else
#define DEBUG_SECOND_FORM MsgDebugSecondForm
#endif

#define DEBUG_VIEW_LIMIT \
	(MsgLaliveroHowDidSearchForSheba - MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely + 5)

void DebugBattle_ViewMessages(void)
{
	u16				text[64];
	s32				second = 0;
	struct BattleUnit *unit = UnitGetFar(0);
	s32				i;
	s32				index;
	s32				window;
	s32				message;

	Ui_AdjustValueWithoutLimitFar(MsgDebugSampleName, text);
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
		gGameState.messageSpeed = 2;
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
				message = MsgBabiLighthouseTheyWereTerribleWeWereAbsolutely;
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
			gGameState.special = (s32)&ResourceId_SuhallaGateBackdrop;
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
	struct BattleWork			 *work = slot[HEAP_SLOT_BATTLE - HEAP_SLOT_CAMERA];
	struct FixedPointPosition local;
	s32						 *pos;
	s16						  delta;
	u32						  id;

	if (work->twoSided != 0)
	{
		if ((gLinkStatus & 3) != 3)
		{
			work->linkMisses++;
			if (work->linkMisses > 24)
			{
				work->linkPaused = 1;
			}
		}
		else
		{
			id = (REG_SIOCNT << 0x1A) >> 0x1E;
			if (work->linkSide != id)
			{
				work->linkPaused = 1;
			}
			work->linkMisses = 0;
		}
	}

	if (transition->frames != 0)
	{
		delta = transition->targetYaw - state->yaw;
		delta /= 16;
		state->yaw += delta;
		transition->frames--;
	}

	pos = state->pos;
	if (state->followPos != 0)
	{
		pos = state->followPos;
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

void BattleIntro_AnnounceEncounter(s32 enemyCount)
{
	s16				enemies[8];
	struct BattleWork *battle;
	s16			   *enemy;
	s32				announced;

	battle = gBattleWork;
	UiWork_ClearValueNameTablesFar();
	BattleEnemyList(enemies);

	announced = 0;
	if (enemyCount != 0)
	{
		enemy = enemies;
		do
		{
			UiWork_PushValueSlotFar((u16)*enemy++, 1);
			if (announced == enemyCount - 1)
				UiText_ShowMessageAndWaitCoreFar(MsgLastEnemyAppeared);
			else
				UiText_ShowMessageAndWaitCoreFar(MsgEnemyAppeared);
			announced++;
			BattlePresentation_WaitForAdvance();
		} while (announced != enemyCount);
	}

	UiWork_FinalizeSharedSlotFar();
	if (battle->encounterMode == BATTLE_ENCOUNTER_PARTY_FIRST)
	{
		UiWork_ClearValueNameTablesFar();
		UiWork_PushValueSlotFar(0, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgPartyStrikesFirst);
		BattlePresentation_WaitForAdvance();
	}
	else if (battle->encounterMode == BATTLE_ENCOUNTER_ENEMIES_FIRST)
	{
		UiWork_ClearValueNameTablesFar();
		UiWork_PushValueSlotFar(0, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgPartySurprised);
		BattlePresentation_WaitForAdvance();
	}
}

void BattleParty_CollectUnitList(void)
{
	u16				buf[14];
	struct BattleWork *battle;
	s32				count;
	s32				i;
	s32				index;
	s32				kind;

	battle = gBattleWork;
	count = BattleParty_PrepareActiveOwners(buf);
	for (i = 0; i < count; i++)
		battle->partyUnits[i] = buf[i];
	battle->partyUnits[count] = 0xff;

	count = BattleEnemyList(buf);
	kind = battle->battleType;
	if (kind >= 0)
	{
		if (kind <= 1)
		{
			for (i = 0; i < count; i++)
				battle->enemyIds[i] = buf[i];
			goto done;
		}
	}
	for (i = 0; i < count; i++)
	{
		index = gBattleParty_CenterOrderOffsets[i] + count / 2;
		battle->enemyIds[index] = buf[i];
	}
done:
	battle->enemyIds[count] = 0xff;
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
		struct BattleUnit *unit;
		u8			   *cursor;
		s32				remaining;

		do
		{
			unit = UnitGetFar(owners[i]);
			cursor = (u8 *)&unit->elementLevels[3];
			remaining = 3;

			do
			{
				remaining--;
				*cursor-- = 0;
			} while (remaining >= 0);

			unit->atkCnt = 0;
			unit->atkLv = 0;
			unit->defCnt = 0;
			unit->defLv = 0;
			unit->resCnt = 0;
			unit->resLv = 0;
			unit->delusion = 0;
			unit->confusion = 0;
			unit->charm = 0;
			unit->stun = 0;
			unit->sleep = 0;
			unit->psySeal = 0;
			unit->refrain = 0;
			unit->reflect = 0;
			unit->grimReaper = 0;
			unit->unk142[0] = 0;
			unit->unk142[1] = 0;
			unit->ready = 0;
			unit->immobile = 0;
			unit->agiCnt = 0;
			unit->agiLv = 0;
			unit->challenge = 0;

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
			s32 permanentTimer = -1;

			entry = list->entries;
			do
			{
				if (entry->turns == permanentTimer && BattleObjGet(entry->unitId) == 0)
				{
					u8 id = entry->unitId;
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
	u16 partyMembers[10];
	s32 partySize;
	s32 element;
	u32 availableMask;

	partySize = BattleParty_PrepareActiveOwners(partyMembers);
	availableMask = 0;

	element = 0;
	do
	{
		totals[element] = 0;
		{
			s32 partySlot;

			for (partySlot = 0; partySlot < partySize; partySlot++)
			{
				struct BattleUnit *member = UnitGetFar(partyMembers[partySlot]);

				totals[element] += member->djinnOwnedCounts[element];
			}
		}
		element++;
	} while (element <= 3);

	element = 0;
	do
	{
		const struct SummonDefinition *summon = SummonDefinition_Get(element);
		s32							   elementsMet;

		if (summon != 0)
		{
			const u8 *required = summon->djinnRequired;

			elementsMet = 0;
			if (totals[0] >= required[0])
			{
				u8 *total = totals;

				do
				{
					elementsMet++;
					if (elementsMet > 3)
						break;
					total++;
					required++;
				} while (*total >= *required);
			}

			if (elementsMet == 4)
				availableMask |= 1u << element;
		}
		element++;
	} while (element <= 31);

	{
		struct BattleSummonState *state = Trade_GetOfferStateFar(0);
		state->availableMask = availableMask;
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
	struct BattleUnit *name_entry;
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
		Ui_AdjustValueWithoutLimitFar(MsgEnemyLabel, name_text);
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
	struct BattleWork			 *work;
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
	work = gBattleWork;
	for (i = 7; i >= 0; i--)
		work->ownerSlots[i] = mark;
	count = BattleParty_PrepareActiveOwners(owners);
	for (i = 0; i < count; i++)
	{
		Iwram_CopyWords(buffer, UnitGetFar(owners[i]), size);
		((struct BattleUnit *)buffer)->side = 2;
		work->ownerSlots[owners[i]] = i - 128;
		if (SerialRuntime_BeginTransferA(buffer, LINK_PARTY_UNIT_BYTES) == -1)
			break;
		SerialRuntime_WaitForTransferA();
		WaitFrames(2);
	}
	for (; i <= 2; i++)
	{
		((struct BattleUnit *)buffer)->side = 0;
		if (SerialRuntime_BeginTransferA(buffer, LINK_PARTY_UNIT_BYTES) == -1)
			break;
		SerialRuntime_WaitForTransferA();
		WaitFrames(2);
	}
	size = LINK_PARTY_OFFER_BYTES;
	MemFree(buffer);
	buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
	/* FAKEMATCH: a one-pass loop is a sched2 barrier, so the list pointer's copy is emitted before the count's address as in the reference. 2026-10-13: the plain call differs 2 of 152 (adds r4, r6, #0 is scheduled after the count's load); list set before the copy differs 6; list as buffer + 8, list set in the for's initializer, and the offer state read into a local first differ the same 2. */
	do
	{
		Iwram_CopyWords(buffer, Trade_GetOfferStateFar(0), size);
	} while (0);
	list = &((struct DjinnRecoveryTable *)buffer)->list;
	for (j = 0; j < list->count; j++)
		list->entries[j].unitId = work->ownerSlots[list->entries[j].unitId];
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
	struct BattleWork *work = gBattleWork;
	u16			   *peer;
	u16			   *sig;
	s32				lost = 0;
	s32				i;

	if (work->twoSided != 0)
	{
		u32 other = work->linkSide ^ 1;

		peer = (u16 *)(gSerialPeerPayloads[other] + 4);
		sig = (u16 *)gSerialTransfer.payload;
		if (work->linkPaused == 0)
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
	struct BattleWork *work = gBattleWork;
	u16			   *peer;
	u16			   *sig;
	s32				lost = 0;
	/* FAKEMATCH: never used: the reference's frame keeps twenty bytes that no instruction touches; without it 20 of 232 differ (no sub sp, #20, and the branches and literal offsets shift). */
	u16 spare[10];

	if (work->twoSided != 0)
	{
		peer = gLinkPeerSignatures[work->linkSide ^ 1];
		sig = (u16 *)gSerialTransfer.payload;
		if (work->linkPaused != 0)
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
	u16				activeMembers[8];
	struct BattleWork *battleState = gBattleWork;
	s32				partySize = BattleParty_PrepareActiveOwners(activeMembers);
	s32				memberSlot;
	s32				unitId;

	for (memberSlot = 0; memberSlot < partySize; memberSlot++)
	{
		unitId = activeMembers[memberSlot];
		battleState->ownerSlots[unitId] = memberSlot - 128;
	}
}

s32 Battle_ClearSerialTransferPayload(void)
{
	return Iwram_ClearWords(gSerialTransfer.payload, 16);
}

/* FAKEMATCH: the one-pass block keeps the seed store in place in Battle_RunEncounter: the reference stores the zero held in r5 and only then sets wait's zero in r5. 2026-10-13: as a plain assignment 4 of 645 differ (the seed's address and wait's zero are scheduled around the store); the store in the for's initializer, after a comma or chained with wait, differs the same 4; wait = 0 set before it 18. */
#define BattleRandom_ClearSeed() \
	do                           \
	{                            \
		gBattleRandomSeed = 0;   \
	} while (0)

s32 Battle_RunEncounter(s32 enemyGroup)
{
	struct BattleCamera					*camera;
	s32									*target;
	struct BattleWork						*battle;
	u8									*ownerStates;
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
	battle = Runtime_AllocateBlock(HEAP_SLOT_BATTLE, sizeof(struct BattleWork));
	ownerStates = Runtime_AllocateBlock(54, 0x7c8);
	transition = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_BACKGROUND, 32);
	Runtime_AllocateBlock(11, 0x280);
	target = camera->pos;
	/* FAKEMATCH: needed to match: result starts at 0 and is never read before each exit sets it; CSE takes the setup zeros from its register, r7 as listed (without it 262). 2026-10-13: initialised at its declaration 130; set after Scheduler_ResetTaskTable 262; the zero in handle 20 (setup matches, handle and result swap r5/r7); in wait 120, pos 113, actor 121, i 323, count 363. */
	result = 0;
	Iwram_ClearWords(ownerStates, 0x7c8);
	Scheduler_ResetTaskTable();
	transition->frames = 0;
	transition->targetYaw = 0x2000;
	transition->flag = 1;
	transition->unk18 = 0;
	transition->unk1c = 0;
	REG_DISPCNT = 1;
	GameFlag_SetBitFar(0x103);
	GameFlag_SetBitFar(0x169);
	Render_ResetTransformState();

	Dma_Fill32Cell(fill, 0, camera, sizeof(struct BattleCamera));
	Dma_Fill32Cell(fill, 0, battle, sizeof(struct BattleWork));
	battle->promptSlot = -1;
	battle->enemyGroup = enemyGroup;
	display = Runtime_AllocateBlock(37, sizeof(struct BattleDisplayWork));
	Dma_Fill32Cell(fill, 0, display, sizeof(struct BattleDisplayWork));
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
	object = BattleFormation_BuildEnemyList(battle->enemyGroup);

	if (GameFlag_TestFar(FLAG_LINK_BATTLE) != 0)
	{
		battle->twoSided = 1;
		Battle_SetStart(4);
	}
	if (battle->twoSided != 0)
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
				battle->linkPaused = 1;
				break;
			}
		}
		battle->linkSide = (REG_SIOCNT << 26) >> 30;
		dest = gBattleOwnerStates;
		for (pos = 0; pos <= 0x7c7; pos++)
			*dest++ = gActorSpriteSlots[pos];
		object = GameFlag_GetByteFar(0x3f0);
		BattleParty_AssignMemberSlots();
		battle->battleType = 0;
	}
	Scheduler_AddOrUpdateCallback(BattlePresentation_UpdateCamera, 0xc7f);

	cue = gGameState.sceneCue;
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
	if (((struct BattleSummonState *)Trade_GetOfferStateFar(0))->availableMask != 0)
		battle->partyStatus = 3;
	else
		battle->partyStatus = 1;
	UiWindow_CreateWithLayoutBoundsFar(9);
	Camera_InitDefaultTransform();
	BattleObjPut();
	BattlePresentation_InitializeWorkAndResetState();
	BattleBackground_Load(1, battle->background, 0);
	BattleCamera_SetRange(0xa00000, 0x500000, 0, 0, 0x20000);
	BattlePres_SetupTransitionScene(0, 0, 0, 190);
	Party_ReservedNoOp5B14(1);
	REG_BLDCNT = 0;
	Summon_ClearWorkFields();
	battle->promptSlot = Resource_LoadIntoFreeSlot(128);
	battle->encounterMode = 0;
	if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) != 0)
	{
		battle->encounterMode = 1;
	}
	else if (gGameState.battleStart == 0)
	{
		if ((BattleRandomFar() & 15) == 0)
			battle->encounterMode = 1;
		else if ((BattleRandomFar() & 31) == 0)
			battle->encounterMode = 2;
	}
	BattlePres_RunEntryTransition(object, enemyGroup);
	transition->flag = 0;
	gCpuLoadDisplayEnabled = 0;
	Scheduler_AddOrUpdateCallback(BattleActor_UpdateSpritePriorities, 0xc80);

	for (;;)
	{
		Battle_ReservedNoOp9B2C();
		BattleSummon_UpdateAvailability();
		if (((struct BattleSummonState *)Trade_GetOfferStateFar(0))->availableMask != 0)
			battle->partyStatus = 3;
		else
			battle->partyStatus = 1;
		transition->targetYaw = 0x2800;
		transition->frames = 60;
		PartyStatusDrawFar(battle->partyStatus);
		Iwram_ClearWords(battle->actions, 0x140);
		Resource_ResetEntry(battle->promptSlot);
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
		battle->promptSlot = Resource_LoadIntoFreeSlot(128);
		PartyStatusDrawFar(battle->partyStatus);
		if (count < 0)
			goto aborted;

		for (i = 0; i < count; i++)
		{
			actor = battle->actions[i].unitId;
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
					battle->defeatState = 3;
				goto resolved;
			}
			if (BattlePres_SyncTurn() < 0)
				goto aborted;
		}

		battle->encounterMode = 0;
		Battle_ReservedNoOpF674();
		Battle_ProcessRoundEnd();
		BattleMotion_DestroyAllSlotObjects();
		if (battle->twoSided != 0)
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

		handle = UiText_OpenMessageWindowFar(MsgTutorialUnleashed, 0, 4, 1);
		while (UiWork_IsCompleteFar() == 0)
			WaitFrames(1);
		UiWork_FinalizeFar(handle, 1);
		WaitFrames(1);
		handle = UiText_OpenMessageWindowFar(MsgTutorialStandby, 10, 4, 1);
		BattlePresentation_WaitForPromptAt(92, 24);
		UiWork_FinalizeFar(handle, 1);
		WaitFrames(1);
	}

resolved:
	Battle_ClearSerialTransferPayload();
	if (GameFlag_TestFar(FLAG_DJINN_TUTORIAL) == 0)
	{
		if (battle->twoSided != 0)
			AudioCommand_PlayFar(58);
		if (battle->spoils.defeated != 0)
		{
			AudioCommand_PlayFar(58);
			if (battle->defeatState <= 1)
			{
				BattleUnitSetFar(128, battle->enemyClasses[battle->firstDefeated], 26);
				UiWork_ClearValueNameTablesFar();
				UiWork_PushValueSlotFar(128, 1);
				UiText_ShowMessageAndWaitCoreFar(battle->defeatState + MsgDowned);
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
	UiWork_PushValueSlotFar(gGameState.activeOwners[0], 1);
	if (BattleParty_PrepareActiveOwners(0) == 1)
		UiText_ShowMessageAndWaitCoreFar(MsgGoesDownInDefeat);
	else
		UiText_ShowMessageAndWaitCoreFar(MsgPartyDefeated);
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
	if (gBattleWork->twoSided != 0)
		limit = 3;

	count = Party_CountActiveOwnersFar();
	if (count > limit)
		count = limit;

	for (index = 0; index < count; index++)
	{
		s32 owner = gGameState.activeOwners[index];

		if (owners != 0)
			*owners++ = owner;
		UnitGetFar(owner)->side = BATTLE_UNIT_PARTY;
	}

	if (owners != 0)
		*owners = BATTLE_UNIT_LIST_END;
	return count;
}

s32 BattleEnemyList(s16 *unitIds)
{
	s16 *output;
	s32	 battleResult;
	s32	 id;
	s32	 entryLimit;
	s32	 entryCount;

	output = unitIds;
	entryCount = 0;
	entryLimit = 6;
	battleResult = 0;
	if (output != NULL)
	{
		if (GameFlag_TestFar(0x16C) != 0)
		{
			entryLimit = 3;
		}
		id = 0x80;
		entryLimit += 0x80;
		for (; id < entryLimit; id += 1)
		{
			if (UnitGetFar(id)->side != 0)
			{
				*output = id;
				entryCount += 1;
				output += 1;
			}
		}
		*output = BATTLE_UNIT_LIST_END;
		battleResult = entryCount;
	}
	return battleResult;
}

s32 BattleParty_ListLivingUnits(s32 sideMask, u16 *unitIds)
{
	u16				activeMembers[8];
	u16			   *base;
	s32				remaining;
	s32				enemyLimit;
	u16			   *output;
	s32				livingCount;
	s32				enemyCapacity;
	u16			   *member;
	s32				unitId;
	s16				hp;
	s32				activeCount;
	struct BattleUnit *unit;

	output = unitIds;
	livingCount = 0;
	enemyCapacity = 6;
	if (GameFlag_TestFar(0x16C) != 0)
	{
		enemyCapacity = 3;
	}
	if (sideMask & BATTLE_SIDE_PARTY)
	{
		base = activeMembers;
		activeCount = BattleParty_PrepareActiveOwners(base);
		if (livingCount < activeCount)
		{
			member = base;
			remaining = activeCount;
			do
			{
				unitId = *member;
				member += 1;
				hp = UnitGetFar(unitId)->hp;
				if (hp > 0)
				{
					if (output != NULL)
					{
						*output = unitId;
						output += 1;
					}
					livingCount += 1;
				}
				remaining -= 1;
			} while (remaining != 0);
		}
	}
	if (sideMask & BATTLE_SIDE_ENEMIES)
	{
		remaining = 0x80;
		enemyLimit = enemyCapacity + 0x80;
		if (remaining < enemyLimit)
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
					livingCount += 1;
				}
				remaining += 1;
			} while (remaining < enemyLimit);
		}
	}
	if (output != NULL)
	{
		*output = BATTLE_UNIT_LIST_END;
	}
	return livingCount;
}

s32 BattleParty_ListActorIds(s32 groups, u16 *dst)
{
	struct BattleWork *order;
	s32				count;
	s32				i;

	order = gBattleWork;
	count = 0;
	if (groups & BATTLE_SIDE_PARTY)
	{
		for (i = 0; order->partyUnits[i] != BATTLE_UNIT_LIST_END; i++)
		{
			if (order->partyUnits[i] != BATTLE_UNIT_REMOVED)
			{
				if (dst != NULL)
					*dst++ = order->partyUnits[i];
				count++;
			}
		}
	}
	if (groups & BATTLE_SIDE_ENEMIES)
	{
		for (i = 0; order->enemyIds[i] != BATTLE_UNIT_LIST_END; i++)
		{
			if (order->enemyIds[i] != BATTLE_UNIT_REMOVED)
			{
				if (dst != NULL)
					*dst++ = order->enemyIds[i];
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
void BattleObjPut(void)
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
	return BattleObjGet(id)->palette;
}

s32 EnemyCheck(s32 summon)
{
	struct BattleWork *ptr;
	s32				retval;
	s32				i;

	retval = Summon_IsEntryFlagged(summon);
	ptr = gBattleWork;
	for (i = 0; i <= 5; i++)
	{
		if (ptr->spriteSlots[i] != 0)
			continue;
		if (retval != 0)
			break;
		if (i <= 4 && ptr->spriteSlots[i + 1] == 0)
			break;
	}
	return i != 6;
}

s32 SummonSlot_RegisterActorSprites(s32 unit)
{
	s32				pass;
	struct BattleWork *table = gBattleWork;
	struct BattleUnit *actor = UnitGetFar(unit);
	s32				singleSlot = Summon_IsEntryFlagged(actor->classId);
	/* FAKEMATCH: initializing the two halves preserves the word copy into pass, mov r4, r10; one s32 zero folds that copy into movs r4, #0 (one instruction differs). 2026-10-13: result as an s32 set to 0, with pass = result or pass = 0, differs 2 of 114; pass = result = 0 differs 6, result = pass = 0 19. */
	union
	{
		s32 value;
		u16 halves[2];
	} result;
	s32 spriteValue;

	result.halves[0] = 0;
	result.halves[1] = 0;
	spriteValue = Summon_GetEntryValue(actor->classId);
	pass = result.value;
	do
	{
		s32 slot;

		if (actor->classIndex != 0)
			continue;

		for (slot = 0; slot <= 5; slot++)
		{
			if (table->spriteSlots[slot] != 0)
				continue;
			if (singleSlot)
				break;
			if (slot <= 4 && table->spriteSlots[slot + 1] == 0)
				break;
		}

		if (slot == 6)
			break;

		{
			s32 flag = Summon_GetEntryFlag1Field(actor->classId);
			s32 bufferAddr = (slot << 14) + (s32)Ram_ActorSpriteSlots;

			if (ResourceSlot_LoadFar(slot, (u32 *)bufferAddr, spriteValue + pass, flag) == 0)
				return 0;
		}

		/* FAKEMATCH: cancelling updates retain spriteValue's allocation
		 * priority; a self-assignment instead differs in 18 rows.
		 * 2026-10-13: without them 18 of 114 differ; the exit test written
		 * !(== || ==) or as a switch on spriteValue 18; the load's
		 * value as pass + spriteValue 16. */
		spriteValue ^= 1;
		spriteValue ^= 1;
		if (pass == 0)
			result.value = (slot << 12) | spriteValue;

		table->spriteSlots[slot] = unit;
		if (!singleSlot)
			table->spriteSlots[slot + 1] = unit;

		if (spriteValue != 476 && spriteValue != 483)
			break;
	} while (++pass <= 1);

	return result.value;
}

s32 BattleMotion_ReleaseObjectSlotByValue(s32 value)
{
	struct BattleWork *work = gBattleWork;
	s32				index;

	for (index = 0; index <= 5; index++)
	{
		if (work->spriteSlots[index] == value)
		{
			ResourceSlot_LoadFar(index, 0, 0, 0);
			work->spriteSlots[index] = 0;
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
		entry = gResource_SlotAssignments[index];
		if (key == (entry & 0x1ff))
		{
			/* FAKEMATCH: the volatile array member preserves the native second ldrh [table, index]. A scalar pointer needs an extra address add; a nonvolatile member reuses the first load. */
			value = ((struct SlotAssignmentView *)gResource_SlotAssignments)->entries[index];
			result = value >> 9;
			break;
		}
		if ((s16)entry == -1)
		{
			result = 6;
			break;
		}
	}
	return result;
}

s32 BattleUnit_LookupWeaponValueByClass(s32 id)
{
	struct BattleUnit *state;
	s32				entry;
	s32				result;

	state = UnitGetFar(id);
	entry = Inventory_FindEquippedFar(id, 1);
	result = 0;
	if (entry >= 0)
	{
		s32 selection;

		selection = Resource_FindFreeSlot(state->inventory[entry] & 0x1FF);
		switch (state->classId)
		{
		case ISAAC:
			result = gBattleUnit_WeaponAnimsClass0[selection];
			break;
		case GARET:
			result = gBattleUnit_WeaponAnimsClass1[selection];
			break;
		case IVAN:
			result = gBattleUnit_WeaponAnimsClass2[selection];
			break;
		case MIA:
			result = gBattleUnit_WeaponAnimsClass3[selection];
			break;
		case FELIX:
			break;
		case JENNA:
			result = gBattleUnit_WeaponAnimsClass5[selection];
			break;
		}
	}
	return result;
}

void BattleObjMake(struct BattleObj *actor, s32 unit, s32 x, s32 y)
{
	s32					 fixedX;
	s32					 fixedY;
	s32					 actorFlag;
	s32					 sprite;
	s32					 existingSprite;
	struct BattleUnit		*unitRecord;
	struct MotionObject *object;
	s32					 position;
	s32					 anim;
	u8					 classId;

	fixedX = x << 16;
	fixedY = y << 16;
	object = Object_CreateFar(0xf000, fixedX, 0, fixedY);
	unitRecord = UnitGetFar(unit);
	actorFlag = 0;
	existingSprite = SummonSlot_RegisterActorSprites(unit);

	if (unitRecord->classIndex == 0)
	{
		sprite = Summon_GetEntryValue(unitRecord->classId);
		if (existingSprite == 0)
			actorFlag = Summon_GetEntryFlag1Field(unitRecord->classId);
		else
			sprite = existingSprite;
	}
	else
	{
		switch (unitRecord->classId)
		{
		case GARET:
			sprite = BATTLE_CHARACTER(GARET);
			break;
		case MIA:
			sprite = BATTLE_CHARACTER(MIA);
			break;
		case IVAN:
			sprite = BATTLE_CHARACTER(IVAN);
			break;
		case JENNA:
			sprite = BATTLE_CHARACTER(JENNA);
			break;
		case ISAAC:
		default:
			sprite = BATTLE_CHARACTER(ISAAC);
			break;
		}
		if (unit < 0 || unit > 7)
			actorFlag = 1;
	}

	actor->scale = 0x10000;
	switch (unitRecord->classId)
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
	actor->anchorX = fixedX;
	actor->anchorZ = fixedY;
	actor->palette = actorFlag;
	actor->resource = sprite;
	anim = BattleUnit_LookupWeaponValueByClass(unit);
	actor->animation = 0;
	actor->animationEntry = 0;
	actor->effectEntry = 0;
	actor->active = 0;
	actor->fading = 0;
	actor->effect = 0x1fe;
	classId = unitRecord->classId;
	actor->overlay = anim;

	if (classId <= GARET && Inventory_GetEquippedItemFar(unitRecord, 1) == 15)
	{
		if (unitRecord->classId == ISAAC)
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
	object->unk59 = 3;
	object->motionFlags = 2;
	if (unitRecord->classIndex == 0)
	{
		object->scaleX = 0x14ccc;
		object->scaleY = 0x14ccc;
	}
	else
	{
		object->scaleX = 0x10000;
		object->scaleY = 0x10000;
	}
	ObjectDispatch_InitializeFar((struct DispatchObject *)object, gBattlePres_ActorObjectScript);
}

void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
	index *= 2;
	*x = gBattlePlacement_StepPairs[index];
	*y = gBattlePlacement_StepPairs[index + 1];
}

void Summon_LayoutPositions(u16 *actorIds, s32 count, s32 *xPositions, s32 *zPositions)
{
	s32 totalSpacing = count <= 4 ? 30 : 27;
	s32 z = ((count - 1) * totalSpacing) / 2;
	s32 index;

	for (index = 0; index != count; index++)
	{
		s32 spacing = 0;

		xPositions[index] = -80;
		if (index != 0)
		{
			struct BattleUnit *actor;

			spacing = 25;
			if ((u16)(actorIds[index] - 254) > 1)
			{
				actor = UnitGetFar(actorIds[index]);
				spacing = Summon_IsEntryFlagged(actor->classId) ? 27 : 38;
				if (actor->classId == 148 || actor->classId == 121)
				{
					xPositions[index] = -50;
				}
			}
		}
		z -= spacing / 2;
		zPositions[index] = z;
		spacing = 25;
		if ((u16)(actorIds[index] - 254) > 1)
		{
			struct BattleUnit *actor = UnitGetFar(actorIds[index]);

			spacing = Summon_IsEntryFlagged(actor->classId) ? 27 : 38;
		}
		z -= spacing / 2;
	}
}

s32 EnemySlotGet(void)
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

s32 EnemyUpdate(void)
{
	struct BattleWork *work = gBattleWork;
	u16				unitIds[14];
	s32				x[6];
	s32				z[6];
	s32				count;
	s32				i;

	for (i = 0; i < 6 && work->enemyIds[i] != 0xff; i++)
		unitIds[i] = work->enemyIds[i];
	count = i;
	Summon_LayoutPositions(unitIds, count, x, z);
	for (i = 0; i < count; i++)
	{
		s32 id = work->enemyIds[i];

		if (id != 0xfe)
		{
			struct BattleObj *object = BattleObjGet(id);

			object->anchorX = x[i] << 16;
			object->anchorZ = z[i] << 16;
		}
	}
}

void BattleUnit_RefreshPlacement(void)
{
	struct BattleWork *work;
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
	work = gBattleWork;
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
		BattleObjMake(BattleObjGet(id), id, gBattlePlacement_StepPairs[place * 2],
			gBattlePlacement_StepPairs[place * 2 + 1]);
	}
	for (i = 0; i < 6 && work->enemyIds[i] != 0xff; i++)
		ids[i] = work->enemyIds[i];
	count = i;
	Summon_LayoutPositions(ids, count, x, z);
	for (i = 0; i < count; i++, stand++)
	{
		s32 id = work->enemyIds[i];

		if (id != 0xfe)
			BattleObjMake(BattleObjGet(id), id, x[stand], z[stand]);
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
			struct BattleObj *slot = BattleObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				BattleStatusIcon_Cycle(slot);
				if (slot->effectEntry != 0)
				{
					rec = GetMotionRecord(obj, 0);

					if (rec != 0)
					{
						struct SpriteEntry *effect;
						s32					p = 0;

						if (obj->y != 0) // 高さがあれば
							p = 9;
						effect = slot->effectEntry;
						if (effect->priority != p)
						{
							effect->priority = p;
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
			struct BattleObj *slot = BattleObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				switch (obj->recordStorageKind & 15)
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
			struct BattleObj *slot = BattleObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				switch (obj->recordStorageKind & 15)
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
#define ICON_POISON 0x01		// 毒
#define ICON_VENOM 0x02	// もう毒
#define ICON_STUN 0x04	// しびれ
#define ICON_PSY_SEAL 0x08		// エナジーふうじ
#define ICON_SLEEP 0x10	// ねむり
#define ICON_DELUSION 0x20 // まぼろし
#define ICON_HAUNT 0x40	// あくりょう
#define ICON_GRIM_REAPER 6		// しりょう：残りの数 1-7 が bit 7-13

#define ICON_MAX 14 // アイコンの種類

//--------------------------------------------------------------------------
// 状態異常から状態アイコンのビットを作る
//--------------------------------------------------------------------------
s32 BattleUnit_BuildStatusFlags(s32 id, struct BattleObj *slot)
{
	struct BattleUnit *unit = UnitGetFar(id);
	s8				poison = unit->poison;
	u32				flags = 0;

	if (poison == 1)
		flags = ICON_POISON;
	if (poison == 2)
		flags |= poison; // もう毒 (ICON_VENOM)
	if (unit->delusion != 0)
		flags |= ICON_DELUSION;
	if (unit->stun != 0)
	{
		s32 cls;
		flags |= ICON_STUN;
		cls = unit->classId;
		if (cls == 121 || cls == 148) // この二体には出さない
			flags &= ~ICON_STUN;
	}
	if (unit->psySeal != 0)
		flags |= ICON_PSY_SEAL;
	if (unit->haunt != 0)
		flags |= ICON_HAUNT;
	if (unit->sleep != 0)
		flags |= ICON_SLEEP;
	if (unit->grimReaper != 0)
		flags |= 1 << (unit->grimReaper + ICON_GRIM_REAPER);
	slot->activeConditions = flags;
}

//--------------------------------------------------------------------------
// 状態アイコンを 80 フレームごとに次の状態へ切り替える
//--------------------------------------------------------------------------
s32 BattleStatusIcon_Cycle(struct BattleObj *slot)
{
	struct AnimationEntry  *old;
	struct AnimationEntry  *icon;
	struct AnimationObject *rec;
	struct MotionObject	   *owner;
	s32						anim;
	s32						handle;
	s32						prev;
	s32						changed = 0;

	if (slot->cycleTimer >= 0)
		slot->cycleTimer--;

	// 表示中の状態が消えたら、すぐに切り替える
	old = (struct AnimationEntry *)slot->animationEntry;
	if ((old == 0 ? (s16)slot->activeConditions != 0
				  : (((s16)slot->activeConditions >> slot->selectedCondition) & 1) == 0)
		|| slot->cycleTimer == 0)
	{
		anim = -1;
		owner = slot->object;
		if ((s16)slot->activeConditions != 0)
		{
			prev = slot->selectedCondition;
			for (anim = prev + 1;; anim++)
			{
				if (anim > ICON_MAX - 1)
					anim = 0;
				if ((((s16)slot->activeConditions >> anim) & 1) != 0)
					break;
			}

			if (prev != anim || old == 0)
			{
				slot->selectedCondition = anim;
				changed = 1;
			}
			slot->cycleTimer = 80;
		}
		else
		{
			changed = 1;
		}

		rec = GetMotionRecord(owner, 0);
		if (rec != 0)
		{
			// アイコンのアニメ番号は、幅 32 のキャラと他とで別の並び
			if (anim >= 0)
			{
				if (rec->width == 32)
					anim += 340;
				else
					anim += 355;
			}

			if (slot->animationEntry != 0 && changed != 0)
			{
				ResourceMetadata_UnregisterFar(rec, (s32)slot->animationEntry);
				slot->animationEntry = 0;
			}

			if (anim >= 0 && changed != 0)
			{
				// The handle is stored first and cleared again when registration fails.
				handle = ResourceMetadata_RegisterFar(rec, anim);
				slot->animationEntry = (struct SpriteEntry *)handle;
				if (handle == -1)
					slot->animationEntry = 0;
				icon = (struct AnimationEntry *)slot->animationEntry;
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
		}
	}
}

//--------------------------------------------------------------------------
// 状態に合わせてキャラのポーズを決める
// 動けなければ味方 4・敵 0、倒れていれば味方 4・敵 5、ほかは 1
//--------------------------------------------------------------------------
s32 BattlePres_SetActorModeAndAction(s32 id)
{
	struct BattleUnit *unit;
	s32				mode;

	unit = UnitGetFar(id);
	mode = 1;
	if (unit->hp != 0)
	{
		if (unit->sleep != 0 || unit->stun != 0 || unit->immobile != 0)
			mode = (unit->side != BATTLE_UNIT_ENEMY) * 4;
	}
	else
	{
		s32 ally = unit->side ^ mode;
		mode = (u32)(-ally | ally) >> 31;
		mode = 5 - mode;
	}

	ObjectDispatch_ApplyArgumentToChildrenFar(BattleObjGet(id)->object, mode);
	ObjectDispatch_ApplyValueToChildrenFar(BattleObjGet(id)->object, (id & 3) + 14);
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
	struct BattleObj		*slot;
	struct MotionObject *obj;
	s32					 i;
	void				*rec;

	slot = BattleObjGet(no);
	if (slot != NULL)
	{
		obj = slot->object;
		if (obj != NULL)
		{
			slot->animationEntry = 0;
			slot->effectEntry = 0;
			i = 0;
			while ((rec = GetMotionRecord(obj, i)) != NULL)
			{
				ResourceObject_ReleaseFar(rec);
				i += 1;
			}
			obj->recordStorageKind = 0; // 絵なし
			obj->records = NULL;
		}
	}
}

void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh)
{
	s32						 i;
	s32						 id;
	struct BattleObj			*slot;
	struct MotionObject		*object;
	struct AnimationObject	*res;
	struct SpriteEntry		*entry;
	struct AnimationObject **objects;
	s32						 resource;
	s32						 objectId;

	for (i = 0; i <= 13; i++)
	{
		if (!BattlePlacement_ContainsId(list, i))
		{
			objectId = i + 120;
			if (i <= 7)
				objectId = i;
			ReleaseBattleObjectRecords(objectId);
		}
	}
	if (gSchedulerStatus == 0)
		WaitFrames(1);
	for (i = 0; i <= 13 && (id = list[i]) != 255; i++)
	{
		if (id == 254)
			continue;
		slot = BattleObjGet(id);
		if (slot == 0)
			continue;
		BattleUnit_BuildStatusFlags(id, slot);
		object = slot->object;
		if (object == 0)
			continue;
		if (object->recordStorageKind != 0)
			continue;
		if ((slot->resource & 0xfff) == 476 || (slot->resource & 0xfff) == 483)
		{
			objects = (struct AnimationObject **)gMenuCtrlWork->unk08 + gMenuCtrlWork->count;
			resource = slot->resource;
			object->recordStorageKind = 2;
			object->records = objects;
			Iwram_ClearWords(objects, 16);
			res = ResourceObject_CreateFar(resource);
			if (res != 0)
			{
				res->scale = Iwram_MulQ16(res->scale, slot->scale);
				object->height = Resource_GetMetadataRecordFar(resource)->boxY >> 1;
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
				object->recordStorageKind = 1;
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
					slot->animationEntry = entry;
					Animation_SetWorkEntryFar((struct AnimationEntry *)entry, 0);
					entry->priority = 3;
				}
				if ((resource = slot->effect) != 0)
				{
					if (res->width == 32)
						resource = 0x1ff;
					entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(res, resource);
					slot->effectEntry = entry;
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
				if (list[i] != 254 && (slot = BattleObjGet(resource)) != 0
					&& (object = slot->object) != 0)
					BattlePres_SetActorModeAndAction(resource);
				i++;
				if (i >= 14 || (resource = list[i]) == 255)
					break;
			}
		}
	}
}

struct BattleObj *BattleObjGet(s32 objectId)
{
	struct BattleWork *work = gBattleWork;
	struct BattleObj  *slots = work->slots;
	s32				index;
	if (objectId > 7)
	{
		objectId -= 0x78;
	}
	index = objectId;
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
	s32	   storageKind;
	s32	   remaining;
	void **recordGroups;
	void  *recordGroup;

	if (object != NULL)
	{
		storageKind = object->recordStorageKind & 0xF;
		switch (storageKind)
		{
		case 1:
			ResetMotionRecordGroup(object->records);
			return;
		case 2:
			recordGroups = object->records;
			remaining = 3;
			do
			{
				recordGroup = *recordGroups++;
				if (recordGroup != NULL)
				{
					ResetMotionRecordGroup(recordGroup);
				}
				remaining -= 1;
			} while (remaining >= 0);
			break;
		}
	}
}

s32 ActivateBattleObjectSlot(s32 objectId)
{
	struct BattleObj *slot;
	BattleMotion_ReleaseObjectSlotByValue(objectId);
	slot = BattleObjGet(objectId);
	slot->active = 1;
	return 0;
}

void *BattleMotion_DestroyAllSlotObjects(void)
{
	// 戻り値の型はポインタだが return 文は無い（pop {r1}; bx r1）
	s32			   no;
	s32			   i;
	struct BattleObj *slot;

	i = 0;
	do
	{
		no = i + 0x78;
		if (i <= 7)
		{
			no = i;
		}
		slot = BattleObjGet(no);
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
	struct BattleWork *state = gBattleWork;

	out->x = state->slots[no].anchorX;
	out->y = 0;
	out->z = state->slots[no].anchorZ;
	return 0;
}

s32 Camera_ApplyTransformByFlag(void)
{
	struct BattleCamera *state = gCameraWork;
	Render_ResetTransformState();
	if (GameFlag_TestFar(0x16B) != 0)
	{
		Iwram_TransformMatrix((s32 *)gCamera_FlagTransformWork);
		Graphics_PrepareTransferAndRun(state->eye, state->pos);
	}
	else
	{
		Graphics_PrepareTransferInIwramWork(state->eye, state->pos);
	}
}

s32 BattleMotion_ProjectPosition(s32 id, s32 *projected)
{
	struct MotionObject	   *object = BattleObjGet(id)->object;
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

void *GetMotionRecord(struct MotionObject *object, s32 recordIndex)
{
	s32 storageKind = object->recordStorageKind & 0xF;
	if (storageKind == 1)
	{
		if (recordIndex == 0)
		{
			return object->records;
		}
	}
	else if (storageKind == 2)
	{
		return ((void **)object->records)[recordIndex];
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
	state->followPos = 0;
	state->unk18 = 0;

	Render_ResetTransformState();
	SceneTransform_ApplyPosition(&state->pos[0]);
	SceneTransform_ApplyYaw((s16)state->yaw);
	SceneTransform_ApplyPitch((s16)state->pitch);

	transfer.x = 0;
	transfer.y = 0;
	transfer.z = state->distance;
	Iwram_TransformVector((s32 *)&transfer, (s32 *)state);
}

void ObjMotRst(s32 slotId)
{
	s32					 z;
	s32					 zero;
	struct BattleObj		*slot;
	struct MotionObject *object;

	slot = BattleObjGet(slotId);
	object = slot->object;
	zero = 0;
	object->acceleration = 0x20000;
	object->speedLimit = 0x80000;
	object->verticalMotionStrength = 0xab85;
	object->velocityY = zero;
	object->verticalMotionPhase = zero;
	object->autoFaceMotion = zero;
	object->snapToTarget = 1;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, slot->anchorX, 0, slot->anchorZ);
	z = slot->anchorZ;
	if (z < 0)
		z += 7;
	object->angle = ArcTan2(z >> 3, slot->anchorX) - 0x8000;
}

void BattleMotion_SetupEscapeObject(s32 objectId)
{
	struct BattleObj		*slot;
	struct MotionObject *object;

	slot = BattleObjGet(objectId);
	object = slot->object;
	object->acceleration = 0x20000;
	object->speedLimit = 0x80000;
	object->velocityY = 0x50000;
	object->verticalMotionStrength = 0x7851;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, slot->anchorX * 3, 0, slot->anchorZ);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

void BattleMotion_InterpolatePosition(
	struct BattleObj *startSlot, struct BattleObj *endSlot, s32 progress)
{
	s32					 zStep;
	s32					 startX;
	s32					 x;
	s32					 startZ;
	s32					 endZ;
	struct MotionObject *start;
	struct MotionObject *end;

	start = startSlot->object;
	end = endSlot->object;
	startX = start->x;
	x = startX + progress * (end->x - startX) / 100;
	endZ = end->z;
	startZ = start->z;
	zStep = progress * (endZ - startZ) / 100;
	REG_BLDCNT = 0;
	start->acceleration = 0x20000;
	start->speedLimit = 0x80000;
	start->velocityY = 0x40000;
	start->verticalMotionStrength = 0xab85;
	start->verticalMotionPhase = 0;
	start->autoFaceMotion = 1;
	Object_SetMoveTargetFar(start, x, 0, startZ + zStep);
	ObjectDispatch_ApplyArgumentToChildrenFar(start, 2);
}

void BattleMotion_SetObjectPosition(struct BattleObj *slot)
{
	struct MotionObject *object = slot->object;

	object->acceleration = 0x20000;
	object->speedLimit = 0x80000;
	object->verticalMotionStrength = 0xab85;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	Object_SetMoveTargetFar(object, slot->anchorX, 0, slot->anchorZ);
}

void BattleMotion_ResetObjectAtScaledAnchor(s32 objectId)
{
	u32					 scaledX;
	struct BattleObj		*slot;
	struct MotionObject *object;

	slot = BattleObjGet(objectId);
	object = slot->object;
	object->acceleration = 0x10000;
	object->speedLimit = 0x40000;
	object->velocityY = 0x30000;
	object->verticalMotionStrength = 0x9999;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	scaledX = slot->anchorX * 3;
	Object_SetMoveTargetFar(object, (s32)(scaledX + (scaledX >> 0x1F)) >> 1, 0, slot->anchorZ);
}

void BattleMotion_InitializeObject(s32 id)
{
	struct BattleObj		*slot = BattleObjGet(id);
	struct MotionObject *object = slot->object;

	object->acceleration = 128 << 9;
	object->speedLimit = 128 << 11;
	object->velocityY = 128 << 11;
	object->verticalMotionStrength = 0x9999;
	object->verticalMotionPhase = 0;
	object->autoFaceMotion = 0;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, Iwram_MulQ16(slot->anchorX, 0x14ccc), 0, slot->anchorZ);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

void BattleMotion_ApplyVariantMotion(s32 id, s32 variant)
{
	struct BattleObj		*slot;
	struct MotionObject *object;
	s32					 x;
	s32					 scale;

	slot = BattleObjGet(id);
	object = slot->object;
	if (UnitGetFar(id)->classId != 0x94)
	{
		object->acceleration = gBattleMotion_VariantAcceleration[variant];
		object->speedLimit = gBattleMotion_VariantSpeedLimit[variant];
		if (object->y == 0 || variant > 4)
			object->velocityY = gBattleMotion_VariantVelocityY[variant];
		object->verticalMotionStrength = 0x9999;
		object->verticalMotionPhase = 0;
		object->autoFaceMotion = 0;
		ObjectDispatch_InitFromTable6Far(object);
		scale = slot->anchorX;
		x = (scale * gBattleMotion_VariantDistancePercent[variant]) / 100;
		Object_SetMoveTargetFar(object, x, 0, slot->anchorZ);
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 5);
}

void BattleMotion_ApproachTarget(
	s32 actorId, s32 targetId, s32 travelDivisor, s32 initialVelocityY)
{
	struct BattleObj		*actorSlot = BattleObjGet(actorId);
	struct BattleObj		*targetSlot = BattleObjGet(targetId);
	struct MotionObject *object = actorSlot->object;
	struct MotionObject *target = targetSlot->object;
	s32					 scale = 75;
	s32					 dx = target->x - object->x;
	s32					 startX = object->x;
	s32					 stepX = scale * dx / 100;
	s32					 dz = target->z - object->z;
	s32					 startZ = object->z;
	s32					 stepZ = scale * dz / 100;
	s32					 x = startX + stepX;
	s32					 z = startZ + stepZ;
	s32					 cellX = stepX >> 8;
	s32					 cellZ = stepZ >> 8;
	s32					 dist;

	dist = Iwram_Sqrt(cellX * cellX + cellZ * cellZ);
	dist = (dist << 8) / travelDivisor;
	object->acceleration = dist;
	object->speedLimit = dist;
	object->snapToTarget = 1;

	if (object->motionFlags & 4)
		object->velocityY = initialVelocityY;
	object->velocityY = initialVelocityY;
	object->verticalMotionStrength = 0xab85;
	object->autoFaceMotion = 1;
	ObjectDispatch_InitFromTable6Far(object);
	Object_SetMoveTargetFar(object, x, 0, z);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

void BattleMotion_ResetSlotObjectMode2(s32 id)
{
	struct MotionObject *object;

	object = BattleObjGet(id)->object;
	ObjectDispatch_InitFromTable6Far(object);
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
}

void BattleMotion_ReservedNoOp83B0()
{
}

void BattleMotion_ApplyPairMidpoint(s32 firstId, s32 secondId)
{
	struct BattleObj			 *secondSlot;
	struct MotionObject		 *first;
	struct MotionObject		 *second;
	s32						  x1;
	s32						  x2;
	s32						  z1;
	s32						  z2;
	struct FixedPointPosition pos;

	first = BattleObjGet(firstId)->object;
	secondSlot = BattleObjGet(secondId);
	second = secondSlot->object;

	x1 = first->targetX;
	if (x1 == 0x80000000)
	{
		x1 = first->x;
	}
	z1 = first->targetZ;
	if (z1 == 0x80000000)
	{
		z1 = first->z;
	}
	x2 = second->targetX;
	if (x2 == 0x80000000)
	{
		x2 = second->x;
	}
	z2 = second->targetZ;
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

	object = BattleObjGet(id)->object;
	if ((object != NULL) && ((0xF & object->recordStorageKind) == 1))
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
	struct MotionObject	   *object = BattleObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						scaled;
	s32						factor;
	/* FAKEMATCH: never used: the input position BattleMotion_ProjectPosition fills, unused here; it keeps the native's 12-byte frame. Without it 4 of 42 differ (no sub sp, #12 and add sp, #12). */
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
	struct MotionObject	   *object = BattleObjGet(id)->object;
	struct AnimationObject *record = GetMotionRecord(object, 0);
	s32						scaled;
	s32						factor;

	Camera_ApplyTransformByFlag();
	scaled = Render_ProjectPoint(&object->x, projected);
	factor = Iwram_MulQ16(scaled, record->scale);
	if (Summon_IsEntryFlagged(UnitGetFar(id)->classId) != 0)
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

	no = UnitGetFar(id)->classId;
	value = (u8)Summon_GetEntryByte4(no) << 16;
	if (value == 0)
	{
		no = UnitGetFar(id)->classId;
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
	u16						   unitIds[14];
	s32						   firstCount;
	s32						   count = 0;
	s32						   secondCount;
	s32						   priorityRange;
	s32						   index;
	struct BattleActionRecord *entry;
	struct BattleUnit			  *unit;

	firstCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unitIds);
	for (index = 0; index != 4; index++)
		UnitGetFar(index);
	for (index = 0; index < firstCount; index++)
	{
		s32 unitId = unitIds[index];

		unit = UnitGetFar(unitId);
		entry = &entries[index];
		entry->unitId = unitId;
		entry->priority = unit->agi;
		entry->command = 0;
		entry->parameter = 0;
		entry->target = 0x80;
		count++;
	}
	secondCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unitIds);
	entry = &entries[firstCount];
	priorityRange = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, NULL);
	for (index = 0; index < secondCount; index++)
	{
		s32 unitId = unitIds[index];

		unit = UnitGetFar(unitId);
		entry->unitId = unitId;
		entry->priority = unit->agi >> 1;
		if (entry->priority != 0)
			entry->priority += (Random16() * unit->agi) >> 16;
		entry->command = 0;
		entry->parameter = 0;
		entry->target = (Random16() * priorityRange) >> 16;
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
	s32									 actorId;
	s32									 battleMode;
	struct BattleUnit						*actor;

	actorId = action->unitId;
	actor = UnitGetFar(actorId);
	if (actor->hp == 0)
		return -1;

	action->target = BattleTarget_ReplaceDefeated(action);
	transition = gTransitionWork;
	if (action->unitId > 4)
		battleMode = -0x2000;
	else
		battleMode = 0x2000;
	transition->targetYaw = battleMode;
	transition->frames = 60;
	UiWork_ClearValueNameTablesFar();

	switch (action->command)
	{
	case 99:
		UiText_ShowMessageAndWaitCoreFar(MsgPartyFlees);
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

s32 BattleObject_IsValidId(u32 objectId)
{
	if (objectId <= 7)
	{
		return 0;
	}
	if (objectId - 128 <= 5)
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
	UiText_ShowMessageAndWaitCoreFar(MsgCannotEscape);
	return 0;
}

s32 BattlePres_ShowDefendMessage(const struct BattleActionRecord *action)
{
	s32				objectId;
	s32				result;
	struct BattleUnit *unit;

	objectId = action->unitId;
	unit = UnitGetFar(objectId);
	if (BattleObject_IsValidId(objectId) < 0)
	{
		return -1;
	}
	result = 0;
	if (unit->hp <= 0)
	{
		return result;
	}
	UiWork_ClearValueNameTablesFar();
	UiWork_PushValueSlotFar(objectId, 1);
	UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
	return 0;
}

s32 BattlePresentation_RunPairedUnitTransition(struct BattleActionRecord *action)
{
	u16							  visibleUnits[14];
	struct BattlePresentationWork context;
	s32							  actorId;
	s32							  targetId;
	u32							  sideStart;
	u32							  sideCount;
	u32							  livingCount;
	struct MotionObject			 *actorRecord;
	u32							  index;

	livingCount = 0;
	actorId = action->unitId;
	if (BattleObject_IsValidId(actorId) < 0)
	{
		return -1;
	}
	targetId = action->target;
	if (BattleObject_IsValidId(targetId) < 0)
	{
		return -1;
	}

	{
		struct BattlePresentationTransition *transition = gTransitionWork;
		transition->targetYaw = action->unitId > 4 ? 0x5000 : 0x2000;
		transition->frames = 60;
	}
	WaitFrames(10);
	Random16();
	actorRecord = BattleObjGet(actorId)->object;

	if (targetId >= 0 && targetId <= 7)
	{
		sideCount = BattleParty_ListLivingUnits(2, visibleUnits);
		sideStart = 0x80;
	}
	else
	{
		sideCount = BattleParty_ListLivingUnits(1, visibleUnits);
		sideStart = 0;
	}

	for (index = 0; index != sideCount; index++)
	{
		if (index + sideStart == actorId)
		{
			ObjectDispatch_ApplyArgumentToChildrenFar(actorRecord, 3);
		}
	}

	WaitFrames(30);
	REG_BLDCNT = 0x3f40;
	for (index = 0; index != sideCount; index++)
	{
		BattlePres_SetActorRecordMode(index + sideStart, 1);
	}
	for (index = 0; index != 16; index++)
	{
		REG_BLDALPHA = (16 - index) | 0x1000;
		WaitFrames(1);
	}
	PartyStatusDrawFar(9);

	if (targetId > 0x7f)
	{
		u32 count = BattleParty_ListLivingUnits(2, visibleUnits);
		for (index = 0; livingCount != count; index++)
		{
			u32 unit = index + 0x80;
			if (UnitGetFar(unit)->hp > 0)
			{
				visibleUnits[livingCount++] = unit;
			}
		}
	}
	else
	{
		u32 count = BattleParty_ListLivingUnits(1, visibleUnits);
		for (index = 0; livingCount != count; index++)
		{
			if (UnitGetFar(index)->hp > 0)
			{
				visibleUnits[livingCount++] = index;
			}
		}
	}
	visibleUnits[livingCount] = 0xff;
	BattleActor_SpawnObjectsForList(visibleUnits, 0);

	context.kind = action->parameter;
	context.actor = actorId;
	for (index = 0; index != livingCount; index++)
	{
		context.actors[index] = visibleUnits[index];
	}
	context.count = livingCount;
	if (targetId >= 0 && targetId <= 7)
	{
		context.side = 1;
	}
	else
	{
		context.side = 0;
	}
	BattleFx_InitializeModeFar((s32 *)&context);

	WaitFrames(10);
	BattleObjPut();
	REG_BLDCNT = 0x3f40;
	for (index = 0; index != sideCount; index++)
	{
		BattlePres_SetActorRecordMode(index + sideStart, 1);
	}
	for (index = 0; index != 16; index++)
	{
		REG_BLDALPHA = index | 0x1000;
		WaitFrames(1);
	}
	for (index = 0; index != sideCount; index++)
	{
		BattlePres_SetActorRecordMode(index + sideStart, 0);
	}
	BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
	WaitFrames(3);
	return 0;
}

s32 BattlePres_RunApproachAction(struct BattleActionRecord *input)
{
	struct BattlePresentationWork work;

	if (gTransitionWork->targetYaw == 0x2000)
	{
		gTransitionWork->targetYaw = 0x2000;
		WaitFrames(10);
	}
	else
	{
		gTransitionWork->targetYaw = 0x2000;
		WaitFrames(30);
	}

	work.actor = input->unitId;
	if (BattleObject_IsValidId(work.actor) < 0)
		return -1;

	work.actors[0] = input->target;
	if (BattleObject_IsValidId(work.actors[0]) < 0)
		return -1;

	UnitGetFar(work.actor);
	UnitGetFar(work.actors[0]);
	Random16();
	UiWork_PushValueSlotFar(work.actor, 1);
	UiText_ShowMessageAndWaitCoreFar(MsgActorAttacks);
	BattleMotion_ApproachTarget(work.actor, work.actors[0], 13, 0);
	ObjectDispatch_ApplyValueToChildrenFar(BattleObjGet(work.actor)->object, 16);
	BattleObjGet(work.actors[0]);

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

	if (action->unitId > 4)
		facing = -0x2000;
	else
		facing = 0x2000;
	if (transition->targetYaw == facing)
	{
		transition->frames = 40;
		WaitFrames(40);
	}
	else
	{
		transition->targetYaw = facing;
		transition->frames = 40;
		WaitFrames(40);
	}
	work.kind = action->parameter;
	work.initialValue = action->range;
	work.actor = action->unitId;
	work.firstTarget = action->target;
	if (BattleObject_IsValidId(work.actor) < 0)
		return -1;
	if (work.firstTarget > 127)
		work.count = BattleParty_ListLivingUnits(2, work.actors);
	else
		work.count = BattleParty_ListLivingUnits(1, work.actors);
	PartyStatusDrawFar(gBattleWork->partyStatus & ~1);
	object = BattleObjGet(work.actor)->object;
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
		struct AnimationObject *animation = BattleObjGet(work.actors[i])->object->records;
		s32						j;

		for (j = 0; j != animation->count - 1; j++)
			work.params[i][j] = animation->entries[j]->param;
	}
	/* FAKEMATCH: needed to match: the one-pass stages force fresh work pointers in the reset loop and place the dispatch argument before the kind store. As plain statements the allocation changes from the side choice on: differ 103, also with the variant stored before the kind. */
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

void BattlePresentation_ApplyUnitDamage(u32 unitId, s32 damage, s32 showMessage, u8 *context)
{
	u8				localContext[4];
	struct BattleUnit *character;
	struct BattleObj  *slot;

	if (context == 0)
	{
		context = localContext;
		context[0] = 0;
		context[1] = 0;
		context[2] = 0;
		context[3] = 0;
	}

	character = UnitGetFar(unitId);
	character->hp -= damage;
	if (character->hp < 0)
		character->hp = 0;

	slot = BattleObjGet(unitId);
	ObjectDispatch_ApplyArgumentToChildrenFar(slot->object, 5);
	PartyStatusDrawFar(0);
	UiWork_ClearValueNameTablesFar();

	if (unitId <= 7)
	{
		if (showMessage != 0)
			UiText_ShowMessageAndWaitCoreFar(MsgBitterBlow);
		UiWork_PushValueSlotFar(damage, 5);
		UiWork_PushValueSlotFar(unitId, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgDmgP);
	}
	else
	{
		if (showMessage != 0)
			UiText_ShowMessageAndWaitCoreFar(MsgCritical);
		UiWork_PushValueSlotFar(damage, 5);
		UiWork_PushValueSlotFar(unitId, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgDmgE);
		UiWork_PushValueSlotFar(unitId, 1);
	}

	BattleMotion_SetMode5AndActivateSlot(unitId);
	if (unitId <= 7)
	{
		if (character->hp <= 0)
		{
			UiWork_PushValueSlotFar(unitId, 1);
			UiText_ShowMessageAndWaitCoreFar(MsgGoesDown);
		}
	}
	else if (character->hp <= 0)
	{
		UiWork_PushValueSlotFar(unitId, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgDowned);
	}

	slot = BattleObjGet(unitId);
	ObjectDispatch_ApplyArgumentToChildrenFar(slot->object, 1);
}

void BattleMotion_SetMode5AndActivateSlot(s32 unitId)
{
	struct AnimationEntry  *actorData;
	struct AnimationObject *object;
	if (UnitGetFar(unitId)->hp <= 0)
	{
		object = BattleObjGet(unitId)->object->records;
		AnimationObjects_SelectAnimationFar(object, 5);
		actorData = object->entries[0];
		actorData->param = 6;
		actorData->frame = 0xFF;
		WaitFrames(4);
		Map_RenderAnimatedTileFramesForObjectFar(object);
		ActivateBattleObjectSlot(unitId);
	}
}

s32 BattleTarget_ReplaceDefeated(const struct BattleActionRecord *action)
{
	u16 livingUnits[14];
	s32 targetId;
	s32 livingCount;

	targetId = action->target;
	if (UnitGetFar(targetId)->hp != 0)
	{
		return targetId;
	}

	if (targetId > 0x7F)
	{
		livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
	}
	else
	{
		livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
	}

	if (livingCount == 0)
	{
		return 0x100;
	}

	return (s16)livingUnits[(Random16() * livingCount) >> 0x10];
}

void Palette_UpdatePulseBrightness(void)
{
	u16 *palette;
	s32	 i;
	s32	 mask;

	palette = (u16 *)0x050001C0;
	mask = 31;
	i = 15;
	do
	{
		u32 color;
		s32 amount;
		s32 tick;
		s32 angle;
		s32 red;
		s32 green;
		s32 blue;

		tick = gFrameCount;
		angle = (tick * 3) << 10;
		color = palette[16];
		amount = (0x10000 - Trig_Cos(angle)) / 0x2AAA;

		red = (color >> 10) & mask;
		green = (color >> 5) & mask;
		blue = color & mask;
		red += amount;
		green += amount;
		blue += amount;

		if (red < 0 || red > 31)
			red = 31;
		if (green < 0 || green > 31)
			green = 31;
		if (blue < 0 || blue > 31)
			blue = 31;

		*palette++ = (red << 10) | (green << 5) | blue;
		i--;
	} while (i >= 0);
}

void Camera_ConfigureScene(s32 pos)
{
	void							   **slots = (void **)&gCameraWork;
	struct BattleCamera					*state = slots[0];
	struct BattlePresentationTransition *secondary =
		slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_CAMERA];
	struct FixedPointPosition local;
	u32						  result;

	state->pos[1] = 160 << 11;
	state->pos[0] = 0;
	state->pos[2] = 0;
	secondary->targetYaw = 128 << 7;
	state->yaw = 128 << 7;
	state->pitch = 244 << 8;
	state->followPos = 0;
	state->distance = 0x02ee0000;
	state->unk18 = 0;

	Render_ResetTransformState();
	SceneTransform_ApplyPosition(&state->pos[0]);
	SceneTransform_ApplyYaw((s16)state->yaw);
	SceneTransform_ApplyPitch((s16)state->pitch);

	local.x = 0;
	local.y = 0;
	local.z = state->distance;
	Iwram_TransformVector((s32 *)&local, (s32 *)state);

	result = Iwram_RatioMulQ14(0x03c90000, 192 << 8);
	Camera_StoreSceneParameters(0, result, 0x07920000);

	gProjection.centerY = pos + 120;
	secondary->active = 1;
	BattleCamera_SetRange(240 << 15, (0x76 - pos) << 16, 0, 128 << 4, 128 << 10);
	secondary->flag = 1;
	secondary->active = 0;
}

void BattleUnit_ResetGuardLevels(void)
{
	u16 ids[14];
	s32 count;
	s32 index;

	count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
	for (index = 0; index < count; index++)
	{
		struct BattleUnit *actor;

		actor = UnitGetFar(ids[index]);
		actor->guardLv = 0;
		UnitCalcFar(ids[index]);
	}
}

s32 BattleEscape_CheckSuccess(void)
{
	s32				escaped;
	u8			   *failedAttempts;
	s16				livingUnits[14];
	s32				livingCount;
	s32				levelTotal;
	s32				unitIndex;
	s32				chance;
	struct BattleWork *escapeState;

	escaped = 0;
	escapeState = gBattleWork;
	if (escapeState->encounterMode == 1)
	{
		escaped = 1;
	}
	else
	{
		failedAttempts = &escapeState->escapeFailures;
		chance = 0x1388 + (escapeState->escapeFailures * 0x7D0);
		livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
		levelTotal = 0;
		for (unitIndex = escaped; unitIndex < livingCount; unitIndex++)
		{
			levelTotal += UnitGetFar(livingUnits[unitIndex])->level;
		}
		chance += levelTotal * 0x1F4 / livingCount;
		livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
		levelTotal = 0;
		for (unitIndex = 0; unitIndex < livingCount; unitIndex++)
		{
			levelTotal += UnitGetFar(livingUnits[unitIndex])->level;
		}
		chance -= levelTotal * 0x1F4 / livingCount;
		if ((chance > 0) && (((0x2710 * Random16()) >> 0x10) < chance))
		{
			escaped = 1;
		}
		*failedAttempts += 1;
	}
	if (gGameState.battleStart == 2)
	{
		escaped = 0;
	}
	return escaped;
}

s32 BattlePres_BuildUnitEntries(struct BattleActionRecord *entries)
{
	u16 *excludedUnits = Runtime_BumpAllocateAlternatePool(17);
	u16 *unitIds = Runtime_BumpAllocateAlternatePool(9);
	s32	 unitCount = BattleParty_ListLivingUnits(1, unitIds);
	s32	 excludedCount = 0;
	s32	 entryCount = 0;
	s32	 unitIndex;

	for (unitIndex = 0; unitIndex < unitCount; unitIndex++)
	{
		struct BattleUnit *unit = UnitGetFar(unitIds[unitIndex]);
		s32				copyIndex;

		for (copyIndex = 0; copyIndex < unit->actionEntryCount; copyIndex++)
		{
			if (unit->sleep != 0 || (*(u32 *)&unit->delusion & 0xffffff00))
			{
				struct BattleActionRecord *entry = &entries[entryCount];
				entry->unitId = unitIds[unitIndex];
				entry->priority = unit->agi;
				entry->command = 8;
				entry->parameter = 0;
				entry->target = 0x180;
				entryCount++;
			}
			else
			{
				excludedUnits[excludedCount++] = unitIds[unitIndex];
			}
		}
	}

	{
		s32 appended;

		entries += entryCount;
		appended = Battle_CollectPartyCommandsFar(entries, excludedUnits, excludedCount);
		if (appended < 0)
		{
			unitCount = -1;
		}
		else
		{
			unitCount = entryCount + appended;
		}
		MemFree(unitIds);
		MemFree(excludedUnits);
		return unitCount;
	}
}

s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries)
{
	u16				unitIds[14];
	s32				entryCount = 0;
	struct BattleWork *battle = gBattleWork;
	s32				unitCount;
	s32				i;

	if (battle->encounterMode == 1)
	{
		return 0;
	}

	unitCount = BattleParty_ListLivingUnits(2, unitIds);
	if (unitCount == 0)
	{
		return 0;
	}

	for (i = 31; i >= 0; i--)
	{
		u32 first = (unitCount * Random16()) >> 16;
		u32 second = (unitCount * Random16()) >> 16;
		s32 swap = unitIds[first];
		unitIds[first] = unitIds[second];
		unitIds[second] = swap;
	}

	if (battle->encounterMode == 2)
	{
		s32 limit = ((Random16() * 5) >> 16) + 1;

		if (limit <= 1)
		{
			limit = 2;
		}
		if (limit < unitCount)
		{
			unitCount = limit;
		}
	}

	for (i = 0; i < unitCount; i++)
	{
		s32				unitId = unitIds[i];
		struct BattleUnit *unit = UnitGetFar(unitId);
		s32				copyIndex;

		for (copyIndex = 0; copyIndex < unit->actionEntryCount; copyIndex++)
		{
			struct BattleActionRecord *entry = &entries[entryCount];
			s32						   value;

			entry->unitId = unitId;
			value = unit->agi;
			entry->priority = value;
			if (copyIndex != 0)
			{
				entry->priority = (s16)value / 2;
			}

			if (unit->sleep != 0 || unit->stun != 0)
			{
				entry->command = 8;
				entry->parameter = 0;
				entry->target = 0x100;
			}
			else
			{
				BattleCommand_SelectAutomatic(entry, 0);
			}

			entryCount++;

			if (battle->encounterMode == 2)
			{
				break;
			}
		}
	}

	return entryCount;
}

void BattleQueue_SortByPriority(struct BattleActionRecord *entries, s32 count)
{
	s32 i;
	s32 j;
	s32 swapped;

	for (i = 0; i < count; i++)
	{
		struct BattleActionRecord *entry = &entries[i];

		if (entry->command == 5)
		{
			struct BattleAct *action;

			UnitGetFar(entry->unitId);
			action = BattleActGet(
				Djinn_GetDefinitionHeaderFar(entry->parameter >> 8 & 15, entry->parameter & 0xff));
			if (action->effect == 46 || action->effect == 47 || action->effect == 53)
			{
				entry->priority += 10000;
			}
		}
	}

	do
	{
		swapped = 0;
		for (j = count - 1; j > 0; j--)
		{
			if ((s16)entries[j].priority > (s16)entries[j - 1].priority)
			{
				struct BattleActionRecord temporary;

				Iwram_CopyWords(&temporary, &entries[j], sizeof(struct BattleActionRecord));
				Iwram_CopyWords(&entries[j], &entries[j - 1], sizeof(struct BattleActionRecord));
				Iwram_CopyWords(&entries[j - 1], &temporary, sizeof(struct BattleActionRecord));
				swapped++;
			}
		}
	} while (swapped != 0);
}

struct BattleLinkedActionState
{
	s32 count;
	s32 check;
	s32 seed;
	u8	unk0c[28];
};

s32 BattlePresentation_AppendLinkedActions(struct BattleActionRecord *actions, s32 count)
{
	struct BattleWork				   *battle = gBattleWork;
	s32								result = 0;
	s32								allocationSize = (u32)(count * 16 + 19) / 20 * 20;
	struct BattleLinkedActionState *state =
		(struct BattleLinkedActionState *)Runtime_BumpAllocateAlternatePool(40);
	s32 index;

	s32 BattleLink_SendActions(void)
	{
		s32 timeout;
		s32 disconnected;
		s32 status = SerialRuntime_BeginTransferA(state, 20);

		disconnected = 0;
		timeout = 300;
		if (status == -1)
			return status;
		while (SerialRuntime_GetActiveTransfers())
		{
			WaitFrames(1);
			if (--timeout < 0)
				return -1;
			if ((gLinkStatus & 3) != 3)
			{
				if (++disconnected > 24)
					return -1;
			}
			else
			{
				disconnected = 0;
			}
		}
		if (allocationSize)
		{
			status = SerialRuntime_BeginTransferA(actions, allocationSize);
			if (status == -1)
				return status;
			while (SerialRuntime_GetActiveTransfers())
			{
				WaitFrames(1);
				if (--timeout < 0)
					return -1;
				if ((gLinkStatus & 3) != 3)
				{
					if (++disconnected > 24)
						return -1;
				}
				else
				{
					disconnected = 0;
				}
			}
		}
		return 0;
	}

	s32 BattleLink_ReceiveActions(void)
	{
		/* FAKEMATCH: gSerialReceivedSize is volatile (serial_runtime.h), so every read here is a load of its own. Measured on tbs-en: without the qualifier differ 116, 129 instructions for 131. */
		s32 timeout;
		s32 disconnected;
		s32 status = SerialRuntime_BeginTransferB(state);

		disconnected = 0;
		timeout = 300;
		if (status == -1)
			return status;
		while (SerialRuntime_GetActiveTransfers())
		{
			if (gSerialReceivedSize > 20)
				return -1;
			WaitFrames(1);
			if (--timeout < 0)
				return -1;
			if ((gLinkStatus & 3) != 3)
			{
				if (++disconnected > 24)
					return -1;
			}
			else
			{
				disconnected = 0;
			}
		}
		if (gSerialReceivedSize != 20)
			return -1;
		result = state->count;
		if (state->count)
		{
			status = SerialRuntime_BeginTransferB(actions + count);
			if (status == -1)
				return status;
			while (SerialRuntime_GetActiveTransfers())
			{
				if (gSerialReceivedSize > (u32)(result * 16 + 19) / 20 * 20)
					return -1;
				WaitFrames(1);
				if (--timeout < 0)
					return -1;
				if ((gLinkStatus & 3) != 3)
				{
					if (++disconnected > 24)
						return -1;
				}
				else
				{
					disconnected = 0;
				}
			}
			if (gSerialReceivedSize != (u32)(result * 16 + 19) / 20 * 20)
				return -1;
		}
		return 0;
	}

	for (index = 0; index < count; index++)
	{
		struct BattleActionRecord *action = &actions[index];

		action->slot = battle->ownerSlots[action->unitId];

		if (battle->linkSide == 0)
		{
			if (action->priority & 1)
				action->priority++;
		}
		else
		{
			action->priority |= 1;
		}
	}

	if (battle->linkPaused == 0)
	{
		if (battle->linkSide == 0)
		{
			u32 saved;

			state->count = count;
			state->check = BattleRandomFar();
			IRQ_LOCK(saved);
			gBattleRandomSeed = state->seed = gRandomSeed;
			IRQ_UNLOCK(saved);
			if (BattleLink_SendActions() < 0 || BattleLink_ReceiveActions() < 0)
				goto fail;
			result = state->count;
		}
		else
		{
			if (BattleLink_ReceiveActions() < 0)
				goto fail;
			result = state->count;
			state->count = count;
			if (BattleLink_SendActions() < 0 || BattleRandomFar() != state->check)
				goto fail;
			gBattleRandomSeed = state->seed;
		}

		for (index = 0; index < result; index++)
		{
			struct BattleActionRecord *action = &actions[count + index];

			action->unitId = action->slot;
			action->target ^= 0x80;
		}

		MemFree(state);
		return result;
	}

fail:
	BattleLink_ResetTransferState();
	SerialRuntime_RemoveIrqHandlers();
	MemFree(state);
	return -1;
}

//--------------------------------------------------------------------------
// actions.c
// バトル：ターンの行動の組み立てと、並びの位置の変換
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// BG パレットのバンク 15 を 14 へ、5 を 4 へ、明るさを変えて写す
// offset : RGB 各成分に足す値（0 から 31 におさめる）
//--------------------------------------------------------------------------
void Palette_CopyBanksWithBrightnessOffset(s32 offset)
{
	s32 n;
	s32 bank;
	s32 col;
	s32 mask;
	s32 idx;

	bank = 15;
	n = 0;
	mask = 31;
	do
	{
		for (col = 0; col <= 15; col++)
		{
			u32 color;
			s32 b;
			s32 g;
			s32 r;
			s32 val;

			idx = bank * 16 + col;
			color = ((u16 *)0x05000000)[idx];
			b = (color >> 10) & mask;
			g = (color >> 5) & mask;
			r = color & mask;
			b += offset;
			g += offset;
			r += offset;
			if (b > 31)
				b = 31;
			if (g > 31)
				g = 31;
			if (r > 31)
				r = 31;
			if (b < 0)
				b = 0;
			if (g < 0)
				g = 0;
			if (r < 0)
				r = 0;
			val = b << 10;
			val |= g << 5;
			val |= r;
			((s16 *)0x04ffffe0)[idx] = val; // ひとつ前のバンクへ
		}
		n++;
		bank = 5;
	} while (n <= 1);
}

//--------------------------------------------------------------------------
// このターンの行動を組み立て、優先度の高い順にならべる
// 味方のコマンドと、通信対戦の相手または敵の行動を集める
// 返り値は行動の数。負の値なら中止（通信の失敗など）
// acts : 行動の並び（20 まで）
//--------------------------------------------------------------------------
s32 BattlePresentation_BuildActions(struct BattleActionRecord *acts)
{
	struct BattleUnit *unit;
	struct BattleWork *work;
	u8			   *mode;
	s32				cnt;
	s32				add;
	s32				i;

	work = gBattleWork;
	{
		struct BattleActionRecord *rec = work->actions;
		u32						   n;

		for (n = 0; n < 20; n++)
		{
			rec[n].unitId = BATTLE_UNIT_LIST_END;
			rec[n].priority = 0x8000; // いちばん低い優先度
		}
	}

	BattleUnit_ResetGuardLevels();
	Palette_CopyBanksWithBrightnessOffset(8);
	// コマンドを選ぶあいだ立てるフラグ
	GameFlag_SetBitFar(363);
	mode = &work->encounterMode;
	Camera_ConfigureScene(0);
	UiWork_FinalizeSharedSlotFar();

	// 味方のコマンド（敵の先手なら選べない）
	if (*mode != 2)
	{
		cnt = BattlePres_BuildUnitEntries(acts);
		if (cnt < 0)
			goto finish;
		// にげるに失敗したら、このターンは敵の先手と同じあつかい
		if (cnt != 0 && acts[0].command == 99 && BattleEscape_CheckSuccess() == 0)
			*mode = 2;
	}
	else
	{
		cnt = 0;
	}

	// 通信対戦の相手か、敵の行動
	if (gBattleWork->twoSided != 0)
	{
		add = BattlePresentation_AppendLinkedActions(acts, cnt);
		if (BattlePres_WaitSync() < 0)
		{
			cnt = -1;
			goto finish;
		}
		cnt += add;
		if (add < 0)
		{
			cnt = -1;
			goto finish;
		}
	}
	else
	{
		cnt += BattlePres_BuildOpponentEntries(acts + cnt);
	}

	// 優先度の順にならべ、ぼうぎょ（3 と 7）のガードを先に立てる
	BattleQueue_SortByPriority(acts, cnt);
	if (cnt > 0)
	{
		struct BattleActionRecord *act = acts;

		i = cnt;
		do
		{
			if (act->command == 3 || act->command == 7)
			{
				unit = UnitGetFar(act->unitId);
				unit->guardLv = act->command == 3 ? 1 : 2;
			}
			act++;
			i--;
		} while (i != 0);
	}

finish:
	GameFlag_ClearBitFar(363);
	Camera_InitDefaultTransform();
	gTransitionWork->targetYaw = 0x2000;
	return cnt;
}

//--------------------------------------------------------------------------
// 並びの位置からユニット番号を返す
// slot : 下位 4 ビットが位置、0x80 が立っていれば敵の並び
//--------------------------------------------------------------------------
s32 Battle_GetTaggedSlotValue(s32 slot)
{
	struct BattleWork *work = gBattleWork;

	if ((slot & 0x80) != 0)
		return work->enemyIds[slot & 0xf];
	return work->partyUnits[slot & 0xf];
}

//--------------------------------------------------------------------------
// ユニット番号から並びの位置を返す（いなければ -1）
// 味方は 0x100 | 位置、敵は 0x180 | 位置
// id : ユニット番号（0-7 味方、それ以外は敵）
//--------------------------------------------------------------------------
s32 Battle_FindTaggedSlotByValue(u32 id)
{
	struct BattleWork *work = gBattleWork;
	s32				i;
	s16				unit;

	if (id <= 7)
	{
		for (i = 0;; i++)
		{
			unit = work->partyUnits[i];
			if (unit == BATTLE_UNIT_LIST_END)
				return -1;
			if (unit != BATTLE_UNIT_REMOVED && unit == id)
				return i | 0x100;
		}
	}
	for (i = 0;; i++)
	{
		unit = work->enemyIds[i];
		if (unit == BATTLE_UNIT_LIST_END)
			return -1;
		if (unit != BATTLE_UNIT_REMOVED && unit == id)
			return i | 0x180;
	}
}

DEFINE_ADJUST_CAMERA(BattlePres_AdjustCameraByShoulderKeysAlt)

void Battle_ReservedNoOp9B2C(void)
{
}

s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request, s32 delay)
{
	struct BattlePresentationTransition *view;
	struct BattleWork						*battle;
	struct BattleCamera					*camera;
	struct BattleUnit						*unit;
	u16									 actor[2];
	u16									 ids[14];
	s32									 kept;
	s32									 result;
	s32									 count;
	s32									 i;

	kept = 0;
	if (request->unitId == 0xff)
		return 0;
	unit = UnitGetFar(request->unitId);
	if (unit->hp == 0)
		return -1;
	if (unit->classIndex == 0)
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
		view->targetYaw = 0x2000;
		WaitFrames(delay);
	}
	actor[0] = request->unitId;
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
	BattleObjPut();
	count = BattleParty_ListActorIds(3, ids);
	for (i = 0; i < count; i++)
		ObjMotRst(ids[i]);
	request->unitId = 0xff;
finish:
	BattlePresentation_ConfigurePaletteFade(2, gBattleWork->background, 0);
	return kept;
}

s32 BattlePres_BuildTargetList(struct BattlePlan *plan, struct BattlePresentationWork *work)
{
	s16 *dst;
	u32	 flags;
	s32	 count;
	s32	 i;
	u8	*target;

	count = 0;
	work->flags = 0;
	flags = plan->presentationFlags;
	work->kind = flags & 0xfff;
	work->variant = (flags & 0x3000) >> 12;
	work->actor = plan->aid;
	i = 0;
	if (i < plan->targetCount)
	{
		target = plan->targetIds;
		dst = work->actors;
		do
		{
			if (UnitGetFar(*target)->hp != 0 || (plan->presentationFlags & 0x10000))
			{
				count++;
				*dst++ = *target;
			}
			i++;
			target++;
		} while (i < plan->targetCount);
	}
	if (count == 0)
	{
		work->actors[0] = plan->targetIds[0];
		count = 1;
	}
	work->firstTarget = plan->targetIds[0];
	work->count = count;
	work->initialValue = 1;
}

s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BattlePlan *plan)
{
	struct BattlePresentationTransition *presentation = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
	struct BattleWork						*scene = WORK_SLOT(HEAP_SLOT_BATTLE);
	/* FAKEMATCH: the halfword initialization preserves the word copy into partyMode instead of folding it into an immediate zero (one instruction differs). 2026-10-13: completed as an s32 set to 0, with partyMode = completed or partyMode = 0, differs 2 of 105 (movs r2, #0 for adds r2, r7, #0). */
	union
	{
		s32 value;
		u16 halves[2];
	} completed;
	s32 partyMode;

	completed.halves[0] = 0;
	completed.halves[1] = 0;
	presentation->targetYaw = 0x2000;
	presentation->active = 1;
	BattlePres_SetActorModes(0, 0);

	if (plan->aid <= 7)
	{
		partyMode = completed.value;

		if (scene->encounterMode != 2)
		{
			partyMode = 1;
		}

		if (!partyMode)
		{
			UiText_ShowMessageAndWaitCoreFar(MsgNoEscape);
			BattlePresentation_WaitForAdvance();
		}
		else
		{
			s16 unitIds[14];
			s32 index = BattleParty_ListLivingUnits(1, (u16 *)unitIds) - 1;

			while (index != -1)
			{
				struct BattleUnit *unit = UnitGetFar(unitIds[index]);

				if (unit->stun == 0 && unit->sleep == 0)
				{
					BattleMotion_SetupEscapeObject(unitIds[index]);
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
		UiText_ShowMessageAndWaitCoreFar(MsgNoEscape);
		BattlePresentation_WaitForAdvance();
	}

	presentation->active = 0;
	return completed.value;
}

s32 BattlePresentation_RunUnitTransition(struct BattlePlan *plan, s32 mode)
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
	void						**slots = (void **)&gBattleWork;

	BattlePres_BuildTargetList(plan, &work);
	actor = plan->aid;
	target = plan->targetIds[0];
	if (plan->presentationFlags & 0x8000)
	{
		struct BattlePresentationTransition *transition;
		s32									 yaw;
		transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
		yaw = 0x2000;
		if (actor > 7)
			yaw = 0x5000;
		transition->targetYaw = yaw;
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
		if (transition->targetYaw != yaw)
			transition->targetYaw = yaw;
	}
	BattlePres_SetActorModes(0, 0);
	PartyStatusDrawFar(gBattleWork->partyStatus & ~1);
	object = BattleObjGet(actor)->object;
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
	BattleFx_PlayUnitElementEffect(work.actor, plan->element, 0, 0);
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
			BattleEvPush(0, plan->aid);
			BattleEvPush(4, MsgPsynergyBlocked);
		}
		else
		{
			BattleEvPush(4, MsgCastLacksPp);
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
		for (n = 0; n < plan->targetCount; n++)
			units[n] = plan->targetIds[n];
		units[n] = BATTLE_UNIT_LIST_END;
		for (i = 0; i != work.count; i++)
		{
			struct AnimationObject *animation;
			u32						child;

			animation = GetMotionRecord(BattleObjGet(work.actors[i])->object, 0);
			for (child = 0; child != animation->count - 1; child++)
				work.params[i][child] = animation->entries[child]->param;
		}
		if (plan->presentationFlags & 0x8000)
		{
			if (target > 7)
				work.side = 0;
			else
				work.side = 1;
		}
		else
		{
			if (plan->targetIds[0] <= 7)
				work.side = 1;
			else
				work.side = 0;
		}
		if (plan->presentationFlags & 0x20000)
			work.side ^= 1;
		Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
		if (plan->presentationFlags & 0x8000)
			BattleFx_InitializeModeFar((s32 *)&work);
		else if (plan->presentationFlags & 0x4000)
			BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
		else
			BattleFx_DispatchModeFar((s32 *)&work);
		BattleEventRuntime_WaitForReady();
	}
	BattleObjPut();
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
s32 BattlePres_RunActorEntries(struct BattlePlan *plan, s32 unused)
{
	s32 i;

	BattlePres_SetActorModes(0, 0);
	if (plan->targetCount == 0)
	{
		BattlePresentation_WaitForAdvance();
	}
	else
	{
		for (i = 0; i < plan->targetCount; i++)
		{
			BattleTargetExec(plan, i);
			BattleEv_DispatchQueued();
		}
	}
	WaitFrames(1);
}

s32 RunBattlePresentation(struct BattlePlan *plan, s32 mode)
{
	struct BattlePresentationWork		 work;
	struct MotionObject					*object;
	struct AnimationObject				*animation;
	struct BattleUnit						*unit;
	s16									 position[3];
	s32									 scripted;
	struct BattlePresentationTransition *transition;
	s32									 angle;
	s32									 adjusted;
	s32									 facingAngle;
	s32									 x;
	s32									 z;
	s32									 divisor;
	s32									 direct;
	s32									 index;
	s32									 phase;

	transition = gTransitionWork;
	{
		struct BattlePlan *savedPlan = plan;

		/* FAKEMATCH: the temporary null assignment and restoration end the
		 * native first CSE block, so the first plan read reloads its stack
		 * carrier. Both conditional arms disappear from the generated code.
		 * Without them, 288 of 289 instructions remain and 17 rows differ. */
		if (transition)
			plan = NULL;
		if (transition)
			plan = savedPlan;
	}
	object = BattleObjGet(plan->aid)->object;
	z = object->z;
	x = object->x;
	angle = (u16)ArcTan2(x, z);
	adjusted = angle - 0x2000;
	if (plan->aid > 7)
		adjusted = angle + 0x6000;
	adjusted &= 0x7fff;
	facingAngle = (adjusted - 0x2000) / 2 + 0x2000;

	if (transition->targetYaw == facingAngle)
	{
		transition->targetYaw = facingAngle;
		WaitFrames(5);
	}
	else
	{
		transition->targetYaw = facingAngle;
		WaitFrames(10);
	}

	BattlePres_SetActorModes(0, 0);
	BattlePres_BuildTargetList(plan, &work);
	if (work.kind == 0x87)
		PartyStatusDrawFar(gBattleWork->partyStatus & ~1);

	unit = UnitGetFar(work.actor);
	UnitGetFar(work.actors[0]);
	scripted = plan->targetCrit[0];
	direct = plan->targetAdj[0] == 0;

	animation = GetMotionRecord(BattleObjGet(plan->aid)->object, 0);
	divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->animId, 2, 1);
	BattleMotion_ApproachTarget(
		work.actor, work.actors[0], divisor, Battle_GetSpriteFlagHighBits(unit->classId) << 16);
	ObjectDispatch_ApplyValueToChildrenFar(
		(struct DispatchObject *)BattleObjGet(work.actor)->object, 16);
	BattleObjGet(work.actors[0]);

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
		BattleEvPush(0, plan->targetIds[0]);
		BattleEvPush(4, MsgNimblyDodgesBlow);
		BattleEv_DispatchQueued();
		ObjMotRst(work.actors[0]);
	}
	else
	{
		phase = 0;
		work.flags = 0;
		if (plan->presentationFlags != 0)
			work.flags = 1;

		if (scripted != 0)
		{
			work.kind += 200;
			phase = 1;
			transition->flag = 1;
			position[0] = work.actor;
			position[1] = work.firstTarget;
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
					second = work.firstTarget;
					BattlePres_SetupTransitionAtPairMidpoint(
						first, second, index * 30 / divisor + 100);
				}
				WaitFrames(1);
			}
		}

		Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
		if (work.kind != 0)
		{
			if (plan->presentationFlags & 0x4000)
				BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
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

s32 BattlePres_RunSimple(struct BattlePlan *input, s32 flags)
{
	struct BattlePresentationWork		 work;
	struct BattlePlan						*savedInput;
	struct MotionObject					*object;
	struct AnimationObject				*animation;
	struct BattlePresentationTransition *transition;
	s32									 angle;
	s32									 adjusted;
	s32									 facingAngle;
	s32									 x;
	s32									 z;
	s32									 divisor;
	s32									 scripted;

	transition = gTransitionWork;
	savedInput = input;
	object = BattleObjGet(savedInput->aid)->object;
	z = object->z;
	x = object->x;
	angle = (u16)ArcTan2(x, z);
	adjusted = angle - 0x2000;
	if (savedInput->aid > 7)
		adjusted = angle + 0x6000;
	adjusted &= 0x7fff;
	facingAngle = (adjusted - 0x2000) / 2 + 0x2000;

	if (transition->targetYaw == facingAngle)
	{
		transition->targetYaw = facingAngle;
		WaitFrames(5);
	}
	else
	{
		transition->targetYaw = facingAngle;
		WaitFrames(20);
	}

	BattlePres_SetActorModes(0, 0);
	BattlePres_BuildTargetList(savedInput, &work);
	UnitGetFar(work.actor);
	UnitGetFar(savedInput->targetIds[0]);

	scripted = flags & 2;
	animation = GetMotionRecord(BattleObjGet(savedInput->aid)->object, 0);
	divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->animId, 2, 1);
	BattleMotion_ApproachTarget(work.actor, savedInput->targetIds[0], divisor, 0);
	ObjectDispatch_ApplyValueToChildrenFar(
		(struct DispatchObject *)BattleObjGet(work.actor)->object, 16);
	BattleObjGet(savedInput->targetIds[0]);

	if (savedInput->targetIds[0] <= 7)
		work.side = 1;
	else
		work.side = 0;
	if (scripted != 0)
	{
		WaitFrames(10);
		BattleMotion_ResetObjectAtScaledAnchor(savedInput->targetIds[0]);
		WaitFrames(2);
		WaitFrames(4);
		WaitFrames(10);
		ObjMotRst(savedInput->targetIds[0]);
	}
	else
	{
		BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
		BattleEv_DispatchQueued();
		ObjMotRst(savedInput->targetIds[0]);
	}
	ObjMotRst(work.actor);
	return 0;
}

s32 BattlePres_RunItem(struct BattlePlan *plan, s32 unused, struct BattleActionRecord *selection)
{
	struct BattlePresentationWork		 work;
	struct MotionObject					*object;
	s32									 i;
	s32									 item;
	struct BattleUnit						*unit;
	struct ItemDefinition				*def;
	struct BattlePresentationTransition *transition = gTransitionWork;
	s32									 facing;

	if (plan->aid > 4)
		facing = -0x2000;
	else
		facing = 0x2000;
	if (transition->targetYaw != facing)
		transition->targetYaw = facing;
	BattlePres_BuildTargetList(plan, &work);
	BattlePres_SetActorModes(0, 0);
	object = BattleObjGet(work.actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
	ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
	if (plan->targetIds[0] <= 7)
		work.side = 1;
	else
		work.side = 0;

	for (i = 0; i != work.count; i++)
	{
		struct AnimationObject *animation = GetMotionRecord(BattleObjGet(work.actors[i])->object, 0);
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
			struct BattleWork *battle = gBattleWork;

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
		if (plan->presentationFlags & 0x4000)
			BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
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

	unit = UnitGetFar(selection->unitId);
	item = unit->inventory[selection->parameter];
	def = Item_GetDirectFar(item);
	if (def->useType == 1)
	{
		s32 result = Inventory_RemoveFar(selection->unitId, selection->parameter);
		s32 index = selection->parameter;
		if (result == 2)
		{
			struct BattleWork *battle = gBattleWork;
			u32				row;

			for (row = 0; row < 20; row++)
			{
				if (battle->actions[row].command == 2
					&& battle->actions[row].unitId == selection->unitId)
				{
					if (battle->actions[row].parameter == index)
						battle->actions[row].parameter = -1;
					else if (battle->actions[row].parameter > index)
						battle->actions[row].parameter--;
				}
			}
		}
	}
	else if (def->useType == 2)
	{
		if ((BattleRandomFar() & 7) == 0)
		{
			BattleEvPush(BATTLE_EVENT_ITEM, unit->inventory[selection->parameter]);
			BattleEvPush(BATTLE_EVENT_MESSAGE, MsgUsedItemBroke);
			Inventory_BreakFar(selection->unitId, selection->parameter);
			BattleEv_DispatchQueued();
		}
	}
	else if (def->useType == 4)
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
	/* FAKEMATCH: the mask is a value and the first frame is read into a temporary: loop.c keeps 255 in sl across the call and the frame is or-ed with it, as the native. 2026-10-13: a constant (child->frame = 0xff, |= 0xff, |= -1, = -1) stores movs #255, 18 of 46 differ; child->frame |= mask or = child->frame | mask without the temporary, with a u8, s32 or u32 mask, differ 28 (registers and the order of the two stores). */
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

s32 BattlePres_RunPsynergyCast(struct BattlePlan *plan, s32 mode)
{
	struct BattlePresentationWork		 work;
	struct BattlePresentationTransition *transition = gTransitionWork;
	struct MotionObject					*object;
	s32									 scripted;
	s32									 i;

	if (plan->presentationFlags & 0x40000)
	{
		transition->targetYaw = plan->aid <= 7 ? -0x2000 : 0x5000;
		transition->frames = 60;
	}
	else
	{
		struct MotionObject *actor = BattleObjGet(plan->aid)->object;
		s32					 angle = (u16)ArcTan2(actor->x, actor->z);
		s32					 current;

		if (plan->aid <= 7)
			current = angle - 0x1800;
		else
			current = angle + 0x1800;
		current = (s16)current;
		current += ((plan->aid <= 7 ? 0x2000 : -0x2000) - current) * 3 / 4;
		if (!((plan->aid <= 7) ^ (plan->targetIds[0] <= 7)))
			current = plan->aid <= 7 ? 0x2400 : -0x2400;
		if (transition->targetYaw != current)
			transition->targetYaw = current;
	}
	if (plan->presentationFlags & 0x80000)
	{
		transition->targetYaw = plan->aid <= 7 ? -0x2000 : 0x2000;
		transition->frames = 60;
	}

	BattlePres_BuildTargetList(plan, &work);
	scripted = mode & 1;
	if (scripted)
		work.flags = 1;
	BattlePres_SetActorModes(0, 0);
	PartyStatusDrawFar(gBattleWork->partyStatus & ~1);
	object = BattleObjGet(work.actor)->object;
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
	ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
	AudioCommand_PlayFar(0x9a);
	if (mode & 2)
		BattleFx_PlayUnitElementEffect(work.actor, plan->element, 1, 0);
	else if (!scripted)
		BattleFx_PlayUnitElementEffect(work.actor, plan->element, 0, 0);
	if (plan->targetIds[0] <= 7)
		work.side = 1;
	else
		work.side = 0;

	for (i = 0; i != work.count; i++)
	{
		struct AnimationObject *animation = GetMotionRecord(BattleObjGet(work.actors[i])->object, 0);
		s32						j;

		for (j = 0; j != animation->count - 1; j++)
			work.params[i][j] = animation->entries[j]->param;
	}
	if (plan->failure != 0)
	{
		if (plan->failure == 1)
		{
			BattleEvPush(0, plan->aid);
			BattleEvPush(4, MsgPsynergyBlocked);
		}
		else
		{
			BattleEvPush(4, MsgCastLacksPp);
		}
		BattleEv_DispatchQueued();
		BattlePres_RunWithZeroArguments();
	}
	else
	{
		Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
		if (work.kind)
		{
			if (plan->presentationFlags & 0x4000)
				BattleFx_DispatchByIdRangeFar((struct BattleEffectArgument *)&work);
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

void BattleMotion_RunValueSequence(s32 id, s32 unused0, s32 unused1)
{
	u16	 selection[2];
	u16 *selected;
	s32	 target;
	s32	 remaining;

	UnitGetFar(id);
	ObjectDispatch_ApplyArgumentToChildrenFar(BattleObjGet(id)->object, 5);

	selected = selection;
	remaining = 1;
	do
	{
		target = 0xff;
		selected[1] = target;
		selected[0] = id;
		BattleLayout_HighlightPartyPanelsFar(selected);
		BattleMotion_SetRecordChildValues(BattleObjGet(id)->object, 7);
		WaitFrames(2);

		selected[0] = id;
		BattleLayout_HighlightPartyPanelsFar(selected);
		BattleMotion_SetRecordChildValues(BattleObjGet(id)->object, BattleMotion_GetSlotPalette(id));
		WaitFrames(2);
		remaining--;
	} while (remaining >= 0);

	PartyStatusDrawFar(gBattleWork->partyStatus);
}

//--------------------------------------------------------------------------
// actor.c
// バトル：倒れたユニットの絵と並びの後しまつ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ユニットを味方か敵の並びから外し、行動の予定からも消す
// id : ユニット番号
//--------------------------------------------------------------------------
void BattleActor_RemoveFromLists(s32 id)
{
	struct BattleWork *work;
	s32				i;
	u32				j;
	s32				unit;

	// 味方の並び
	work = gBattleWork;
	UnitGetFar(id)->side = BATTLE_UNIT_ABSENT;
	for (i = 0;; i++)
	{
		if (work->partyUnits[i] == id)
		{
			work->partyUnits[i] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		if (work->partyUnits[i] == BATTLE_UNIT_LIST_END)
			break;
	}

	// 敵の並び
	/* FAKEMATCH: the one-pass block around the enemy scan keeps the party loop's shape: its
	 * REMOVED constant loaded once ahead of it and the store inside it; without the block
	 * the party loop's first test is peeled ahead of it (differ 26 in tbs-en). The enemy
	 * scan is a label loop: as for (;;) left by return it walks a pointer and keeps 0xfe
	 * in r4 (differ 24), as a do-while on the end mark differ 51. */
	do
	{
		j = 0;
	again:
		unit = work->enemyIds[j];
		if (unit == id)
		{
			work->enemyIds[j] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		j++;
		if (unit == BATTLE_UNIT_LIST_END)
			return;
		goto again;
	} while (0);

removed:
	EnemyCountRelease(id);
	for (j = 0; j < 20; j++)
	{
		if (work->actions[j].unitId == id)
			work->actions[j].unitId = BATTLE_UNIT_LIST_END;
	}
}

//--------------------------------------------------------------------------
// 倒れたユニットの絵：味方はアニメ 4、敵はアニメ 5 にする
// 敵はさらに 4 フレーム待って並びから外し、そのあと絵を 1 ドットずつ消す
// id : ユニット番号
//--------------------------------------------------------------------------
void BattleMotion_InitializeActorRecords(s32 id)
{
	void				   *recs[4];
	struct BattleUnit		   *unit;
	struct AnimationObject *rec;
	struct AnimationEntry  *ent;
	s32						n;

	unit = UnitGetFar(id);
	n = 0;
	while ((rec = GetMotionRecord(BattleObjGet(id)->object, n)) != 0)
	{
		if (unit->side != BATTLE_UNIT_ENEMY)
			AnimationObjects_SelectAnimationFar(rec, 4);
		else
			AnimationObjects_SelectAnimationFar(rec, 5);
		n++;
	}

	if (unit->side == BATTLE_UNIT_ENEMY)
	{
		n = 0;
		while ((rec = GetMotionRecord(BattleObjGet(id)->object, n)) != 0)
		{
			ent = rec->entries[0];
			recs[n] = rec;
			ent->param = 6;
			ent->frame = 0xff;
			n++;
		}
		WaitFrames(4);
		BattleActor_RemoveFromLists(id);
		Map_RenderAllAnimatedTileFramesFar(recs, n); // 1 ドットずつ消す
		ActivateBattleObjectSlot(id);
	}
}

struct SlotArray
{
	s16 items[64];
};

s32 BattleTarget_SelectRandomPosition(s32 requireLivingUnit)
{

	u16				  positions[6];
	struct SlotArray *order;
	s16				 *entry;
	u16				 *cursor;
	s32				  value;
	s32				  count;
	s32				  index;
	s32				  slot;
	s32				  tail;
	s32				  offset;

	count = 0;
	order = (struct SlotArray *)gBattleWork;

	if (requireLivingUnit != 0)
	{
		/* FAKEMATCH: a loop that runs once, left by its closing break. Written plain, without the loop, the function differs 35 instructions in EN (85 -> 84): the first branch's zero offset takes r2 for r1 and the else branch sets up its pointers in another order. */
		for (;;)
		{
			index = 0;
			slot = (u32) & ((struct BattleWork *)0)->partyUnits / sizeof(s16);
			if (order->items[slot] != 255)
			{
				entry = order->items;
				do
				{
					value = entry[slot];
					if (value != 254)
					{
						if (UnitGetFar(value)->hp != 0)
						{
							positions[count] = index | 0x100;
							count++;
						}
					}
					slot++;
					index++;
				} while (entry[slot] != 255);
			}
			break;
		}
	}
	else
	{
		index = 0;
		slot = (u32) & ((struct BattleWork *)0)->enemyIds / sizeof(s16) - 1;
		tail = (u32) & ((struct BattleWork *)0)->enemyIds / sizeof(s16) - 1;
		offset = tail * 2;
		entry = (order->items + 1);
		if (*(s16 *)((char *)entry + offset) != 255)
		{
			cursor = entry;
			do
			{
				if ((s16)cursor[slot] != 254)
				{
					positions[count] = index | 0x180;
					count++;
				}
				slot++;
				tail++;
				index++;
			} while (entry[tail] != 255);
		}
	}

	if (count == 0)
		return 0;
	return positions[(Random16() * count) >> 16];
}

#define COUNT_PARTIAL_CURES(unit, count) \
	{                                    \
		if ((unit)->delusion != 0)      \
			(count)++;                   \
		if ((u8)(unit)->confusion != 0)     \
			(count)++;                   \
		if ((unit)->charm != 0)          \
			(count)++;                   \
		if ((unit)->sleep != 0)         \
			(count)++;                   \
		if ((unit)->psySeal != 0)          \
			(count)++;                   \
		if ((unit)->grimReaper != 0)         \
			(count)++;                   \
	}

#define COUNT_ALL_CURES(unit, count)          \
	{                                         \
		COUNT_PARTIAL_CURES((unit), (count)); \
		if ((unit)->haunt != 0)              \
			(count)++;                        \
		if ((unit)->poison != 0)                \
			(count)++;                        \
	}

#define COUNT_POSITIVE_MODIFIERS(unit, count) \
	{                                         \
		if ((unit)->atkLv > 0)               \
			(count)++;                        \
		if ((unit)->defLv > 0)               \
			(count)++;                        \
		if ((unit)->resLv > 0)               \
			(count)++;                        \
		if ((unit)->elementLevels[0] > 0)            \
			(count)++;                        \
		if ((unit)->elementLevels[1] > 0)            \
			(count)++;                        \
		if ((unit)->elementLevels[2] > 0)            \
			(count)++;                        \
		if ((unit)->elementLevels[3] > 0)            \
			(count)++;                        \
	}

#define CHECK_EFFECT_TARGET(unit, action, applies, damageClass)    \
	{                                                               \
		(applies) = 0;                                              \
		switch ((action)->effect)                                      \
		{                                                           \
		case EFFECT_ATK_UP2:                                           \
		case EFFECT_ATK_UP1:                                           \
			if ((unit)->atkLv + 1 <= 4)                            \
				(applies) = 1;                                      \
			if ((unit)->atkCnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFFECT_ATK_DOWN2:                                         \
		case EFFECT_ATK_DOWN1:                                         \
			if ((unit)->atkLv - 1 >= -4)                           \
				(applies) = 1;                                      \
			if ((unit)->atkCnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFFECT_DEF_UP2:                                           \
		case EFFECT_DEF_UP1:                                           \
			if ((unit)->defLv + 1 <= 4)                            \
				(applies) = 1;                                      \
			if ((unit)->defCnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFFECT_DEF_DOWN2:                                         \
		case EFFECT_DEF_DOWN1:                                         \
			if ((unit)->defLv - 1 >= -4)                           \
				(applies) = 1;                                      \
			if ((unit)->defCnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFFECT_RES_UP2:                                           \
		case EFFECT_RES_UP1:                                           \
			if ((unit)->resLv + 1 <= 4)                            \
				(applies) = 1;                                      \
			if ((unit)->resCnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFFECT_RES_DOWN2:                                         \
		case EFFECT_RES_DOWN1:                                         \
			if ((unit)->resLv - 1 >= -4)                           \
				(applies) = 1;                                      \
			if ((unit)->resCnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFFECT_CURE_POISON:                                       \
			if ((unit)->poison != 0)                                  \
				(applies) = 1;                                      \
			break;                                                  \
		case EFFECT_CURE_PART:                                         \
			COUNT_PARTIAL_CURES((unit), (applies));                 \
			break;                                                  \
		case EFFECT_HEAL_60:                                           \
		case EFFECT_HEAL_30:                                           \
			if ((unit)->hp < (unit)->maxHp)                        \
				(applies) = 1;                                      \
			break;                                                  \
		case EFFECT_BUFF_CLEAR:                                        \
			COUNT_POSITIVE_MODIFIERS((unit), (applies));            \
			break;                                                  \
		case EFFECT_CURE_ALL:                                          \
			COUNT_ALL_CURES((unit), (applies));                     \
			break;                                                  \
		case EFFECT_DEATH_CURSE:                                       \
			if ((unit)->grimReaper == 0)                                \
				(applies) = 1;                                      \
			break;                                                  \
		case EFFECT_POISON:                                            \
			if ((unit)->poison == 0)                                  \
				(applies) = 1;                                      \
			break;                                                  \
		case EFFECT_VENOM:                                             \
			if ((unit)->poison <= 1)                                  \
				(applies) = 1;                                      \
			break;                                                  \
		case EFFECT_STUN:                                              \
			if ((unit)->stun == 0)                               \
				(applies) = 1;                                      \
			break;                                                  \
		case EFFECT_SLEEP:                                             \
			if ((unit)->sleep == 0)                                \
				(applies) = 1;                                      \
			break;                                                  \
		case EFFECT_EVIL_SPIRIT:                                       \
			if ((unit)->haunt == 0)                                \
				(applies) = 1;                                      \
			break;                                                  \
		case EFFECT_REVIVE_FULL:                                       \
		case EFFECT_REVIVE_HALF:                                       \
		case EFFECT_REVIVE_80:                                         \
			if ((unit)->hp == 0)                                    \
				(applies) = 100;                                    \
			break;                                                  \
		case 0:                                                     \
		case 1:                                                     \
		case 2:                                                     \
			break;                                                  \
		default:                                                    \
			(applies) = 1;                                          \
			break;                                                  \
		}                                                           \
		if ((unit)->hp == 0 && BattleEffectRevCheckFar((action)->effect) == 0) \
			(applies) = 0;                                          \
		if ((applies) == 0)                                         \
		{                                                           \
			(damageClass) = ((action)->targetFlag & 0x0f) - 1;        \
			switch (damageClass)                                   \
			{                                                       \
			case 2:                                                 \
			case 3:                                                 \
				if ((unit)->hp != 0)                                \
					(applies)++;                                    \
				break;                                              \
			case 7:                                                 \
			case 8:                                                 \
				if ((action)->power != 0 && (unit)->hp != 0)          \
					(applies)++;                                    \
				break;                                              \
			case 9:                                                 \
				if ((unit)->pp != 0)                                \
					(applies)++;                                    \
				break;                                              \
			case 0:                                                 \
				if ((unit)->hp != 0 && (unit)->hp < (unit)->maxHp) \
					(applies)++;                                    \
				break;                                              \
			case 1:                                                 \
			case 4:                                                 \
			case 5:                                                 \
				if ((action)->power != 0 && (unit)->hp != 0)          \
					(applies)++;                                    \
				break;                                              \
			}                                                       \
		}                                                           \
	}

s32 BattleTarget_SelectForAction(s32 actorId, struct BattleAct *action)
{
	u16				orderPositions[6];
	s32				unitIds[6];
	s32				targetPositions[6];
	struct BattleWork *turnOrder;
	struct BattleUnit *unit;
	struct BattleUnit *nextUnit;
	struct BattleUnit *thisUnit;
	s32				candidateCount;
	s32				targetIndex;
	s32				targetCount;
	s32				innerIndex;
	s32				selected;
	s32				unitId;
	s32				value;
	s32				nextValue;
	s32				temp;
	s32				opposing;
	s32				damageClass;
	s32				applies;
	u32				roll;
	s16			   *slot;

	turnOrder = gBattleWork;
	targetCount = 0;
	candidateCount = 0;

	if (action->targetMode != 0)
	{
		opposing = 0;
		if (action->targetMode == 2 || action->targetMode == 4)
			opposing = 1;

		if (actorId < 0 || actorId > 7)
		{
			if (opposing != 0)
				goto scan_mirrored_order;
		}
		else if (opposing == 0)
		{
			goto scan_mirrored_order;
		}

		targetIndex = 0;
		while (turnOrder->partyUnits[targetIndex] != 255)
		{
			slot = &turnOrder->partyUnits[targetIndex];
			unitId = *slot;
			if (unitId != 254)
			{
				if (action->targetMode != 4 || unitId == actorId)
				{
					unitIds[candidateCount] = unitId;
					orderPositions[candidateCount] = 0x100;
					orderPositions[candidateCount] |= targetIndex;
					candidateCount++;
				}
			}
			targetIndex++;
		}
		goto scan_complete;

	scan_mirrored_order:
		targetIndex = 0;
		while (turnOrder->enemyIds[targetIndex] != 255)
		{
			unitId = turnOrder->enemyIds[targetIndex];
			if (unitId != 254)
			{
				if (action->targetMode != 4 || unitId == actorId)
				{
					unitIds[candidateCount] = unitId;
					orderPositions[candidateCount] = 0x180;
					orderPositions[candidateCount] |= targetIndex;
					candidateCount++;
				}
			}
			targetIndex++;
		}
	}

scan_complete:

	if (candidateCount == 0)
		return -2;

	for (targetIndex = 0; targetIndex < candidateCount; targetIndex++)
	{
		unit = UnitGetFar(unitIds[targetIndex]);
		CHECK_EFFECT_TARGET(unit, action, applies, damageClass);
		if (applies != 0)
		{
			unitIds[targetCount] = unitIds[targetIndex];
			targetPositions[targetCount] = orderPositions[targetIndex];
			targetCount++;
		}
	}

	if (targetCount == 0)
		return -1;

	if (action->targetMode == 1 && action->range == 1
		&& ((struct EnemyDefinition *)Owner_GetRecordFar(UnitGetFar(actorId)->classId))
				   ->targetStrategy
			   != 2
		&& (u32)((action->targetFlag & 0x0f) - 3) <= 2)
	{
		selected = -1;
		for (targetIndex = 0; targetIndex < targetCount; targetIndex++)
		{
			for (innerIndex = targetIndex; innerIndex < targetCount - 1; innerIndex++)
			{
				thisUnit = UnitGetFar(unitIds[innerIndex]);
				nextUnit = UnitGetFar(unitIds[innerIndex + 1]);
				if (((struct EnemyDefinition *)Owner_GetRecordFar(UnitGetFar(actorId)->classId))
						->targetStrategy
					== 0)
				{
					value = thisUnit->hp;
					nextValue = nextUnit->hp;
				}
				else
				{
					value = thisUnit->maxHp;
					nextValue = nextUnit->maxHp;
				}
				if (value < nextValue)
				{
					temp = unitIds[innerIndex];
					unitIds[innerIndex] = unitIds[innerIndex + 1];
					unitIds[innerIndex + 1] = temp;
					temp = targetPositions[innerIndex];
					targetPositions[innerIndex] = targetPositions[innerIndex + 1];
					targetPositions[innerIndex + 1] = temp;
				}
			}
		}

		switch (targetCount)
		{
		case 1:
			selected = 0;
			break;
		case 2:
			roll = (11 * Random16()) >> 16;
			selected = 0;
			if (roll > 5)
				selected = 1;
			break;
		case 3:
			selected = (15 * Random16()) >> 16;
			if (selected <= 5)
				selected = 0;
			else if (selected <= 10)
				selected = 1;
			else
				selected = 2;
			break;
		case 4:
			selected = (18 * Random16()) >> 16;
			if (selected <= 5)
				selected = 0;
			else if (selected <= 10)
				selected = 1;
			else if (selected <= 14)
				selected = 2;
			else
				selected = 3;
			break;
		}

		if (selected >= 0)
			return targetPositions[selected];
	}
	return targetPositions[(Random16() * targetCount) >> 16];
}

//--------------------------------------------------------------------------
// actor_reset_runtime_fields.c
// バトル：状態変化の消去と、メッセージ送りの待ち
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 状態異常と能力の変化をすべて消す
// 戦闘不能になったときに呼ばれる
//--------------------------------------------------------------------------
s32 BattleActor_ResetRuntimeFields(s32 id)
{
	struct BattleUnit *unit;
	s8			   *lv;
	s32				n;
	u8				zero;

	unit = UnitGetFar(id);
	n = 3;
	zero = 0;
	lv = &unit->elementLevels[3];
	do
	{
		n--;
		*lv-- = zero;
	} while (n >= 0);

	unit->poison = 0;
	unit->atkCnt = 0;
	unit->atkLv = 0;
	unit->defCnt = 0;
	unit->defLv = 0;
	unit->resCnt = 0;
	unit->resLv = 0;
	unit->delusion = 0;
	unit->confusion = 0;
	unit->charm = 0;
	unit->stun = 0;
	unit->sleep = 0;
	unit->psySeal = 0;
	unit->refrain = 0;
	unit->reflect = 0;
	unit->haunt = 0;
	unit->grimReaper = 0;
	unit->unk142[0] = 0;
	unit->unk142[1] = 0;
	unit->ready = 0;
	unit->immobile = 0;
	unit->agiCnt = 0;
	unit->agiLv = 0;
	unit->challenge = 0;

	UnitCalcFar(id);
	return BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
}

//--------------------------------------------------------------------------
// メッセージ送りの矢印
//--------------------------------------------------------------------------
#define KEY_ADVANCE (KEY_A | KEY_B | KEYS_SHOULDERS) // 送りのキー

// OAM の一枚
// A second view of struct BattlePromptSprite, kept: plain halfwords, where its bit-fields
// change BattlePresentation_WaitForAdvance (differ 50).
struct AdvanceSprite
{
	struct AdvanceSprite *next;
	u16					  attr0;
	u16					  attr1;
	u16					  attr2;
	u16					  attr3;
};

//--------------------------------------------------------------------------
// ウィンドウの位置に矢印を出し、送りのキーを待つ
// B を押し続けるか、送りのキーを押すと進む
// 16 フレームを過ぎれば、押し続けていても進む
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForAdvance(void)
{
	struct AdvanceSprite	   *spr;
	struct AdvanceSprite		sprite;
	s32							frame;
	s32							slot;
	s32							pat;
	struct UiWindow			   *window;
	struct BattleDisplayOffset *offset;
	s32							attr2;
	s32							attr1;
	s32							x;
	s16							tile;

	while (!UiWork_IsCompleteFar())
		WaitFrames(1);
	spr = &sprite;
	slot = Resource_LoadIntoFreeSlot(128);
	for (frame = 0;; frame++)
	{
		// 4 フレームごとに 8 枚のパターンを回す
		pat = ((gFrameCount >> 2) & 7) * 128 + (s32)gBattlePres_AdvanceArrowTiles;
		window = gBattleDisplayWork->window;
		offset = gBattleDisplayWork->offset;
		QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
		QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
		Io_Write16(16, &REG_BLDALPHA);
		*(u32 *)&spr->attr0 = 0xa400;
		*(u32 *)&spr->attr2 = 0;
		tile = Resource_GetBuffer(slot, pat) & 0x3ff;
		attr2 = spr->attr2;
		spr->attr2 = (attr2 & ~0x3ff) | tile;
		// ウィンドウの位置はキャラ単位、オフセットは 1/256 ドット
		x = (u16)((window->x * 8 + (offset->x >> 8) + 4) & 0x1ff);
		attr1 = spr->attr1;
		spr->attr1 = (attr1 & ~0x1ff) | x;
		// 上下に 2 ドットまでゆれる
		*(u8 *)&spr->attr0 =
			Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 + (offset->y >> 8) + 6;
		Runtime_PushSlotEntry(spr, 240);
		if ((gKeysHeld & KEY_B) || (gKeyState & KEY_ADVANCE)
			|| (frame > 15 && (gKeysHeld & KEY_ADVANCE)))
			break;
		WaitFrames(1);
	}
	AudioCommand_PlayFar(111);
	Resource_ResetEntry(slot);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// 指定の位置でカーソルを動かし、送りのキーを待つ
// x, y : ドット
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y)
{
	struct MenuPoint	 pos;
	s32					 pat;
	struct CursorSprite	 sprite;
	struct CursorSprite *spr;
	s32					 slot;

	pat = Ui_GetTableWordZeroFar(0);
	pos.x = x;
	pos.y = y;
	while (!UiWork_IsCompleteFar())
		WaitFrames(1);
	spr = &sprite;
	slot = Resource_LoadIntoFreeSlot(128);
	while (1)
	{
		QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
		QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
		Io_Write16(16, &REG_BLDALPHA);
		((s32 *)spr)[1] = MENU_SPRITE_SIZE_16;
		((s32 *)spr)[2] = 0;
		spr->tile = Resource_GetBuffer(slot, pat);
		// 4 フレームごとに右上へゆらす
		spr->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
		spr->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
		Runtime_PushSlotEntry(spr, 240);
		if (gKeyState & KEY_ADVANCE)
			break;
		WaitFrames(1);
	}
	Resource_ResetEntry(slot);
	WaitFrames(1);
}

void BattleEv_MarkDisplay(void)
{
	gBattleDisplayWork->marked = 1;
}

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
	// 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
	struct BattleUnit *creature;
	struct BattleObj  *runtime;

	creature = UnitGetFar(arg0);
	if (creature->side == 1)
	{
		Owner_UpdateRatioPairFar(creature, 0);
		BattleActor_RemoveFromLists(arg0);
		ActivateBattleObjectSlot(arg0);
		runtime = BattleObjGet(arg0);
		ObjectDispatch_ReleaseFar((struct DispatchObject *)runtime->object);
		runtime->object = 0;
		runtime->active = 0;
	}
}

void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand)
{
	runtime->flags |= 1;
}

u32 BattleEv_DispatchQueued(void)
{
	struct BattleEventState *runtime = &gBattleWork->events;
	struct BattleEventQueue *queue = &runtime->queue;
	s32						 i;

	for (i = 0; i < queue->count; i++)
	{
		u8 opcode = queue->opcodes[i];
		if (opcode <= BATTLE_EVENT_SCR)
			switch (opcode)
			{
			case BATTLE_EVENT_SCR:
				Battle_SetRuntimeFlagBit0(runtime, queue->operands[i]);
				break;
			case BATTLE_EVENT_OBJ_EFFECT:
				BattleActor_DestroyTemporaryObject(queue->operands[i]);
				break;
			case BATTLE_EVENT_UNIT:
				UiWork_PushValueSlotFar(queue->operands[i], 1);
				break;
			case BATTLE_EVENT_NUM:
				UiWork_PushValueSlotFar(queue->operands[i], 5);
				break;
			case BATTLE_EVENT_ITEM:
				UiWork_PushValueSlotFar(queue->operands[i] & ITEM_ID_MASK, 2);
				break;
			case BATTLE_EVENT_ACT:
				UiWork_PushValueSlotFar(queue->operands[i] & OWNER_ACTION_ID_MASK, 4);
				break;
			case BATTLE_EVENT_MARK:
				gBattleDisplayWork->marked = 1;
				break;
			case BATTLE_EVENT_CLEAR:
				UiWork_ClearValueNameTablesFar();
				break;
			case BATTLE_EVENT_MESSAGE:
				if ((s32)queue->operands[i] >= 0)
					UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
				BattlePresentation_WaitForAdvance();
				UiWork_ClearValueNameTablesFar();
				break;
			case BATTLE_EVENT_MESSAGE_ADD:
				if ((s32)queue->operands[i] >= 0)
					UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
				UiWork_ClearValueNameTablesFar();
				break;
			case BATTLE_EVENT_OBJ_BEGIN:
				if (runtime->pendingCue > 0)
					AudioCommand_PlayFar(runtime->pendingCue);
				BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
				break;
			case BATTLE_EVENT_OBJ_RESET:
				BattleEnemy_RecordDefeat(queue->operands[i], runtime->flags);
				BattleActor_ResetRuntimeFields(queue->operands[i]);
				BattleMotion_InitializeActorRecords(queue->operands[i]);
				break;
			case BATTLE_EVENT_REDRAW:
				PartyStatusDrawFar(gBattleWork->partyStatus);
				break;
			case BATTLE_EVENT_OBJ_END:
				BattleUnit_BuildStatusFlags(queue->operands[i], BattleObjGet(queue->operands[i]));
				BattlePres_SetActorModeAndAction(queue->operands[i]);
				break;
			}
	}
	return BattleEventRuntime_Reset();
}

u32 BattleEvPush(u32 opcode, u32 operand)
{
	struct BattleEventQueue *queue = &gBattleWork->events.queue;
	u32						 index = queue->count;

	queue->opcodes[index] = opcode;
	queue->operands[index] = operand;
	queue->count = index + 1;
	return opcode;
}

//--------------------------------------------------------------------------
// tgtexec.c
// バトル：対象ごとの効果処理
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 対象が倒れていても追加効果を出す効果なら 1 を返す
// うばう効果は、与えたダメージを行動側に戻すため
// effect : 効果番号
//--------------------------------------------------------------------------
s32 BattleEffectDeadCheck(s32 effect)
{
	switch (effect)
	{
	case 31: // HPをうばう
	case 32: // PPをうばう
	case 60: // HPをうばう（半分）
	case 69: // PPをうばう（1/10）
		return 1;
	default:
		return 0;
	}
}

//--------------------------------------------------------------------------
// 対象の判定・効果計算・メッセージ登録
//--------------------------------------------------------------------------
#define ALLY 2 // 味方

#define ALLY_CHECK(id) ((id) >= 0 && (id) < 8)

#define STEP_LIM(v)   \
	{                 \
		if ((v) < -4) \
			(v) = -4; \
		if ((v) > 4)  \
			(v) = 4;  \
	}

#define ELEMENT_BONUS()                             \
	{                                           \
		if (element != 4)                           \
			bonus = power - target->element[element].res; \
	}

#define MESSAGE_SIDE(m, e)       \
	{                        \
		if (ALLY_CHECK(tid)) \
			MESSAGE_PUSH(m);     \
		else                 \
			MESSAGE_PUSH(e);     \
	}

#define GUARD_CUT()               \
	{                             \
		guard = target->guardLv; \
		if (guard != 0)           \
		{                         \
			if (guard == 1)       \
				dmg /= 2;         \
			else                  \
				dmg /= 10;        \
		}                         \
	}

#define RES_STEP(d, v, m)         \
	{                             \
		target->resLv += (d);    \
		STEP_LIM(target->resLv); \
		BattleEvPush(BATTLE_EVENT_NUM, (v));  \
		MESSAGE_PUSH(m);              \
		target->resCnt = 7;      \
	}

#define STAT_SET(f, m) \
	{                  \
		MESSAGE_PUSH(m);   \
		(f) = 7;       \
	}

//--------------------------------------------------------------------------
// わざの効果を対象ひとりにかける
// plan : 行動と対象の情報
// slot : 対象配列の位置
//--------------------------------------------------------------------------
s32 BattleTargetExec(struct BattlePlan *plan, s32 slot)
{
	s32				dist;
	struct BattleAct  *action;
	struct BattleUnit *actor;
	s32				aid;
	s32				abilityId;
	s32				bonus;
	struct BattleWork *work;
	s32				half;
	s32				adj;
	s32				dealt;
	s32				crush;
	s32				hit;
	s32				crit;
	s32				skip;
	s32				type;
	s32				aff;
	struct BattleUnit *bak;
	s32				power;
	s32				hp;
	s32				rate;
	s16			   *cmd;
	s32				tid;
	s32				element;
	struct BattleUnit *target;
	u32				n;
	s32				value;
	s32				dmg;
	s32				scale;
	s32				pass;
	s32				guard;
	s16				list[8];
	s32				cnt;
	s32				tmp;
	s32				size;

	// 変化量の計算用に、対象の状態を保存する
	bonus = 0;
	work = gBattleWork;
	half = 0;
	dealt = 0;
	crush = 0;
	skip = 0;
	aff = 0;
	size = sizeof(struct BattleUnit);
	bak = MemAlloc(size);

	aid = plan->aid;
	tid = plan->targetIds[slot];
	abilityId = plan->abilityId;
	element = plan->element;
	adj = plan->targetAdj[slot];
	crit = plan->targetCrit[slot];

	action = BattleActGet(abilityId);
	actor = UnitGetFar(aid);
	target = UnitGetFar(tid);
	Iwram_CopyWords(bak, target, size);

	// 範囲内での距離と属性の相性
	if (action->range != 255)
	{
		dist = plan->targetDist[slot];
		if (dist < 0)
			dist = -dist;
	}
	else
	{
		dist = 0;
	}

	if (element != 4)
	{
		s32 i;

		value = target->baseElement[element].res;
		for (i = 0; i < 4 && value >= target->baseElement[i].res; i++)
			;
		if (i == 4)
			aff = -1;

		for (i = 0; i < 4 && value <= target->baseElement[i].res; i++)
			;
		if (i == 4)
			aff = 1;
	}

	if (plan->element >= 0 && plan->element <= 3 && plan->cmd != 2)
		power = actor->element[plan->element].power;
	else
		power = 100;
	cmd = &plan->cmd;

	if (plan->cmd == 5 && (plan->element >= 0 && plan->element <= 3) && aff > 0)
	{
		s32 chance;

		chance = power - target->element[plan->element].res;
		chance += 30;
		chance *= 65536 / 100;
		if (chance > (BattleRandomFar() & 0xffff))
			BattleEvPush(BATTLE_EVENT_SCR, 5);
	}

	// 命中判定と、効果に応じた事前処理
	type = action->targetFlag & 15;
	{
		s32 res;

		res = plan->targetHit[slot];
		if (res == -1)
			hit = BattleHitCheck(aid, tid, element, action->effect, gHitTable[dist]);
		else
			hit = res;
	}
	if (action->effect == 50 || action->effect == 51)
	{ /* なかまをよぶ */
		s32 cls;
		s32 ent;

		cls = actor->classId;
		ent = EnemySlotGet();
		if (action->effect == 51)
			cls = BattleGroupRandom(work->enemyGroup);
		if (hit != 0 && EnemyCheck(cls) != 0 && ent >= 0)
		{
			s32 chg;

			chg = EnemyCountDecrement(cls, 1);
			if (chg & 0x8000)
				EnemyCountClear(cls);
			BattleUnitSetFar(ent, cls, chg & 0x7fff);
			{
				s32 i;

				for (i = 0; i < 6; i++)
				{
					if (work->enemyIds[i] == 254)
					{
						work->enemyIds[i] = ent;
						break;
					}
					if (work->enemyIds[i] == 255)
					{
						work->enemyIds[i] = ent;
						work->enemyIds[i + 1] = 255;
						break;
					}
				}
			}
			EnemyUpdate();
			{
				struct BattleObj *obj;

				obj = BattleObjGet(ent);
				BattleObjMake(obj, ent, obj->anchorX / 0x10000, obj->anchorZ / 0x10000);
			}
			BattleObjPut();
			{
				s32 num;

				num = BattleEnemyList(list);
				for (cnt = 0; cnt < num; cnt++)
					ObjMotRst((u16)list[cnt]);
			}
			BattleEvPush(BATTLE_EVENT_UNIT, ent);
			if (abilityId != 503)
				MESSAGE_PUSH(MsgAppears);
			else
				MESSAGE_PUSH(MsgSplitOff);
		}
		else if (abilityId == 503)
		{
			MESSAGE_PUSH(MsgSplitFail);
		}
		else
		{
			MESSAGE_PUSH(MsgNoOneCame);
		}
	}

	if (hit != 0)
	{
		if (action->effect == 53)
		{
			hit = 0;
			for (n = 0; n < 20; n++)
			{
				if (work->actions[n].unitId == tid)
				{
					hit = 1;
					break;
				}
			}
		}
		else if (action->effect == 35)
		{
			half = 1;
		}
		else if (action->effect == 34)
		{
			crush = 1;
		}
		else if (action->effect == 27)
		{
			skip = 1;
		}
		else if (action->effect == 55)
		{
			if (actor->hp != 0)
				BattleEvPush(BATTLE_EVENT_OBJ_EFFECT, aid);
		}
		else if (action->effect == 32)
		{
			if (target->pp != 0)
				type = 10;
			else
				hit = 0;
		}
	}

	//------------------------------------------------------------------
	// ダメージ・回復
	//------------------------------------------------------------------
	if (skip == 0 && (target->hp != 0 || BattleEffectRevCheckFar(action->effect) != 0))
	{
		s32 pp;
		s32 cur;
		s32 ap;

		switch (type)
		{
		case 3: /* こうげき */
		case 4:
		{
			s32 def;

			hp = target->hp;
			def = target->def;
			scale = def;
			if (half != 0)
				scale = (u32)def >> 1;
			pass = 1;
			do
			{
				ELEMENT_BONUS();
				if (pass == 0)
					bonus = 0;
				ap = action->power;
				if (type == 4)
					dmg = BattleAtkCalc(actor->atk, scale, 0, bonus) * ap / 10;
				else
					dmg = BattleAtkCalc(actor->atk, scale, ap, bonus);
				dmg *= adj;
				if (crit != 0)
				{
					if (crit == 1)
						dmg = dmg * 5 / 4;
					else
						dmg = dmg * 3 / 2;
					dmg += (target->level / 5) + 6;
					if (pass == 0)
					{
						BattleEvPush(BATTLE_EVENT_MARK, 0);
						{
							s32 message;

							message = MsgCritical;
							if (ALLY_CHECK(tid))
								message += 1;
							BattleEvPush(BATTLE_EVENT_MESSAGE_ADD, message);
						}
					}
				}
				dmg += BattleRandomFar() & 3;
				GUARD_CUT();
				if (dmg <= 0)
					dmg = 1;
				if (crush != 0)
				{
					if (dmg < hp - 1)
					{
						dmg = hp - 1;
						if (dmg <= 0)
							dmg = 1;
					}
				}
				if (GameFlag_TestFar(366) != 0 && *cmd == 5 && hp <= dmg)
				{
					dmg = hp - 1;
				}
				pass++;
			} while (pass <= 1);
			hp -= dmg;
			BattleEvPush(BATTLE_EVENT_OBJ_BEGIN, tid);
			BattleEvPush(BATTLE_EVENT_UNIT, tid);
			BattleEvPush(BATTLE_EVENT_NUM, dmg);
			MESSAGE_PUSH((ALLY_CHECK(tid) ? MsgDmgEmphP : MsgDmgEmphE) + aff);

			if (hp <= 0)
			{
				BattleEvPush(BATTLE_EVENT_OBJ_RESET, tid);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
				hp = 0;
				MESSAGE_SIDE(MsgGoesDown, MsgFelled);
			}
			else
				BattleEvPush(BATTLE_EVENT_OBJ_END, tid);
			dealt = target->hp - hp;
			target->hp = hp;
			UnitRatioFar(tid);
			break;
		}

		case 10: /* PPダメージ */
		{
			if (action->power == 0)
				break;
			pp = target->pp;
			ELEMENT_BONUS();
			ap = action->power;
			dmg = BattlePowerCalc(ap, bonus, 256);
			dmg = dmg * gPpLossTable[dist] / 100;
			dmg *= adj;
			GUARD_CUT();
			if (action->effect == 32 && dmg > pp)
				dmg = pp;

			BattleEvPush(BATTLE_EVENT_OBJ_BEGIN, tid);
			BattleEvPush(BATTLE_EVENT_NUM, dmg);
			BattleEvPush(BATTLE_EVENT_UNIT, tid);
			MESSAGE_PUSH(ALLY_CHECK(tid) ? MsgPpLossP : MsgPpLossE);
			pp -= dmg;
			if (pp <= 0)
				pp = 0;
			BattleEvPush(BATTLE_EVENT_OBJ_END, tid);
			dealt = target->pp - pp;
			target->pp = pp;
			UnitRatioFar(tid);
			break;
		}

		case 1: /* HPかいふく */
		{
			if (action->power == 0)
				break;
			cur = target->hp;

			ap = action->power;
			dmg = BattleRecoveryCalc(ap, element == 4 ? 100 : power, 256);
			dmg = dmg * gHpRecoveryTable[dist] / 100;
			dmg *= adj;
			dmg += BattleRandomFar() & 3;
			cur += dmg;
			if (cur > target->maxHp)
			{
				cur = target->maxHp;
				dmg = cur - target->hp;
			}
			BattleEvPush(BATTLE_EVENT_UNIT, tid);
			if (cur == target->maxHp)
				MESSAGE_PUSH(MsgHpFull);
			else
			{
				BattleEvPush(BATTLE_EVENT_NUM, dmg);
				MESSAGE_PUSH(MsgHpRecover);
			}
			dealt = target->hp - cur;
			target->hp = cur;
			UnitRatioFar(tid);
			break;
		}

		case -1:
		{
			if (action->power == 0)
				break;
			pp = target->pp;
			ELEMENT_BONUS();
			ap = action->power;
			dmg = BattlePowerCalc(ap, bonus, 256);
			dmg = dmg * gPpDmgTable[dist] / 100;
			dmg *= adj;
			GUARD_CUT();
			BattleEvPush(BATTLE_EVENT_OBJ_BEGIN, tid);
			BattleEvPush(BATTLE_EVENT_NUM, dmg);
			BattleEvPush(BATTLE_EVENT_UNIT, tid);
			MESSAGE_PUSH(ALLY_CHECK(tid) ? MsgDmgP : MsgDmgE);
			pp -= dmg;
			if (pp <= 0)
				pp = 0;
			BattleEvPush(BATTLE_EVENT_OBJ_END, tid);
			target->pp = pp;
			UnitRatioFar(tid);
			break;
		}

		case 5: /* HPダメージ */
		case 6:
		case 8:
		{
			s32 round;

			if (action->power == 0)
				break;
			cur = target->hp;
			round = 1;
			do
			{
				ELEMENT_BONUS();
				if (round == 0)
					bonus = 0;
				ap = action->power;
				if (*cmd == 6)
				{
					s32 item;

					item = abilityId - 380;
					if (item >= 0 && item <= 21)
					{
						switch (item)
						{
						case 0:
						case 6:
						case 12:
						case 18:
							rate = 3;
							break;
						case 1:
						case 7:
						case 13:
						case 19:
							rate = 6;
							break;
						case 2:
						case 8:
						case 14:
						case 20:
							rate = 9;
							break;
						case 3:
						case 9:
						case 15:
						case 21:
							rate = 12;
							break;
						}
					}
					ap += target->maxHp * rate / 100;
				}
				dmg = BattlePowerCalc(ap, bonus, 256);
				dmg *= adj;
				switch (type & 15)
				{
				case 5:
					dmg = gHpDmgTable5[dist] * dmg / 100;
					break;
				case 8:
					dmg = dmg * gHpDmgTable8[dist] / 100;
					break;
				case 6:
					dmg = dmg * gHpDmgTable6[dist] / 100;
					break;
				}
				dmg += BattleRandomFar() & 3;
				GUARD_CUT();
				if (GameFlag_TestFar(366) != 0 && *cmd == 6 && cur > dmg)
				{
					dmg = cur;
				}
				round++;
			} while (round <= 1);
			BattleEvPush(BATTLE_EVENT_OBJ_BEGIN, tid);
			BattleEvPush(BATTLE_EVENT_NUM, dmg);
			BattleEvPush(BATTLE_EVENT_UNIT, tid);
			MESSAGE_PUSH((ALLY_CHECK(tid) ? MsgDmgEmphP : MsgDmgEmphE) + aff);
			cur -= dmg;

			if (cur <= 0)
			{
				BattleEvPush(BATTLE_EVENT_OBJ_RESET, tid);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
				cur = 0;
				MESSAGE_PUSH(ALLY_CHECK(tid) ? MsgGoesDown : MsgFelled);
			}
			else
				BattleEvPush(BATTLE_EVENT_OBJ_END, tid);
			dealt = target->hp - cur;
			target->hp = cur;
			UnitRatioFar(tid);
			break;
		}

		case 11: /* PPかいふく */
		{
			if (action->power == 0)
				break;
			pp = target->pp;
			ap = action->power;
			dmg = BattleRecoveryCalc(ap, element == 4 ? 100 : power, 256);
			dmg = dmg * gPpRecoveryTable[dist] / 100;
			dmg *= adj;
			pp += dmg;
			if (pp > target->maxPp)
			{
				pp = target->maxPp;
				dmg = pp - target->pp;
			}
			BattleEvPush(BATTLE_EVENT_UNIT, tid);
			if (pp == target->maxPp)
				MESSAGE_PUSH(MsgPpFull);
			else
			{
				BattleEvPush(BATTLE_EVENT_NUM, dmg);
				MESSAGE_PUSH(MsgPpRecover);
			}
			target->pp = pp;
			UnitRatioFar(tid);
			break;
		}

		case 0:
		case 7:
		case 9:
			break;

		case 2:
			if (hit != 0)
			{
				if (action->power == 0)
					break;
				cur = target->hp;
				ELEMENT_BONUS();
				ap = action->power;
				dmg = BattlePowerCalc(ap, bonus, 256);
				dmg *= adj;
				dmg = dmg * gHpDmgTable[dist] / 100;
				GUARD_CUT();
				BattleEvPush(BATTLE_EVENT_OBJ_BEGIN, tid);
				BattleEvPush(BATTLE_EVENT_NUM, dmg);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
				MESSAGE_PUSH(ALLY_CHECK(tid) ? MsgDmgP : MsgDmgE);
				cur -= dmg;

				if (cur <= 0)
				{
					BattleEvPush(BATTLE_EVENT_OBJ_RESET, tid);
					BattleEvPush(BATTLE_EVENT_UNIT, tid);
					cur = 0;
					MESSAGE_SIDE(MsgGoesDown, MsgFelled);
				}
				else
					BattleEvPush(BATTLE_EVENT_OBJ_END, tid);
				dealt = target->hp - cur;
				target->hp = cur;
				UnitRatioFar(tid);
				break;
			}
			BattleEvPush(BATTLE_EVENT_OBJ_END, tid);
			BattleEvPush(BATTLE_EVENT_UNIT, tid);
			MESSAGE_PUSH(MsgNoEffect);
			break;
		}
	}

	//------------------------------------------------------------------
	// 状態異常・能力変化などの追加効果
	//------------------------------------------------------------------
	BattleEvPush(BATTLE_EVENT_UNIT, tid);
	if ((BattleEffectRevCheckFar(action->effect) != 0 || target->hp != 0 || BattleEffectDeadCheck(action->effect) != 0)
		&& hit != 0)
	{
		switch (action->effect)
		{

		case 64: /* 状態異常をすべてなおす */
			if (target->delusion != 0)
			{
				target->delusion = 0;
				MESSAGE_PUSH(MsgCureDelusion);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
			}
			if (target->stun != 0)
			{
				target->stun = 0;
				BattleEvPush(BATTLE_EVENT_CLEAR, 0);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
				MESSAGE_PUSH(MsgCureStun);
			}
			target->sleep = 0;
			if (target->psySeal != 0)
			{
				target->psySeal = 0;
				MESSAGE_PUSH(MsgCureSeal);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
			}
			if (target->grimReaper != 0)
			{
				target->grimReaper = 0;
				MESSAGE_PUSH(MsgCureCurse);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
			}
			if (target->haunt != 0)
			{
				target->haunt = 0;
				MESSAGE_PUSH(MsgCureSpirit);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
			}
			if (target->poison != 0)
			{
				MESSAGE_PUSH(MsgCurePoison);
				target->poison = 0;
			}
			BattleEvPush(BATTLE_EVENT_CLEAR, 0);
			break;

		case 4: /* 状態異常をなおす */
			if (target->delusion != 0)
			{
				target->delusion = 0;
				BattleEvPush(BATTLE_EVENT_CLEAR, 0);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
				MESSAGE_PUSH(MsgCureDelusion);
			}
			if (target->stun != 0)
			{
				target->stun = 0;
				BattleEvPush(BATTLE_EVENT_CLEAR, 0);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
				MESSAGE_PUSH(MsgCureStun);
			}
			target->sleep = 0;
			if (target->psySeal != 0)
			{
				target->psySeal = 0;
				BattleEvPush(BATTLE_EVENT_CLEAR, 0);
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
				MESSAGE_PUSH(MsgCureSeal);
			}
			if (target->grimReaper == 0)
				break;
			target->grimReaper = 0;
			BattleEvPush(BATTLE_EVENT_CLEAR, 0);
			BattleEvPush(BATTLE_EVENT_UNIT, tid);
			MESSAGE_PUSH(MsgCureCurse);
			break;

		case 61: /* HPかいふく */
		case 62: /* HPかいふく */
		{
			s32	 old;
			s32	 maxu;
			s32	 maxv;
			s32	 heal;
			u16 *pt;

			/* FAKEMATCH: walking a pointer down from hp to maxHp reads each stat twice, unsigned and signed, as the ROM does; plain member reads, (u16)target->hp beside target->hp and the same for maxHp, share one load each (differ 116 in tbs-en). */
			pt = (u16 *)&target->hp;
			old = *pt--;
			heal = *(s16 *)(pt + 1);
			pt--;
			maxu = *pt--;
			maxv = *(s16 *)(pt + 1);
			if (action->effect == 61)
				heal += maxv * 60 / 100;
			else
				heal += maxv * 30 / 100;
			if (heal > (s16)maxu)
				heal = (s16)maxu;
			tmp = heal - (s16)old;
			if (tmp == 0 && type != 1)
				break;
			if (heal == (s16)maxu)
				MESSAGE_PUSH(MsgHpFull);
			else
			{
				BattleEvPush(BATTLE_EVENT_NUM, tmp);
				MESSAGE_PUSH(MsgHpRecover);
			}
			target->hp = heal;
			UnitRatioFar(tid);
			break;
		}

		case 63: /* PPかいふく */
		{
			s32 old;
			s32 maxv;
			s32 heal;

			heal = target->pp;
			maxv = target->maxPp;
			old = heal;
			heal += maxv * 7 / 100;
			if (heal > maxv)
				heal = maxv;
			tmp = heal - old;
			if (tmp == 0 && type != 11)
				break;
			if (heal == maxv)
				MESSAGE_PUSH(MsgPpFull);
			else
			{
				BattleEvPush(BATTLE_EVENT_NUM, tmp);
				MESSAGE_PUSH(MsgPpRecover);
			}
			target->pp = heal;
			UnitRatioFar(tid);
			break;
		}

		case 59: /* すばやさアップ */
			target->agiLv = 8;
			target->agiCnt = 5;
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, target->agi - bak->agi);
			MESSAGE_PUSH(MsgAgiUp);
			break;

		case 58: /* すばやさダウン */
			target->agiLv = -4;
			target->agiCnt = 5;
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, bak->agi - target->agi);
			MESSAGE_PUSH(MsgAgiDown);
			break;

		case 9: /* こうげき下がる */
			target->atkLv -= 1;
			STEP_LIM(target->atkLv);
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, bak->atk - target->atk);
			MESSAGE_PUSH(MsgAtkDown);
			target->atkCnt = 7;
			break;

		case 8: /* こうげき下がる */
			target->atkLv -= 2;
			STEP_LIM(target->atkLv);
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, bak->atk - target->atk);
			MESSAGE_PUSH(MsgAtkDown);
			target->atkCnt = 7;
			break;

		case 7: /* こうげき上がる */
			target->atkLv += 1;
			STEP_LIM(target->atkLv);
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, target->atk - bak->atk);
			MESSAGE_PUSH(MsgAtkUp);
			target->atkCnt = 7;
			break;

		case 6: /* こうげき上がる */
			target->atkLv += 2;
			STEP_LIM(target->atkLv);
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, target->atk - bak->atk);
			MESSAGE_PUSH(MsgAtkUp);
			target->atkCnt = 7;
			break;

		case 13: /* ぼうぎょ下がる */
			target->defLv -= 1;
			STEP_LIM(target->defLv);
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, bak->def - target->def);
			MESSAGE_PUSH(MsgDefDown);
			target->defCnt = 7;
			break;

		case 12: /* ぼうぎょ下がる */
			target->defLv -= 2;
			STEP_LIM(target->defLv);
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, bak->def - target->def);
			MESSAGE_PUSH(MsgDefDown);
			target->defCnt = 7;
			break;

		case 11: /* ぼうぎょ上がる */
			target->defLv += 1;
			STEP_LIM(target->defLv);
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, target->def - bak->def);
			MESSAGE_PUSH(MsgDefUp);
			target->defCnt = 7;
			break;

		case 10: /* ぼうぎょ上がる */
			target->defLv += 2;
			STEP_LIM(target->defLv);
			UnitCalcFar(tid);
			BattleEvPush(BATTLE_EVENT_NUM, target->def - bak->def);
			MESSAGE_PUSH(MsgDefUp);
			target->defCnt = 7;
			break;

		case 5: /* ふっき */
			if (target->hp != 0)
				break;
			MESSAGE_PUSH(MsgRevived);
			target->hp = target->maxHp;
			UnitRatioFar(tid);
			break;

		case 56: /* ふっき */
			if (target->hp != 0)
				break;
			MESSAGE_PUSH(MsgRevived);
			target->hp = target->maxHp / 2;
			UnitRatioFar(tid);
			break;

		case 57: /* ふっき */
			if (target->hp != 0)
				break;
			MESSAGE_PUSH(MsgRevived);
			target->hp = target->maxHp * 8 / 10;
			UnitRatioFar(tid);
			break;

		case 3: /* 毒がきえる */
			if (target->poison != 0)
				MESSAGE_PUSH(MsgCurePoison);
			target->poison = 0;
			break;

		case 17: /* たいせいダウン */
			RES_STEP(-1, (bak->resLv - target->resLv) * 20, MsgResDown);
			break;

		case 16: /* たいせいダウン */
			RES_STEP(-2, (bak->resLv - target->resLv) * 20, MsgResDown);
			break;

		case 15: /* たいせいアップ */
			RES_STEP(1, (target->resLv - bak->resLv) * 20, MsgResUp);
			break;

		case 14: /* たいせいアップ */
			RES_STEP(2, (target->resLv - bak->resLv) * 20, MsgResUp);
			break;

		case 18: /* 毒 */
			if (target->poison != 0)
				break;
			MESSAGE_PUSH(MsgPoisoned);
			target->poison = 1;
			break;

		case 19: /* もう毒 */
			if (target->poison > 1)
				break;
			MESSAGE_PUSH(MsgVenom);
			target->poison = 2;
			break;

		case 20: /* まぼろし */
			STAT_SET(target->delusion, MsgDelusion);
			break;

		case 21: /* こんらん */
			STAT_SET(target->confusion, MsgConfused);
			break;

		case 22: /* こころをうばう */
			STAT_SET(target->charm, MsgCharmed);
			break;

		case 23: /* しびれ */
			STAT_SET(target->stun, MsgStunned);
			break;

		case 24: /* ねむり */
			STAT_SET(target->sleep, MsgAsleep);
			break;

		case 25: /* エナジーふうじ */
			MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
			target->psySeal |= 7;
			break;

		case 67: /* エナジーふうじ */
			MESSAGE_SIDE(MsgPsyBlock, MsgPsySeal);
			target->psySeal |= 16;
			break;

		case 27: /* せんとうふのう */
			BattleEvPush(BATTLE_EVENT_OBJ_RESET, tid);
			if (target->side == ALLY)
				MESSAGE_PUSH(MsgKoDown);
			else if (abilityId == 219)
				MESSAGE_PUSH(MsgSuffocate);
			else
				MESSAGE_PUSH(MsgSpiritDrain);
			target->hp = 0;
			UnitRatioFar(tid);
			break;

		case 29: /* リフレイン */
			STAT_SET(target->refrain, MsgRefrain);
			break;

		case 30: /* リフレクト */
			STAT_SET(target->reflect, MsgReflect);
			break;

		case 31: /* HPをうばう */
		case 60:
		{
			s32 heal;

			heal = actor->hp;
			dmg = dealt;
			if (action->effect == 60)
				dmg /= 2;
			heal += dmg;
			if (heal > actor->maxHp)
			{
				heal = actor->maxHp;
				dmg = heal - actor->hp;
			}
			BattleEvPush(BATTLE_EVENT_CLEAR, 0);
			BattleEvPush(BATTLE_EVENT_UNIT, aid);
			if (heal == actor->maxHp)
				MESSAGE_PUSH(MsgHpFull);
			else
			{
				BattleEvPush(BATTLE_EVENT_NUM, dmg);
				MESSAGE_PUSH(MsgHpRecover);
			}
			actor->hp = heal;
			UnitRatioFar(aid);
			break;
		}

		case 32: /* PPをうばう */
		{
			s32 heal;
			s32 amt;

			heal = actor->pp;
			amt = dealt;
			heal += amt;
			if (heal > actor->maxPp)
			{
				heal = actor->maxPp;
				amt = heal - actor->pp;
			}
			BattleEvPush(BATTLE_EVENT_CLEAR, 0);
			BattleEvPush(BATTLE_EVENT_UNIT, aid);
			if (heal == actor->maxPp)
				MESSAGE_PUSH(MsgPpFull);
			else
			{
				BattleEvPush(BATTLE_EVENT_NUM, amt);
				MESSAGE_PUSH(MsgPpRecover);
			}
			actor->pp = heal;
			UnitRatioFar(aid);
			break;
		}

		case 69: /* PPをうばう */
			dmg = dealt / 10;
			if (target->pp < dmg)
				dmg = target->pp;
			if (actor->pp + dmg > actor->maxPp)
				dmg = actor->maxPp - actor->pp;
			if (dmg == 0)
				break;
			BattleEvPush(BATTLE_EVENT_NUM, dmg);
			MESSAGE_SIDE(MsgLeechTake, MsgLeechGain);
			UnitPpAddFar(aid, dmg);
			break;

		case 33: /* つよさがもどる */
			if (target->atkLv > 0)
			{
				target->atkLv = 0;
				target->atkCnt = 0;
			}
			if (target->defLv > 0)
			{
				target->defLv = 0;
				target->defCnt = 0;
			}
			if (target->resLv > 0)
			{
				target->resLv = 0;
				target->resCnt = 0;
			}
			if (target->agiLv > 0)
				target->agiLv = 0;
			target->elementLevels[0] = 0;
			target->elementLevels[1] = 0;
			target->elementLevels[2] = 0;
			target->elementLevels[3] = 0;
			MESSAGE_PUSH(MsgBuffsReset);
			break;

		case 26: /* あくりょう */
			MESSAGE_PUSH(MsgEvilSpirit);
			target->haunt = 1;
			break;

		case 28: /* しりょう */
			if (target->grimReaper == 0)
			{
				MESSAGE_PUSH(MsgDeathCurse);
				target->grimReaper = 7;
				break;
			}
			if (target->grimReaper <= 1)
				break;
			target->grimReaper -= 1;
			BattleEvPush(BATTLE_EVENT_NUM, target->grimReaper);
			MESSAGE_PUSH(MsgDeathCount);
			break;

		case 66: /* そなえ */
			MESSAGE_PUSH(MsgReadies);
			target->ready = 2;
			break;

		case 54: /* ちょうはつ */
			MESSAGE_PUSH(MsgChallenge);
			target->challenge = 1;
			if (ALLY_CHECK(tid))
				work->challengeFlag |= 2;
			break;

		case 53: /* みうごき */
			MESSAGE_PUSH(MsgImmobile);
			target->immobile = 1;
			break;

		case 46: /* まもりのオーラ */
			MESSAGE_PUSH(MsgAura);
			if (target->guardLv > 0)
				break;
			target->guardLv = 1;
			break;

		case 47: /* まもりのオーラ */
			MESSAGE_PUSH(MsgAura2);
			if (target->guardLv > 1)
				break;
			target->guardLv = 2;
			break;

		case 45:
			BattleEvPush(BATTLE_EVENT_MESSAGE, -1);
			break;

		default:
			break;
		}
	}

	//------------------------------------------------------------------
	// ダメージによる目覚めと、能力・表示の更新
	//------------------------------------------------------------------
	BattleEvPush(BATTLE_EVENT_CLEAR, 0);

	if (target->hp != 0)
	{
		if (target->sleep != 0)
			if (target->sleep <= 6 && dealt > 0 && (3 & BattleRandomFar()) == 0)
			{
				target->sleep = 0;
				BattleEvPush(BATTLE_EVENT_UNIT, tid);
				MESSAGE_PUSH(MsgWakes);
			}
	}
	MemFree(bak);
	UnitCalcFar(tid);
	PartyStatusDrawFar(gBattleWork->partyStatus);
	if (target->hp != 0)
		BattleEvPush(BATTLE_EVENT_OBJ_END, tid);
	// あくりょうによる反動を行動側に加算する
	if (actor->haunt != 0 && (BattleRandomFar() & 3) == 0 && dealt > 0)
	{
		s32 share;

		share = dealt >> 2;
		if (share == 0)
			share = 1;
		plan->hauntDmg += share;
	}
}

//--------------------------------------------------------------------------
// check_status_or_special_id.c
// バトル：PP を使うわざの判定
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// PP を使うわざなら 1 を返す。エナジードレインは PP に関係なく 1
// abilityId : わざ番号
//--------------------------------------------------------------------------
u32 Ability_CheckStatusOrSpecialId(s32 abilityId)
{
	u8 pp;

	if (abilityId == 126) // エナジードレイン
		return 1;
	pp = BattleActGet(abilityId)->ppCost;
	return (u32)((0 - pp) | pp) >> 31; // pp が 0 でなければ 1
}

//--------------------------------------------------------------------------
// cmdplay.c
// バトル：敵の行動の選択と、バトルイベントの再生
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 行動の種類
//--------------------------------------------------------------------------
#define CMD_ATTACK 0 // こうげき
#define CMD_PSYNERGY 1	  // エナジー
#define CMD_ITEM 2	  // アイテム
#define CMD_DEFEND 3  // ぼうぎょ
#define CMD_ABILITY 4	  // 敵のわざ
#define CMD_DEFEND2 7 // ぼうぎょ (効果 47)
#define CMD_FLEE 99 // にげる

#define ABILITY_ATTACK 1 // わざ番号：こうげき
#define ALLY_MAX 7   // 仲間の番号の上限

// 敵の行動の記録 (BattleUnit の rewards を使う)
struct BattleAutomaticState
{
	unsigned initialized : 1;
	unsigned selection : 3; // 行動表の番号
	unsigned remaining : 28;
};

//--------------------------------------------------------------------------
// 敵の行動を選ぶ
// retry : 0 ならぼうぎょ・にげるだけをここで決め、1 なら残りの行動を決める
//--------------------------------------------------------------------------
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *req, s32 retry)
{
	u32							 selection;
	struct BattleAutomaticState *memo;
	struct EnemyDefinition		*enemy;
	s32							 itemOk;
	s32							 tries;
	s32							 keepGoing;
	s8							*strategy;
	u8							*itemMask;
	s16							 cmd;
	s32							 target;
	s32							 target2;
	s32							 side;
	s32							 side2;
	struct ItemDefinition		*item;
	s32							 useItem;
	s8							 mode;
	struct BattleAct				*itemAct;
	struct BattleAct				*act;
	struct BattleUnit				*unit;
	s32							 abilityId;
	s32							 targetFlag;
	u8							 effect;
	u8							 targetMode;

	// 0-255 の乱数で、重みの表から行動表の番号を引く
	s32 WeightRandom(u8 * weights)
	{
		s32 random;
		s32 total;
		s32 no;
		s32 n;

		random = BattleRandomFar() & 0xff;
		total = weights[0];
		no = 0;
		n = 0;
		if (random >= total)
		{
			while (++n <= 7)
			{
				total += weights[n];
				if (random < total)
				{
					no = n;
					break;
				}
			}
		}
		return no;
	}

	unit = UnitGetFar(req->unitId);
	keepGoing = itemOk = 1;
	tries = 0;
	selection = -1U;
	if (unit->classIndex != 0) // 仲間は選ばない
		return;
	if (retry != 0 && req->command != CMD_ABILITY)
		return;

	enemy = Owner_GetRecordFar(unit->classId);
	memo = (struct BattleAutomaticState *)&unit->rewards;
	strategy = &enemy->actionStrategy;
	itemMask = &enemy->itemActions;

	// 行動表の番号を選ぶ
select_again:
	mode = *strategy;
	switch (mode)
	{
	case 0: // 重みの表から
		selection = WeightRandom(gBattleCommand_SelectionWeights[0]);
		break;
	case 1:
		selection = WeightRandom(gBattleCommand_SelectionWeights[1]);
		break;
	case 2:
		selection = WeightRandom(gBattleCommand_SelectionWeights[2]);
		break;
	case 3: // 乱数で始め、順に進む
		if (memo->initialized == 0)
		{
			memo->selection = BattleRandomFar();
			memo->initialized = 1;
		}
		// fall through
	case 4: // 記録した番号から順に進む
		selection = memo->selection;
		if (retry != 0)
		{
			memo->selection = selection + 1;
		}
		break;
	case 5: // 選び直すたびに次へ
		selection++;
		break;
	case 6: // 選び直さない
		break;
	}

	// アイテムを使う番号なら、持っている道具を確かめる
	useItem = (*itemMask >> selection) & 1;
	useItem &= itemOk;
	abilityId = enemy->actions[selection];
	req->command = CMD_ABILITY;
	if (useItem != 0 && retry != 0)
	{
		if (!(0x1ff & unit->inventory[0]))
		{
			useItem = 0;
			if (enemy->targetStrategy == 0)
			{
				req->command = CMD_ITEM;
				req->parameter = 0x1fd;
				return;
			}
		}
		if (useItem != 0)
		{
			item = Item_GetDirectFar(unit->inventory[0]);
			if (item->useType == 1)
			{
				itemAct = BattleActGet(item->actionId);
				abilityId = item->actionId;
				req->command = CMD_ITEM;
				targetFlag = itemAct->targetFlag;
				req->parameter = 0;
				switch (targetFlag)
				{
				case 1:
				case 2:
					break;
				default:
					useItem = 0;
					break;
				}
			}
			else
			{
				useItem = 0;
			}
		}
		if (useItem == 0)
			itemOk = 0;
	}

	if (keepGoing != 0)
	{
		// ぼうぎょ・にげるのわざは、自分を対象にする
		act = BattleActGet(abilityId);
		effect = act->effect;
		switch (effect)
		{
		case 46:
			req->command = CMD_DEFEND;
			req->target = Battle_FindTaggedSlotByValue(req->unitId);
			break;
		case 47:
			req->command = CMD_DEFEND2;
			req->target = Battle_FindTaggedSlotByValue(req->unitId);
			break;
		case 49:
			req->command = CMD_FLEE;
			req->target = Battle_FindTaggedSlotByValue(req->unitId);
			break;
		}

		if (retry == 0)
		{
			switch (req->command)
			{
			case CMD_DEFEND:
			case CMD_DEFEND2:
				break;
			default:
				goto done;
			}
		}

		// エナジーならPPとふうじを確かめる
		// targetStrategy が 0 でなければPP不足を、2 ならふうじも避ける
		if (req->command != CMD_ITEM && Ability_CheckStatusOrSpecialId(abilityId) != 0)
		{
			req->command = CMD_PSYNERGY;
			req->parameter = abilityId;
			if (act->ppCost > unit->pp && enemy->targetStrategy != 0)
				goto next_candidate;
			if (unit->psySeal != 0 && enemy->targetStrategy == 2)
				goto next_candidate;
		}
		cmd = req->command;
		if (cmd == CMD_FLEE && (u8)unit->challenge != 0)
			goto next_candidate;
		if (retry != 0 && (cmd == CMD_DEFEND || cmd == CMD_DEFEND2))
			goto next_candidate;

		if (cmd == CMD_ABILITY)
		{
			req->parameter = abilityId;
			if (abilityId == ABILITY_ATTACK)
				req->command = CMD_ATTACK;
		}

		// 対象を選ぶ
		req->range = act->range;
		targetMode = act->targetMode;
		switch (targetMode)
		{
		case 2:
		case 4:
			target = BattleTarget_SelectForAction(req->unitId, act);
			if (target == -2)
			{
				side = 0;
				if (req->unitId <= (u32)ALLY_MAX)
					side = 1;
				target = BattleTarget_SelectRandomPosition(side);
			}
			if (target != -1)
			{
				req->target = target;
				keepGoing = 0;
			}
			break;
		case 1:
			target2 = BattleTarget_SelectForAction(req->unitId, act);
			if (target2 == -2)
			{
				side2 = 0;
				if (req->unitId <= (u32)ALLY_MAX)
					side2 = 1;
				target2 = BattleTarget_SelectRandomPosition(side2);
			}
			if (target2 != -1)
			{
				req->target = target2;
				keepGoing = 0;
			}
			break;
		case 3:
			req->target = Battle_FindTaggedSlotByValue(req->unitId);
			break;
		default:
			target2 = Battle_FindTaggedSlotByValue(req->unitId);
			req->target = target2;
			keepGoing = 0;
			break;
		}
	}
	if (retry == 0)
		keepGoing = 0;
next_candidate:
	// 18 回選べなければ、ぼうぎょ
	if (keepGoing != 0 && tries > 16)
	{
		req->command = CMD_DEFEND;
		keepGoing = 0;
	}
	tries++;
	if (keepGoing != 0)
		goto select_again;
done:
	return;
}

//--------------------------------------------------------------------------
// DMA3 を三回設定し、IRQ 復帰命令のワードを呼び出し先として読む
//--------------------------------------------------------------------------
void Dma_StopAllThenRunHook(void)
{
	Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	_call_via_r0(IwramIrqModeWord);
}

//--------------------------------------------------------------------------
// 止まっている再生を、対象の処理から始める
// cue : 当たったときの効果音 (0 なら変えない)
//--------------------------------------------------------------------------
void BattleEventRuntime_BeginPhase(s32 cue)
{
	struct BattleEventState *ev;

	ev = &gBattleWork->events;
	if (ev->phase == BATTLE_PLAYBACK_IDLE)
	{
		ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
		if (cue != 0)
		{
			ev->pendingCue = cue;
		}
	}
}

//--------------------------------------------------------------------------
// イベントの列を空にして、再生を登録する
// frames : 最初のイベントまでの待ち
//--------------------------------------------------------------------------
s32 BattleEventRuntime_SchedulePhase(s32 frames)
{
	struct BattleEventState *ev;

	ev = &gBattleWork->events;
	ev->queue.count = 0;
	ev->eventIndex = 0;
	ev->timer = frames;
	ev->phase = BATTLE_PLAYBACK_DISPATCH;
	gBattleWork->plan.targetCount = 0;
	return Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
}

//--------------------------------------------------------------------------
// バトルイベントの再生
//--------------------------------------------------------------------------
#define PLAY_MESSAGE 3		// メッセージの表示を待つ
#define PLAY_ADVANCE 5	// 送りのキーを待つ
#define PLAY_HIT 10		// 当たった者を点滅させる
#define PLAY_FELLED 11	// 倒れた敵を消す
#define PLAY_MESSAGE_ADD 13 // 続きのメッセージ (キーを待たない)

#define POSE_DAMAGE 5 // ダメージを受けたポーズ
#define PAL_HIT 7	  // 点滅の色

#define FELLED_FLASH 0x400 // 色を変えている間のタイマーの基準

#define OBJ_VRAM 0x06010000

//--------------------------------------------------------------------------
// スケジューラから呼ばれ、BattleTargetExec が積んだイベントを順に実行する
//--------------------------------------------------------------------------
void BattleEvent_Playback(void)
{
	u16						 selection[8];
	struct AnimationObject	*recs[4];
	struct BattleWork			*work = gBattleWork;
	struct BattleEventState *ev = &work->events;

	// キャラのタイルを消す
	void ClrRecTiles(struct AnimationObject * rec, s32 value)
	{
		u8 *tiles = (u8 *)OBJ_VRAM + gVramBlockCache[rec->slot].offset;

		Iwram_ClearWords(tiles, rec->width * rec->height);
	}

	if (work->events.phase == BATTLE_PLAYBACK_IDLE)
		return;
	for (;;)
	{
		if (ev->phase == BATTLE_PLAYBACK_READY)
			return;
		if (ev->phase == BATTLE_PLAYBACK_RESOLVE_TARGET)
		{
			// 次の対象の効果を計算し、イベントを積む
			if (ev->queue.targetIndex < work->plan.targetCount)
			{
				ev->queue.count = 0;
				ev->eventIndex = 0;
				ev->timer = 0;
				BattleTargetExec(&work->plan, ev->queue.targetIndex);
				ev->queue.targetIndex++;
				ev->phase = BATTLE_PLAYBACK_DISPATCH;
			}
			else
			{
				ev->phase = BATTLE_PLAYBACK_READY;
			}
		}
		else if (ev->phase == BATTLE_PLAYBACK_DISPATCH)
		{
			// 積んだイベントを順に実行する
			while (ev->eventIndex < ev->queue.count)
			{
				{
					s32 i = ev->eventIndex;

					if (ev->phase != BATTLE_PLAYBACK_DISPATCH)
						break;
					if (ev->timer != 0)
					{
						ev->timer--;
						return;
					}
					switch (ev->queue.opcodes[i])
					{
					case BATTLE_EVENT_SE: // 効果音
						AudioCommand_PlayFar(ev->queue.operands[i]);
						break;
					case BATTLE_EVENT_SCR:
						Battle_SetRuntimeFlagBit0(ev, ev->queue.operands[i]);
						break;
					case BATTLE_EVENT_UNIT: // メッセージに入れる名前
						UiWork_PushValueSlotFar(ev->queue.operands[i], 1);
						break;
					case BATTLE_EVENT_NUM: // メッセージに入れる数
						UiWork_PushValueSlotFar(ev->queue.operands[i], 5);
						break;
					case BATTLE_EVENT_ITEM: // メッセージに入れるアイテム名
						UiWork_PushValueSlotFar(ev->queue.operands[i] & 0x1ff, 2);
						break;
					case BATTLE_EVENT_ACT: // メッセージに入れるわざ名
						UiWork_PushValueSlotFar(ev->queue.operands[i] & 0x3fff, 4);
						break;
					case BATTLE_EVENT_MARK:
						gBattleDisplayWork->marked = 1;
						break;
					case BATTLE_EVENT_MESSAGE: // メッセージを出し、送りを待つ
						if ((s32)ev->queue.operands[i] >= 0)
							UiText_PrepareMessageWorkFar(ev->queue.operands[i]);
						ev->phase = PLAY_MESSAGE;
						gKeysPressedLatch = 0;
						break;
					case BATTLE_EVENT_MESSAGE_ADD: // メッセージを出し、送りを待たない
						if ((s32)ev->queue.operands[i] >= 0)
							UiText_PrepareMessageWorkFar(ev->queue.operands[i]);
						ev->phase = PLAY_MESSAGE_ADD;
						break;
					case BATTLE_EVENT_CLEAR: // メッセージに入れたものを消す
						UiWork_ClearValueNameTablesFar();
						break;
					case BATTLE_EVENT_OBJ_EFFECT:
						BattleActor_DestroyTemporaryObject(ev->queue.operands[i]);
						break;
					case BATTLE_EVENT_OBJ_BEGIN: // 当たった
						if (ev->pendingCue > 0)
							AudioCommand_PlayFar(ev->pendingCue);
						ev->actorId = ev->queue.operands[i];
						ObjectDispatch_ApplyArgumentToChildrenFar(
							BattleObjGet(ev->actorId)->object, POSE_DAMAGE);
						ev->phase = PLAY_HIT;
						ev->timer = 0;
						break;
					case BATTLE_EVENT_OBJ_RESET: // 倒れた
					{
						struct BattleUnit		   *unit;
						struct AnimationObject *rec;
						s32						n;

						ev->actorId = ev->queue.operands[i];
						BattleEnemy_RecordDefeat(ev->actorId, ev->flags);
						BattleActor_ResetRuntimeFields(ev->actorId);
						unit = UnitGetFar(ev->actorId);
						for (n = 0;
							(rec = GetMotionRecord(BattleObjGet(ev->actorId)->object, n)) != 0; n++)
						{
							if (unit->side != BATTLE_UNIT_ENEMY)
								AnimationObjects_SelectAnimationFar(rec, 4);
							else
								AnimationObjects_SelectAnimationFar(rec, 5);
						}
						if (unit->side == BATTLE_UNIT_ENEMY)
						{
							ev->phase = PLAY_FELLED;
							ev->timer = 0;
						}
						break;
					}
					case BATTLE_EVENT_REDRAW: // 仲間の状態を描き直す
						PartyStatusDrawFar(gBattleWork->partyStatus);
						break;
					case BATTLE_EVENT_OBJ_END: // 状態アイコン・色・ポーズを今の状態に合わせる
					{
						struct BattleObj *slot;

						BattleUnit_BuildStatusFlags(
							ev->queue.operands[i], BattleObjGet(ev->queue.operands[i]));
						slot = BattleObjGet(ev->queue.operands[i]);
						BattleMotion_SetRecordChildValues(
							slot->object, BattleMotion_GetSlotPalette(ev->queue.operands[i]));
						BattlePres_SetActorModeAndAction(ev->queue.operands[i]);
						break;
					}
					}
					ev->eventIndex++;
				}
			}
			if (ev->phase == BATTLE_PLAYBACK_DISPATCH)
				ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
		}
		else if (ev->phase == PLAY_MESSAGE || ev->phase == PLAY_MESSAGE_ADD)
		{
			// メッセージが出きるまで待つ
			if (UiWork_IsCompleteFar() == 0)
				return;
			if (ev->phase == PLAY_MESSAGE_ADD)
			{
				ev->phase = BATTLE_PLAYBACK_DISPATCH;
				ev->timer = 0;
			}
			else
			{
				ev->phase = PLAY_ADVANCE;
				ev->displaySource = -1;
				ev->timer = gFrameTick;
			}
		}
		else if (ev->phase == PLAY_ADVANCE)
		{
			// 矢印を出して送りのキーを待つ
			// B を押し続けるか送りのキーを押すと進み、10 ティックを過ぎれば押し続けていても進む
			struct BattlePromptSprite  *spr = &ev->displayEntry;
			u8						   *arrow = gBattlePres_AdvanceArrowTiles;
			s32							pat = (s32)arrow + ((gFrameCount >> 2) & 7) * 128;
			struct BattleDisplayWork   *disp = gBattleDisplayWork;
			struct UiWindow			   *window = disp->window;
			struct BattleDisplayOffset *offset = disp->offset;
			s32							x = 0;

			if (ev->displaySource == -1)
				ev->displaySource = work->promptSlot;
			UiWork_ClearValueNameTablesFar();
			QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
			QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
			((u32 *)spr)[1] = 0xa000;
			((u32 *)spr)[2] = x;
			spr->tile = Resource_GetBuffer(ev->displaySource, pat);
			x = window->x * 8 + (offset->x >> 8) + 4;
			spr->x = x;
			spr->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 + (offset->y >> 8) + 6;
			if ((BATTLE_OPTIONS & KEY_B) || (gKeysPressedLatch & KEY_ADVANCE)
				|| ((u32)(gFrameTick - ev->timer) > 10 && (BATTLE_OPTIONS & KEY_ADVANCE)))
			{
				AudioCommand_PlayFar(111);
				ev->phase = BATTLE_PLAYBACK_DISPATCH;
				ev->timer = 0;
			}
			else
			{
				Runtime_PushSlotEntry(spr, 240);
				return;
			}
		}
		else if (ev->phase == PLAY_HIT)
		{
			// 2 フレームごとに色を変え、9 フレームで戻る
			if (ev->timer & 1)
			{
				if (ev->timer & 2)
				{
					struct BattleObj *slot;

					selection[0] = 0xff;
					slot = BattleObjGet(ev->actorId);
					BattleMotion_SetRecordChildValues(
						slot->object, BattleMotion_GetSlotPalette(ev->actorId));
				}
				else
				{
					selection[0] = ev->actorId;
					selection[1] = 0xff;
					BattleMotion_SetRecordChildValues(BattleObjGet(ev->actorId)->object, PAL_HIT);
				}
				BattleLayout_HighlightPartyPanelsFar(selection);
			}
			ev->timer++;
			if (ev->timer <= 8)
				return;
			ev->phase = BATTLE_PLAYBACK_DISPATCH;
			ev->timer = 0;
		}
		else if (ev->phase == PLAY_FELLED)
		{
			// flags があれば、まず 30 フレームのあいだ色を変える
			if (ev->timer == 0 || ev->timer >= FELLED_FLASH)
			{
				s32 frame = 6;

				if (ev->timer == 0 && ev->flags != 0)
				{
					s32 kind = Summon_GetLayoutKind(UnitGetFar(ev->actorId)->classId);

					if (kind >= 0)
					{
						kind--;
						if (kind < 0)
							kind = 0;
						AudioCommand_PlayFar(kind + 146);
					}
					ev->timer = FELLED_FLASH;
				}
				if (ev->timer > FELLED_FLASH + 29)
					ev->timer = 0;
				if (ev->timer == 0)
				{
					s32 kind = Summon_GetLayoutKind(UnitGetFar(ev->actorId)->classId);

					if (kind >= 0)
						AudioCommand_PlayFar(kind + 146);
				}
				if (ev->timer >= FELLED_FLASH)
					frame = (ev->timer - FELLED_FLASH) / 8 % 5 + 1;
				if (frame == 6 || (ev->timer & 7) == 0)
				{
					struct AnimationObject	*rec;
					s32						 n;
					struct AnimationObject **slot;
					s32						 none;

					for (n = 0, slot = recs, none = 0xff;
						(rec = GetMotionRecord(BattleObjGet(ev->actorId)->object, n)) != 0; n++)
					{
						struct AnimationEntry *child;
						u8					   old;

						*slot++ = rec;
						child = rec->entries[0];
						old = child->frame;
						child->param = frame;
						child->frame = old | none;
					}
				}
				ev->timer++;
				return;
			}
			if (ev->timer == 4)
			{
				BattleActor_RemoveFromLists(ev->actorId);
				ev->timer++;
				return;
			}
			if (ev->timer > 4)
			{
				// タイルを順に描き換えて消す
				struct BattleObj		   *slot;
				struct AnimationObject *rec;
				s32						count;
				s32						frame;
				s32						base;

				slot = BattleObjGet(ev->actorId);
				slot->fading = 1;
				for (count = 0; (rec = GetMotionRecord(slot->object, count)) != 0; count++)
					recs[count] = rec;
				base = ev->timer * 4;
				frame = base - 20;
				if (frame > 127)
				{
					s32 k;

					for (k = 0; k < count; k++)
						ClrRecTiles(recs[k], 0);
					ActivateBattleObjectSlot(ev->actorId);
					ev->phase = BATTLE_PLAYBACK_DISPATCH;
					ev->timer = 0;
					return;
				}
				{
					s32 k;

					for (k = 0; k < count; k++)
					{
						render_animated_tile_frameFar((u8 *)recs[k], frame);
						render_animated_tile_frameFar((u8 *)recs[k], base - 19);
						render_animated_tile_frameFar((u8 *)recs[k], base - 18);
						render_animated_tile_frameFar((u8 *)recs[k], base - 17);
					}
				}
				ev->timer++;
				return;
			}
			ev->timer++;
			return;
		}
	}
}

u32 BattleEventRuntime_Reset(void)
{
	struct BattleEventState *events;

	events = &gBattleWork->events;
	events->phase = BATTLE_PLAYBACK_IDLE;
	events->queue.count = 0;
	events->eventIndex = 0;
	events->timer = 0;
	events->queue.targetIndex = 0;
	events->pendingCue = 0x86;
	events->flags = 0;
}

s32 BattleEventRuntime_WaitForReady(void)
{
	s32						 state;
	struct BattleEventState *events;

	events = &gBattleWork->events;
	state = events->phase;
	if (state == BATTLE_PLAYBACK_IDLE)
	{
		events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
		state = BATTLE_PLAYBACK_RESOLVE_TARGET;
	}
	if (state != BATTLE_PLAYBACK_READY)
	{
		do
		{
			WaitFrames(1U);
		} while (events->phase != BATTLE_PLAYBACK_READY);
	}
	Scheduler_RemoveCallback(BattleEvent_Playback);
	return BattleEventRuntime_Reset();
}

//--------------------------------------------------------------------------
// is_unit_listed.c
// バトル：ユニットが戦闘中の並びにいるかの判定
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ユニットが自分の側の並びにいれば 1 を返す
// id : ユニット番号（0-7 味方、それ以外は敵）
//--------------------------------------------------------------------------
u32 BattleParty_IsUnitListed(u32 id)
{
	u16 list[8];
	s32 side;
	s32 cnt;
	s32 i;

	side = BATTLE_SIDE_PARTY;
	if (id > 7)
		side = BATTLE_SIDE_ENEMIES;
	cnt = BattleParty_ListActorIds(side, list);

	for (i = 0; i < cnt; i++)
	{
		if (list[i] == id)
			break;
	}

	return i != cnt;
}

s32 BattlePlacement_CountValidEntries(u32 unitId, u8 *counts)
{
	u16						  unitIds[8];
	struct DjinnRecoveryList *list;
	s32						  total;
	s32						  count;
	s32						  i;
	s32						  j;
	s32						  sideMask;
	s32						  side;

	count = 0;
	sideMask = BATTLE_SIDE_PARTY;
	if (unitId > 7)
		sideMask = BATTLE_SIDE_ENEMIES;
	total = BattleParty_ListActorIds(sideMask, unitIds);
	side = 0;
	if (unitId > 7)
		side = 1;
	list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(side))->list;
	if (counts != 0)
		for (j = 3; j >= 0; j--)
			counts[j] = 0;
	i = 0;
	if (list->count != 0)
	{
		do
		{
			if (list->entries[i].turns == -1)
			{
				for (j = 0; j < total; j++)
					if (unitIds[j] == list->entries[i].unitId)
						break;
				if (j != total)
				{
					if (counts != 0)
						counts[list->entries[i].element]++;
					count++;
				}
			}
			i++;
		} while (i != list->count);
	}
	return count;
}

//--------------------------------------------------------------------------
// command.c
// バトル：行動の宣言と、対象・効果の計画
//--------------------------------------------------------------------------
// ジンの指定：属性 (bit 8-11) と番号 (bit 0-7)
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v) ((v) & 255)
#define CMD_DJINN 5	  // ジン
#define CMD_SUMMON 6 // しょうかん
#define CMD_NONE 8	  // 何もしない
#define TARGET_MAX 14

//--------------------------------------------------------------------------
// 行動を宣言し、対象と効果の計画を作る
// 返り値 : 0 = 実行する、-1 = 失敗 (メッセージ済み)、-2 = 何もしない
//--------------------------------------------------------------------------
s32 BattleCommand_BuildPlan(struct BattleActionRecord *req, struct BattlePlan *plan)
{
	struct BattleUnit *unit = UnitGetFar(req->unitId);
	struct BattleWork *work = gBattleWork;
	s32				tid;
	s32				abilityId;

	//----------------------------------------------------------------------
	// わざの範囲にいる者を対象に並べる
	// 誰もいなければ、身を守って -1
	//----------------------------------------------------------------------
	s32 TargetSet(s32 no)
	{
		struct BattleAct *act = BattleActGet(no);
		s32			   mode = act->targetMode;
		s32			   deadOk = 0;
		s32			   cnt;
		s32			   partyCount;
		s32			   enemyCount;
		s32			   center;
		s32			   range;
		s32			   pos;
		s32			   id;

		switch (act->effect)
		{
		case 5: // ふっき
		case 56:
		case 57:
			deadOk = 1;
			break;
		}
		switch (mode)
		{
		case 0: // ひとり
			plan->targetDist[0] = mode;
			plan->targetCount = 1;
			plan->targetIds[0] = tid;
			plan->targetAdj[0] = 1;
			break;
		case 4:
			plan->targetDist[0] = 0;
			plan->targetCount = 1;
			plan->targetIds[0] = tid;
			plan->targetAdj[0] = 1;
			break;
		default:
			// 狙った位置から range - 1 までの並び
			cnt = 0;
			for (pos = 0; work->partyUnits[pos] != BATTLE_UNIT_LIST_END; pos++)
				;
			partyCount = pos;
			for (pos = 0; work->enemyIds[pos] != BATTLE_UNIT_LIST_END; pos++)
				;
			enemyCount = pos;
			center = req->target & 15;
			range = req->range;
			{
				s32 first = center - range + 1;
				s32 last = center + range - 1;

				for (pos = first; pos <= last; pos++)
				{
					if (pos < 0)
						continue;
					if (req->target & 128) // 敵の側
					{
						if (pos >= enemyCount)
							continue;
						id = work->enemyIds[pos];
						if (id == BATTLE_UNIT_REMOVED)
							continue;
						if (!deadOk && UnitGetFar(id)->hp == 0)
							continue;
						plan->targetAdj[cnt] = 1;
						plan->targetDist[cnt] = pos - center;
						plan->targetIds[cnt] = id;
						cnt++;
					}
					else
					{
						if (pos >= partyCount)
							continue;
						id = work->partyUnits[pos];
						if (id == BATTLE_UNIT_REMOVED)
							continue;
						if (!deadOk && UnitGetFar(id)->hp == 0)
							continue;
						plan->targetAdj[cnt] = 1;
						plan->targetDist[cnt] = pos - center;
						plan->targetIds[cnt] = id;
						cnt++;
					}
				}
			}
			plan->targetCount = cnt;
			if (cnt <= 0)
			{
				UiWork_PushValueSlotFar(req->unitId, 1);
				UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
				if (unit->guardLv == 0)
					unit->guardLv = 1;
				return -1;
			}
		}
	}

	tid = Battle_GetTaggedSlotValue(req->target);
	BattleEventRuntime_Reset();
	plan->hauntDmg = 0;
	plan->aid = req->unitId;
	plan->targetCount = 0;
	plan->presentationFlags = 0;
	plan->failure = 0;
	plan->element = 4;
	UiWork_ClearValueNameTablesFar();
	if (unit->hp == 0)
		return -2;

	// デバッグ：R を押していれば敵全員 (SELECT も押せば味方全員) を倒す
	if (BATTLE_AUTO_MODE && GameFlag_TestFar(0x16d) && (BATTLE_OPTIONS & KEY_R))
	{
		s32 enemy = 1;
		s32 i;
		s32 id;

		if (BATTLE_OPTIONS & KEY_SELECT)
			enemy = 0;
		for (i = 0;; i++)
		{
			if (enemy)
				id = work->enemyIds[i];
			else
				id = work->partyUnits[i];
			if (id == BATTLE_UNIT_LIST_END)
				break;
			if (id == BATTLE_UNIT_REMOVED)
				continue;
			if (Owner_AdjustFirstValueFar(id, 0xc0000000) == 0)
			{
				BattleEvPush(BATTLE_EVENT_OBJ_BEGIN, id);
				BattleEvPush(BATTLE_EVENT_OBJ_RESET, id);
			}
		}
		BattleEv_DispatchQueued();
		return -2;
	}

	// 動けない状態
	UiWork_ClearValueNameTablesFar();
	if (unit->immobile)
	{
		unit->immobile = 0;
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgUnableToMove);
		return -1;
	}
	if (unit->sleep)
	{
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgIsAsleep);
		return -1;
	}
	if (unit->stun)
	{
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgIsParalyzed);
		return -1;
	}
	// かなしばりは 1/4 で動けない
	if ((unit->restraint & 1) && req->command != CMD_DEFEND && (BattleRandomFar() & 3) == 0)
	{
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgIsBound);
		return -1;
	}
	if (req->command == CMD_NONE)
		return -2;
	abilityId = ABILITY_ATTACK;
	{
		s32 i;

		for (i = 0; i < TARGET_MAX; i++)
			plan->targetCrit[i] = 0;
		for (i = 0; i < TARGET_MAX; i++)
			plan->targetHit[i] = -1;
	}

	//----------------------------------------------------------------------
	// 行動ごとの宣言
	//----------------------------------------------------------------------
	switch (req->command)
	{
	case CMD_FLEE:
		if ((u16)req->unitId <= ALLY_MAX)
		{
			UiText_ShowMessageAndWaitCoreFar(MsgPartyFlees);
		}
		else
		{
			UiWork_PushValueSlotFar(req->unitId, 1);
			UiText_ShowMessageAndWaitCoreFar(MsgActorRuns);
		}
		BattlePresentation_WaitForAdvance();
		plan->outcome = 7;
		return 0;

	case CMD_ATTACK:
		// 武器の力が発動すれば、そのわざになる
		abilityId = RollWeaponUnleashFar(unit);
		if (TargetSet(abilityId) == -1)
			return -1;
		if (abilityId != ABILITY_ATTACK)
		{
			UiWork_PushValueSlotFar(req->unitId, 1);
			UiWork_PushValueSlotFar(Inventory_GetEquippedItemFar(unit, 1), 2);
			UiText_ShowMessageAndWaitCoreFar(MsgWeaponHowls);
			BattleEv_MarkDisplay();
			UiWork_PushValueSlotFar(abilityId, 4);
			UiText_ShowMessageAndWaitCoreFar(MsgWeaponHowls + 1);
		}
		break;

	case CMD_PSYNERGY:
	{
		struct BattleAct *act;
		s32			   ok = 1;

		abilityId = req->parameter;
		act = BattleActGet(abilityId);
		if (TargetSet(abilityId) == -1)
			return -1;
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiWork_PushValueSlotFar(abilityId, 4);
		UiText_ShowMessageAndWaitCoreFar(MsgActorCasts);
		// PPが足りないか、ふうじなら失敗
		if (unit->pp < act->ppCost)
		{
			plan->failure = 2;
			ok = 0;
		}
		if (unit->psySeal)
		{
			plan->failure = 1;
			ok = 0;
		}
		if (ok)
		{
			plan->failure = 0;
			unit->pp -= act->ppCost;
			UnitRatioFar(req->unitId);
			if (unit->pp < 0)
				unit->pp = 0;
			if (unit->pp > unit->maxPp)
				unit->pp = unit->maxPp;
		}
	}
	break;

	case CMD_ITEM:
	{
		struct ItemDefinition *item;

		if (req->parameter < 0)
		{
			UiWork_PushValueSlotFar(req->unitId, 1);
			UiText_ShowMessageAndWaitCoreFar(MsgItemAlreadyUsed);
			return -1;
		}
		item = Item_GetDirectFar(unit->inventory[req->parameter]);
		abilityId = item->actionId;
		if (abilityId == 0 || (unit->inventory[req->parameter] & 0x400))
		{
			UiWork_PushValueSlotFar(req->unitId, 1);
			UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
			if (unit->guardLv == 0)
				unit->guardLv = 1;
			return -1;
		}
		if (TargetSet(abilityId) == -1)
			return -1;
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiWork_PushValueSlotFar(unit->inventory[req->parameter], 2);
		if (item->useType == 2 || item->useType == 0)
		{
			switch (item->type)
			{
			case 1:
			case 3:
			case 6:
			case 7:
			case 8:
				UiText_ShowMessageAndWaitCoreFar(MsgActorRaisesItem); // かざした
				break;
			default:
				UiText_ShowMessageAndWaitCoreFar(MsgActorUsesBattleItem); // つかった
				break;
			}
		}
		else
		{
			UiText_ShowMessageAndWaitCoreFar(MsgActorUsesBattleItem);
		}
	}
	break;

	case CMD_DEFEND:
	case CMD_DEFEND2:
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgActorDefends);
		return -1;

	case CMD_NONE:
		return -2;

	case CMD_ABILITY:
	{
		s32 message;

		abilityId = req->parameter;
		if (TargetSet(abilityId) == -1)
			return -1;
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiWork_PushValueSlotFar(abilityId, 4);
		if ((BattleActGet(abilityId)->targetFlag & 15) == 6)
			message = MsgActorUnleashesAbility;
		else
			message = MsgActorUsesAbility;
		// わざごとの言い方
		switch (abilityId)
		{
		case 435: // ちょうおんぱ
		case 436:
			message = MsgEmitsUltrasonicWaves;
			break;
		case 224: // ヒートミラージュ
			message = MsgActorCasts;
			break;
		case 500: // おどし
			message = MsgGlowersFerociously;
			break;
		case 501: // てがふるえている
			message = MsgIsIntimidated;
			break;
		case 499: // けんせい
			message = MsgBalefulGaze;
			break;
		case 494: // ミミズをたべる
			message = MsgEatsWorms;
			break;
		case 437: // スクウィールロアー ～ じごくのとおぼえ
		case 438:
		case 439:
		case 440:
		case 441:
			message = MsgLetsOutAbility;
			break;
		case 442: // ウイングビート ～ ウイングストローク
		case 443:
		case 444:
			message = MsgActorUsesAbility;
			break;
		case 472: // くさったたいえき
			message = MsgEnemyUnleashesAbility;
			break;
		case 488: // せつないひとみ
			message = MsgGlowersMiserably;
			break;
		case 492: // ふらんしゅう
			message = MsgSmellOfDecay;
			break;
		case 495: // きょうぼうか
			message = MsgFuriousRage;
			break;
		case 503: // ぞうしょくうんどう
			message = MsgAttemptsToDivide;
			break;
		case 504: // なかまをさがす
			message = MsgLooksForAllies;
			break;
		case 508: // たすけをさがす
			message = MsgLooksForHelp;
			break;
		}
		UiText_ShowMessageAndWaitCoreFar(message);
	}
	break;

	case CMD_DJINN:
		// セットされていなければ、スタンバイのジンをセットし直す
		abilityId = Djinn_GetDefinitionHeaderFar(
			DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
		if (!Djinn_IsActiveFar(
				req->unitId, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter)))
		{
			if (Trade_CanOfferDjinnFar(
					req->unitId, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter)))
			{
				BattleActGet(abilityId);
				BattlePres_SetActorModes(0, 0);
				Djinn_ActivateFar(
					req->unitId, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
				Trade_RemoveOfferFar(
					req->unitId, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
				UnitCalcFar(req->unitId);
				BattleEventRuntime_Reset();
				BattleEventRuntime_SchedulePhase(30);
				BattleEvPush(BATTLE_EVENT_UNIT, req->unitId);
				BattleEvPush(BATTLE_EVENT_ACT,
					DJINN_ELEMENT(req->parameter) * 20 + DJINN_INDEX(req->parameter) + 300);
				BattleEvPush(BATTLE_EVENT_SE, 175);
				BattleEvPush(BATTLE_EVENT_REDRAW, 0);
				BattleEvPush(BATTLE_EVENT_MESSAGE, MsgDjinnSet);
				BattleEvPush(BATTLE_EVENT_OBJ_END, req->unitId);
				AudioCommand_PlayFar(212);
				ObjectDispatch_ApplyArgumentToChildrenFar(BattleObjGet(req->unitId)->object, 3);
				ObjectDispatch_ApplyValueToChildrenFar(
					(struct DispatchObject *)BattleObjGet(req->unitId)->object, 32);
				BattleFx_PlayUnitElementEffect(req->unitId, DJINN_ELEMENT(req->parameter), 3, 0);
				BattleEventRuntime_WaitForReady();
				return -2;
			}
			// リカバリー中
			UiWork_PushValueSlotFar(req->unitId, 1);
			UiWork_PushValueSlotFar(abilityId, 4);
			AudioCommand_PlayFar(114);
			UiText_ShowMessageAndWaitCoreFar(MsgDjinnInRecovery);
			WaitFrames(60);
			return -1;
		}
		// セットされたジンを解き放つ
		{
			struct BattleAct *act;

			if (TargetSet(abilityId) == -1)
				return -1;
			Trade_AddOfferFar(
				req->unitId, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
			act = BattleActGet(abilityId);
			UiWork_PushValueSlotFar(req->unitId, 1);
			UiWork_PushValueSlotFar(abilityId, 4);
			UiText_ShowMessageAndWaitCoreFar(MsgActorUnleashesDjinn);
			plan->element = act->damageClass;
		}
		break;

	case CMD_SUMMON:
	{
		const struct SummonDefinition *summon;
		struct DjinnRecoveryList	  *list;
		s32							   side;
		u8							   counts[4];
		/* FAKEMATCH: never read: the native frame keeps 8 bytes below counts, exactly this array; without it the frame is 40 bytes, not 48, and every slot above it moves (differ 28 in tbs-en). */
		u8	unused[8];
		s32 i;

		// 属性ごとのスタンバイの数が足りるか
		summon = SummonDefinition_Get(req->parameter);
		BattlePlacement_CountValidEntries(req->unitId, counts);
		side = 0;
		if ((u16)req->unitId > ALLY_MAX)
			side = 1;
		list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(side))->list;
		for (i = 0; i < 4 && counts[i] >= summon->djinnRequired[i]; i++)
			counts[i] = summon->djinnRequired[i];
		abilityId = summon->nameMessageId;
		if (TargetSet(abilityId) == -1)
			return -1;
		if (i != 4)
		{
			UiWork_PushValueSlotFar(req->unitId, 1);
			UiWork_PushValueSlotFar(abilityId, 4);
			UiText_ShowMessageAndWaitCoreFar(MsgSummonLacksDjinn);
			return -1;
		}
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiWork_PushValueSlotFar(abilityId, 4);
		UiText_ShowMessageAndWaitCoreFar(MsgActorSummons);
		// 使うスタンバイのジンに印 (-2) をつける
		for (i = 0; i != list->count; i++)
		{
			struct DjinnRecoveryEntry *entry = &list->entries[i];

			if (entry->turns == -1 && BattleParty_IsUnitListed(entry->unitId)
				&& counts[entry->element] != 0)
			{
				entry->turns = -2;
				counts[entry->element]--;
			}
		}
	}
	break;
	}

	//----------------------------------------------------------------------
	// こうげきの計画
	//----------------------------------------------------------------------
	if (abilityId == ABILITY_ATTACK)
	{
		struct BattleUnit *target = UnitGetFar(plan->targetIds[0]);

		plan->abilityId = ABILITY_ATTACK;
		plan->element = Item_GetEquippedElementFar(req->unitId);
		plan->outcome = 2;
		if (unit->classIndex == 0) // 敵
		{
			plan->presentationFlags = Battle_GetSpriteFlagNibble(unit->classId) | 0x4000;
		}
		else
		{
			plan->presentationFlags = 0;
			switch (unit->classId)
			{
			case ISAAC:
				plan->presentationFlags = 0x4001;
				break;
			case GARET:
				plan->presentationFlags = 0x4001;
				break;
			case IVAN:
				plan->presentationFlags = 0x4004;
				break;
			case MIA:
				plan->presentationFlags = 0x4004;
				break;
			case JENNA:
				plan->presentationFlags = 0x4001;
				break;
			}
		}
		UiWork_PushValueSlotFar(req->unitId, 1);
		UiText_ShowMessageAndWaitCoreFar(MsgActorAttacks);
		// 外れ：まぼろしなら 153/256、ほかに 1/32
		// 相手が動けなければ外れない
		/* FAKEMATCH: the one-pass block places the body ahead of its five tests, reached by a
		 * backward beq from the last one, whose zero (r5) the first store reuses. As a plain
		 * if, with the tests negated or not, or as a while loop left by break, the tests come
		 * first (differ 105 in tbs-en). */
		do
		{
			if (target->hp == 0 || target->sleep != 0 || target->stun != 0 || target->immobile != 0
				|| target->charm != 0)
				break;
			if (unit->delusion != 0 && (BattleRandomFar() & 255) <= 152)
				plan->targetAdj[0] = 0;
			if ((BattleRandomFar() & 31) == 0)
				plan->targetAdj[0] = 0;
		} while (0);
		if (GameFlag_TestFar(366))
			plan->targetAdj[0] = 0;
		// 会心：1/32、または装備による確率
		if (target->hp != 0)
		{
			if ((BattleRandomFar() & 31) == 0)
				plan->targetCrit[0] = 1;
			else if ((Equipment_GetUnleashRateBonusFar(unit) << 16) / 200 > (BattleRandomFar() & 0xffff))
				plan->targetCrit[0] = 1;
		}
	}

	//----------------------------------------------------------------------
	// わざの計画
	//----------------------------------------------------------------------
	else
	{
		struct BattleAct *act = BattleActGet(abilityId);

		plan->element = act->damageClass;
		plan->presentationFlags = 0;
		plan->abilityId = abilityId;
		if (act->effect == 65 || act->effect == 41 || act->effect == 42 || act->effect == 43 || act->effect == 44
			|| act->effect == 68)
		{
			// ある確率で、ダメージの倍率を上げる
			s32 chance;
			s32 amount;

			if (act->effect == 65 || act->effect == 68)
				chance = 153;
			else if (act->effect == 41 || act->effect == 43)
				chance = 32;
			else
				chance = 64;
			if (act->effect == 65 || act->effect == 41 || act->effect == 42)
				amount = 1;
			else
				amount = 2;
			if ((BattleRandomFar() & 255) < chance)
			{
				s32 i;

				for (i = 0; i < plan->targetCount; i++)
					plan->targetAdj[i] += amount;
			}
		}
		else if (act->effect >= 36 && act->effect <= 40)
		{
			// 会心の確率 1/64 ～ 1/4
			s32 mask;

			switch (act->effect)
			{
			case 36:
				mask = 63;
				break;
			case 37:
				mask = 31;
				break;
			case 38:
				mask = 15;
				break;
			case 39:
				mask = 7;
				break;
			case 40:
			default:
				mask = 3;
				break;
			}
			if ((BattleRandomFar() & mask) == 0)
			{
				s32 i;

				for (i = 0; i < plan->targetCount; i++)
					plan->targetCrit[i] = 2;
			}
		}
		else if (abilityId == 178) // めっさつのじゅつ：命中を先に決める
		{
			s32 i;

			for (i = 0; i < plan->targetCount; i++)
				plan->targetHit[i] =
					BattleHitCheck(req->unitId, plan->targetIds[i], act->damageClass, act->effect, 100);
		}
		if (abilityId >= 0 && abilityId <= 518)
		{
			plan->presentationFlags = gBattle_ActionFlags[abilityId];
			if (plan->targetAdj[0] > 1)
				plan->presentationFlags =
					gBattle_ActionFlags[abilityId] + (plan->targetAdj[0] << 12) - 0x1000;
		}
		if ((abilityId >= 0 && abilityId <= 517) && gBattle_ActionStatus[abilityId] != 0)
			plan->outcome = gBattle_ActionStatus[abilityId];
		else if (Ability_CheckStatusOrSpecialId(abilityId))
			plan->outcome = 3;
		else if (plan->presentationFlags != 0)
		{
			if (unit->classIndex == 0)
				plan->outcome = 8;
			else
				plan->outcome = 3;
		}
		else
			plan->outcome = 1;
		if (BattleEffectRevCheckFar(act->effect))
			plan->presentationFlags |= 0x10000;
		if (abilityId == 178 && plan->targetHit[0] != 0)
			plan->presentationFlags |= 0x1000;
	}
	if (req->command == CMD_ITEM && plan->outcome != 5 && plan->outcome != 9)
		plan->outcome = 4;
	plan->cmd = req->command;
	return 0;
}

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias)
{
	struct BattleUnit *state = UnitGetFar(unitId);

	if (turns <= 5)
	{
		s32 threshold = ((state->luck * 3 - turns * 5) + bias) * 0x28f;
		if (threshold >= (BattleRandomFar() & 0xffff))
			return 1;
	}
	return 0;
}

s32 BattleUnit_TickAttackModifier(s32 unitId)
{
	struct BattleUnit *state = UnitGetFar(unitId);

	if (state->atkCnt != 0)
	{
		s32 zero;
		state->atkCnt--;
		zero = 0;
		if (state->atkCnt == 0)
		{
			state->atkLv = zero;
			return 1;
		}
		if (state->atkLv < 0)
		{
			if (BattleUnit_RollCounterEarlyEnd(unitId, state->atkCnt, 30) != 0)
			{
				state->atkLv = zero;
				state->atkCnt = zero;
				return 1;
			}
		}
	}
	return 0;
}

// 能力変化の残りターン (counter) を 1 減らす。0 になったとき、または下がった能力
// (level が負) が BattleUnit_RollCounterEarlyEnd で早く切れたときに、level を戻して 1 を返す
#define DEFINE_TICK_MODIFIER(name, counter, level)                                   \
	s32 name(s32 unitId)                                                            \
	{                                                                                \
		struct BattleUnit *state = UnitGetFar(unitId);                                 \
                                                                                     \
		if (state->counter != 0)                                                     \
		{                                                                            \
			state->counter--;                                                        \
			if (state->counter == 0)                                                 \
			{                                                                        \
				state->level = 0;                                                    \
				return 1;                                                            \
			}                                                                        \
			if (state->level < 0                                                     \
				&& BattleUnit_RollCounterEarlyEnd(unitId, state->counter, 20) != 0) \
			{                                                                        \
				state->level = 0;                                                    \
				state->counter = 0;                                                  \
				return 1;                                                            \
			}                                                                        \
		}                                                                            \
		return 0;                                                                    \
	}

DEFINE_TICK_MODIFIER(BattleUnit_TickDefenseModifier, defCnt, defLv)
DEFINE_TICK_MODIFIER(BattleUnit_TickResistanceModifier, resCnt, resLv)

// 状態異常の残りターン (counter) を 1 減らす。0 になったとき、または
// BattleUnit_RollCounterEarlyEnd (bias) で早く切れたときに 1 を返す
#define DEFINE_TICK_STATUS(name, counter, bias)                                     \
	s32 name(s32 unitId)                                                           \
	{                                                                               \
		struct BattleUnit *state = UnitGetFar(unitId);                                \
		if (state->counter != 0)                                                    \
		{                                                                           \
			state->counter--;                                                       \
			if (state->counter == 0)                                                \
				return 1;                                                           \
			if (BattleUnit_RollCounterEarlyEnd(unitId, state->counter, bias) != 0) \
			{                                                                       \
				state->counter = 0;                                                 \
				return 1;                                                           \
			}                                                                       \
		}                                                                           \
		return 0;                                                                   \
	}

DEFINE_TICK_STATUS(BattleUnit_TickDelusion, delusion, 30)

s32 BattleUnit_TickConfusion(s32 unitId)
{
	struct BattleUnit *state = UnitGetFar(unitId);
	if ((u8)state->confusion != 0)
	{
		(*(u8 *)&state->confusion)--;
		if (state->confusion == 0)
			return 1;
		if (BattleUnit_RollCounterEarlyEnd(unitId, (u8)state->confusion, 60) != 0)
		{
			state->confusion = 0;
			return 1;
		}
	}
	return 0;
}

DEFINE_TICK_STATUS(BattleUnit_TickCharm, charm, 70)
DEFINE_TICK_STATUS(BattleUnit_TickStun, stun, 40)
DEFINE_TICK_STATUS(BattleUnit_TickSleep, sleep, 50)

s32 Battle_AdvanceCounterAndCheckChance(s32 id)
{
	s32				next;
	u8				remaining;
	s32				turns;
	s32				packed;
	struct BattleUnit *unit;

	unit = UnitGetFar(id);
	packed = unit->psySeal;
	turns = packed & 0xFF;
	if (turns != 0)
	{
		if (turns > 7U)
		{
			turns += 0xF8;
			unit->psySeal = turns;
			packed = turns;
		}
		if (packed & 7)
		{
			next = packed + 0xFF;
			unit->psySeal = next;
			packed = next;
		}
		remaining = packed;
		if (remaining == 0)
			return 1;
		if (remaining <= 7U && BattleUnit_RollCounterEarlyEnd(id, unit->psySeal, 0x1E) != 0)
		{
			unit->psySeal = 0U;
			return 1;
		}
	}
	return 0;
}

s32 BattleUnit_TickRefrain(s32 unitId)
{
	u8 *value = &UnitGetFar(unitId)->refrain;
	if (*value != 0)
	{
		(*value)--;
		if (*value == 0)
		{
			return 1;
		}
	}
	return 0;
}

s32 BattleUnit_TickReflect(s32 id)
{
	u8 *value = (u8 *)&UnitGetFar(id)->reflect;
	if (*value != 0)
	{
		(*value)--;
		if (*value == 0)
		{
			return 1;
		}
	}
	return 0;
}

s32 BattleUnit_TickAgilityModifier(s32 id)
{
	struct BattleUnit *unit = UnitGetFar(id);
	u8			   *value = &unit->agiCnt;
	if (*value != 0)
	{
		(*value)--;
		if (*value == 0)
		{
			unit->agiLv = 0;
			return 1;
		}
	}
	return 0;
}

s32 BattlePlacement_UpdateTimedEntries(void)
{
	struct DjinnRecoveryList  *list;
	struct DjinnRecoveryEntry *timedEntry;
	struct DjinnRecoveryEntry *expiredEntry;
	s32						   index;
	s32						   removed;
	s32						   initialCount;

	list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(0))->list;
	initialCount = list->count;
	index = 0;
	removed = 0;
	if (index < initialCount)
	{
		timedEntry = list->entries;
		do
		{
			if (timedEntry->turns > 0 && UnitGetFar(timedEntry->unitId)->hp != 0)
			{
				timedEntry->turns--;
			}
			index++;
			timedEntry++;
		} while (index < list->count);
	}
	index = 0;
	if (index < list->count)
	{
		expiredEntry = list->entries;
		do
		{
			if (expiredEntry->turns == 0)
			{
				u8 id = expiredEntry->unitId;

				Djinn_ActivateFar(id, expiredEntry->element, expiredEntry->index);
				Trade_RemoveOfferFar(id, expiredEntry->element, expiredEntry->index);
				UnitCalcFar(id);
				removed = 1;
			}
			else
			{
				expiredEntry++;
				index++;
			}
		} while (index < list->count);
	}
	return removed;
}

s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void)
{
	s32 turns;

	turns = 19;
	do
	{
		turns--;
		BattlePlacement_UpdateTimedEntries();
	} while (turns >= 0);
	return 0;
}

void Battle_ReservedNoOpF674(void)
{
}

struct BattleGroupOrder
{
	s32 group[2];
};

s32 Battle_ProcessRoundEnd(void)
{
	struct BattleWork			  *work;
	s32						   side;
	s32						   sides;
	s32						   count;
	s32						   i;
	struct DjinnRecoveryList  *list;
	struct DjinnRecoveryEntry *timedEntry;
	struct DjinnRecoveryEntry *expiredEntry;

	work = gBattleWork;
	sides = (work->twoSided != 0) + 1;
	for (side = 0; side < sides; side++)
	{
		list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(side))->list;
		i = 0;
		if (i < list->count)
		{
			timedEntry = list->entries;
			do
			{
				if (timedEntry->turns > 0 && BattleObjGet(timedEntry->unitId) != 0
					&& UnitGetFar(timedEntry->unitId)->hp != 0)
				{
					timedEntry->turns--;
				}
				i++;
				timedEntry++;
			} while (i < list->count);
		}
		i = 0;
		if (i < list->count)
		{
			expiredEntry = list->entries;
			do
			{
				if (expiredEntry->turns == 0)
				{
					u8	id = expiredEntry->unitId;
					s32 element;

					if (BattleObjGet(id) != 0)
					{
						BattleEventRuntime_Reset();
						BattleEventRuntime_SchedulePhase(30);
						BattleEvPush(BATTLE_EVENT_UNIT, id);
						BattleEvPush(
							BATTLE_EVENT_ACT, expiredEntry->element * 20 + expiredEntry->index + 300);
						BattleEvPush(BATTLE_EVENT_SE, 175);
						BattleEvPush(BATTLE_EVENT_REDRAW, 0);
						MESSAGE_PUSH(MsgDjinnSet);
						BattleEvPush(BATTLE_EVENT_OBJ_END, id);
						AudioCommand_PlayFar(212);
						ObjectDispatch_ApplyArgumentToChildrenFar(BattleObjGet(id)->object, 3);
						ObjectDispatch_ApplyValueToChildrenFar(
							(struct DispatchObject *)BattleObjGet(id)->object, 32);
						element = expiredEntry->element;
						Djinn_ActivateFar(id, element, expiredEntry->index);
						Trade_RemoveOfferFar(id, expiredEntry->element, expiredEntry->index);
						UnitCalcFar(id);
						BattleFx_PlayUnitElementEffect(id, element, 3, 0);
						BattleEventRuntime_WaitForReady();
					}
				}
				else
				{
					expiredEntry++;
					i++;
				}
			} while (i < list->count);
		}
	}
	BattlePresentation_ConfigurePaletteFade(2, gBattleWork->background, 0);
	{
		s32					   *groupList;
		struct BattleGroupOrder order = gBattleParty_RoundEndGroupOrder;
		u16						ids[14];
		s32						group;
		s32						j;

		if (work->twoSided != 0 && work->linkSide != 0)
		{
			order.group[0] = 2;
			order.group[1] = 1;
		}
		groupList = order.group;
		for (group = 0; group <= 1; group++)
		{
			count = BattleParty_ListActorIds(groupList[group], ids);
			for (j = 0; j < count; j++)
			{
				s32				hpAmount;
				s32				ppAmount;
				s32				id = ids[j];
				struct BattleUnit *unit = UnitGetFar(id);

				if (unit->ready != 0)
				{
					unit->ready--;
				}
				if (unit->hp != 0)
				{
					if (unit->hpRegen != 0 && unit->hp != unit->maxHp)
					{
						hpAmount = unit->hpRegen;
						if (unit->hp + hpAmount > unit->maxHp)
						{
							hpAmount = unit->maxHp - unit->hp;
						}
						Owner_AdjustFirstValueFar(id, hpAmount);
						UiWork_PushValueSlotFar(id, 1);
						UiWork_PushValueSlotFar(hpAmount, 5);
						if (unit->hp == unit->maxHp)
						{
							UiText_ShowMessageAndWaitCoreFar(MsgHpFull);
						}
						else
						{
							UiText_ShowMessageAndWaitCoreFar(MsgHpRecover);
						}
						AudioCommand_PlayFar(175);
						BattlePresentation_WaitForAdvance();
					}
					if (unit->ppRegen != 0 && unit->pp != unit->maxPp)
					{
						ppAmount = unit->ppRegen;
						if (unit->pp + ppAmount > unit->maxPp)
						{
							ppAmount = unit->maxPp - unit->pp;
						}
						UnitPpAddFar(id, ppAmount);
						UiWork_PushValueSlotFar(id, 1);
						UiWork_PushValueSlotFar(ppAmount, 5);
						if (unit->pp == unit->maxPp)
						{
							UiText_ShowMessageAndWaitCoreFar(MsgPpFull);
						}
						else
						{
							UiText_ShowMessageAndWaitCoreFar(MsgPpRecover);
						}
						AudioCommand_PlayFar(175);
						BattlePresentation_WaitForAdvance();
					}
				}
				if (BattleUnit_TickAgilityModifier(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgAgiNormal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickAttackModifier(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgAtkNormal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickDefenseModifier(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgDefNormal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickResistanceModifier(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgResNormal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickDelusion(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgCureDelusion);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickConfusion(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgConscious);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickCharm(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgConscious2);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickStun(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					BattlePres_SetActorModeAndAction(id);
					UiText_ShowMessageAndWaitCoreFar(MsgCureStun);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickSleep(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					BattlePres_SetActorModeAndAction(id);
					UiText_ShowMessageAndWaitCoreFar(MsgWakes);
					BattlePresentation_WaitForAdvance();
				}
				if (Battle_AdvanceCounterAndCheckChance(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgCureSeal);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickRefrain(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgRefrainEnd);
					BattlePresentation_WaitForAdvance();
				}
				if (BattleUnit_TickReflect(id) != 0)
				{
					BattleUnit_BuildStatusFlags(id, BattleObjGet(id));
					UiWork_PushValueSlotFar(id, 1);
					UiText_ShowMessageAndWaitCoreFar(MsgReflectEnd);
					BattlePresentation_WaitForAdvance();
				}
			}
		}
	}
}

s32 BattleUnit_ProcessTurnEnd(struct BattlePlan *plan)
{
	s32						  id;
	struct BattleUnit			 *unit;
	s32						  bothSides;
	s32						  counts[4];
	struct DjinnRecoveryList *list;
	s32						  best;
	s32						  order;

	id = plan->aid;
	bothSides = 0;
	unit = UnitGetFar(id);
	list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar((id < 0 || id > 7)))->list;
	for (best = 0; best < list->count; best++)
	{
		if (list->entries[best].unitId == id && list->entries[best].turns == -1)
		{
			Djinn_DeactivateFar(id, list->entries[best].element, list->entries[best].index);
		}
	}
	if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) != 0
		&& BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) != 0)
	{
		bothSides = 1;
	}
	{
		s32						  i;
		s32						  owner;
		struct DjinnRecoveryList *pending;
		pending = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar((id < 0 || id > 7)))->list;
		for (i = 0; i < 4; i++)
		{
			counts[i] = 0;
		}
		for (;;)
		{
			owner = -1;
			for (i = 0; i < pending->count; i++)
			{
				if (pending->entries[i].turns == -2)
				{
					owner = pending->entries[i].unitId;
					break;
				}
			}
			if (owner == -1)
			{
				break;
			}
			order = -1;
			for (i = 0; i < pending->count; i++)
			{
				if (pending->entries[i].unitId == owner && pending->entries[i].turns > order)
				{
					order = pending->entries[i].turns;
				}
			}
			order++;
			if (order <= 1)
			{
				order = 2;
			}
			for (i = 0; i < pending->count; i++)
			{
				if (pending->entries[i].unitId == owner && pending->entries[i].turns == -2)
				{
					pending->entries[i].turns = order;
					counts[pending->entries[i].element]++;
					order++;
				}
			}
		}
	}
	if (bothSides != 0)
	{
		struct BattleUnit *before;
		s32				most;
		s32				i;
		most = 0;
		before = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleUnit));
		Iwram_CopyWords(before, unit, sizeof(struct BattleUnit));
		best = -1;
		for (i = 0; i <= 3; i++)
		{
			if (counts[i] > most)
			{
				most = counts[i];
				best = i;
			}
		}
		if (best >= 0 && unit->elementLevels[best] < most)
		{
			unit->elementLevels[best] = most;
		}
		UnitCalcFar(id);
		for (i = 0; i <= 3; i++)
		{
			s32 gain = unit->element[i].power - before->element[i].power;

			if (gain > 0)
			{
				BattleEventRuntime_Reset();
				BattleEventRuntime_SchedulePhase(25);
				BattleEvPush(BATTLE_EVENT_UNIT, id);
				BattleEvPush(BATTLE_EVENT_NUM, gain);
				BattleEvPush(BATTLE_EVENT_SE, 175);
				MESSAGE_PUSH(MsgEarthPowerUp + i);
				BattleEvPush(BATTLE_EVENT_OBJ_END, id);
				AudioCommand_PlayFar(212);
				ObjectDispatch_ApplyArgumentToChildrenFar(BattleObjGet(id)->object, 3);
				ObjectDispatch_ApplyValueToChildrenFar(
					(struct DispatchObject *)BattleObjGet(id)->object, 32);
				BattleFx_PlayUnitElementEffect(id, i, 2, most - 1);
				BattleEventRuntime_WaitForReady();
			}
		}
		MemFree(before);
	}
	if (bothSides != 0)
	{
		s8 *poison;
		BattleEventRuntime_Reset();
		if (plan->hauntDmg != 0)
		{
			BattleEvPush(BATTLE_EVENT_OBJ_BEGIN, id);
			BattleEvPush(BATTLE_EVENT_UNIT, id);
			BattleEvPush(BATTLE_EVENT_NUM, plan->hauntDmg);
			MESSAGE_PUSH(MsgCurseDamage);
			if (Owner_AdjustFirstValueFar(id, -plan->hauntDmg) == 0)
			{
				s32 text;
				BattleEvPush(BATTLE_EVENT_OBJ_RESET, id);
				BattleEvPush(BATTLE_EVENT_UNIT, id);
				if (id >= 0 && id <= 7)
				{
					text = MsgGoesDown;
				}
				else
				{
					text = MsgStrengthExhausted;
				}
				MESSAGE_PUSH(text);
			}
			else
			{
				BattleEvPush(BATTLE_EVENT_OBJ_END, id);
			}
		}
		BattleEv_DispatchQueued();
		BattleEventRuntime_Reset();
		poison = &unit->poison;
		if (*poison != 0)
		{
			s32				damage = unit->maxHp * *poison / 10;
			struct BattleWork *state = gBattleWork;
			BattleEvPush(BATTLE_EVENT_OBJ_BEGIN, id);
			BattleEvPush(BATTLE_EVENT_UNIT, id);
			BattleEvPush(BATTLE_EVENT_NUM, damage);
			MESSAGE_PUSH(MsgPoisonDamage);
			if (*poison != 0)
			{
				state->events.pendingCue = SOUND_HEAVY_IMPACT;
			}
			else
			{
				state->events.pendingCue = 133;
			}
			if (Owner_AdjustFirstValueFar(id, -damage) == 0)
			{
				s32 text;
				BattleEvPush(BATTLE_EVENT_OBJ_RESET, id);
				BattleEvPush(BATTLE_EVENT_UNIT, id);
				if (id >= 0 && id <= 7)
				{
					text = MsgGoesDown;
				}
				else
				{
					text = MsgStrengthExhausted;
				}
				MESSAGE_PUSH(text);
			}
			else
			{
				BattleEvPush(BATTLE_EVENT_OBJ_END, id);
			}
		}
		BattleEv_DispatchQueued();
		BattleEventRuntime_Reset();
		if (unit->grimReaper != 0)
		{
			if (--unit->grimReaper == 0 && Owner_AdjustFirstValueFar(id, 0xc0000000) == 0)
			{
				BattleEvPush(BATTLE_EVENT_UNIT, id);
				MESSAGE_PUSH(MsgReaperCalls);
				BattleEvPush(BATTLE_EVENT_OBJ_BEGIN, id);
				BattleEvPush(BATTLE_EVENT_OBJ_RESET, id);
				BattleEvPush(BATTLE_EVENT_UNIT, id);
				if (id >= 0 && id <= 7)
				{
					MESSAGE_PUSH(MsgReaperCalls - 3);
				}
				else
				{
					MESSAGE_PUSH(MsgReaperCalls + 3);
				}
			}
		}
		BattleEv_DispatchQueued();
	}
	UnitCalcFar(id);
}

s32 BattlePres_RunMosaicFadeOut(void)
{
	/* FAKEMATCH: volatile restores the form the bytes prove: the four saved BGxCNT values live at sp+6, 4, 2 and 0, each stored with strh and loaded again with ldrh for the write back (an 8-byte frame). Without all four differ 61, 83 instructions for 90 and no frame; without one alone, 27 to 45; as a u16 saved[4] array, in either order: differ 60. */
	volatile u16  bg0cnt;
	volatile u16  bg1cnt;
	volatile u16  bg2cnt;
	volatile u16  bg3cnt;
	s32			  i;
	volatile u16 *reg;

	bg0cnt = REG_BG0CNT;
	REG_BG0CNT = bg0cnt | 0x40;
	reg = &REG_BG0CNT;
	bg1cnt = *++reg;
	*reg = bg1cnt | 0x40;
	bg2cnt = *++reg;
	*reg = bg2cnt | 0x40;
	bg3cnt = *++reg;
	*reg = bg3cnt | 0x40;
	reg += 0x21;
	*reg = 0x3eee;

	Blend_SetDarkenTarget16(16);
	for (i = 0; i <= 15; i++)
	{
		Random16();
		Random16();
		Random16();
		Random16();
		REG_MOSAIC = (i << 8) | i;
		WaitFrames(1);
	}

	REG_DISPCNT = 1;
	WaitFrames(4);
	reg = &REG_BG0CNT;
	*reg++ = bg0cnt;
	*reg++ = bg1cnt;
	*reg++ = bg2cnt;
	*reg = bg3cnt;
	return 0;
}

//--------------------------------------------------------------------------
// build_sequential_table.c
// タイル：1 バイトずつ連番のマップを作る
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 0〜255 と 0〜223 の連番のバイトを続けて書き、
// 残りの 544 バイトを 0xff で埋める（あわせて 1024 バイト）
//--------------------------------------------------------------------------
void Graphics_BuildSequentialTileTable(s32 *dst)
{
	s32 v;
	s32 n;

	// 4 バイトずつ書くので 1 回に 4 つ進める
	v = 0x03020100;
	n = 0;
	do
	{
		n += 1;
		*dst++ = v;
		v += 0x04040404;
	} while (n >= 0 && n <= 63);

	v = 0x03020100;
	n = 0;
	do
	{
		n += 1;
		*dst++ = v;
		v += 0x04040404;
	} while (n >= 0 && n <= 55);

	Iwram_FillWords(dst, 544, -1);
}

void BattlePresentation_BuildTilemap(s32 *destination)
{
	s32 entry;
	u32 index;

	Iwram_FillWords(destination, 0x100, -1);
	destination += 0x40;
	Iwram_FillWords(destination, 0x80, 0x03ff03ff);
	entry = (0x0201 << 16) | 0x0200;
	destination += 0x20;
	index = 0;
	do
	{
		index++;
		*destination++ = entry;
		entry += 0x00020002;
	} while (index <= 239);
	Iwram_FillWords(destination, 0x280, 0x03ff03ff);
}

void BattlePres_UpdateHBlankScroll(void)
{
	struct BattleAffineHdma *records;
	u16						*record;

	if (((struct BattleBackgroundView *)WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND))->mode == 2)
	{
		records = WORK_SLOT(10);
		record = records->lines[records->page];
		Io_Write16(record[0], &REG_BG2CNT);
		Dma_Set(record + 1, (void *)0x0400000c, 0xa2600001, &REG_DMA0SAD);
		Dma_Set(&records->affine, (void *)0x04000020, 0x84000004, REG_DMA3);
	}
}

void BattlePresentation_UploadTileVariant(void)
{
	u32 variant = *gDisp - 1;
	if (variant <= 31)
		Dma_Set(gBattlePres_TileVariants + (variant >> 2) * 32, (void *)0x06005000, 0x84000008,
			REG_DMA3);
}

void BattlePres_AdvanceTransitionTimer(void)
{
	s32					 scroll;
	struct BattleCamera *camera;
	u32					*timer;
	struct BgScroll		*offset;
	u32					 frame;
	u32					 next;

	timer = *(u32 **)Ram_Disp;
	frame = *timer;
	camera = *(struct BattleCamera **)Ram_CameraWork;
	scroll = 0x34 - frame;
	if (scroll > 0x20)
		scroll = 0x20;
	offset = (struct BgScroll *)Ram_BgScroll;
	if (scroll < 0)
		scroll = 0;
	offset->y = scroll;
	if (frame <= 0x50U)
		camera->yaw = 45 * frame * 8 + 0xaf80;
	next = (*timer = *timer + 1);
	if (next <= 0x50U)
	{
		BattlePres_SetupTransitionScene(0, 0, 0, 0xb4 - next);
		return;
	}
	BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
}

void BattlePres_DrawTransitionRows(void)
{
	u32	 i;
	s32	 frame;
	s32	 aligned;
	s32	 tile;
	u32	 row;
	u16 *tiles;

	frame = *gDisp;
	if (frame >= 0 && frame <= 79)
	{
		tile = (7 & frame) + 0xf081;
		if (frame >= 0)
		{
			aligned = frame;
		}
		else
		{
			aligned = frame + 7;
		}
		row = 13 - (aligned >> 3);
		i = 0;
		tiles = (u16 *)((row << 6) + 0x06006000);
		do
		{
			i++;
			*tiles = tile;
			tiles++;
		} while (i != 32);

		tile |= 0x800;
		aligned = frame;
		if (frame < 0)
		{
			aligned += 7;
		}
		row = (aligned >> 3) + 13;
		if (row <= 20)
		{
			i = 0;
			tiles = (u16 *)((row << 6) + 0x06006000);
			do
			{
				i++;
				*tiles = tile;
				tiles++;
			} while (i != 32);
		}
	}
}

void Graphics_ClearBg0Vofs(void)
{
	u32 zero = 0;

	REG_BG0VOFS = zero;
}

struct DisplayRegisters
{
	u16 control;
	u16 greenSwap;
};

void BattlePres_RunEntryTransition(s32 enemyCount, s32 kind)
{
	volatile u32				  fill;
	struct BattlePresentationWork list;
	u16							  party[14];
	u16							  actors[14];
	struct BattleBackgroundView	 *work;
	s32							 *timer;
	u16							 *map;
	s32							  tile;
	u32							  i;
	u32							  j;
	s32							  count;
	s32							  id;
	struct BattleObj				 *object;
	struct BattleWork				 *session;
	u16							 *ids;
	s32							  n;

	work = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
	timer = Runtime_AllocateHeapBlock(42, 4);
	if (kind != 0x15b)
	{
		{

			volatile u32 *dma;

			Dma_Set(gBattlePres_CurtainTiles, (void *)0x06005020, 0x84000008, REG_DMA3);
			Dma_Set(gBattlePres_CurtainTiles + 32, (void *)0x06005040, 0x84000008, REG_DMA3);
			Dma_Set(gBattlePres_CurtainTiles + 64, (void *)0x06005060, 0x84000008, REG_DMA3);
			Dma_Set(gBattlePres_CurtainTiles + 96, (void *)0x06005080, 0x84000008, REG_DMA3);
			Dma_Set(gBattlePres_CurtainTiles + 128, (void *)0x060050a0, 0x84000008, REG_DMA3);
			Dma_Set(gBattlePres_CurtainTiles + 160, (void *)0x060050c0, 0x84000008, REG_DMA3);
			Dma_Set(gBattlePres_CurtainTiles + 192, (void *)0x060050e0, 0x84000008, REG_DMA3);
			dma = REG_DMA3;
			/* FAKEMATCH: introducing the pointer after the transfers avoids a preserved-register copy; the volatile output orders its subtraction before the control value, differing by two instructions without volatile. 2026-10-11: the native subs r3, #212 is reload_cse_move2add on a constant address that happened to land in r3; plain pointer arithmetic is CSE-reused by the later Dma_Sets (differ 45), REG_DISPCNT = 1 takes a pool constant (63), and a constant struct-field store gets r2 from local-alloc, so no move2add (31). */
			asm volatile("" : "+l"(dma));
			((struct DisplayRegisters *)((u8 *)dma - 212))->control = 1;
		}
		work->secondMode = 1;
		work->mode = 1;
		work->busy = 0;
		Dma_Fill32Cell(fill, 0x33333333, (void *)0x06005000, 0x20);
		Dma_Fill32Cell(fill, 0, (void *)0x06005100, 0x20);
		REG_BG1CNT = 0xc04;
		REG_BG0CNT |= 2;
		work->mode = 2;
		map = (u16 *)0x06006000;
		for (i = 0; i <= 31; i++)
		{
			if (i <= 20)
				tile = 0xf080;
			else
				tile = 0xf088;
			for (j = 0; j <= 31; j++)
				*map++ = tile;
		}
		{
			s32 start = 0;

			gBgScroll[0].y = 32;
			gBgScroll[1].y = 32;
			gBgScroll[1].x = 8;
			WaitFrames(1);
			REG_WIN0H = 0xf0;
			REG_WIN0V = 0x88;
			REG_WIN1H = 0xf0;
			REG_WIN1V = 0x88;
			REG_WININ = 0x3537;
			REG_WINOUT = 0x3f21;
			QueueIoWriteDelay2(0x04000000, 0x7741);
			BattlePres_SetupTransitionScene(0, 0, 0, 180);
			*timer = start;
			Scheduler_AddOrUpdateCallback(BattlePres_AdvanceTransitionTimer, 3200);
			Scheduler_AddOrUpdateCallback(BattlePres_DrawTransitionRows, 0x480);
			Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
			gBgScroll[0].y = 32;
			WaitFrames(1);
			UiWindow_CreateWithLayoutBoundsFar(gBattleWork->partyStatus);
			WaitFrames(20);
			QueueIoWriteDelay10(0x04000008, 2);
			QueueIoWriteDelay6(0x04000008, 0);
			BattleIntro_AnnounceEncounter(enemyCount);
			Scheduler_RemoveCallback(BattlePres_AdvanceTransitionTimer);
			Scheduler_RemoveCallback(BattlePres_DrawTransitionRows);
			gBgScroll[0].y = start;
			Runtime_SetIrqHandler(2, 0, 0);
		}
	}
	else
	{
		session = WORK_SLOT(HEAP_SLOT_BATTLE);
		work->secondMode = 1;
		work->busy = 0;
		count = BattleParty_ListActorIds(3, (u16 *)party);
		for (i = 0; i != count; i++)
		{
			id = i + 120;
			if ((s32)i <= 7)
				id = i;
			object = BattleObjGet(id);
			if (UnitGetFar(id)->classId != 148)
				object->scale = 0xb333;
		}
		IO_WRITE_QUEUE_PUSH(0x04000000, 0x6041, 0x20000);
		WaitFrames(1);
		{
			s32 start = 0;

			session->background = (u32)&ResourceId_VenusAerieStormBackdrop;
			list.count = BattleParty_ListActorIds(2, list.actors);
			list.actors[list.count] = 0xff;
			BattleActor_SpawnObjectsForList(list.actors, 0);
			BattleEffect_RunTileAndPaletteAnimationFar(&list);
			BattlePres_SetupTransitionScene(0, 0, 0, 100);
			*timer = start;
		}
		Runtime_SetIrqHandler(2, 32, Graphics_ClearBg0Vofs);
		WaitFrames(1);
		WaitFrames(20);
		UiWindow_CreateWithLayoutBoundsFar(gBattleWork->partyStatus);
		QueueIoWriteDelay10(0x04000008, 2);
		QueueIoWriteDelay6(0x04000008, 0);
		REG_BLDCNT = 0x3f40;
		ids = actors;
		n = BattleParty_ListActorIds(3, (u16 *)ids);
		actors[n] = 0xff;
		BattleActor_SpawnObjectsForList(ids, 0);
		n = BattleParty_ListActorIds(1, (u16 *)ids);
		for (i = 0; i != n; i++)
			BattlePres_SetActorRecordMode(ids[i], 1);
		for (i = 0; i != 16; i++)
		{
			REG_BLDALPHA = i | 0x1000;
			WaitFrames(1);
		}
		for (i = 0; i != n; i++)
			BattlePres_SetActorRecordMode(ids[i], 0);
		BattleIntro_AnnounceEncounter(enemyCount);
		gBgScroll[0].y = 0;
		WaitFrames(1);
		Runtime_SetIrqHandler(2, 0, 0);
	}
	Runtime_SetIrqHandler(2, 0, 0);
	REG_BG1CNT = 0x1f83;
	WaitFrames(1);
	REG_BG1CNT = 0x1f83;
	REG_BG0CNT &= 0xfffd;
	gBgScroll[1].x = 8;
	REG_DISPCNT = 0x1541;
	Runtime_ReleaseHeapBlock(42);
}

void BattlePresentation_SetPaletteLevel(s32 unused, s32 level)
{
	struct BattleWork *screen = gBattleWork;
	u16			   *palette = screen->palette;
	u32				ime;
	u32			   *slot = &ime;

	/* FAKEMATCH: needed to match: the one-pass block and the loop on a zero local keep the IME save through slot's register. As a plain block, also with a volatile register pointer, the save is stored sp-relative, ahead of the palette address: differ 8; as IRQ_LOCK(*slot) and IRQ_UNLOCK the save is stored sp-relative and the restore loads the register address first: differ 8; plain statements restoring REG_IME = *slot: differ 14. */
	do
	{
		u16 *ime_reg = (u16 *)0x04000208;
		s32	 once = 0;

		do
		{
			*slot = *ime_reg;
			*ime_reg = (u32)ime_reg;
		} while (once);
	} while (0);

	if (level == 0)
	{
		Dma_Set(palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
	}
	else
	{
		Graphics_ScaleRgb555Clamped(
			palette, (u16 *)0x050000c0, screen->brightness = 0x10000 - level * 1092, 128);
	}
	{
		u32 saved = *slot;

		REG_IME = saved;
	}
}

void BattlePresentation_ConfigurePaletteFade(s32 mode, s32 value, s32 fade)
{
	struct BattleBackgroundView *transition = (struct BattleBackgroundView *)gTransitionWork;

	if (transition->mode == 0)
	{
		Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
	}
	transition->mode = mode;

	if (mode == 1)
	{
		IO_WRITE_QUEUE_PUSH(0x0400000a, 0x1f83, 0x20000);
	}

	Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
	*(u16 *)0x050000bc = *(u16 *)0x050001e8;

	if (fade == 0x80)
	{
		Dma_Set(gBattleWork->palette, (void *)0x050000c0, 0x80000080, REG_DMA3);
	}
	else if (fade != 0)
	{
		u16 *source = gBattleWork->palette;
		u16 *destination = (u16 *)0x050000c0;
		s32	 i;

		for (i = 0; i != 128; i++)
		{
			s32 red = source[i] & 31;
			s32 green = (source[i] >> 5) & 31;
			s32 blue = (source[i] >> 10) & 31;

			if (red > fade)
			{
				red -= fade;
			}
			else
			{
				red = 0;
			}
			if (green > fade)
			{
				green -= fade;
			}
			else
			{
				green = 0;
			}
			if (blue > fade)
			{
				blue -= fade;
			}
			else
			{
				blue = 0;
			}
			destination[i] = (blue << 10) | (green << 5) | red;
		}
	}

	Graphics_BuildSequentialTileTable((void *)0x06003800);
	BattlePresentation_BuildTilemap((void *)0x0600f800);
}

void BattlePresentation_InitializeWorkAndResetState(void)
{
	struct BattleAffineHdma		*work;
	struct BattleBackgroundView *state;
	work = Runtime_AllocateBlock(10, sizeof *work);
	state = (struct BattleBackgroundView *)gTransitionWork;
	Dma_Fill32(0, work, sizeof *work);
	state->mode = 0;
}

void Runtime_ReleaseHeapBlock10(void)
{
	Runtime_ReleaseHeapBlock(10);
}

typedef void (*BitDecoder)(const u8 *source, void *destination);

void BattleBackground_Load(s32 mode, s32 resource, s32 level)
{
	struct BattleBackgroundView *view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
	u8							*data = Resource_GetTableEntry(resource);
	struct BattleWork				*session = WORK_SLOT(HEAP_SLOT_BATTLE);
	u16							*palette;

	RamCode_LoadSlot(49, BitDecoder_DecodeImage, BitDecoder_Size);
	{
		u32	  offset = 0x100;
		void *vram;

		/* FAKEMATCH: conditional cancelling updates end CSE before the
		 * decoder arguments, keeping the VRAM load before the data add.
		 * They disappear from all 103 instructions in every edition;
		 * a reassignment or XOR pair instead differs by four rows.
		 * Plain, with data + 0x100 and the VRAM address as the call's
		 * arguments, the data add is scheduled first: differ 2; through
		 * the vram local alone: differ 4; through a decoder local:
		 * differ 79. */
		if (session)
		{
			offset++;
			offset--;
		}
		vram = (void *)0x06008000;
		((BitDecoder)WORK_SLOT(49))(data + offset, vram);
	}
	Runtime_ReleaseHeapBlock(49);
	palette = session->palette;
	Dma_Set(data, palette, 0x84000040, REG_DMA3);
	if (level >= 0)
		Graphics_ScaleRgb555Clamped(
			palette, (u16 *)0x050000c0, session->brightness = 0x10000 - level * 1092, 128);
	Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, REG_DMA3);
	*(u16 *)0x050000bc = *(u16 *)0x050001e8;
	Graphics_BuildSequentialTileTable((void *)0x06003800);
	BattlePresentation_BuildTilemap((void *)0x0600f800);
	Iwram_ClearWords((void *)0x0600ffc0, 64);
	if (view->mode == 0)
		Scheduler_AddOrUpdateCallback(BattlePres_UpdateHBlankScroll, 0x4ff);
	view->mode = mode;
	if (mode == 1)
		REG_BG1CNT = 0x1f83;
}

void BattleCamera_SetRange(s32 cx, s32 cy, s32 ox, s32 oy, s32 scale)
{
	struct BattleBackgroundView *view = WORK_SLOT(HEAP_SLOT_BATTLE_BACKGROUND);
	struct BattleAffineHdma		*work = WORK_SLOT(10);
	struct BattleCamera			*camera = WORK_SLOT(HEAP_SLOT_CAMERA);
	u16							 wrap;
	s32							 horizon;
	u16							*line;
	struct BattleAffine			*affine;
	s32							 ratio;
	s32							 step;
	s32							 x;
	s32							 y;
	u32							 i;
	u32							 count;

	horizon = 0x800;
	wrap = 0;
	if (scale >= 0x10000)
	{
		wrap = 0x2000;
		horizon = 0x6800 + -(s16)camera->yaw * 3;
	}
	if (work == NULL)
		return;
	if ((view->mode == 1 || view->secondMode == 1) && view->busy == 0)
		gBgScroll[1].x = horizon >> 8;
	if (view->mode != 2)
		return;
	line = work->lines[work->page ^ 1];
	ratio = Iwram_RatioMulQ14(scale, 0x10000);
	step = ratio >> 8;
	affine = &work->affine;
	affine->pa = step;
	affine->pb = 0;
	affine->pc = 0;
	affine->pd = step;
	x = Iwram_MulQ16(ratio, Iwram_MulQ16(cx, scale - 0x10000));
	y = Iwram_MulQ16(ratio, Iwram_MulQ16(cy, scale - 0x10000));
	affine->x = ((x + 0x7fff) >> 8) + ox + horizon;
	y = ((y + 0x7fff) >> 8) + oy - 0x1000;
	affine->y = y;
	count = (Iwram_RatioMulQ14((s16)step, 0x4000 - y) >> 16) + 1;
	i = 0;
	if (!GameFlag_TestFar(0x16b))
	{
		for (; i < 16; i++)
			*line++ = 0x3f8e;
	}
	if (count > 136)
		count = 136;
	for (; i < count; i++)
		*line++ = wrap | 0x478a;
	for (; i < 136; i++)
		*line++ = wrap | 0x478e;
	for (; i < 160; i++)
		*line++ = 0x3f8e;
	work->page ^= 1;
}

void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y)
{
	s32 span = 0x01fe0000;
	// 元のバグ：mode は初期化されず、入口の r4 をそのまま読む（lsls r0, r4, #16）
	s32					 mode;
	struct BattleCamera *scene = gCameraWork;
	s32					*pos = scene->pos;
	s32					 scale = (mode << 16) / 100;
	s32					 renderBounds[3];
	s32					 measuredBounds[3];
	s32					 sourceBounds[3];
	s32					 alpha;
	s32					 half;
	s32					 width;
	u32					 result;

	pos[0] = x;
	pos[1] = depth;
	pos[2] = y;
	sourceBounds[0] = 0;
	alpha = 0xc000;
	sourceBounds[1] = 0;
	sourceBounds[2] = 0;

	result = Iwram_RatioMulQ14(span, alpha);
	Camera_StoreSceneParameters(span, result, span * 2);
	Render_ResetTransformState();
	SceneTransform_ApplyPosition(pos);
	SceneTransform_ApplyYaw((s16)scene->yaw);
	SceneTransform_ApplyPitch((s16)scene->pitch);
	renderBounds[0] = 0;
	renderBounds[1] = 0;
	renderBounds[2] = scene->distance;
	Iwram_TransformVector(renderBounds, (s32 *)scene);
	gProjection.centerX = 120;
	gProjection.centerY = 120;
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(scene->eye, pos);
	Render_ProjectPoint(sourceBounds, measuredBounds);

	BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
		(120 - measuredBounds[1]) << 8, scale);
	half = scale * 0xff;
	width = half * 2;
	result = Iwram_RatioMulQ14(width, alpha);
	Camera_StoreSceneParameters(width, result, half * 4);
}

void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 zoom)
{
	struct BattleCamera *scene = gCameraWork;
	s32					*pos = scene->pos;
	s32					 scale = (zoom << 16) / 100;
	s32					 renderBounds[3];
	s32					 measuredBounds[3];
	s32					 sourceBounds[3];
	s32					 span = 0x01fe0000;
	s32					 alpha;
	s32					 half;
	s32					 width;
	u32					 result;

	pos[0] = x;
	pos[1] = depth;
	pos[2] = y;
	sourceBounds[0] = 0;
	alpha = 0xc000;
	sourceBounds[1] = 0;
	sourceBounds[2] = 0;

	result = Iwram_RatioMulQ14(span, alpha);
	Camera_StoreSceneParameters(span, result, span * 2);
	Render_ResetTransformState();
	SceneTransform_ApplyPosition(pos);
	SceneTransform_ApplyYaw((s16)scene->yaw);
	SceneTransform_ApplyPitch((s16)scene->pitch);
	renderBounds[0] = 0;
	renderBounds[1] = 0;
	renderBounds[2] = span;
	Iwram_TransformVector(renderBounds, (s32 *)scene);
	gProjection.centerX = 120;
	gProjection.centerY = 120;
	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(scene->eye, pos);
	Render_ProjectPoint(sourceBounds, measuredBounds);

	BattleCamera_SetRange(0x780000, 0x780000, (120 - measuredBounds[0]) << 8,
		(120 - measuredBounds[1]) << 8, scale);
	half = scale * 0xff;
	width = half * 2;
	result = Iwram_RatioMulQ14(width, alpha);
	Camera_StoreSceneParameters(width, result, half * 4);
}

//--------------------------------------------------------------------------
// setup_transition_at_pair_midpoint.c
// バトル表示：ふたりの中間へのカメラ合わせ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ふたつのオブジェクトの中間（x, z）にカメラを合わせる
// a, b : オブジェクト番号
// zoom : 拡大率（%）
//--------------------------------------------------------------------------
void BattlePres_SetupTransitionAtPairMidpoint(s32 a, s32 b, s32 zoom)
{
	struct MotionObject *obj_a = BattleObjGet(a)->object;
	struct MotionObject *obj_b = BattleObjGet(b)->object;
	s32					 ax = obj_a->x;
	s32					 bx = obj_b->x;
	s32					 az = obj_a->z;
	s32					 bz = obj_b->z;
	s32					 x = (bx + ax) / 2;
	s32					 z = (bz + az) / 2;

	BattlePres_SetupTransitionScene(x, 0, z, zoom);
}

void Graphics_FadeAlphaOut(void)
{
	s32 alpha_step;

	REG_BLDCNT = 0x2044;
	for (alpha_step = 1; alpha_step <= 16; alpha_step += 2)
	{
		REG_BLDALPHA = 0x1010 - alpha_step;
		WaitFrames(1);
	}
}

void Graphics_FadeAlphaIn(void)
{
	s32 step;

	REG_BLDCNT = 0x2044;
	for (step = 1; step <= 16; step += 2)
	{
		REG_BLDALPHA = step + 0x1000;
		WaitFrames(1);
	}
}

struct BattleTransitionEntry
{
	u32 value;
	u32 sum;
	u32 field8;
	u32 fieldc;
};

void Graphics_SetBlendControl(void)
{
	REG_BLDCNT = 0xbf;
}

void BattlePresentation_InitializeTransitionEntries(struct BattleTransitionEntry *entries)
{
	u32 previous = entries[0].value;

	CAMELOT_ASM("the fixed-register identity store of transform.h")
	Transform_SetIdentity(entries);
	entries[0].sum = previous + entries[0].value;
}

s32 BattlePres_DivideBy16(s32 value)
{
	return value / 16;
}

#if EDITION_INTERNATIONAL
#define DEBUG_SELECT_VALUE 0x18f
#else
#define DEBUG_SELECT_VALUE 480
#endif

s32 Runtime_AdjustDebugValueWithButtons(s32 ret)
{
	struct BattleWork *battle;

	if (gKeysHeld & KEY_START)
	{
		for (;;)
		{
			battle = gBattleWork;
			if (gKeysRepeat & 0x20)
				battle->debugValue -= 1;
			if (gKeysRepeat & 0x10)
				battle->debugValue += 1;
			if (gKeysRepeat & 0x40)
				battle->debugValue -= 100;
			if (gKeysRepeat & 0x80)
				battle->debugValue += 100;
			if (gKeyState & KEY_A)
				ret = battle->debugValue;
			else
			{
				WaitFrames(1);
				continue;
			}
			break;
		}
	}
	if (gKeysHeld & KEY_SELECT)
		ret = DEBUG_SELECT_VALUE;
	return ret;
}

void BattlePres_SetActorRecordMode(s32 actorId, s32 mode)
{
	struct BattleObj			*slot = BattleObjGet(actorId);
	struct MotionObject		*object;
	struct AnimationObject	*record;
	struct AnimationObject **records;
	s32						 i;

	if (slot == 0)
		return;
	object = slot->object;
	if (object == 0)
		return;
	switch (object->recordStorageKind & 15)
	{
	case 1:
	{
		record = object->records;
		record->part[0].objectMode = mode;
		record->part[1].objectMode = mode;
		break;
	}
	case 2:
	{
		records = object->records;
		i = 0;
		do
		{
			record = *records++;
			if (record == 0)
				break;
			record->part[0].objectMode = mode;
			record->part[1].objectMode = mode;
			i++;
		} while (i <= 3);
		break;
	}
	}
}

//--------------------------------------------------------------------------
// present.c
// バトル表示：ユニットの絵の半透明の切りかえ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// except 以外の全ユニットの絵を半透明（OBJ モード 1）にする
// except : そのままにするユニット番号
//--------------------------------------------------------------------------
void BattlePres_SetOtherActorRecordModes(s32 except)
{
	s16 list[14];
	s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	s32 i;

	for (i = 0; i < cnt; i++)
	{
		if (list[i] != except)
			BattlePres_SetActorRecordMode(list[i], 1);
	}
}

//--------------------------------------------------------------------------
// 全ユニットの絵をふつう（OBJ モード 0）にもどす
//--------------------------------------------------------------------------
void BattlePres_ClearAllActorRecordModes(void)
{
	s16 list[14];
	s32 cnt = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, list);
	s32 i;

	for (i = 0; i < cnt; i++)
		BattlePres_SetActorRecordMode(list[i], 0);
}

void Graphics_AdvancePaletteCycle(void)
{
	struct BattleWork *battle = gBattleWork;
	s8			   *table;
	u16				step;
	s32				next;

	if (battle != NULL && battle->highlightMode != 0)
	{
		REG_BLDCNT = 0x3f90;
		REG_BLDALPHA = 0x10;
		table = gBattleFx_HighlightBlendRamp;
		REG_BLDY = table[battle->highlightStep];
		step = battle->highlightStep;
		next = (step + 1) & 0xf;
		if (step > 14)
			next |= 0x10;
		battle->highlightStep = next;
	}
}

void BattlePres_SetActorModes(u16 *actors, s32 mode)
{
	s16				activeActors[14];
	struct BattleWork *battle = gBattleWork;
	u32				count;
	u32				i;

	if (mode == 0)
	{
		Scheduler_RemoveCallback(Graphics_AdvancePaletteCycle);
		REG_BLDY = mode;
		BattlePres_ClearAllActorRecordModes();
		WaitFrames(1);
		QueueIoWriteDelay2(0x04000050, 0);
	}
	if (battle != 0 && mode != 0)
	{
		battle->highlightMode = mode;
		battle->highlightStep = 0;
		REG_BLDY = 0;
		Io_Write16(16, &REG_BLDALPHA);

		count = BattleParty_ListActorIds(3, activeActors);
		for (i = 0; i < count; i++)
			BattlePres_SetActorRecordMode(activeActors[i], mode & 1);

		if (actors != 0)
		{
			u32 actor = *actors;
			i = 0;
			actors++;
			if (actor != 0xff)
			{
				do
				{
					BattlePres_SetActorRecordMode(actor, (mode & 1) ^ 1);
					i++;
					if (i > 13)
						break;
					actor = *actors++;
				} while (actor != 0xff);
			}
		}
		WaitFrames(1);
		QueueIoWriteDelay2(0x04000050, 0);
		Scheduler_AddOrUpdateCallback(Graphics_AdvancePaletteCycle, 0x480);
	}
}

struct Spark
{
	s32 pos[6];
	s32 life;
};

struct SparkWork
{
	u8			 sheet[0x11c0];
	struct Spark sparks[16];
	struct Ring	 rings[3];
	s32			 frames;
	s32			 ready;
	s32			 originX;
	s32			 originY;
	s32			 originPending;
};

typedef void (*DrawFunc)(void *dest, u8 *src, s32 x, s32 y, s32 width, s32 height);

s32 BattleFx_UpdateStarField(void)
{
	// 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
	s32				  j;
	s32				  frame;
	u32				  size;
	s32				  ringX;
	s32				  ringY;
	s32				  phase;
	void			 *dest;
	s32				  i;
	struct SparkWork *work;
	DrawFunc		  draw;
	struct Spark	 *spark;
	struct Ring		 *ring;
	s32				 *pos;
	s32				  k;
	s32				  dist;
	s32				  scale;
	s32				  value;
	s32				  step;
	u32				  radius;
	s32				  angle;
	u32				  half;
	s32				  x;
	s32				  y;
	s32				  life;

	dest = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	work->ready = 0;
	draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER];
	for (i = 0; i < 16; i++)
	{
		spark = &work->sparks[i];
		life = spark->life;
		if (life != 0)
		{
			dist = FixedSqrt((spark->pos[0] >> 8) * (spark->pos[0] >> 8)
							 + (spark->pos[1] >> 8) * (spark->pos[1] >> 8)
							 + (spark->pos[2] >> 8) * (spark->pos[2] >> 8));
			if (dist <= 0xfff)
			{
				spark->life = 0;
			}
			else
			{
				scale = Iwram_RatioMulQ14(dist, 0x10000);
				spark->life--;
				pos = spark->pos;
				for (k = 2; k >= 0; k--)
				{
					value = *pos;
					step = Iwram_MulQ16(Iwram_MulQ16(-value >> 8, scale), 0x13000);
					pos[3] = pos[3] - (pos[3] >> 7) + step;
					*pos = value + pos[3];
					pos++;
				}
			}
			life = spark->life;
		}
		if (life == 0 && work->frames <= 24)
		{
			angle = Random16();
			radius = Random16() + 0x10000;
			half = radius >> 1;
			spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
			spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
			if (spark->pos[0] & 1)
				spark->pos[0] = -spark->pos[0];
			if (spark->pos[1] & 1)
				spark->pos[1] = -spark->pos[1];
			spark->pos[2] = (Random16() + 0x8000) >> 2;
			spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
			spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
			spark->pos[5] = 0;
			life = spark->life = (radius >> 13) + 1;
		}
		if (life != 0)
		{
			x = (spark->pos[0] >> 10) + 64;
			y = (spark->pos[1] >> 10) + 64;
			frame = life;
			if (frame < 0)
				frame = 0;
			else if (frame > 6)
				frame = 6;
			size = gBattleFx_StarSizes[frame];
			draw(dest, (u8 *)work + gBattleFx_StarWorkOffsets[frame], x - (size >> 1),
				y - (size >> 1), size, size);
		}
	}
	draw = (DrawFunc)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	for (ring = work->rings, j = 2; j >= 0; j--, ring++)
	{
		ring->x += ring->velX;
		ring->y += ring->velY;
		ringX = ring->x >> 10;
		ringY = ring->y >> 10;
		phase = 3 - ring->age / 8;
		if (phase >= 0)
		{
			ring->age++;
			draw(dest, (u8 *)work + gBattleFx_RingFrameOffsets[phase], ringX + 48, ringY + 48, 32,
				32);
		}
	}
	work->frames++;
	work->ready = 1;
}

u32 Graphics_UploadVramBlock(void)
{
	void			 *source = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	struct SparkWork *work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];

	if (source != 0)
	{
		if (work->ready != 0)
		{
			work->ready = 0;
			ColorBuffer_BackupAndScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
		}
	}
}

s32 BattleFx_InitializeStarField(s32 mode)
{
	struct SparkWork *work;
	struct Spark	 *spark;
	struct Ring		 *ring;
	s32				  i;
	u8				 *data;
	s32				  zero;
	/* FAKEMATCH: needed to match: an unused Spark, the 28 bytes the native frame reserves. */
	struct Spark unused;

	((s32 *)gTransitionWork)[2] = 1;
	work = Runtime_AllocateBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof *work);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);
	/* FAKEMATCH: the empty loop ends a scheduling region, keeping the
	 * multiply routine's address load ahead of the spark cursor. Measured:
	 * without it the two swap (differ 2); the sparks indexed in place
	 * still differ 2, the cursor walked as a pointer 4, and the two heap
	 * allocations in the other order 12. */
	do
	{
	} while (0);
	for (i = 0; i < 16; i++)
	{
		s32 angle = Random16();
		u32 radius = Random16() + 0x10000;
		u32 half = radius >> 1;

		spark = &work->sparks[i];
		spark->pos[0] = Iwram_MulQ16(Trig_Cos(angle), half);
		spark->pos[1] = Iwram_MulQ16(Trig_Sin(angle), half);
		if (spark->pos[0] & 1)
			spark->pos[0] = -spark->pos[0];
		if (spark->pos[1] & 1)
			spark->pos[1] = -spark->pos[1];
		spark->pos[2] = (Random16() + 0x8000) >> 2;
		spark->pos[3] = (-spark->pos[0] >> 7) + (spark->pos[1] >> 8);
		spark->pos[4] = (-spark->pos[1] >> 7) + (-spark->pos[0] >> 8);
		spark->pos[5] = 0;
		spark->life = (radius >> 13) + 1;
	}
	for (i = 0; i < 3; i++)
	{
		s32 radius = 0x1000;
		s32 angle;

		ring = &work->rings[i];
		angle = i * 0x5555;
		ring->x = Iwram_MulQ16(Trig_Cos(angle), radius);
		ring->y = Iwram_MulQ16(Trig_Sin(angle), radius);
		ring->velX = Iwram_MulQ16(Trig_Cos(angle), 0x200);
		ring->velY = Iwram_MulQ16(Trig_Sin(angle), 0x200);
		ring->age = 0;
	}
	work->frames = 0;
	work->ready = 0;
	work->originPending = 0;
	Iwram_ClearWords(gWorkSlot[HEAP_SLOT_BATTLE_CANVAS], 0x4000);
	data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
	Iwram_CopyWords(BG_PLTT, data, 128);
	data += 128;
	Resource_DecodeType01(data, work);
	switch (mode)
	{
	case 0:
		data = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteB);
		break;
	case 1:
		data = Resource_GetTableEntry((s32)&ResourceId_BlueRingSheet);
		break;
	case 2:
		data = Resource_GetTableEntry((s32)&ResourceId_RedPaletteC);
		break;
	default:
		data = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteD);
		break;
	}
	Dma_Set(data, BG_PLTT, DMA_ENABLE32 | DMA_32BIT | 32, REG_DMA3);
	/* FAKEMATCH: the one-pass block keeps the zero ahead of the BG2X
	 * address, which reload derives from the DMA register's. Measured:
	 * plain stores of 0, or of zero outside a block, load it after the
	 * address (differ 2); REG_BG2X = REG_BG2Y = 0 differs 13. */
	do
	{
		zero = 0;
		REG_BG2X = zero;
		REG_BG2Y = zero;
	} while (0);
	Io_Write16(0x100, &REG_BG2PA);
	Io_Write16(0, &REG_BG2PB);
	Io_Write16(0, &REG_BG2PC);
	Io_Write16(0x100, &REG_BG2PD);
	BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
	BattleEffect_LoadWorkFar(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
	Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_UpdateStarField, 0xc80);
	return Scheduler_AddOrUpdateCallback((SchedulerCallback)Graphics_UploadVramBlock, 0xc80);
}

s32 Graphics_ResetVramBlockAndReleaseHeapBlocks(s32 unused)
{
	Iwram_ClearWords((void *)0x06004000, 0x4000);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	REG_DISPCNT = 0x1341;
	Scheduler_RemoveCallback((SchedulerCallback)Graphics_UploadVramBlock);
	return Scheduler_RemoveCallback((SchedulerCallback)BattleFx_UpdateStarField);
}

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count)
{
	s32 remaining;
	u32 mask_red;
	u32 mask_green;
	u32 mask_blue;
	u32 pixel;
	u32 red;
	u32 green;
	u32 blue;

	if (scale > 0x10000)
		scale = 0x10000;
	if (count > 0)
	{
		mask_red = 0x1f;
		mask_green = 0x3e0;
		mask_blue = 0x7c00;
		remaining = count;
		do
		{
			pixel = *source;
			red = pixel & mask_red;
			green = pixel & mask_green;
			blue = mask_blue & pixel;
			red *= scale;
			green *= scale;
			blue *= scale;
			pixel = ((red >> 16) & mask_red) | ((green >> 16) & mask_green);
			pixel |= (blue >> 16) & mask_blue;
			*destination = pixel;
			source++;
			destination++;
			remaining--;
		} while (remaining != 0);
	}
	return 0;
}

void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant)
{
	/* FAKEMATCH: needed to match: an unread ten-word launch tail, the native frame's 40 extra bytes. */
	s32							  spare[10];
	struct EffectPosition		  position;
	struct EffectPosition		  position2;
	struct BattlePresentationWork launch;
	struct BattlePresentationWork launch2;
	/* FAKEMATCH: one variable, assigned twice, holds slot 12's address
	 * and later the IME address, as the bytes do; the session is slot 9,
	 * three words below. Measured: the session read as gWorkSlot[9] with
	 * ime set only in the loop differs 124 (247 instructions for 249);
	 * with IO_WRITE_QUEUE_PUSH in the loop as well, 140. */
	volatile u16		  *ime = (volatile u16 *)&gWorkSlot[12];
	struct BattleWork		  *session = *(struct BattleWork **)((u8 *)ime - 12);
	struct SparkWork	  *work;
	s32					   frame;
	s32					  *brightness;
	struct EffectPosition *pos;

	WaitFrames(1);
	BattlePresentation_ConfigurePaletteFade(1, session->background, 0);
	Iwram_ClearWords((void *)0x06004000, 0x4000);
	QueueIoWriteDelay2(0x04000000, 0x3741);
	QueueIoWriteDelay2(0x0400000c, 0x784);
	QueueIoWriteDelay2(0x04000050, 0x3f44);
	WaitFrames(1);
	Io_Write16(240, &REG_WIN0H);
	Io_Write16(0x1088, &REG_WIN0V);
	Io_Write16(63, &REG_WININ);
	Io_Write16(17, &REG_WINOUT);
	if (mode == 0)
	{
		QueueIoWriteDelay2(0x04000052, 0x100e);
		BattleFx_InitializeStarField(kind);
		for (frame = 0, pos = &position; frame <= 44; frame++)
		{
			brightness = &session->brightness;
			ime = &REG_IME;
			work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
			if (frame <= 24)
				Graphics_ScaleRgb555Clamped(
					session->palette, (u16 *)0x050000c0, *brightness = 0x10000 - frame * 1092, 128);
			BattleMotion_ProjectScaledPosition(unit, (s32 *)pos);
			work->originX = (64 - pos->x) << 8;
			work->originY = (64 - pos->y) << 8;
			IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000028, (u32)&work->originX, 0x84000002);
			work->originPending = 1;
			WaitFrames(1);
		}
		Graphics_ResetVramBlockAndReleaseHeapBlocks(kind);
	}
	else if (mode == 1)
	{
		// ここのワークは effect.c の struct BattleEffectWork だが、星のワークと同じオフセットに書く
		BattlePresentation_PrepareSceneFar(kind);
		for (frame = 0; frame < 40; frame++)
		{
			work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
			BattleMotion_ProjectScaledPosition(unit, (s32 *)&position2);
			work->originX = (64 - position2.x) << 8;
			work->originY = (64 - position2.y) << 8;
			IO_WRITE_QUEUE_PUSH(0x04000028, (u32)&work->originX, 0x84000002);
			work->originPending = 1;
			WaitFrames(1);
		}
		BattleFx_ScheduleCallbacksAndReleaseBlocksFar();
	}
	else if (mode == 2)
	{
		launch.flags = 0;
		launch.kind = kind;
		launch.variant = variant;
		launch.actor = unit;
		launch.actors[0] = unit;
		launch.firstTarget = unit;
		launch.count = 1;
		launch.initialValue = 1;
		BattleFx_RunIceFleckBurstFar((struct BattleEffectArgument *)&launch);
	}
	else
	{
		launch2.kind = kind;
		launch2.flags = 0;
		launch2.variant = 0;
		launch2.actor = unit;
		launch2.actors[0] = unit;
		launch2.firstTarget = unit;
		launch2.count = 1;
		launch2.initialValue = 1;
		BattleFx_RunGatheringBurstFar(&launch2);
	}
}

void BattlePres_RunWithZeroArguments(void)
{
	BattlePresentation_SetPaletteLevel(0, 0);
}

void Runtime_RemoveIrqHandlerSlot2(void)
{
	Runtime_SetIrqHandler(2, 0, 0);
}

s32 Party_ComputeEligibleMemberAverage(s32 recordId)
{
	struct BattleFormationRecord *record;
	struct EnemyDefinition		 *enemy;
	u32							  i;
	s32							  id;
	s32							  sum;
	s32							  count;
	/* FAKEMATCH: needed to match: an unread seven-word level table, the native frame's 28 bytes */
	s32 levels[7];

	count = 0;
	sum = 0;
	record = &gBattleFormation_Records[recordId];
	for (i = 0; i < 5; i++)
	{
		if (record->minimumCounts[i] != 0)
			break;
	}
	if (i == 5)
		return -1;
	for (i = 0; i < 5; i++)
	{
		if (record->maximumCounts[i] == 0)
			continue;
		id = record->memberIds[i];
		enemy = Owner_GetRecordFar(id + 8);
		if (enemy == 0)
			continue;
		if (enemy->level > 3 && !GameFlag_TestFar(372) && !GameFlag_TestFar(id + 1544))
			return -2;
		sum += enemy->level;
		count++;
	}
	if (count == 0)
		return -3;
	return sum / count;
}

struct FormationCandidate
{
	s16 recordId;
	s16 score;
};

struct BattleBaseStats
{
	s16						  baseHp;
	s16						  basePp;
	s16						  hpGauge;
	s16						  ppGauge;
	u16						  baseAttack;
	u16						  baseDefense;
	u16						  baseAgility;
	u8						  baseLuck;
	u8						  actionCount;
	u8						  hpRegen;
	u8						  ppRegen;
	u8						  unk12[2];
	struct BattleElementStats element[4];
};

s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin)
{
	s32						   matchCount = 0;
	struct FormationCandidate *pool = Runtime_BumpAllocateAlternatePool(128);
	u16						   party[8];
	s32						   levelTotal = 0;
	s32						   unitCount = BattleParty_PrepareActiveOwners(party);
	s32						   i;
	s32						   j;
	s32						   chance;
	s32						   result;

	if (unitCount > 0)
	{
		u16 *p = party;

		i = unitCount;
		do
		{
			u8 level = UnitGetFar(*p)->level;

			i--;
			p++;
			levelTotal += level;
		} while (i != 0);
	}

	chance = levelTotal / unitCount;
	chance += (s8)GameFlag_GetByteFar(1016);
	if (chance <= 0)
		chance = 1;
	if (chance > 99)
		chance = 99;

	for (i = 0; i <= 31; i++)
		pool[i].score = -1;

	for (i = 0; (u32)i <= 19; i++)
	{
		Owner_GetRecordFar(gBattleFormation_LevelMatchedEnemies[i]);
		GameFlag_ClearBitFar(gBattleFormation_LevelMatchedEnemies[i] + 1536);
	}

	for (i = 0; (u32)i <= 379; i++)
	{
		s32 score = Party_ComputeEligibleMemberAverage(i);

		if (score >= 0 && score <= chance + 3)
		{
			s32 minIndex = -1;
			s32 minValue;

			for (j = 0, minValue = 999; j <= 31; j++)
			{
				if (pool[j].score < minValue)
				{
					minValue = pool[j].score;
					minIndex = j;
				}
			}

			if (minIndex >= 0)
			{
				pool[minIndex].score = score;
				pool[minIndex].recordId = i;
				matchCount++;
			}
		}
	}

	if (matchCount > 32)
		matchCount = 32;

	if (matchCount != 0)
	{
		struct FormationCandidate *chosen = &pool[(matchCount * Random16()) >> 16];

		result = chosen->recordId;
		*outMargin = chance - chosen->score;
	}
	else
	{
		*outMargin = matchCount;
		result = 1;
	}

	MemFree(pool);
	return result;
}

s32 Owner_ApplyLevelGains(s32 owner, s32 levels)
{
	struct BattleBaseStats *base;
	struct BattleUnit		   *state;
	s32						value;
	s32						floor;
	s32						i;

	base = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleBaseStats));
	state = UnitGetFar(owner);
	Iwram_CopyWords(base, &state->baseHp, sizeof(struct BattleBaseStats));

	value = state->baseHp;
	value += levels * 97 / 10;
	floor = base->baseHp * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 9999)
		value = 9999;
	state->baseHp = value;

	value = state->basePp;
	value += levels * 15 / 10;
	floor = base->basePp * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 9999)
		value = 9999;
	state->basePp = value;

	floor = levels * 123 / 10;
	value = state->baseAttack;
	value += floor;
	floor = base->baseAttack * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 999)
		value = 999;
	state->baseAttack = value;

	floor = levels * 33 / 10;
	value = state->baseDefense;
	value += floor;
	floor = base->baseDefense * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 999)
		value = 999;
	state->baseDefense = value;

	floor = levels * 51 / 10;
	value = state->baseAgility;
	value += floor;
	floor = base->baseAgility * 7 / 10;
	if (value < floor)
		value = floor;
	if (value > 999)
		value = 999;
	state->baseAgility = value;

	for (i = 0; i < 4; i++)
	{
		value = state->baseElement[i].power + levels * 15;
		floor = base->element[i].power * 7 / 10;
		if (value < floor)
			value = floor;
		if (value > 200)
			value = 200;
		state->baseElement[i].power = value;
	}

	state->level += levels;
	UnitCalcFar(owner);
	MemFree(base);
}

#if EDITION_INTERNATIONAL
#define CH_CNT 9
#else
#define CH_CNT 26
#endif

s32 EnemyCountDecrement(s32 no, s32 n)
{
	struct BattleWork *w;
	s32				num;
	s32				i;
	s32				retry;
	s32				ch;

	w = gBattleWork;
	num = w->summonCount;
	for (i = 0; i < num; i++)
	{
		if (w->enemyClasses[i] == no)
			break;
	}
	if (i != num)
	{
		retry = 0;
		if (w->summonChannels[i] < 0)
		{
			w->summonChannels[i] = 1;
			w->summonMasks[i] = 3;
			return 0x8001;
		}
		for (; retry <= 31; retry++)
		{
			ch = (w->summonChannels[i] + 1) % CH_CNT;
			w->summonChannels[i] = ch;
			if ((w->summonMasks[i] & (1 << (s8)ch)) == 0)
				break;
		}
		w->summonMasks[i] |= 1 << w->summonChannels[i];
		return w->summonChannels[i];
	}
	if (num <= 4)
	{
		w->summonChannels[num] = -1;
		w->enemyClasses[num] = no;
		w->summonMasks[num] = 0;
		w->summonCount = num + 1;
		return CH_CNT;
	}
	return -1;
}

s32 EnemyCountRelease(s32 actorId)
{
	struct BattleWork *state = gBattleWork;
	struct BattleUnit *actor;
	s32				count = state->summonCount;
	s32				index;
	s32				nameLength;
	s32				bit;
	s32				classId;

	actor = UnitGetFar(actorId);
	if (actor->classIndex != 0)
		return;

	classId = actor->classId;
	for (index = 0; index < count; index++)
	{
		if (state->enemyClasses[index] == classId)
			break;
	}
	if (index == count || state->summonMasks[index] == 0)
		return;

	for (nameLength = 0; nameLength <= 13; nameLength++)
	{
		if (actor->name[nameLength] == 0)
			break;
	}

	bit = 32;
	if (nameLength > 0)
		bit = actor->name[nameLength - 1] - LIST_MARKER_CHAR;
	state->summonMasks[index] &= ~(1 << bit);
}

s32 EnemyCountClear(s32 classId)
{
	struct BattleUnit *summon;
	s32				objectIndex;
	s32				slot;
	s32				nextSlot;
	u8				occupied;
	u8				marker;

	objectIndex = 0;
	marker = LIST_MARKER_CHAR;
	for (; objectIndex <= 5; objectIndex++)
	{
		summon = UnitGetFar(objectIndex + 0x80);
		occupied = summon->side;
		if (occupied != 1)
			continue;
		if (summon->classId != classId)
			continue;
		slot = 0;
		if (summon->name[0] == 0)
		{
			summon->name[0] = marker;
			summon->name[occupied] = slot;
			return;
		}
	scan_slots:
		slot++;
		if (slot > 13)
			return;
		occupied = summon->name[slot];
		if (occupied != 0)
			goto scan_slots;
		nextSlot = slot + 1;
		summon->name[slot] = marker;
		summon->name[nextSlot] = occupied;
		return;
	}
}

s32 BattleGroupRandom(s32 recordId)
{
	s32							  choices[5];
	struct BattleFormationRecord *record;
	s32							 *choicesBase;
	s32							 *choiceCursor;
	u8							 *maximumCount;
	u8							 *memberId;
	u32							  count;
	s32							  remaining;

	count = 0;
	if (recordId < 0 || recordId >= 380)
		recordId = 1;

	choicesBase = choices;
	record = &gBattleFormation_Records[recordId];
	memberId = record->memberIds;
	maximumCount = record->maximumCounts;
	choiceCursor = choicesBase;
	remaining = 4;
	do
	{
		if (*maximumCount++ != 0)
		{
			*choiceCursor++ = *memberId + 8;
			count++;
		}
		memberId++;
		remaining--;
	} while (remaining >= 0);

	return choices[(Random16() * count) >> 16];
}

s32 BattleFormation_BuildEnemyList(s32 recordId)
{
	u16							 *list;
	struct BattleWork				 *work;
	struct BattleFormationRecord *record;
	s32							  count;
	s32							  margin;
	u16							  listBuffer[14];
	s32							  extra[5];
	s32							  counts[5];
	s32							  order[5];
	s32							  budget;
	s32							  changed;
	s32							  room;
	s32							  size;
	s32							  fit;
	s32							  id;
	s32							  k;
	s32							  n;
	s32							  total;
	u32							  i;
	s32							  j;

	work = gBattleWork;
	margin = 0;
	work->summonCount = 0;
	list = listBuffer;
	if (GameFlag_TestFar(0x173))
		recordId = BattleFormation_SelectLevelMatchedCandidate(&margin);
	if (recordId < 0 || recordId >= 380)
		recordId = 1;
	record = &gBattleFormation_Records[recordId];
	for (j = 0; j <= 4 && record->minimumCounts[j] == 0; j++)
		;
	if (j == 5)
		record = &gBattleFormation_Records[1];

	budget = 6;
	count = 0;
	for (i = 0; i <= 4; i++)
	{
		if (record->minimumCounts[i] != 0)
		{
			s32 size1 = 2 - (Summon_IsEntryFlagged(record->memberIds[i] + 8) != 0);

			budget -= size1 * record->minimumCounts[i];
		}
	}

	for (i = 0; i <= 4; i++)
	{
		s32 minimum;
		s32 maximum;

		counts[i] = minimum = record->minimumCounts[i];
		maximum = record->maximumCounts[i];
		room = maximum - minimum;
		if (room > 0)
		{
			s32 size2 = 2 - (Summon_IsEntryFlagged(record->memberIds[i] + 8) != 0);

			fit = budget / size2;
			if (fit < room)
				room = fit;
			extra[i] = ((room + 1) * Random16()) >> 16;
		}
		else
		{
			extra[i] = 0;
		}
	}

	do
	{
		changed = 0;
		for (i = 0; i <= 4; i++)
		{
			id = record->memberIds[i] + 8;
			if (extra[i] != 0)
			{
				size = 2 - (Summon_IsEntryFlagged(id) != 0);
				if (size > budget)
				{
					extra[i] = 0;
				}
				else
				{
					counts[i]++;
					extra[i]--;
					budget -= size;
					changed = 1;
				}
			}
		}
	} while (changed);

	work->battleType = record->battleType;
	switch (work->battleType)
	{
	case 0:
		for (i = 0; i <= 4; i++)
			order[i] = i;
		for (i = 0; i <= 9; i++)
		{
			s32 first = (Random16() * 5) >> 16;
			s32 second = (Random16() * 5) >> 16;
			s32 swap = order[first];

			order[first] = order[second];
			order[second] = swap;
		}
		for (i = 0; i <= 4; i++)
		{
			s32 m;
			s32 slot = order[i];

			for (m = 0; m < counts[slot]; m++)
				list[count++] = record->memberIds[slot] + 8;
		}
		break;
	case 1:
		for (;;)
		{
			n = 0;
			for (i = 0; i <= 4; i++)
			{
				if (counts[i] != 0)
					order[n++] = i;
			}
			if (n == 0)
				break;
			k = order[(Random16() * n) >> 16];
			list[count++] = record->memberIds[k] + 8;
			counts[k]--;
		}
		break;
	default:
		for (i = 0; i <= 4; i++)
		{
			total = counts[i];
			for (k = 0; k < total; k++)
				list[count++] = record->memberIds[i] + 8;
		}
		break;
	}
	list[count] = 0;
	work->firstDefeated = 6;
	work->defeatState = 0;
	for (i = 128; i <= 133; i++)
		Iwram_ClearWords(UnitGetFar(i), BATTLE_UNIT_SIZE);

	i = 0;
	if (list[0] == 0)
		goto assigned;
assign_enemy:
	{
		s32 charge = EnemyCountDecrement(list[i], 1);

		if (charge & 0x8000)
			EnemyCountClear(list[i]);
		BattleUnitSetFar(i + 128, list[i], charge & 0x7fff);
		UnitGetFar(i + 128);
		if (margin != 0)
			Owner_ApplyLevelGains(i + 128, margin);
	}
	i++;
	if (i > 5)
		goto assigned;
	if (list[i] != 0)
		goto assign_enemy;
assigned:
	return i;
}

u32 Summon_GetLayoutKind(s32 index)
{
	u32 kind = gSummon_EntryTable[index].layoutKind;

	if ((s32)kind > 4)
		kind = -1U;
	return kind;
}

s32 Summon_GetEntryValue(s32 index)
{
	if (index < 0 || index > 171)
		return gSummon_EntryTable[0].animation;
	return gSummon_EntryTable[index].animation;
}

s32 Summon_GetEntryFlag1Field(s32 index)
{
	if (index < 0 || index > 171)
		return gSummon_EntryTable[0].animation;
	return gSummon_EntryTable[index].layoutBits1;
}

s32 Summon_IsEntryFlagged(s32 index)
{
	s32 result;

	if (index < 0 || index > 171)
		return 0;
	result = 0;
	if (gSummon_EntryTable[index].spriteBit0)
		result = 1;
	return result;
}

u32 Battle_GetSpriteFlagNibble(u32 index)
{
	u16 value;

	if (index > 171)
		return 1;
	value = gSummon_EntryTable[index].spriteBits1;
	return value ? value : 1;
}

u32 Battle_GetSpriteFlagHighBits(u32 index)
{
	u16 value;

	if (index > 171)
		return 0;
	value = gSummon_EntryTable[index].spriteBits5;
	return value ? value : 0;
}

s32 Summon_IsEntrySecondaryFlagged(s32 index)
{
	if (index < 0 || index > 171)
		return 0;
	return gSummon_EntryTable[index].layoutBit0;
}

s32 Summon_GetEntryByte4(s32 index)
{
	if (index < 0 || index > 171)
		return 0;
	return gSummon_EntryTable[index].unk04[0];
}

u32 Item_EncodeBankedId(u32 value)
{
	u32 bank = 0;
	u32 base = value & 0x1ff;
	if (base == 0)
		return 0;
	{
		u8 flags = Item_GetDirectFar(base)->flags;

		if (flags & 8)
			bank = 1;
		bank <<= 1;
		if (flags & 4)
			bank++;
		bank <<= 9;
		bank += base;
	}
	return bank;
}

void Summon_ClearWorkFields(void)
{
	struct BattleWork		*work = gBattleWork;
	struct BattleSpoils *spoils = &work->spoils;
	u16					*items;
	s32					 index;

	gGameState.pendingItem = 0;
	spoils->coins = 0;
	spoils->experience = 0;
	spoils->defeated = 0;
	items = work->spoils.items;
	for (index = 3; index >= 0; index--)
		items[index] = 0;
}

static __inline__ s32 BattleEnemy_CalculateCoinReward(struct BattleUnit *unit, u16 *reward)
{
	s32 i;
	s32 bonus = 0;
	s32 base;
	s32 floor;

	for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
		bonus += ((Random16() * 6) >> 16) + 1;
	base = *reward;
	floor = base * 3 / 10;
	if (bonus < floor)
		bonus = floor;
	return bonus + base;
}

static __inline__ s32 BattleEnemy_CalculateExperienceReward(struct BattleUnit *unit, u16 *reward)
{
	s32 i;
	s32 bonus = 0;
	s32 base;
	s32 floor;

	for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
		bonus += ((Random16() * 4) >> 16) + 1;
	base = *reward;
	floor = base * 3 / 10;
	if (bonus < floor)
		bonus = floor;
	return bonus + base;
}

s32 BattleEnemy_RecordDefeat(s32 unitId, s32 earned)
{
	struct BattleUnit		   *unit;
	struct EnemyDefinition *rec;
	struct BattleWork		   *formation;
	struct BattleSpoils	   *spoils;
	s32						i;
	s32						chance;
	s32						slot;
	s32						lowest;
	s32						lowestSlot;
	s32						value;

	unit = UnitGetFar(unitId);
	formation = gBattleWork;
	spoils = &formation->spoils;
	if (unitId >= 0 && unitId < 8)
		return -1;
	if (unit->classIndex != 0)
		return -2;
	slot = 0;
	i = 0;
	if (formation->enemyClasses[0] != unit->classId)
	{
		do
		{
			if (++i > 5)
				break;
		} while (formation->enemyClasses[i] != unit->classId);
	}
	if (i != 6)
		slot = i;
	if (formation->defeatState != 2)
	{
		if (slot < formation->firstDefeated)
			formation->firstDefeated = slot;
		if (spoils->defeated)
			formation->defeatState = 1;
	}
	spoils->defeated++;
	if (GameFlag_TestFar(0x173))
		return 0;
	GameFlag_SetBitFar(unit->classId + 0x600);
	rec = Owner_GetRecordFar(unit->classId);
	if (earned != 0)
	{
		if (rec->coins != 0)
			spoils->coins += BattleEnemy_CalculateCoinReward(unit, &rec->coins);
		if (rec->experience != 0)
			spoils->experience += BattleEnemy_CalculateExperienceReward(unit, &rec->experience);
	}
	else
	{
		spoils->coins += rec->coins;
		spoils->experience += rec->experience;
	}
	if (rec->item == 0 || rec->itemChance == 0)
		return 0;
	i = 0;
	if (spoils->items[0] != rec->item)
	{
		do
		{
			if (++i > 3)
				break;
		} while (spoils->items[i] != rec->item);
	}
	if (i != 4)
		return 0;
	chance = rec->itemChance;
	if (earned != 0)
		chance -= 2;
	if (chance < 0)
		chance = 0;
	if ((0x20000 >> chance) > (BattleRandomFar() & 0xffff))
	{
		lowestSlot = -1;
		lowest = 0x40000000;
		for (i = 0; i <= 3; i++)
		{
			value = Item_EncodeBankedId(spoils->items[i]);
			if (value < lowest)
			{
				lowest = value;
				lowestSlot = i;
			}
		}
		if ((s32)Item_EncodeBankedId(rec->item) > lowest)
			spoils->items[lowestSlot] = rec->item;
	}
	return 0;
}

void Battle_AwardSpoils(void)
{
	struct BattleSpoils *spoils;
	s32					 count;
	u16					*list;
	s32					 i;
	s32					 unitId;
	struct BattleUnit		*unit;
	struct BattleUnit		*backup;
	s32					 cnt;
	u32					 learned;
	s32					 j;
	s32					 best;
	s32					 bestSlot;
	s32					 slot;
	u16					*found;
	s32					 value;
	struct LevelUpResult gains; // 上がった分は符号つきで読む
	u16					 units[8];

	spoils = &gBattleWork->spoils;
	if (spoils->experience != 0)
	{
		UiWork_PushValueSlotFar(spoils->experience, 5);
		UiText_ShowMessageAndWaitCoreFar(MsgExpGained);
		BattlePresentation_WaitForAdvance();
	}
	list = units;
	count = BattleParty_ListLivingUnits(1, list);
	backup = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleUnit));
	for (i = 0; i < count; i++)
	{
		unitId = list[i];
		unit = UnitGetFar(unitId);
		unit->experience += spoils->experience;
		while (Iwram_CopyWords(backup, unit, sizeof(struct BattleUnit)),
			Owner_GetValueIfLevelThresholdReachedFar(unitId, &gains) != 0)
		{
			AudioCommand_PlayFar(0x59);
			UiWork_ClearValueNameTablesFar();
			UiWork_PushValueSlotFar(unit->classIndex, 3);
			UiWork_PushValueSlotFar(list[i], 1);
			UiWork_PushValueSlotFar(unit->level, 5);
			UiText_ShowMessageAndWaitCoreFar(MsgLevelUp);
			BattlePresentation_WaitForAdvance();
			for (cnt = 0; cnt < 32; cnt++)
			{
				learned = unit->actionSlots[cnt].encodedAction;
				if ((learned & OWNER_ACTION_ID_MASK) && (learned >> 15))
				{
					for (j = 0; j < 32; j++)
					{
						if (learned == backup->actionSlots[j].encodedAction)
							break;
					}
					if (j == 32)
					{
						UiWork_ClearValueNameTablesFar();
						UiWork_PushValueSlotFar(unit->classIndex, 3);
						UiWork_PushValueSlotFar(unitId, 1);
						UiWork_PushValueSlotFar(learned & OWNER_ACTION_ID_MASK, 4);
						AudioCommand_PlayFar(0x9a);
						UiText_ShowMessageAndWaitCoreFar(MsgAbilityMastered);
						BattlePresentation_WaitForAdvance();
					}
				}
			}
			if ((s16)gains.hp != 0)
			{
				UiWork_PushValueSlotFar((s16)gains.hp, 5);
				UiText_ShowMessageAndWaitCoreFar(MsgMaxHpRises);
				BattlePresentation_WaitForAdvance();
			}
			if ((s16)gains.pp != 0)
			{
				UiWork_PushValueSlotFar((s16)gains.pp, 5);
				UiText_ShowMessageAndWaitCoreFar(MsgMaxPpRises);
				BattlePresentation_WaitForAdvance();
			}
			if ((s16)gains.atk != 0)
			{
				UiWork_PushValueSlotFar((s16)gains.atk, 5);
				UiText_ShowMessageAndWaitCoreFar(MsgAttackRises);
				BattlePresentation_WaitForAdvance();
			}
			if ((s16)gains.def != 0)
			{
				UiWork_PushValueSlotFar((s16)gains.def, 5);
				UiText_ShowMessageAndWaitCoreFar(MsgDefenseRises);
				BattlePresentation_WaitForAdvance();
			}
			if ((s16)gains.agi != 0)
			{
				UiWork_PushValueSlotFar((s16)gains.agi, 5);
				UiText_ShowMessageAndWaitCoreFar(MsgAgilityRises);
				BattlePresentation_WaitForAdvance();
			}
			if ((s16)gains.luck != 0)
			{
				UiWork_PushValueSlotFar((s16)gains.luck, 5);
				UiText_ShowMessageAndWaitCoreFar(MsgLuckRises);
				BattlePresentation_WaitForAdvance();
			}
		}
	}
	MemFree(backup);
	if (spoils->coins != 0)
	{
		UiWork_PushValueSlotFar(spoils->coins, 5);
		UiText_ShowMessageAndWaitCoreFar(MsgCoinsGained);
		Party_AdjustCoinsFar(spoils->coins);
		BattlePresentation_WaitForAdvance();
	}
	found = &gOverflowItem;
	for (;;)
	{
		bestSlot = -1;
		best = -1;
		for (slot = 0; slot <= 3; slot++)
		{
			if (spoils->items[slot] != 0)
			{
				value = Item_EncodeBankedId(spoils->items[slot]);
				if (value >= best)
				{
					best = value;
					bestSlot = slot;
				}
			}
		}
		if (bestSlot == -1)
			break;
		UiWork_PushValueSlotFar(spoils->items[bestSlot], 2);
		UiText_ShowMessageAndWaitCoreFar(MsgItemGained);
		BattlePresentation_WaitForAdvance();
		if (PartyInventory_AddFar(spoils->items[bestSlot]) == -1)
		{
			*found = spoils->items[bestSlot];
			break;
		}
		spoils->items[bestSlot] = 0;
	}
}

void Battle_ReservedNoOp2A08(void)
{
}
