#ifndef ALCHEMY_BATTLE_INTERNAL_H
#define ALCHEMY_BATTLE_INTERNAL_H

/// @file
/// What every part of the battle object shares: its includes, in their
/// original order, and the names two or more parts use. battle.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves. It places no bytes.

#include "types.h"
#include "ability_ids.h"
#include "owner_state.h"
#include "system/input.h"
#include "system/main_routine_work.h"
#include "system/fixed_point_position.h"
#include "system/iwram_call.h"
#include "battle_display.h"
#include "battle_work.h"
#include "system/ram_buffer.h"
#include "system/serial.h"
#include "text/msg_ids.h"
#include "battle/battle_side.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "battle/battle.h"
#include "module/window/window.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "battle/battle_system.h"
#include "battle_type.h"
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
#include "actor.h"
#include "battle/battle_obj.h"
#include "battle/battle_motion.h"
#include "battle_message.h"
#include "battle/battle_escape.h"
#include "battle/battle_target.h"
#include "actor_view.h"
#include "minigame.h"
#include "battle/battle_random.h"
#include "system/irq_lock.h"
#include "battle/battle_event.h"
#include "battle/first_strike.h"
#include "module/battle/battle.h"
#include "module/system/system.h"
#include "system/io_write_queue.h"
#include "game/item.h"
#include "field_event.h"
#include "game/character.h"
#include "battle/battle_status_icon.h"
#include "system/io_write.h"
#include "system/frame.h"
#include "battle/range_falloff.h"
#include "battle/battle_effect.h"
#include "graphics/vram_tab.h"
#include "battle/b5_context.h"
#include "game/owner_value.h"
#include "bg_scroll.h"
#include "system/irq.h"
#include "system/ram_code.h"
#include "system/transform.h"
#include "field/particle.h"
#include "battle_formation.h"
#include "enemy_ids.h"
#include "item_ids.h"

/// BattleEnemy_ClaimNameLetter returns the letter in ENEMY_LETTER_MASK, with
/// ENEMY_LETTER_FIRST_TOO set for the second enemy of a class: its callers then
/// give the first one its letter too (BattleEnemy_LetterFirstOfKind) before
/// passing the letter to BattleUnit_SetFar.
#define ENEMY_LETTER_FIRST_TOO 0x8000
#define ENEMY_LETTER_MASK 0x7fff

/// The most actions BattleWork.actions holds for a turn.
#define BATTLE_ACTION_CAPACITY 20

/// BattleAI_ChooseTarget's results when it picks no one: the side it
/// may target lists no one, or no one there is worth the ability.
#define TARGET_PICK_SIDE_EMPTY -2
#define TARGET_PICK_NONE -1

/// The list place codes the target pickers return (targeting.c,
/// Battle_FindUnitPlace): a place in the party's list or the enemies',
/// or'd with the side's tag.
#define PLACE_PARTY 0x100
#define PLACE_ENEMY 0x180

/// BattleWork.placement of a unit with no place on the battlefield:
/// BattleUnit_RefreshPlacement fills the list with it.
#define BATTLE_OBJECT_NONE 0xff

/// The party's battle sprites, and Isaac's and Garet's while they hold a
/// Machete, drawn as part of it (BattleObject_Make).
#define SPRITE_BATTLE_ISAAC 490
#define SPRITE_BATTLE_GARET 495
#define SPRITE_BATTLE_IVAN 500
#define SPRITE_BATTLE_MIA 504
#define SPRITE_BATTLE_FELIX 508
#define SPRITE_BATTLE_JENNA 513
#define SPRITE_BATTLE_SHEBA 517
#define SPRITE_BATTLE_PIERS 521
#define SPRITE_ISAAC_MACHETE 635
#define SPRITE_GARET_MACHETE 637
/// The shadow sprite under a unit (BattleObj.effect).
#define SPRITE_BATTLE_SHADOW 686

