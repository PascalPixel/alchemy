#ifndef ALCHEMY_BATTLE_WORK_H
#define ALCHEMY_BATTLE_WORK_H

#include "unit/battle/battle.h"
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
#include "types.h"
#include "layout_guard.h"
#include "btltype.h"
#include "btlev.h"
#include "battle_session.h"

struct BattlePromptSprite
{
	struct BattlePromptSprite *next;
	u16						   y : 8;
	u16						   flags : 8;
	u16						   x : 9;
	u16						   size : 7;
	u16						   tile : 10;
	u16						   palette : 6;
	u16						   unknown_0a;
};

struct UiWindow;

struct BattleDisplayOffset
{
	s32 unknown_00;
	u16 x;
	u16 y;
};

struct BattleDisplayWork
{
	struct UiWindow			   *window;
	struct BattleDisplayOffset *offset;
	s32							marked;
};

LAYOUT_SIZE_GUARD(BattleDisplayOffset_Size, struct BattleDisplayOffset, 8);
LAYOUT_SIZE_GUARD(BattleDisplayWork_Size, struct BattleDisplayWork, 12);
LAYOUT_OFFSET_GUARD(BattleDisplayWork_Marked, struct BattleDisplayWork, marked, 8);
extern struct BattleDisplayWork *gBattleDisplayWork;

struct BattleEventState
{
	struct BattleEventQueue	  queue;
	s32						  phase;
	s32						  event_index;
	s32						  timer;
	struct BattlePromptSprite display_entry;
	s32						  display_source;
	s32						  actor_id;
	s32						  pending_cue;
	s32						  flags;
};

struct BtlWork
{
	s32						  enm_grp;
	s16						  sprite_slots[6];
	u16						  enemy_classes[6];
	u32						  summon_masks[6];
	s8						  summon_channels[6];
	u8						  unknown_03a[2];
	u16						  first_defeated;
	u16						  defeat_state;
	u8						  summon_count;
	u8						  pty_st;
	u8						  battle_type;
	u8						  chouhatsu_flg;
	u8						  two_sided;
	u8						  encounter_mode;
	u8						  escape_failures;
	u8						  unknown_047;
	u8						  owner_slots[8];
	u8						  link_side;
	u8						  link_misses;
	u8						  link_paused;
	u8						  unknown_053;
	s32						  prompt_slot;
	s16						  party_units[7];
	s16						  enm_no[7];
	struct BtlObj			  slots[14];
	u8						  placement[14];
	u8						  unknown_2ea[2];
	struct BattleActionRecord actions[20];
	u8						  unknown_42c[0x104];
	struct BattleSpoils		  spoils;
	u16						  palette[128];
	s32						  brightness;
	u16						  background;
	u8						  unknown_64a[4];
	u16						  highlight_step;
	u16						  highlight_mode;
	u8						  unknown_652[2];
	struct BtlPlan			  plan;
	struct BattleEventState	  events;
	s32						  debug_value;
};

LAYOUT_SIZE_GUARD(BattleSession_Size, struct BtlWork, 0x82c);

LAYOUT_OFFSET_GUARD(BattleSession_PartyStatusMode, struct BtlWork, pty_st, 0x041);
LAYOUT_OFFSET_GUARD(BtlWork_ChouhatsuFlg, struct BtlWork, chouhatsu_flg, 0x043);
LAYOUT_OFFSET_GUARD(BattleSession_PartyUnits, struct BtlWork, party_units, 0x058);
LAYOUT_OFFSET_GUARD(BattleSession_EnemyUnits, struct BtlWork, enm_no, 0x066);
LAYOUT_OFFSET_GUARD(BattleSession_Slots, struct BtlWork, slots, 0x074);
LAYOUT_OFFSET_GUARD(BattleSession_Placement, struct BtlWork, placement, 0x2dc);
LAYOUT_OFFSET_GUARD(BattleSession_Actions, struct BtlWork, actions, 0x2ec);
LAYOUT_OFFSET_GUARD(BattleSession_Spoils, struct BtlWork, spoils, 0x530);
LAYOUT_OFFSET_GUARD(BattleSession_Palette, struct BtlWork, palette, 0x544);
LAYOUT_OFFSET_GUARD(BattleSession_Brightness, struct BtlWork, brightness, 0x644);
LAYOUT_OFFSET_GUARD(BattleSession_Background, struct BtlWork, background, 0x648);
LAYOUT_OFFSET_GUARD(BattleSession_Plan, struct BtlWork, plan, 0x654);
LAYOUT_OFFSET_GUARD(BattleSession_Events, struct BtlWork, events, 0x6b8);
LAYOUT_OFFSET_GUARD(BattleEventState_Phase, struct BattleEventState, phase, 0x148);
LAYOUT_OFFSET_GUARD(BattleEventState_Flags, struct BattleEventState, flags, 0x16c);

extern struct BtlWork *BtlWk;

#endif
