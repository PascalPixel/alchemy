#ifndef ALCHEMY_BATTLE_INTERNAL_H
#define ALCHEMY_BATTLE_INTERNAL_H

// What every part of the battle object shares: its includes, in their original order, and the names
// two or more parts use. battle.c includes it before the parts, and each part includes it too, so
// that a part read on its own still resolves. It places no bytes.

#include "ability_ids.h"
#include "debug/debug.h"
#include "enemy_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/ability_staging.h"
#include "battle/battle_actor.h"
#include "battle/battle_camera.h"
#include "battle/battle_effect_state.h"
#include "battle/battle_random.h"
#include "battle/battle_side.h"
#include "battle/blitter.h"
#include "battle/debug_parameter_set.h"
#include "battle/first_strike.h"
#include "battle/formations.h"
#include "battle/range_falloff.h"
#include "battle/weapon_kind.h"
#include "field/script.h"
#include "field/map_grid_scroll.h"
#include "game/inventory.h"
#include "graphics/graphics.h"
#include "sound/sound_driver_symbols.h"
#include "system/sprite_metadata.h"
#include "window/battle_status.h"
#include "text/msg_ids.h"

// The battle's windows: window 0 shows BG0 to BG2, sprites and blending, window 1 BG0, BG2, sprites
// and blending, and outside them only BG0 and blending.
#define BATTLE_WININ                                                                               \
	(WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ | WININ_WIN0_CLR            \
	 | WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_OBJ | WININ_WIN1_CLR)
#define BATTLE_WINOUT     (WINOUT_WIN01_BG0 | WINOUT_WIN01_CLR | WINOUT_WINOBJ_ALL)

// The battle background's colours: BG palettes 6 to 13, filled from BATTLE_STATE.palette, darkened
// or not.
#define BATTLE_BG_PALETTE ((Uint16 *)&BG_PLTT_COLOR(6, 0))

// The opening curtain's tiles: 4-bit tiles 128 to 136 of character block 1, which BG1 shows from
// its map in screen block 12.
#define CURTAIN_TILE(n)   (BG_CHAR_BLOCK(1) + (128 + (n)) * TILE_SIZE_4BPP)

// BG1 during a battle: 256 colours, priority 3, its map in screen block BATTLE_BG1_SCREEN_BLOCK.
#define BATTLE_BG1CNT                                                                              \
	(BGCNT_PRIORITY(3) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK))

// In a link battle each console sends LINK_PARTY_SIZE places, at most three party members
// (btLinkBattleSendParty), and the opponent's take the first three enemy ids from FIRST_ENEMY_UNIT.
#define LINK_PARTY_SIZE              3

// BATTLE_STATE.objectIndex of a unit with no place on the battlefield: btUnitRefreshPlacement fills
// the list with it, and btObjectGet returns NULL for it.
#define BATTLE_OBJECT_NONE           255

// BATTLE_STATE.defeatState: whether the last enemy fell alone, or with others before it, or a party
// member's action ended the battle with a single defeat. The messages are "has been downed!" and
// "'s party goes down!", MsgDowned plus the state; DEFEAT_BY_PARTY_MEMBER shows neither.
#define DEFEAT_ONE                   0
#define DEFEAT_SEVERAL               1
#define DEFEAT_BY_PARTY_MEMBER       3

// The game flag of an enemy the party has defeated, set by btEnemyRecordDefeat outside the monster
// battles; the monster battles choose only formations of defeated enemies
// (btFormationAverageLevel).
#define ENEMY_DEFEATED_FLAG(classId) ((classId) + 0x600)

// btEnemyClaimNameLetter returns the letter in ENEMY_LETTER_MASK, with ENEMY_LETTER_FIRST_TOO set
// for the second enemy of a class: its callers then give the first one its letter too
// (btEnemyLetterFirstOfKind) before passing the letter to gmBattleUnitSetStub.
#define ENEMY_LETTER_FIRST_TOO       0x8000
#define ENEMY_LETTER_MASK            0x7fff

// The length of a unit's name, its terminator excluded.
#define UNIT_NAME_LENGTH             14

