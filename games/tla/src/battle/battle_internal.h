#ifndef ALCHEMY_BATTLE_INTERNAL_H
#define ALCHEMY_BATTLE_INTERNAL_H

/// @file
/// What every part of the battle object shares: its includes, in their
/// original order, and the names two or more parts use. battle.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves. It places no bytes.

#include "types.h"
#include "sprite_ids.h"
#include "ability_ids.h"
#include "djinn_ids.h"
#include "game/class_djinn.h"
#include "system/input.h"
#include "system/heap_slots.h"
#include "system/main_routine_state.h"
#include "system/point.h"
#include "system/iwram_call.h"
#include "battle/battle_camera.h"
#include "battle/battle_state.h"
#include "system/ram_buffer.h"
#include "system/serial.h"
#include "text/msg_ids.h"
#include "battle/battle_side.h"
#include "battle/battle_effect_params.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "battle/battle.h"
#include "window/window.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "battle/battle_system.h"
#include "battle/battle_type.h"
#include "battle/ability_staging.h"
#include "battle/debug_parameter_set.h"
#include "battle/weapon_kind.h"
#include "battle/battle_summon.h"
#include "battle/battle_unit.h"
#include "system/heap_release.h"
#include "tla_edition.h"
#include "edition.h"
#include "battle/battle_action.h"
#include "battle/motobj.h"
#include "macros.h"
#include "fixed_math.h"
#include "version.h"
#include "game/character_ids.h"
#include "field/objdisp.h"
#include "graphics/resource.h"
#include "field/sprite.h"
#include "system/sprite_metadata.h"
#include "system/task.h"
#include "system/scene.h"
#include "field/actor.h"
#include "battle/battle_obj.h"
#include "battle/battle_motion.h"
#include "battle/battle_message.h"
#include "battle/battle_escape.h"
#include "battle/battle_target.h"
#include "field/actor_view.h"
#include "battle/battle_random.h"
#include "system/irq_lock.h"
#include "battle/battle_event.h"
#include "battle/first_strike.h"
#include "system/io_write_queue.h"
#include "game/item.h"
#include "field/field_event.h"
#include "game/character.h"
#include "battle/battle_status_icon.h"
#include "system/io_write.h"
#include "system/frame.h"
#include "battle/range_falloff.h"
#include "battle/battle_effect.h"
#include "graphics/vram_tab.h"
#include "game/unit_hp.h"
#include "system/bg_scroll.h"
#include "system/irq.h"
#include "system/ram_code.h"
#include "system/matrix.h"
#include "field/particle.h"
#include "battle/formations.h"
#include "enemy_ids.h"
#include "item_ids.h"

/// BattleEnemy_ClaimNameLetter returns the letter in ENEMY_LETTER_MASK, with
/// ENEMY_LETTER_FIRST_TOO set for the second enemy of a class: its callers then
/// give the first one its letter too (BattleEnemy_LetterFirstOfKind) before
/// passing the letter to BattleUnit_SetFar.
#define ENEMY_LETTER_FIRST_TOO 0x8000
#define ENEMY_LETTER_MASK 0x7fff

/// A list place code's side bit, set for the enemies' list, and its place in
/// the list (Battle_GetUnitAtPlace): PLACE_ENEMY is PLACE_PARTY with the side
/// bit set.
#define PLACE_ENEMY_SIDE 0x80
#define PLACE_INDEX_MASK 0xf

/// What an action that comes first adds to its priority (a Djinni that
/// shields, guards or defends, an enemy's Counter or Guard Aura stance): more
/// than any agility, so it is taken before every other action of the turn.
#define PRIORITY_FIRST_IN_TURN 10000

/// The length of a unit's name, its terminator excluded.
#define UNIT_NAME_LENGTH 14

/// In a link battle each console sends LINK_PARTY_SIZE places, at most three
/// party members (LinkBattle_SendParty), and the opponent's take the first
/// three enemy ids from FIRST_ENEMY_UNIT.
#define LINK_PARTY_SIZE 3

/// gLinkStatus: both consoles are connected.
#define LINK_BOTH_CONNECTED 3

/// BattleAI_ChooseTarget's results when it picks no one: the side it
/// may target lists no one, or no one there is worth the ability.
#define TARGET_PICK_SIDE_EMPTY -2
#define TARGET_PICK_NONE -1

/// BattleState.placement of a unit with no place on the battlefield:
/// BattleUnit_RefreshPlacement fills the list with it.
#define BATTLE_OBJECT_NONE 255

/// BattleState.defeatState: whether the last enemy fell alone, or with others
/// before it, or a party member's action ended the battle with a single
/// defeat. The messages are "has been downed!" and "'s party goes down!",
/// MsgDowned plus the state; DEFEAT_BY_PARTY_MEMBER shows neither.
#define DEFEAT_ONE 0
#define DEFEAT_SEVERAL 1
#define DEFEAT_BY_PARTY_MEMBER 3

