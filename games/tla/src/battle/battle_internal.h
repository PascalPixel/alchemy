#ifndef ALCHEMY_BATTLE_INTERNAL_H
#define ALCHEMY_BATTLE_INTERNAL_H

/* What every part of the battle object shares: its includes, in their original
   order, and the names two or more parts use. battle.c includes it before the
   parts, and each part includes it too, so that a part read on its own still
   resolves. It places no bytes. */

#include "types.h"
#include "sprite_ids.h"
#include "ability_ids.h"
#include "djinn_ids.h"
#include "game/class_djinn.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "battle/battle_camera.h"
#include "battle/battle_state.h"
#include "text/msg_ids.h"
#include "battle/battle_side.h"
#include "battle/battle_effect_params.h"
#include "battle/battle.h"
#include "window/window.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "battle/battle_system.h"
#include "battle/battle_type.h"
#include "battle/ability_staging.h"
#include "battle/debug_parameter_set.h"
#include "battle/weapon_kind.h"
#include "battle/battle_summon.h"
#include "battle/battle_unit.h"
#include "tla_edition.h"
#include "edition.h"
#include "battle/battle_action.h"
#include "battle/motobj.h"
#include "macros.h"
#include "fixed_math.h"
#include "game/character_ids.h"
#include "field/objdisp.h"
#include "graphics/resource.h"
#include "field/sprite.h"
#include "field/actor.h"
#include "battle/battle_obj.h"
#include "battle/battle_motion.h"
#include "text/msg_ids.h"
#include "battle/battle_escape.h"
#include "battle/battle_target.h"
#include "field/actor_view.h"
#include "battle/battle_random.h"
#include "battle/battle_event.h"
#include "battle/first_strike.h"
#include "game/item.h"
#include "field/field_script.h"
#include "game/character.h"
#include "battle/battle_status_icon.h"
#include "battle/range_falloff.h"
#include "battle/battle_effect.h"
#include "graphics/vram_tab.h"
#include "game/unit_hp.h"
#include "field/particle.h"
#include "battle/formations.h"
#include "enemy_ids.h"
#include "item_ids.h"

/* btEnemyClaimNameLetter returns the letter in ENEMY_LETTER_MASK, with
   ENEMY_LETTER_FIRST_TOO set for the second enemy of a class: its callers then
   give the first one its letter too (btEnemyLetterFirstOfKind) before passing
   the letter to gmBattleUnitSetStub. */
#define ENEMY_LETTER_FIRST_TOO 0x8000
#define ENEMY_LETTER_MASK	   0x7fff

/* A list place code's side bit, set for the enemies' list, and its place in the
   list (btGetUnitAtPlace): PLACE_ENEMY is PLACE_PARTY with the side bit set. */
#define PLACE_ENEMY_SIDE 0x80
#define PLACE_INDEX_MASK 0xf

/* What an action that comes first adds to its priority (a Djinni that shields,
   guards or defends, an enemy's Counter or Guard Aura stance): more than any
   agility, so it is taken before every other action of the turn. */
#define PRIORITY_FIRST_IN_TURN 10000

/* The length of a unit's name, its terminator excluded. */
#define UNIT_NAME_LENGTH 14

/* In a link battle each console sends LINK_PARTY_SIZE places, at most three
   party members (btLinkBattleSendParty), and the opponent's take the first
   three enemy ids from FIRST_ENEMY_UNIT. */
#define LINK_PARTY_SIZE 3

/* LinkStatusWork: both consoles are connected. */
#define LINK_BOTH_CONNECTED 3

/* btAIChooseTarget's results when it picks no one: the side it may target lists
   no one, or no one there is worth the ability. */
#define TARGET_PICK_SIDE_EMPTY -2
#define TARGET_PICK_NONE	   -1

/* BATTLE_STATE.placement of a unit with no place on the battlefield:
   btUnitRefreshPlacement fills the list with it. */
#define BATTLE_OBJECT_NONE 255

/* BATTLE_STATE.defeatState: whether the last enemy fell alone, or with others
   before it, or a party member's action ended the battle with a single defeat.
   The messages are "has been downed!" and "'s party goes down!", MsgDowned plus
   the state; DEFEAT_BY_PARTY_MEMBER shows neither. */
#define DEFEAT_ONE			   0
#define DEFEAT_SEVERAL		   1
#define DEFEAT_BY_PARTY_MEMBER 3

/* The game flag of an enemy the party has defeated, set by btEnemyRecordDefeat
   outside the monster battles; the monster battles choose only formations of
   defeated enemies (btFormationAverageLevel). */
#define ENEMY_DEFEATED_FLAG(classId) ((classId) + FLAG_ENEMY_DEFEATED_FIRST)
/* The enemies that have a defeated flag, the flags 0x600 to 0x7ff. */
#define ENEMY_DEFEATED_FLAG_COUNT	 512