/// Enemy ids the battle tests, each named from the English release
/// (MsgEnemyName plus the id less ENEMY_ID_FIRST). Rows that share a name take
/// _2 onwards in row order, as the game itself numbers some of them ("Serpent
/// 6").
enum BattleEnemyId {
  ENEMY_RUFFIAN = 68,
  ENEMY_RUFFIAN_2 = 69,
  ENEMY_RUFFIAN_3 = 70,
  ENEMY_CHESTBEATER = 74,
  ENEMY_WILD_GORILLA = 75,
  ENEMY_CRAZY_GORILLA = 76,
  ENEMY_DEVIL_SCORPION = 78,
  ENEMY_SAND_SCORPION = 79,
  ENEMY_BRIGGS = 80,
  ENEMY_SEA_FIGHTER = 81,
  ENEMY_CHAMPA_2 = 82,
  ENEMY_CHAMPA_3 = 83,
  ENEMY_AQUA_HYDRA = 88,
  ENEMY_HYDRA = 89,
  ENEMY_PYRODRA = 90,
  ENEMY_SERPENT = 91,
  ENEMY_SERPENT_2 = 92,
  ENEMY_SERPENT_3 = 93,
  ENEMY_SERPENT_4 = 94,
  ENEMY_SERPENT_5 = 95,
  ENEMY_SERPENT_6 = 96,
  ENEMY_SERPENT_7 = 97,
  ENEMY_AVIMANDER = 98,
  ENEMY_POSEIDON = 101,
  ENEMY_POSEIDON_2 = 102,
  ENEMY_POSEIDON_3 = 103,
  ENEMY_MOAPA = 104,
  ENEMY_MOAPA_2 = 105,
  ENEMY_MOAPA_3 = 106,
  ENEMY_KNIGHT = 107,
  ENEMY_KNIGHT_2 = 108,
  ENEMY_KNIGHT_3 = 109,
  ENEMY_AGATIO = 110,
  ENEMY_AGATIO_2 = 111,
  ENEMY_AGATIO_3 = 112,
  ENEMY_KARST = 113,
  ENEMY_KARST_2 = 114,
  ENEMY_KARST_3 = 115,
  ENEMY_WILD_WOLF = 117,
  ENEMY_DIRE_WOLF = 118,
  ENEMY_WHITE_WOLF = 119,
  ENEMY_DINO = 149,
  ENEMY_DINOX = 150,
  ENEMY_DINOSAURUS = 151,
  ENEMY_PIXIE = 158,
  ENEMY_FAERY = 159,
  ENEMY_WEIRD_NYPMH = 160,
  ENEMY_STAR_MAGICIAN = 164,
  ENEMY_SEA_DRAGON = 176,
  ENEMY_TURTLE_DRAGON = 177,
  ENEMY_OCEAN_DRAGON = 178,
  ENEMY_FLAME_DRAGON = 194,
  ENEMY_FLAME_DRAGON_2 = 195,
  ENEMY_MINOTAURUS = 197,
  ENEMY_MINOS_WARRIOR = 198,
  ENEMY_MINOS_KNIGHT = 199,
  ENEMY_AKA_MANAH = 206,
  ENEMY_DRUJ = 207,
  ENEMY_AESHMA = 208,
  ENEMY_VALUKAR = 209,
  ENEMY_VALUKAR_2 = 210,
  ENEMY_VALUKAR_3 = 211,
  ENEMY_DULLAHAN = 218,
  ENEMY_DULLAHAN_2 = 219,
  ENEMY_DULLAHAN_3 = 220,
  ENEMY_SENTINEL = 221,
  ENEMY_SENTINEL_2 = 222,
  ENEMY_SENTINEL_3 = 223,
  ENEMY_SLIME = 234,
  ENEMY_SLIME_2 = 235,
  ENEMY_OOZE = 236,
  ENEMY_SLIME_BEAST = 237,
  ENEMY_BANDIT = 245,
  ENEMY_THIEF = 246,
  ENEMY_TROLL = 263,
  ENEMY_CAVE_TROLL = 264,
  ENEMY_BRUTAL_TROLL = 265,
  ENEMY_RECLUSE = 268,
  ENEMY_SIREN = 281,
  ENEMY_SUCCUBUS = 282,
  ENEMY_NIGHTMARE = 283,
  ENEMY_KILLER_APE = 291,
  ENEMY_ORC_CAPTAIN = 297,
  ENEMY_ORC_LORD = 298,
  ENEMY_MANTICORE = 302,
  ENEMY_KOBOLD = 305,
  ENEMY_HOBGOBLIN = 307,
  ENEMY_GRYPHON = 311,
  ENEMY_CERBERUS = 318,
  ENEMY_CHIMERA = 323,
  ENEMY_LIZARD_FIGHTER = 331,
  ENEMY_LIZARD_KING = 332,
  ENEMY_ANT_LION = 333,
  ENEMY_TOADONPA = 336,
  ENEMY_LIVING_STATUE = 339,
  ENEMY_HYDROS_STATUE = 340,
  ENEMY_AZART = 341,
  ENEMY_AZART_2 = 342,
  ENEMY_SATRAGE = 343,
  ENEMY_SATRAGE_2 = 344,
  ENEMY_NAVAMPA = 345,
  ENEMY_NAVAMPA_2 = 346,
  ENEMY_TRET = 347,
  ENEMY_KRAKEN = 348,
  ENEMY_TORNADO_LIZARD = 349,
  ENEMY_STORM_LIZARD = 350,
  ENEMY_TEMPEST_LIZARD = 351,
  ENEMY_MYSTERY_MAN = 352,
  ENEMY_SATUROS = 353,
  ENEMY_SATUROS_2 = 354,
  ENEMY_MYSTERY_WOMAN = 355,
  ENEMY_MENARDI = 356,
  ENEMY_FUSION_DRAGON = 357,
  ENEMY_DEADBEARD = 358,
  ENEMY_MIMIC = 359,
  ENEMY_MIMIC_2 = 360,
  ENEMY_MIMIC_3 = 361,
  ENEMY_MIMIC_4 = 362,
  ENEMY_MIMIC_5 = 363,
  ENEMY_MIMIC_6 = 364,
  ENEMY_MIMIC_7 = 365,
  ENEMY_MIMIC_8 = 366,
  ENEMY_MIMIC_9 = 367,
  ENEMY_DOOM_DRAGON = 369,
  ENEMY_DOOM_DRAGON_2 = 370,
  ENEMY_DOOM_DRAGON_3 = 371,
  ENEMY_DOOM_DRAGON_4 = 372,
  ENEMY_DOOM_DRAGON_5 = 373,
  ENEMY_DOOM_DRAGON_6 = 374,
  ENEMY_DOOM_DRAGON_7 = 375,
  ENEMY_DOOM_DRAGON_8 = 376,
  ENEMY_DOOM_DRAGON_9 = 377,
  ENEMY_REFRESH_BALL = 378,
  ENEMY_THUNDER_BALL = 379,
  ENEMY_ANGER_BALL = 380,
  ENEMY_GUARDIAN_BALL = 381,
  ENEMY_BANDIT_2 = 385,
  ENEMY_THIEF_2 = 386
};

