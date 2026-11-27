#ifndef ALCHEMY_BATTLE_EFFECT_PARAMS_H
#define ALCHEMY_BATTLE_EFFECT_PARAMS_H

// The command a battle effect is run with: its kind, its user and its targets.

#include "types.h"
#include "static_assert.h"

// BATTLE_EFFECT_PARM.side: the side the targets are on.
typedef enum BATTLE_EFFECT_SIDE
{
    EFFECT_SIDE_ENEMIES,
    EFFECT_SIDE_PARTY
} BATTLE_EFFECT_SIDE;

// What a critical hit adds to an attack's effect kind: btStagingRunAttack plays the critical
// version of the blow, as The Broken Seal's does.
#define EFFECT_ID_CRITICAL_OFFSET 200

// The command shared by an effect's caller, dispatcher and handlers.
typedef struct BATTLE_EFFECT_PARM
{
    Sint32 kind;        // the effect's index in the handler table
    Sint32 side;        // 1 when the targets are the party, 0 for enemies
    Sint32 user;        // the user's unit id
    Sint32 firstTarget; // the action's first target
    Sint32 range;       // forwarded action range, otherwise initialized to 1
    Sint32 targetCount; // how many of targets are in use
    Sint32 variant;     // the ability's size, from 0
    Sint32 prelude;     // 1 plays a prelude: in some effects a Djinni appears
    Sint32 unused;      // named by no code in either game
    Sint16 targets[8];  // the targets' unit ids, targetCount of them
} BATTLE_EFFECT_PARM;

STATIC_ASSERT(sizeof(BATTLE_EFFECT_PARM) == 52);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_PARM, targetCount) == 20);
STATIC_ASSERT(offsetof(BATTLE_EFFECT_PARM, targets) == 36);

// Loops `i` over the effect's targets, effect->targets[0] to effect->targets[effect->targetCount -
// 1]: the loops that flash, shake, strike or aim at each target in turn.
#define EFFECT_FOR_EACH_TARGET(i, effect) for ((i) = 0; (i) != (effect)->targetCount; (i)++)

#endif
