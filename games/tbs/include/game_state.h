#ifndef ALCHEMY_GAME_STATE_H
#define ALCHEMY_GAME_STATE_H

#include "types.h"
#include "layout_guard.h"
#include "battle_unit.h"

struct GameState
{
	u32			   build_stamp;
	u32			   play_time;
	u8			   unknown_008[8];
	s32			   coins;
	u8			   unknown_014[0x104];
	s32			   shop_credit;
	s8			   shop_gifts;
	s8			   prize_weights[0x0f];
	s8			   won_prizes[0x10];
	u8			   unknown_13c[0x84];
	s16			   scene;
	s16			   entrance;
	s16			   saved_scene;
	s16			   saved_entrance;
	s16			   default_scene;
	s16			   default_entrance;
	s16			   scene_group;
	s16			   next_scene;
	s16			   next_entrance;
	s16			   defeat_scene;
	s16			   defeat_entrance;
	s16			   special;
	u8			   unknown_1d8[2];
	s16			   map;
	s32			   x;
	s32			   y;
	s32			   z;
	u32			   heading;
	u16			   turn;
	s16			   scene_cue;
	s16			   cue;
	u8			   movement_mode;
	u8			   unknown_1f3;
	s32			   selected_actor;
	u8			   active_owners[8];
	u8			   unknown_200[5];
	u8			   palette_glow[2];
	u8			   unknown_207[3];
	u8			   speech;
	u8			   unknown_20b;
	u8			   message_speed;
	u8			   unknown_20d[2];
	u8			   unknown_20f;
	u16			   a_keys;
	u16			   b_keys;
	u16			   select_keys;
	u16			   start_keys;
	u16			   first_shortcut_keys;
	u16			   second_shortcut_keys;
	u16			   dash_keys;
	s16			   return_cue;
	u16			   first_shortcut;
	u16			   second_shortcut;
	u8			   perspective_scene;
	u8			   unknown_225[5];
	u8			   saved_options;
	u8			   battle_start;
	s16			   hazard_limit;
	s16			   hazard_mode;
	s16			   hazard_damage;
	s16			   hazard_steps;
	s16			   pending_djinn_event;
	s16			   scene_change_reason;
	s32			   encounter_steps;
	u16			   pending_item;
	s16			   scene_variant;
	s16			   retreat_scene;
	s16			   retreat_entrance;
	s32			   avoid_distance;
	s8			   avoid_by_item;
	s8			   paused_pose;
	s16			   paused_object;
	s16			   cloaked;
	u8			   unknown_24e[2];
	u32			   paused_update;
	u8			   unknown_254[0x50];
	u16			   link_tallies[8];
	u8			   unknown_2b4[4];
	u32			   start_stamp;
	u8			   unknown_2bc[4];
	struct BattleUnit owners[8];
};

LAYOUT_OFFSET_GUARD(GameState_BuildStamp, struct GameState, build_stamp, 0);
LAYOUT_OFFSET_GUARD(GameState_PlayTime, struct GameState, play_time, 4);
LAYOUT_OFFSET_GUARD(GameState_SavedOptions, struct GameState, saved_options, 0x22a);
LAYOUT_OFFSET_GUARD(GameState_Coins, struct GameState, coins, 0x10);
LAYOUT_OFFSET_GUARD(GameState_ShopCredit, struct GameState, shop_credit, 0x118);
LAYOUT_OFFSET_GUARD(GameState_ShopGifts, struct GameState, shop_gifts, 0x11c);
LAYOUT_OFFSET_GUARD(GameState_Owners, struct GameState, owners, 0x2c0);
LAYOUT_SIZE_GUARD(GameState_Size, struct GameState, 0xd20);
LAYOUT_OFFSET_GUARD(GameState_WonPrizes, struct GameState, won_prizes, 0x12c);
LAYOUT_OFFSET_GUARD(GameState_SceneGroup, struct GameState, scene_group, 0x1cc);
LAYOUT_OFFSET_GUARD(GameState_DashKeys, struct GameState, dash_keys, 0x21c);
LAYOUT_OFFSET_GUARD(GameState_AvoidDistance, struct GameState, avoid_distance, 0x244);
LAYOUT_OFFSET_GUARD(GameState_Entrance, struct GameState, entrance, 0x1c2);
LAYOUT_OFFSET_GUARD(GameState_Special, struct GameState, special, 0x1d6);
LAYOUT_OFFSET_GUARD(GameState_X, struct GameState, x, 0x1dc);
LAYOUT_OFFSET_GUARD(GameState_MovementMode, struct GameState, movement_mode, 0x1f2);
LAYOUT_OFFSET_GUARD(GameState_SelectedActor, struct GameState, selected_actor, 0x1f4);
LAYOUT_OFFSET_GUARD(GameState_ActiveOwners, struct GameState, active_owners, 0x1f8);
LAYOUT_OFFSET_GUARD(GameState_Speech, struct GameState, speech, 0x20a);
LAYOUT_OFFSET_GUARD(GameState_MessageSpeed, struct GameState, message_speed, 0x20c);
LAYOUT_OFFSET_GUARD(GameState_AKeys, struct GameState, a_keys, 0x210);
LAYOUT_OFFSET_GUARD(GameState_FirstShortcutKeys, struct GameState, first_shortcut_keys, 0x218);
LAYOUT_OFFSET_GUARD(GameState_SecondShortcutKeys, struct GameState, second_shortcut_keys, 0x21a);
LAYOUT_OFFSET_GUARD(GameState_FirstShortcut, struct GameState, first_shortcut, 0x220);
LAYOUT_OFFSET_GUARD(GameState_RetreatEntrance, struct GameState, retreat_entrance, 0x242);
LAYOUT_OFFSET_GUARD(GameState_Cloaked, struct GameState, cloaked, 0x24c);
LAYOUT_OFFSET_GUARD(GameState_LinkTallies, struct GameState, link_tallies, 0x2a4);
LAYOUT_OFFSET_GUARD(GameState_StartStamp, struct GameState, start_stamp, 0x2b8);

#ifndef OWN_gGameState
extern struct GameState gGameState;
#endif

// 戦闘開始の種類を設定
/* A statement macro (S2: a layer, 34 sites): written as one, its store stays in place at every use; as a plain assignment, tbs-en differs. */
#define Battle_SetStart(mode)             \
	do                                    \
	{                                     \
		gGameState.battle_start = (mode); \
	} while (0)

#endif