// Far calls the parts make, each passing its own view of the record:
// ResourceMetadata_Register and SpriteAnimation_Start (field.c, resource_slot/)
// take the metadata slot state and the animation entry.
s32 ResourceMetadata_RegisterFar(void *state, s32 id);
void Sprite_RemoveAnimationFar(void *state, s32 handle);

#include "battle_type.h"
#include "field/sprite.h"

/// A unit's sprite as BattleMotion_PlayFall reads it: its first animation.
struct BattleObjectAnimation {
  u8 unk00[0x28];
  struct AnimationEntry *entry;
};

/// A unit's HP and PP as ScriptedBattle_SaveUnits keeps them.
struct SnapshotVitals {
  u16 hp;
  u16 pp;
};

/// What ScriptedBattle_SaveUnits keeps of the five units of the scripted
/// battles between them: whether each was listed, the first three's summon
/// boosts, and the last two's HP and PP. The statuses between them are
/// SnapshotCopy's.
struct Snapshot {
  u8 listed[8];
  u8 tags[3][4];
  u8 unk14[0x80];
  struct SnapshotVitals vitals[5];
};

/// A unit as the scripted battles' snapshot reads it: its HP and PP, its summon
/// boosts, and the 25 status bytes from its curse to its challenge.
struct SnapshotUnit {
  u8 unk00[0x38];
  s16 hp;
  u16 unk3a;
  u8 unk3c[0xf0];
  u8 tags[4];
  u8 snapshot[25];
};

/// A unit's 25 status bytes, from its curse to its challenge, kept between the
/// scripted battles.
struct SnapshotCopy {
  u8 bytes[25];
};

/// A row of the debug parameter sets: a party member, its level, its Djinn of
/// each element, its equipped items and two Psynergy it learns.
struct DebugPartyEntry {
  s8 owner;
  u8 level;
  s8 djinnCounts[4];
  u16 items[6];
  u16 actions[2];
  u16 unk16;
};