/// The game flag of an enemy the party has defeated, set by
/// BattleEnemy_RecordDefeat outside the monster battles; the monster battles
/// choose only formations of defeated enemies (BattleFormation_AverageLevel).
#define ENEMY_DEFEATED_FLAG(classId) ((classId) + FLAG_ENEMY_DEFEATED_FIRST)
/// The enemies that have a defeated flag, the flags 0x600 to 0x7ff.
#define ENEMY_DEFEATED_FLAG_COUNT 512

// Far calls the parts make, each passing its own view of the record:
// Sprite_AddAnimation and SpriteAnimation_Start (field.c, resource_slot/)
// take the metadata slot state and the animation entry.
/// Adds a sprite animation to a sprite in its first free slot; returns it, 0
/// when none could be taken, or -1 when all four are in use.
s32 Sprite_AddAnimationFar(void *state, s32 id);
/// Removes a sprite animation from a sprite and clears it.
void Sprite_RemoveAnimationFar(void *state, s32 handle);

#include "battle/battle_type.h"
#include "field/sprite.h"

/// A unit's HP and PP as ScriptedBattle_SaveUnits keeps them.
struct SnapshotVitals {
  u16 hp;
  u16 pp;
};

/// A unit's 25 status bytes, from its curse to its challenge, kept between the
/// scripted battles.
struct SnapshotCopy {
  s8 curse;
  s8 poison;
  u8 attackTurns;
  s8 attackStage;
  u8 defenseTurns;
  s8 defenseStage;
  u8 resistanceTurns;
  s8 resistanceStage;
  u8 delusion;
  s8 confusion;
  u8 charm;
  u8 stun;
  u8 sleep;
  u8 psySeal;
  u8 regenerate;
  s8 reflect;
  u8 haunt;
  u8 grimReaper;
  u8 equipBonus[2];
  u8 ready;
  u8 immobile;
  u8 agilityTurns;
  s8 agilityStage;
  s8 challenge;
};

/// What ScriptedBattle_SaveUnits keeps of the five units of the scripted
/// battles between them: whether each was listed, the first three's summon
/// boosts, each one's statuses and the last two's HP and PP.
struct Snapshot {
  u8 listed[8];
  s8 summonBoosts[3][ELEMENT_COUNT];
  /// The units' SnapshotCopy rows, read as gScriptedBattle_SnapshotBytes, each
  /// padded to 28 bytes; the last overruns into vitals' unused first rows.
  u8 statuses[128];
  struct SnapshotVitals
      vitals[5]; ///< also read as gScriptedBattle_SnapshotVitals
};

STATIC_ASSERT(offsetof(struct Snapshot, vitals) == 148);

/// The packet the two consoles exchange each turn of a link battle: how many
/// actions follow, a random check, and the random seed both sides then use.
struct BattleLinkedActionState {
  s32 count;
  s32 check;
  s32 seed;
  u8 unused[28]; ///< no code names it
};

/// The camera's yaw while a party member acts, and while an enemy does, in
/// 1/65536 turns: an eighth of a turn either side of 0.
#define YAW_PARTY_ACTION (DEGtoANG(45))
#define YAW_ENEMY_ACTION (-(DEGtoANG(45)))

/// The camera's yaw from behind the enemies, which an enemy's staged effect
/// and cast take in place of YAW_ENEMY_ACTION.
#define YAW_BEHIND_ENEMIES (TRIG_TURN * 5 / 16)

/// REG_BLDCNT: blend the semi-transparent sprites over every layer, to fade
/// them.
#define BLEND_SPRITES_OVER_ALL (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// The keys that take the player on past a message: A, B, L and R.
#define KEY_ADVANCE (KEY_A | KEY_B | KEYS_SHOULDERS)

/// Inventory_GetEquippedItemFar's slot for the weapon.
#define EQUIP_WEAPON 1

/// A group's sprite word (ACTOR_SPRITE_GROUP) points to a table of four
/// sprites.
#define ACTOR_SPRITE_GROUP_SIZE 4

/// Where a battle object's resource keeps its sprite slot, above its sprite:
/// (slot << RESOURCE_SLOT_SHIFT) | sprite.
#define RESOURCE_SLOT_SHIFT 12

/// An HP change that defeats any unit: the Grim Reaper's call deals it, and the
/// debug defeat.
#define DEFEAT_HP_CHANGE 0xc0000000

// What BattleActionRun_Build returns besides 0, which stages the action.
#define ACTION_REPORTED -1 ///< the action failed and its message is shown
#define ACTION_NOTHING_TO_SHOW -2 ///< nothing to stage or show