// This console's number in a link session (SIOCNT bits 4 and 5, which the hardware sets in
// multi-player mode): 0 for the parent, 1 for the child.
#define SIO_MULTI_PLAYER_ID()        ((REG_SIOCNT << 26) >> 30)

// LinkStatusWork: both consoles are connected.
#define LINK_BOTH_CONNECTED          3

// The inventory's weapon slot, for gmInventoryFindEquippedStub.
#define EQUIP_WEAPON                 1

// A group's sprite word (ACTOR_SPRITE_GROUP) points to a table of four sprites.
#define ACTOR_SPRITE_GROUP_SIZE      4

// btActionRunBuild's results besides 0, which stages the action.
#define ACTION_REPORTED              -1 // the action failed and its message is shown
#define ACTION_NOTHING_TO_SHOW       -2 // nothing to stage or show

// btStagingRunFullScreen's mode: a summon hides its user too.
#define STAGING_HIDE_ACTOR           1

// btStagingRunPsynergyCast's mode: what is cast, which decides the glow.
typedef enum BATTLE_CAST_KIND
{
	CAST_PSYNERGY = 0, // the star field gathers on the caster
	CAST_DJINNI   = 1, // no glow; the effect plays its prelude
	CAST_ENEMY    = 2  // the element's streaks
} BATTLE_CAST_KIND;

// An HP change that defeats any unit: the Grim Reaper's call deals it, and the debug defeat.
#define DEFEAT_HP_CHANGE       0xc0000000

// btAIChooseTarget's results when it picks no one: the side it may target lists no one, or no one
// there is worth the ability.
#define TARGET_PICK_SIDE_EMPTY -2
#define TARGET_PICK_NONE       -1

// BATTLE_ACTION.target of a held unit's lost action (COMMAND_NONE, from btBuildEnemyActions and
// btBuildPartyActions): as a place code, place 0 of the other side, the party's for an enemy and
// the enemies' for a party member.
#define TARGET_NONE_ENEMY      PLACE_PARTY
#define TARGET_NONE_PARTY      PLACE_ENEMY

// The camera's yaw while a party member acts, and while an enemy does, in 1/65536 turns: an eighth
// of a turn either side of 0. The camera event eases toward it (btCameraUpdate).
#define YAW_PARTY_ACTION       (DEGtoANG(45))
#define YAW_ENEMY_ACTION       (-(DEGtoANG(45)))

// The camera's yaw from behind the enemies, which an enemy's staged effect and cast take in place
// of YAW_ENEMY_ACTION.
#define YAW_BEHIND_ENEMIES     (TRIG_TURN * 5 / 16)

// REG_BLDCNT: blend the semi-transparent sprites (SPRITE_SEMI_TRANSPARENT) over every layer, to
// fade them.
#define BLEND_SPRITES_OVER_ALL (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

// Runs a battle effect by its ability's effect word (Ability.battleEffect): with EFFECT_BY_ID_RANGE
// from the second range of kinds (btEffectRunByIdRangeStub), otherwise from the handler table.
#define BATTLE_EFFECT_RUN(args, battleEffect)                                                      \
	((battleEffect) & EFFECT_BY_ID_RANGE ? btEffectRunByIdRangeStub(args)                          \
	                                     : btEffectRunFromTableStub(args))

// The keys that take the player on past a message: A, B, L and R.
#define KEY_ADVANCE        (KEY_A | KEY_B | KEYS_SHOULDERS)

// A stat stage changes the stat by an eighth of it (gmUnitCalculate), from half at the lowest to
// one and a half at the highest.
#define STAT_STAGE_MIN     -4
#define STAT_STAGE_MAX     4

// UNIT.psySeal's low three bits, Bind's turns left; Luff's seal above them loses PSY_SEAL_LUFF_TURN
// a turn.
#define PSY_SEAL_BIND_MASK 7
#define PSY_SEAL_LUFF_TURN 8
// UNIT.psySeal as a seal starts: Bind's lasts seven turns and may end sooner; Luff's lasts two and
// never ends early.
#define PSY_SEAL_BIND      7
#define PSY_SEAL_LUFF      (2 * PSY_SEAL_LUFF_TURN)

#endif
