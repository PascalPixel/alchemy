#ifndef ALCHEMY_ABILITY_STAGING_H
#define ALCHEMY_ABILITY_STAGING_H

// How each ability is staged in battle: the staging and effect tables (ability_staging.c) and the
// values of BATTLE_ACTION_RUN.staging and battleEffect.

#include "edition.h"
#include "battle/battle_action_run.h"
#include "battle/battle_state.h"
#include "battle/playback_queue.h"
#include "game/item.h"
#include "stubs.h"
#include "system.h"

// Each ability's staging bits (BATTLE_ACTION_RUN.battleEffect), by ability id 0 to 518
// (ability_staging.c).
extern const Uint32 AbilityBattleEffects[];
// Each ability's outcome staging (BATTLE_ACTION_RUN.staging), 0 for the default, by ability id 0 to
// 517 (ability_staging.c).
extern const Uint8  AbilityStaging[];

// BATTLE_ACTION_RUN.staging: how btStagingDispatchAction shows the action, set by btActionRunBuild
// from the ability (AbilityStaging where it gives one).
typedef enum BATTLE_STAGING
{
    STAGING_MESSAGES    = 1, // no effect: each target's messages only
    STAGING_ATTACK      = 2, // the acting unit runs up and strikes
    STAGING_CAST        = 3, // Psynergy, and Venus, Mercury, Mars, Jupiter
    STAGING_ITEM        = 4,
    STAGING_SUMMON      = 5, // the larger summons, the acting unit hidden
    STAGING_DJINNI      = 6, // a Djinni with no attack (Granite, Quartz...)
    STAGING_FLEE        = 7, // a unit runs
    // The action plays as a cast, the caster showing its element's streaks in place of the
    // gathering star field. Where AbilityStaging gives no staging, it's how an enemy's ability
    // plays when the ability isn't Psynergy (btAbilityIsPsynergy) and its entry of
    // AbilityBattleEffects isn't 0; an enemy's Psynergy plays as STAGING_CAST. AbilityStaging gives
    // it to Slice and to seven weapon unleashes: Blizzard, Drown, Demon Fire, Vorpal Slash, Rapid
    // Smash, Sonic Smash and Titan Blade.
    STAGING_ENEMY_CAST  = 8,
    STAGING_FULL_SCREEN = 9 // Gaia, Rockfall, Procne and their kin
} BATTLE_STAGING;

// BATTLE_ACTION_RUN.battleEffect: the effect kind in the low twelve bits and its strength (a double
// or triple blow) above them; then how the effect is staged.
#define EFFECT_ID_MASK        0xfff
#define EFFECT_VARIANT_MASK   0x3000
#define EFFECT_VARIANT_SHIFT  12
#define EFFECT_BY_ID_RANGE    0x4000  // btEffectRunByIdRangeStub
#define EFFECT_SUMMON         0x8000  // btEffectRunSummonStub, acting unit's view
#define EFFECT_ON_DOWNED      0x10000 // shown on downed targets too (revivals)
// Plays the effect on the side opposite its targets. Unused: no entry of AbilityBattleEffects in
// either game sets it and no code adds it, so the flip in btStagingRunFullScreen never happens.
#define EFFECT_OTHER_SIDE     0x20000

// The effect kind Severe Blow alone has: the status window drops its PP row while it plays.
#define EFFECT_ID_SEVERE_BLOW 135

// More of BATTLE_ACTION_RUN.battleEffect: the camera takes the other side's view (an enemy's cast a
// view of its own), or turns right round.
#define EFFECT_VIEW_ACROSS    0x40000
#define EFFECT_VIEW_REVERSED  0x80000

#endif