/// A unit's sprite as BattleMotion reads it: its scale.
struct SlotMotionRecord {
  u8 unk00[12];
  s32 height;
};

/// A unit's sprite as the status icon reads it: its width, which picks the icon
/// set, and the flag that redraws it.
struct StatusIconRecord {
  u8 unk00[0x14];
  u8 width;
  u8 unk15[4];
  u8 dirty;
};

/// A unit's sprite as BattleObject_Make builds it: its image, scale, width,
/// redraw flag and outline flags, and its animations.
struct TlaSpriteRecord {
  struct AnimationSpritePart image;
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 unk15[4];
  u8 dirty;
  u8 flags;
  u8 unk1b[13];
  struct AnimationEntry *entries[1];
};

/// A sprite as ResetMotionRecordGroup reads it: its four animations.
struct AnimationEntryGroup {
  u8 unk00[0x28];
  struct AnimationEntry *entries[4];
};

/// A unit's sprite as the battle reads its scale.
struct MotionRecordScale {
  u8 unk00[0xc];
  s32 scale;
};

/// The form change effect's command, as Battle_PlayFormChange fills it for
/// enemy 128.
struct BattleFxRequest {
  s32 field00;
  s32 field04;
  s32 field08;
  s32 field0c;
  s32 field10;
  s32 field14;
  s32 field18;
  u8 unk1c[8];
  s16 field24;
  u8 unk26[0x32];
};

struct Layout {
  u8 pad[4];
  s16 field[6];
};

/// The battle work past BattleWork's members, as the staging reads it: the
/// backdrop's brightness and background, the plan of the action being played,
/// the flag that skips its playback, and how the acting unit is shown.
struct DispatchBattleWork {
  u8 unk000[0x66c];
  s32 brightness;
  u16 background;
  u8 unk672[0x67c - 0x672];
  struct BattlePlan plan;
  s32 skipPlayback;
  u8 unk6e4[0x84e - 0x6e4];
  u16 unk84e;
  u8 unk850[0x869 - 0x850];
  u8 actorMode;
};

/// The packet the two consoles exchange each turn of a link battle: how many
/// actions follow, a random check, and the random seed both sides then use.
struct BattleLinkedActionState {
  s32 count;
  s32 check;
  s32 seed;
  u8 unk0c[28];
};

/// The camera's yaw while a party member acts, and while an enemy does, in
/// 1/65536 turns: an eighth of a turn either side of 0.
#define YAW_PARTY_ACTION (TRIG_TURN / 8)
#define YAW_ENEMY_ACTION (-(TRIG_TURN / 8))

/// The camera's yaw from behind the enemies, which an enemy's staged effect
/// and cast take in place of YAW_ENEMY_ACTION.
#define YAW_BEHIND_ENEMIES (TRIG_TURN * 5 / 16)

/// REG_BLDCNT: blend the semi-transparent sprites over every layer, to fade
/// them.
#define BLEND_SPRITES_OVER_ALL (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// The keys that take the player on past a message: A, B, L and R.
#define KEY_ADVANCE (KEY_A | KEY_B | KEYS_SHOULDERS)

/// The battle camera's distance from its focus in the default view.
#define BATTLE_CAMERA_DISTANCE px(510)

/// Inventory_GetEquippedItemFar's slot for the weapon.
#define EQUIP_WEAPON 1

// What BattleActionRun_Build returns besides 0, which stages the action.
#define ACTION_REPORTED -1 ///< the action failed and its message is shown
#define ACTION_NOTHING_TO_SHOW -2 ///< nothing to stage or show

// COMMAND_DJINN's parameter: the Djinni's element in bits 11..8, its index in
// bits 7..0.
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v) ((v) & 255)

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