/* Stub calls the parts make, each passing its own view of the record:
   fdSprAddAnimation and fdSprAnimationStart (field.c, resource_slot/) take the
   metadata slot state and the animation entry. Adds a sprite animation to a
   sprite in its first free slot; returns it, 0 when none could be taken, or -1
   when all four are in use. */
Sint32 fdSprAddAnimationStub(void *state, Sint32 id);
/* Removes a sprite animation from a sprite and clears it. */
void fdSprRemoveAnimationStub(void *state, Sint32 handle);

#include "battle/battle_type.h"
#include "field/sprite.h"

/* A unit's HP and PP as btScriptedBattleSaveUnits keeps them. */
typedef struct SNAPSHOT_VITALS {
	Uint16 hp;
	Uint16 pp;
} SNAPSHOT_VITALS;

/* A unit's 25 status bytes, from its curse to its challenge, kept between the
   scripted battles. */
typedef struct SNAPSHOT_COPY {
	Sint8 curse;
	Sint8 poison;
	Uint8 attackTurns;
	Sint8 attackStage;
	Uint8 defenseTurns;
	Sint8 defenseStage;
	Uint8 resistanceTurns;
	Sint8 resistanceStage;
	Uint8 delusion;
	Sint8 confusion;
	Uint8 charm;
	Uint8 stun;
	Uint8 sleep;
	Uint8 psySeal;
	Uint8 regenerate;
	Sint8 reflect;
	Uint8 haunt;
	Uint8 grimReaper;
	Uint8 equipBonus[2];
	Uint8 ready;
	Uint8 immobile;
	Uint8 agilityTurns;
	Sint8 agilityStage;
	Sint8 challenge;
} SNAPSHOT_COPY;

/* What btScriptedBattleSaveUnits keeps of the five units of the scripted
   battles between them: whether each was listed, the first three's summon
   boosts, each one's statuses and the last two's HP and PP. */
typedef struct SNAPSHOT {
	Uint8 listed[8];
	Sint8 summonBoosts[3][ELEMENT_COUNT];
	/* One row a unit: 25 status bytes, 28 with the struct's padding. */
	SNAPSHOT_COPY statuses[5];
	/* The HP and PP of the last two units (i = 3 and 4). */
	SNAPSHOT_VITALS vitals[2];
} SNAPSHOT;

STATIC_ASSERT(offsetof(SNAPSHOT, vitals) == 160);
STATIC_ASSERT(sizeof(SNAPSHOT) == 168);

/* The packet the two consoles exchange each turn of a link battle: how many
   actions follow, a random check, and the random seed both sides then use. */
typedef struct BATTLE_LINK_HEADER {
	Sint32 cnt;
	Sint32 check;
	Sint32 seed;
	Uint8  unused[28]; /* no code names it */
} BATTLE_LINK_HEADER;

/* The camera's yaw while a party member acts, and while an enemy does, in
   1/65536 turns: an eighth of a turn either side of 0. */
#define YAW_PARTY_ACTION (DEGtoANG(45))
#define YAW_ENEMY_ACTION (-(DEGtoANG(45)))

/* The camera's yaw from behind the enemies, which an enemy's staged effect and
   cast take in place of YAW_ENEMY_ACTION. */
#define YAW_BEHIND_ENEMIES (TRIG_TURN * 5 / 16)

/* REG_BLDCNT: blend the semi-transparent sprites over every layer, to fade
   them. */
#define BLEND_SPRITES_OVER_ALL (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/* Runs a battle effect by its ability's effect word (Ability.battleEff): with
   EFFECT_BY_ID_RANGE from the second range of kinds (btEffectRunByIdRangeStub),
   otherwise from the handler table. */
#define BATTLE_EFFECT_RUN(args, battleEffect) \
	((battleEffect) & EFFECT_BY_ID_RANGE ? btEffectRunByIdRangeStub(args) \
										 : btEffectRunFromTableStub(args))

/* The keys that take the player on past a message: A, B, L and R. */
#define KEY_ADVANCE (KEY_A | KEY_B | KEYS_SHOULDERS)

/* gmInventoryGetEquippedItemStub's slot for the weapon. */
#define EQUIP_WEAPON 1

/* A group's sprite word (ACTOR_SPRITE_GROUP) points to a table of four
   sprites. */
#define ACTOR_SPRITE_GROUP_SIZE 4

/* Where a battle object's resource keeps its sprite slot, above its sprite:
   (slot << RESOURCE_SLOT_SHIFT) | sprite. */
#define RESOURCE_SLOT_SHIFT 12

/* An HP change that defeats any unit: the Grim Reaper's call deals it, and the
   debug defeat. */
#define DEFEAT_HP_CHANGE 0xc0000000

/* What btActionRunBuild returns besides 0, which stages the action. */
#define ACTION_REPORTED		   -1 /* the action failed and its message is shown */
#define ACTION_NOTHING_TO_SHOW -2 /* nothing to stage or show */

