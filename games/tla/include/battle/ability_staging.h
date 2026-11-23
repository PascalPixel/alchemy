#ifndef ALCHEMY_ABILITY_STAGING_H
#define ALCHEMY_ABILITY_STAGING_H

/// @file
/// How each ability is staged in battle: the staging and effect tables
/// (ability_staging.c) and the values of BattlePlan.staging and battleEffect.

#include "types.h"

/// The effect chance by distance BattleActionRun_Build rolls ahead for
/// Annihilation and Whorl, the same as gRangeFalloff_EffectChance.
extern const u8 gActionRun_EffectChance[];
/// Each ability's outcome staging (BattlePlan.staging), 0 for the default, by
/// ability id.
extern const u8 gAbilityStaging[];
/// Each ability's staging bits (BattlePlan.battleEffect), by ability id.
extern const u32 gAbilityBattleEffects[];

/// BattlePlan.staging: how BattleStaging_DispatchAction shows the action, set
/// by BattleActionRun_Build from the ability (gAbilityStaging where it gives
/// one).
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
  /// (BattlePlan.partnerId) strikes as the Djinni is unleashed.
  STAGING_TEAM_STRIKE = 10,
  STAGING_DJINN_RECOVERY = 11, ///< Eddy: "Speed up Djinn recovery time."
  STAGING_MEGAERA = 12,        ///< the summon, then High Impact on its side
  STAGING_IRIS = 13,           ///< the summon, then the Aurora Field
  /// Daedalus's strike queued for the turn's end, from no ability's row: only
  /// BattleStaging_PlanDaedalus sets it.
  STAGING_DAEDALUS = 14,
  /// The summoned beasts that strike: Wild Wolf, Roc, Orc, Grand Golem, Dinox
  /// and Call Zombie (BattleMotion_CallBeast).
  STAGING_BEAST_ATTACK = 15,
  /// The summoned beasts that cast, Emu to Call Dullahan.
  STAGING_BEAST_CAST = 16,
  /// Unused: a beast called up (BattleMotion_CallBeast takes 15 to 17) whose
  /// call then plays as the party's Psynergy cast (CAST_PSYNERGY). No
  /// ability's row in gAbilityStaging gives it.
  STAGING_BEAST_PSYNERGY = 17
};

/// BattlePlan.battleEffect: the effect kind in the low twelve bits and its
/// strength (a double or triple blow) above them; then how the effect is
/// staged.
#define EFFECT_ID_MASK 0xfff
#define EFFECT_VARIANT_MASK 0x3000
#define EFFECT_VARIANT_SHIFT 12
#define EFFECT_BY_ID_RANGE 0x4000 ///< BattleEffect_RunByIdRangeFar
#define EFFECT_SUMMON 0x8000 ///< BattleEffect_RunSummonFar, acting unit's view
#define EFFECT_ON_DOWNED 0x10000 ///< shown on downed targets too (revivals)
/// Unused: no entry of gAbilityBattleEffects sets it and no code adds it, so
/// BattleStaging_HideField's flip never happens. It would play the effect on
/// the side opposite its targets.
#define EFFECT_OTHER_SIDE 0x20000
/// The camera takes the other side's view (an enemy's cast a view of its
/// own), or turns right round.
#define EFFECT_VIEW_ACROSS 0x40000
#define EFFECT_VIEW_REVERSED 0x80000
/// Unused: The Broken Seal's effect kind for Severe Blow, which no row of
/// gAbilityBattleEffects gives here; BattleStaging_RunAttack would play it with
/// the status window's PP row hidden (Severe Blow is
/// BATTLE_EFFECT_SEVERE_BLOW).
#define EFFECT_ID_SEVERE_BLOW 135

#endif
