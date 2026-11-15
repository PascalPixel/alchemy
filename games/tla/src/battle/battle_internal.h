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
#define ENEMY_RUFFIAN 68
#define ENEMY_RUFFIAN_2 69
#define ENEMY_RUFFIAN_3 70
#define ENEMY_CHESTBEATER 74
#define ENEMY_WILD_GORILLA 75
#define ENEMY_CRAZY_GORILLA 76
#define ENEMY_DEVIL_SCORPION 78
#define ENEMY_SAND_SCORPION 79
#define ENEMY_BRIGGS 80
#define ENEMY_SEA_FIGHTER 81
#define ENEMY_CHAMPA_2 82
#define ENEMY_CHAMPA_3 83
#define ENEMY_AQUA_HYDRA 88
#define ENEMY_HYDRA 89
#define ENEMY_PYRODRA 90
#define ENEMY_SERPENT 91
#define ENEMY_SERPENT_2 92
#define ENEMY_SERPENT_3 93
#define ENEMY_SERPENT_4 94
#define ENEMY_SERPENT_5 95
#define ENEMY_SERPENT_6 96
#define ENEMY_SERPENT_7 97
#define ENEMY_AVIMANDER 98
#define ENEMY_POSEIDON 101
#define ENEMY_POSEIDON_2 102
#define ENEMY_POSEIDON_3 103
#define ENEMY_MOAPA 104
#define ENEMY_MOAPA_2 105
#define ENEMY_MOAPA_3 106
#define ENEMY_KNIGHT 107
#define ENEMY_KNIGHT_2 108
#define ENEMY_KNIGHT_3 109
#define ENEMY_AGATIO 110
#define ENEMY_AGATIO_2 111
#define ENEMY_AGATIO_3 112
#define ENEMY_KARST 113
#define ENEMY_KARST_2 114
#define ENEMY_KARST_3 115
#define ENEMY_WILD_WOLF 117
#define ENEMY_DIRE_WOLF 118
#define ENEMY_WHITE_WOLF 119
#define ENEMY_DINO 149
#define ENEMY_DINOX 150
#define ENEMY_DINOSAURUS 151
#define ENEMY_PIXIE 158
#define ENEMY_FAERY 159
#define ENEMY_WEIRD_NYPMH 160
#define ENEMY_STAR_MAGICIAN 164
#define ENEMY_SEA_DRAGON 176
#define ENEMY_TURTLE_DRAGON 177
#define ENEMY_OCEAN_DRAGON 178
#define ENEMY_FLAME_DRAGON 194
#define ENEMY_FLAME_DRAGON_2 195
#define ENEMY_MINOTAURUS 197
#define ENEMY_MINOS_WARRIOR 198
#define ENEMY_MINOS_KNIGHT 199
#define ENEMY_AKA_MANAH 206
#define ENEMY_DRUJ 207
#define ENEMY_AESHMA 208
#define ENEMY_VALUKAR 209
#define ENEMY_VALUKAR_2 210
#define ENEMY_VALUKAR_3 211
#define ENEMY_DULLAHAN 218
#define ENEMY_DULLAHAN_2 219
#define ENEMY_DULLAHAN_3 220
#define ENEMY_SENTINEL 221
#define ENEMY_SENTINEL_2 222
#define ENEMY_SENTINEL_3 223
#define ENEMY_SLIME 234
#define ENEMY_SLIME_2 235
#define ENEMY_OOZE 236
#define ENEMY_SLIME_BEAST 237
#define ENEMY_BANDIT 245
#define ENEMY_THIEF 246
#define ENEMY_TROLL 263
#define ENEMY_CAVE_TROLL 264
#define ENEMY_BRUTAL_TROLL 265
#define ENEMY_RECLUSE 268
#define ENEMY_SIREN 281
#define ENEMY_SUCCUBUS 282
#define ENEMY_NIGHTMARE 283
#define ENEMY_KILLER_APE 291
#define ENEMY_ORC_CAPTAIN 297
#define ENEMY_ORC_LORD 298
#define ENEMY_MANTICORE 302
#define ENEMY_KOBOLD 305
#define ENEMY_HOBGOBLIN 307
#define ENEMY_GRYPHON 311
#define ENEMY_CERBERUS 318
#define ENEMY_CHIMERA 323
#define ENEMY_LIZARD_FIGHTER 331
#define ENEMY_LIZARD_KING 332
#define ENEMY_ANT_LION 333
#define ENEMY_TOADONPA 336
#define ENEMY_LIVING_STATUE 339
#define ENEMY_HYDROS_STATUE 340
#define ENEMY_AZART 341
#define ENEMY_AZART_2 342
#define ENEMY_SATRAGE 343
#define ENEMY_SATRAGE_2 344
#define ENEMY_NAVAMPA 345
#define ENEMY_NAVAMPA_2 346
#define ENEMY_TRET 347
#define ENEMY_KRAKEN 348
#define ENEMY_TORNADO_LIZARD 349
#define ENEMY_STORM_LIZARD 350
#define ENEMY_TEMPEST_LIZARD 351
#define ENEMY_MYSTERY_MAN 352
#define ENEMY_SATUROS 353
#define ENEMY_SATUROS_2 354
#define ENEMY_MYSTERY_WOMAN 355
#define ENEMY_MENARDI 356
#define ENEMY_FUSION_DRAGON 357
#define ENEMY_DEADBEARD 358
#define ENEMY_MIMIC 359
#define ENEMY_MIMIC_2 360
#define ENEMY_MIMIC_3 361
#define ENEMY_MIMIC_4 362
#define ENEMY_MIMIC_5 363
#define ENEMY_MIMIC_6 364
#define ENEMY_MIMIC_7 365
#define ENEMY_MIMIC_8 366
#define ENEMY_MIMIC_9 367
#define ENEMY_DOOM_DRAGON 369
#define ENEMY_DOOM_DRAGON_2 370
#define ENEMY_DOOM_DRAGON_3 371
#define ENEMY_DOOM_DRAGON_4 372
#define ENEMY_DOOM_DRAGON_5 373
#define ENEMY_DOOM_DRAGON_6 374
#define ENEMY_DOOM_DRAGON_7 375
#define ENEMY_DOOM_DRAGON_8 376
#define ENEMY_DOOM_DRAGON_9 377
#define ENEMY_REFRESH_BALL 378
#define ENEMY_THUNDER_BALL 379
#define ENEMY_ANGER_BALL 380
#define ENEMY_GUARDIAN_BALL 381
#define ENEMY_BANDIT_2 385
#define ENEMY_THIEF_2 386