/* BATTLE_PLAN.tgtCritical: the critical hit rolled for each target. */
typedef enum BATTLE_CRITICAL {
	CRITICAL_NONE,
	CRITICAL_HIT,	/* an attack's: a quarter more damage */
	CRITICAL_STRONG /* an ability's (effects 36 to 40): half as much again */
} BATTLE_CRITICAL;

/* BATTLE_PLAN.tgtHit while the hit is still to be rolled, as the ability lands;
   btActionRunBuild rolls it ahead only for Annihilation and Whorl. */
#define HIT_NOT_ROLLED -1

/* BATTLE_PLAN.failure: why a cast plays no effect, which the staging shows. */
typedef enum BATTLE_ACTION_FAILURE {
	ACTION_FAILURE_NONE,
	ACTION_FAILURE_SEALED, /* "blocked": the user's Psynergy is sealed */
	ACTION_FAILURE_NO_PP   /* the user has too little PP */
} BATTLE_ACTION_FAILURE;

/* btStagingRunFullScreen's mode bit that hides the acting unit too, as the
   summons do. */
#define STAGING_HIDE_ACTOR 1
/* btStagingRunFullScreen's mode bit for Daedalus's strike at the turn's end:
   the acting unit plays no cast, with no pose, sound or star field. */
#define STAGING_NO_CAST	   2

/* btStagingRunAttack's mode bit for Meld: BATTLE_PLAN.partnerId runs in beside
   the attacker. */
#define ATTACK_WITH_PARTNER 32

/* How btStagingRunPsynergyCast shows the caster. */
typedef enum BATTLE_CAST_KIND {
	CAST_PSYNERGY = 0, /* the star field gathers on the caster */
	CAST_DJINNI = 1,   /* no glow; the effect plays its prelude */
	CAST_ENEMY = 2	   /* the element's streaks */
} BATTLE_CAST_KIND;

/* The scripted battles' snapshot (btScriptedBattleSaveUnits), reached by its
   fixed address: the 168 bytes of EWRAM below the map's cell buffer. */
#define RamScriptedBattleSnapshot \
	FIXED_ADDRESS((SNAPSHOT *), 0x0200ff58, ScriptedBattleSnapshot)

/* The five units Karst and Agatio's battle carries over between its parts:
   Felix, Piers and Jenna, and the first two enemies. */
extern Uint8 ScriptedBattleUnits[];

/* A knocked-back unit's motion by variant (BattleMotionKnockback), as The
   Broken Seal's. */
extern const Sint32 BattleKnockbackAcceleration[]; /* acceleration */

extern const Sint32 BattleKnockbackSpeedLimit[]; /* speed limit */

extern const Sint32 BattleKnockbackVelocityY[]; /* upward velocity */

/* distance, in percent of the anchor's x */
extern const Sint32 BattleKnockbackDistancePercent[];

/* The tilt the battle camera adds while commands are chosen
   (btCameraApplyTransform). */
extern const FIXED BattleCameraCommandMatrix[];

/* The script a battle object starts with: none. */
extern const Sint32 BattleObjectEmptyScript[];

/* The order enemies of a fixed formation take from the centre out: 0, -1, 1,
   -2, 2 and on. */
extern const Sint8 BattleFormationCenteredOffsets[];

/* Each character's battle sprite for each weapon kind (btUnitGetWeaponSpr). */
extern const Uint16 BattleUnitWeaponAnimationsIsaac[];

extern const Uint16 BattleUnitWeaponAnimationsGaret[];

extern const Uint16 BattleUnitWeaponAnimationsIvan[];

extern const Uint16 BattleUnitWeaponAnimationsMia[];

extern const Uint16 BattleUnitWeaponAnimationsJenna[];

extern const Uint16 BattleUnitWeaponAnimationsFelix[];

extern const Uint16 BattleUnitWeaponAnimationsSheba[];

extern const Uint16 BattleUnitWeaponAnimationsPiers[];

/* The battle background's colours: BG palettes 6 to 13, filled from
   BATTLE_STATE.pal, darkened or not. */
#define BATTLE_BG_PALETTE ((Uint16 *)&BG_PLTT_COLOR(6, 0))

/* BG1 during a battle: 256 colours, priority 3, its map in screen block
   BATTLE_BG1_SCREEN_BLOCK. */
#define BATTLE_BG1CNT \
	(BGCNT_PRIORITY(3) | BGCNT_256_COLORS | \
		BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK))

/* BATTLE_ACTION_RECORD.tgt of a held unit's lost action (COMMAND_NONE, from
   btBuildEnemyActions): as a place code, place 0 of the other side, the party's
   for an enemy and the enemies' for a party member. */
#define TARGET_NONE_ENEMY PLACE_PARTY

#endif
