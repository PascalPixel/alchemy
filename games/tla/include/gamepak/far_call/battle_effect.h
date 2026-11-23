#ifndef ALCHEMY_FAR_CALL_BATTLE_EFFECT_H
#define ALCHEMY_FAR_CALL_BATTLE_EFFECT_H

/// @file
/// The far calls into the battle effect bank, as the other banks' code calls
/// them.

#include "types.h"

struct BattlePresentationWork;
/// Runs a battle effect from the second range of kinds: the casting impacts,
/// the plain attacks' beams and their critical hits.
void BattleEffect_RunByIdRangeFar(struct BattlePresentationWork *work);
/// Runs an ordinary battle effect from gBattleEffect_Handlers, with the sheet,
/// work and canvas it lends.
void BattleEffect_RunFromTableFar(struct BattlePresentationWork *work);
/// A pillar of light in an element's colours stands at a unit's feet while
/// motes spiral up around it.
void BattleEffect_RunElementPillarFar(struct BattlePresentationWork *work);
/// The Doom Dragon's change of form once a form is beaten; formation says which
/// (FORMATION_DOOM_DRAGON_A to C).
void BattleEffect_RunFormChangeFar(struct BattlePresentationWork *work,
                                   s32 formation);
/// Sparks in an element's colours flare at four corners around a unit and
/// motes spiral in, then a burst of light plays over it.
void BattleEffect_RunElementBurstFar(struct BattlePresentationWork *work);
/// Starts the element streaks, which then draw by themselves while the battle
/// runs on.
void BattleEffect_StartElementStreaksFar(s32 element);
/// Ends the element streaks: removes their tasks, clears their canvas in VRAM
/// and frees what they took.
void BattleEffect_EndElementStreaksFar(void);
/// Loads a blitter for a 128 by 128 canvas into a heap slot, the flags and the
/// mode packed into one word: mode << 4 | flags.
s32 BattleEffect_LoadBlitterFar(s32 slot, s32 modeAndFlags);

#endif