// Far calls the parts make, each passing its own view of the record:
// ResourceMetadata_Register and SpriteAnimation_Start (field.c, resource_slot/)
// take the metadata slot state and the animation entry.
s32 ResourceMetadata_RegisterFar(void *state, s32 id);
void Sprite_RemoveAnimationFar(void *state, s32 handle);

#include "battle_type.h"
#include "field/sprite.h"

struct BattleSortedUnitEntry {
  u16 unitId;
  u16 unk02;
  s16 value;
  s16 width;
  s16 mode;
  s16 priority;
  u8 unk0c[4];
};

struct BattleObjectAnimation {
  u8 unk00[0x28];
  struct AnimationEntry *entry;
};

struct SnapshotVitals {
  u16 hp;
  u16 pp;
};

struct Snapshot {
  u8 listed[8];
  u8 tags[3][4];
  u8 unk14[0x80];
  struct SnapshotVitals vitals[5];
};

struct SnapshotUnit {
  u8 unk00[0x38];
  s16 hp;
  u16 unk3a;
  u8 unk3c[0xf0];
  u8 tags[4];
  u8 snapshot[25];
};

struct SnapshotCopy {
  u8 bytes[25];
};

struct DebugPartyEntry {
  s8 owner;
  u8 level;
  s8 djinnCounts[4];
  u16 items[6];
  u16 actions[2];
  u16 unk16;
};