/// BattlePlan.staging: how
/// BattleStaging_DispatchAction shows the action, set by BattleActionRun_Build
/// from the ability (gAbilityStaging where it gives one). The Lost Age adds the
/// stagings from STAGING_TEAM_STRIKE on.
enum BattleStaging {
  STAGING_MESSAGES = 1, ///< no effect: each target's messages only
  STAGING_ATTACK = 2,   ///< the acting unit runs up and strikes
  STAGING_CAST = 3,     ///< Psynergy, and Venus, Mercury, Mars, Jupiter
  STAGING_ITEM = 4,
  STAGING_SUMMON = 5, ///< the larger summons, the acting unit hidden
  STAGING_DJINNI = 6, ///< a Djinni with no attack (Granite, Quartz...)
  STAGING_FLEE = 7,   ///< a unit runs
  /// The action plays as a cast, the caster showing its element's streaks in
  /// place of the gathering star field: an enemy's ability that is not
  /// Psynergy and has a battle effect, Slice, and weapon unleashes such as
  /// Blizzard and Titan Blade.
  STAGING_ENEMY_CAST = 8,
  STAGING_FULL_SCREEN = 9, ///< Gaia, Rockfall, Procne and their kin
  /// Meld ("Launch a powerful team strike.") and Mold: a second unit
  /// (BattlePlan.actorId2) strikes as the Djinni is unleashed.
  STAGING_TEAM_STRIKE = 10,
  STAGING_DJINN_RECOVERY = 11, ///< Eddy: "Speed up Djinn recovery time."
  STAGING_MEGAERA = 12,        ///< the summon, then High Impact on its side
  STAGING_IRIS = 13,           ///< the summon, then the Aurora Field
  /// The summoned beasts that strike: Wild Wolf, Roc, Orc, Grand Golem, Dinox
  /// and Call Zombie (BattleMotion_CallBeast).
  STAGING_BEAST_ATTACK = 15,
  /// The summoned beasts that cast, Emu to Call Dullahan.
  STAGING_BEAST_CAST = 16
};

/// BattleStaging_HideField's mode bit that hides the acting unit too, as the
/// summons do.
#define STAGING_HIDE_ACTOR 1

/// BattleStaging_RunAttack's mode bit for Meld: BattlePlan.actorId2 runs in
/// beside the attacker.
#define ATTACK_WITH_PARTNER 32

/// How BattleStaging_RunPsynergyCast shows the caster.
enum BattleCastKind {
  CAST_PSYNERGY = 0, ///< the star field gathers on the caster
  CAST_DJINNI = 1,   ///< no glow; the effect plays its prelude
  CAST_ENEMY = 2     ///< the element's streaks
};

// BattlePlan.battleEffect: the effect kind
// in the low twelve bits and its strength (a double or triple blow) above them;
// then how the effect is staged.
#define EFFECT_VARIANT_SHIFT 12
#define EFFECT_BY_ID_RANGE 0x4000 ///< BattleEffect_RunByIdRangeFar
#define EFFECT_SUMMON 0x8000 ///< BattleEffect_RunSummonFar, acting unit's view
#define EFFECT_ON_DOWNED 0x10000 ///< shown on downed targets too (revivals)

/// The scripted battles' snapshot (ScriptedBattle_SaveUnits).
extern struct Snapshot Data_0200ff58;

/// The five units' status bytes within the snapshot.
extern struct SnapshotCopy Data_0200ff6c[];

/// The last two units' HP and PP within the snapshot.
extern struct SnapshotVitals Data_0200ffec[];

/// The five units the scripted battles of formations 74 to 76 carry over:
/// Felix, Piers and Jenna, and the first two enemies.
extern u8 gScriptedBattle_Units[];

/// The debug parameter sets DebugParameterSet_Load loads, a party member a row.
extern struct DebugPartyEntry gDebug_ParameterSets[];

// A knocked-back unit's motion by variant (BattleMotion_Knockback), as The
// Broken Seal's.
extern const s32 gBattleKnockback_Acceleration[]; ///< acceleration

extern const s32 gBattleKnockback_SpeedLimit[]; ///< speed limit

extern const s32 gBattleKnockback_VelocityY[]; ///< upward velocity

extern const s32 gBattleKnockback_DistancePercent[]; ///< distance, in percent
                                                     ///< of the anchor's x

/// The tilt the battle camera adds while commands are chosen
/// (BattleCamera_ApplyTransform).
extern const s32 gBattleCamera_CommandMatrix[];

/// The script a battle object starts with: none.
extern const s32 gBattleObject_EmptyScript[];

/// The order enemies of a fixed formation take from the centre out: 0, -1, 1,
/// -2, 2 and on.
extern const s8 gBattleParty_CenterOrderOffsets[];

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

#endif
