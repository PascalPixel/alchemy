#ifndef ALCHEMY_BATTLE_COMMAND_H
#define ALCHEMY_BATTLE_COMMAND_H

/// @file
/// The battle command names: auto battle and the battle options (aliases of
/// debug and key globals), and the actions' flag and status tables.

#include "battle_system.h"
#include "item.h"
#include "keystate.h"
#include "frame.h"
#include "battle_work.h"

/// gDebugMode under the battle's name. While it and FLAG_DEBUG_BATTLE_KEYS are
/// set, holding R as an action is planned defeats every enemy, or with Select
/// the party (BattleActionRun_Build).
#define BATTLE_AUTO_MODE gDebugMode
/// gKeysHeld under the battle's name: the keys held this frame.
#define BATTLE_OPTIONS gKeysHeld

/// Each ability's staging bits (BattleActionRun.battleEffect), by ability id
/// 0 to 518 (ability_staging.c).
extern const u32 gAbilityBattleEffects[];
/// Each ability's outcome staging (BattleActionRun.staging), 0 for the default,
/// by ability id 0 to 517 (ability_staging.c).
extern const u8 gAbilityStaging[];

/// BattleActionRun.staging: how BattleStaging_DispatchAction shows the
/// action, set by BattleActionRun_Build from the ability
/// (gAbilityStaging where it gives one).
enum BattleStaging {
  STAGING_MESSAGES = 1, ///< no effect: each target's messages only
  STAGING_ATTACK = 2,   ///< the acting unit runs up and strikes
  STAGING_CAST = 3,     ///< Psynergy, and Venus, Mercury, Mars, Jupiter
  STAGING_ITEM = 4,     ///< an item
  STAGING_SUMMON = 5,   ///< the larger summons, the acting unit hidden
  STAGING_DJINNI = 6,   ///< a Djinni with no attack (Granite, Quartz...)
  STAGING_FLEE = 7,     ///< a unit runs
  /// The action plays as a cast, the caster showing its element's streaks in
  /// place of the gathering star field. Where gAbilityStaging gives no
  /// staging, it's how an enemy's ability plays when the ability isn't Psynergy
  /// (Ability_IsPsynergy) and its entry of gAbilityBattleEffects isn't 0; an
  /// enemy's Psynergy plays as STAGING_CAST. gAbilityStaging gives it to Slice
  /// and to seven weapon unleashes: Blizzard, Drown, Demon Fire, Vorpal Slash,
  /// Rapid Smash, Sonic Smash and Titan Blade.
  STAGING_ENEMY_CAST = 8,
  STAGING_FULL_SCREEN = 9 ///< Gaia, Rockfall, Procne and their kin
};

// BattleActionRun.battleEffect: the effect kind in the low twelve bits and
// its strength (a double or triple blow) above them; then how the effect is
// staged.
#define EFFECT_ID_MASK 0xfff
#define EFFECT_VARIANT_MASK 0x3000
#define EFFECT_VARIANT_SHIFT 12
#define EFFECT_BY_ID_RANGE 0x4000 ///< BattleEffect_RunByIdRangeFar
#define EFFECT_SUMMON 0x8000 ///< BattleEffect_RunSummonFar, acting unit's view
#define EFFECT_ON_DOWNED 0x10000 ///< shown on downed targets too (revivals)
/// Plays the effect on the side opposite its targets. Unused: no entry of
/// gAbilityBattleEffects in either game sets it and no code adds it, so the
/// flip in BattleStaging_HideField never happens.
#define EFFECT_OTHER_SIDE 0x20000

/// The effect kind Severe Blow alone has: the status window drops its PP row
/// while it plays.
#define EFFECT_ID_SEVERE_BLOW 0x87

/// More of BattleActionRun.battleEffect: the camera takes the other side's
/// view (an enemy's cast a view of its own), or turns right round.
#define EFFECT_VIEW_ACROSS 0x40000
#define EFFECT_VIEW_REVERSED 0x80000

#endif