/// BattlePlan.targetCritical: the critical hit rolled for each target.
enum BattleCritical {
  CRITICAL_NONE,
  CRITICAL_HIT,   ///< an attack's: a quarter more damage
  CRITICAL_STRONG ///< an ability's (effects 36 to 40): half as much again
};

/// BattlePlan.targetHit while the hit is still to be rolled, as the ability
/// lands; BattleActionRun_Build rolls it ahead only for Annihilation and Whorl.
#define HIT_NOT_ROLLED -1

/// BattlePlan.failure: why a cast plays no effect, which the staging shows.
enum BattleActionFailure {
  ACTION_FAILURE_NONE,
  ACTION_FAILURE_SEALED, ///< "blocked": the user's Psynergy is sealed
  ACTION_FAILURE_NO_PP   ///< the user has too little PP
};

/// BattleStaging_RunFullScreen's mode bit that hides the acting unit too, as
/// the summons do.
#define STAGING_HIDE_ACTOR 1
/// BattleStaging_RunFullScreen's mode bit for Daedalus's strike at the turn's
/// end: the acting unit plays no cast, with no pose, sound or star field.
#define STAGING_NO_CAST 2

/// BattleStaging_RunAttack's mode bit for Meld: BattlePlan.partnerId runs in
/// beside the attacker.
#define ATTACK_WITH_PARTNER 32

/// How BattleStaging_RunPsynergyCast shows the caster.
enum BattleCastKind {
  CAST_PSYNERGY = 0, ///< the star field gathers on the caster
  CAST_DJINNI = 1,   ///< no glow; the effect plays its prelude
  CAST_ENEMY = 2     ///< the element's streaks
};

/// The scripted battles' snapshot (ScriptedBattle_SaveUnits).
extern struct Snapshot gScriptedBattle_Snapshot;

/// The five units' status bytes within the snapshot.
extern struct SnapshotCopy gScriptedBattle_SnapshotBytes[];

/// The last two units' HP and PP within the snapshot.
extern struct SnapshotVitals gScriptedBattle_SnapshotVitals[];

/// The five units Karst and Agatio's battle carries over between its parts:
/// Felix, Piers and Jenna, and the first two enemies.
extern u8 gScriptedBattle_Units[];

// A knocked-back unit's motion by variant (BattleMotion_Knockback), as The
// Broken Seal's.
extern const s32 gBattleKnockback_Acceleration[]; ///< acceleration

extern const s32 gBattleKnockback_SpeedLimit[]; ///< speed limit

extern const s32 gBattleKnockback_VelocityY[]; ///< upward velocity

extern const s32 gBattleKnockback_DistancePercent[]; ///< distance, in percent
                                                     ///< of the anchor's x

/// The tilt the battle camera adds while commands are chosen
/// (BattleCamera_ApplyTransform).
extern const FIXED gBattleCamera_CommandMatrix[];

/// The script a battle object starts with: none.
extern const s32 gBattleObject_EmptyScript[];

/// The order enemies of a fixed formation take from the centre out: 0, -1, 1,
/// -2, 2 and on.
extern const s8 gBattleFormation_CenteredOffsets[];

/// Each character's battle sprite for each weapon kind
/// (BattleUnit_GetWeaponSprite).
extern const u16 gBattleUnit_WeaponAnimationsIsaac[];

extern const u16 gBattleUnit_WeaponAnimationsGaret[];

extern const u16 gBattleUnit_WeaponAnimationsIvan[];

extern const u16 gBattleUnit_WeaponAnimationsMia[];

extern const u16 gBattleUnit_WeaponAnimationsJenna[];

extern const u16 gBattleUnit_WeaponAnimationsFelix[];

extern const u16 gBattleUnit_WeaponAnimationsSheba[];

extern const u16 gBattleUnit_WeaponAnimationsPiers[];

u32 BattleSprite_GetAttackHop(u32 index);
u32 BattleSprite_GetEffectHeight(u32 index);
s32 BattleMessage_WaitForAdvanceAt(s32 x, s32 y);

/// The battle background's colours: BG palettes 6 to 13, filled from
/// BattleState.palette, darkened or not.
#define BATTLE_BG_PALETTE ((u16 *)&BG_PLTT_COLOR(6, 0))

/// BG1 during a battle: 256 colours, priority 3, its map in screen block
/// BATTLE_BG1_SCREEN_BLOCK.
#define BATTLE_BG1CNT                                                          \
  (BGCNT_PRIORITY(3) | BGCNT_256_COLORS |                                      \
   BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK))

/// BattleActionRecord.target of a held unit's lost action (COMMAND_NONE, from
/// Battle_BuildEnemyActions): as a place code, place 0 of the other side, the
/// party's for an enemy and the enemies' for a party member.
#define TARGET_NONE_ENEMY PLACE_PARTY

#endif