struct LinkOfferEntry {
  u8 unk00[2];
  u8 unitId;
  u8 unk03;
};

struct LinkOfferList {
  struct LinkOfferEntry entries[72];
  s32 count;
};

struct LinkOfferTable {
  u8 unk00[8];
  struct LinkOfferList list;
};

struct SlotEffectObject {
  u8 unk00[0x50];
  void *resource;
  u8 kind;
};

struct SlotEffectEntry {
  struct SlotEffectObject *object;
};

struct SlotMotionRecord {
  u8 unk00[12];
  s32 height;
};

struct StatusIconRecord {
  u8 unk00[0x14];
  u8 width;
  u8 unk15[4];
  u8 dirty;
};

struct StatusIconEntry {
  u8 unk00[6];
  u8 priority;
};

struct TlaSpriteRecord {
  struct AnimationSpritePart image;
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 unk15[4];
  u8 dirty;
  u8 flags;
  u8 unk1b[13];
  struct SpriteEntry *entries[1];
};

struct AnimationEntryGroup {
  u8 unk00[0x28];
  struct AnimationEntry *entries[4];
};

struct MotionRecordScale {
  u8 unk00[0xc];
  s32 scale;
};

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

struct BattleSpriteSlots {
  s32 enemyGroup;
  s16 spriteSlots[6];
};

struct ActionTargets {
  u8 actorId;
  u8 count;
  u8 unk02;
  u8 ids[14];
  u8 stateA[14];
  u8 stateB[14];
  u8 stateC[14];
  u8 stateD[14];
  u16 value4a;
  s32 abilityId;
  s32 value50;
  s32 value54;
  s32 presentationFlags;
  s32 unk5c;
  s32 extra;
  s32 unk64;
};

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

struct EscapeTransition {
  s32 targetYaw;
  s32 unk04[3];
  s32 active;
};

struct BattleEscapeState {
  u8 reserved00[0x45];
  u8 guaranteed;
  u8 failedAttempts;
};

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

extern struct Snapshot Data_0200ff58;

extern struct SnapshotCopy Data_0200ff6c[];

extern struct SnapshotVitals Data_0200ffec[];

extern u8 gScriptedBattle_Units[];

extern struct DebugPartyEntry gDebug_ParameterSets[];

// A knocked-back unit's motion by variant (BattleMotion_Knockback), as The
// Broken Seal's.
extern const s32 gBattleKnockback_Acceleration[]; ///< acceleration

extern const s32 gBattleKnockback_SpeedLimit[]; ///< speed limit

extern const s32 gBattleKnockback_VelocityY[]; ///< upward velocity

extern const s32 gBattleKnockback_DistancePercent[]; ///< distance, in percent
                                                     ///< of the anchor's x

extern const s32 gBattleCamera_CommandMatrix[];

extern const s32 gBattleObject_EmptyScript[];

extern const s8 gBattleParty_CenterOrderOffsets[];

extern const u16 gData_081287d4[];

extern const u16 gData_081287e2[];

extern const u16 gData_081287f0[];

extern const u16 gBattleUnit_WeaponAnimationsIsaac[];

extern const u16 gBattleUnit_WeaponAnimationsGaret[];

extern const u16 gBattleUnit_WeaponAnimationsIvan[];

extern const u16 gBattleUnit_WeaponAnimationsMia[];

extern const u16 gBattleUnit_WeaponAnimationsJenna[];

extern struct BattlePresentationTransition *gTransitionWork;

u32 BattleSprite_GetAttackHop(u32 index);
u32 BattleSprite_GetEffectHeight(u32 index);
s32 BattleMessage_WaitForAdvanceAt(s32 x, s32 y);

#endif
