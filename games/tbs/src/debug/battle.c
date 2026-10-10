#include "battle.h"
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

#define Inventory_EquipFar ((void (*)(s32, s32))Inventory_EquipFar)

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

void UiWork_InitializeWithResourceCountersFar(void);

#define gWindowWork (*(u8 **)&gWindowWork)

#if EDITION_INTERNATIONAL
#define WINDOW_WORK_MESSAGE_BUSY 0x12f8
#else
#define WINDOW_WORK_MESSAGE_BUSY 0x1188
#endif

extern volatile s32 gKeysRepeat;

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

void UiWork_PushValueSlotFar(s32, s32);
s32	 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
s32	 UiWork_IsCompleteFar(void);
void UiWork_FinalizeFar(s32, s32);
void AudioCommand_PlayFar(s32);
void WaitFrames(s32);

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
			gWindowWork[WINDOW_WORK_MESSAGE_BUSY] = 0;
		}
	}
}

#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Battle_RunEncounter ((void (*)(s32))Battle_RunEncounter)

extern volatile u32 gKeysHeld;

s32 DebugParty_LoadPreset(s32 preset);

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

#define SceneTransform_ApplyPosition ((void (*)(void *))SceneTransform_ApplyPosition)

extern volatile u16 gLinkStatus;

void BattlePresentation_UpdateCamera(void)
{
	void							   **slot = (void **)&gCameraWork;
	struct BattleCamera					*state = slot[0];
	struct BattlePresentationTransition *transition = slot[32];
	struct BtlWork						*work = slot[-3];
	struct FixedPointPosition			 local;
	s32									*pos;
	s16									 delta;
	u32									 id;

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
