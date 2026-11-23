#ifndef ALCHEMY_FAR_CALL_BATTLE_EFFECT_H
#define ALCHEMY_FAR_CALL_BATTLE_EFFECT_H

/// @file
/// The far calls into the battle effect bank, as the other banks' code calls
/// them.

#include "types.h"

struct BattlePresentationWork;
struct BattlePresentationWork;
void BattleEffect_RunByIdRangeFar(struct BattlePresentationWork *work);
/// Runs an ordinary battle effect from gBattleEffect_Handlers, with the sprite
/// sheet, work and canvas it lends.
void BattleEffect_RunFromTableFar(struct BattlePresentationWork *work);
void BattleEffect_RunElementPillarFar(struct BattlePresentationWork *work);
/// A glow on the canvas map widens over the user for 56 frames while 16
/// spinning rings rise around it, four abreast.
void BattleEffect_RunFormChangeFar(struct BattlePresentationWork *request,
                                   s32 value);
/// Sparks in an element's colours flare at four corners around a unit and
/// motes spiral in, then a burst of light plays over it.
void BattleEffect_RunElementBurstFar(struct BattlePresentationWork *launch);
/// Starts the element streaks, which then draw by themselves while the battle
/// runs on.
void BattleEffect_StartElementStreaksFar(s32 kind);
/// Ends the element streaks: removes their callbacks, clears the canvas in VRAM
/// and frees their heap blocks.
void BattleEffect_EndElementStreaksFar(void);
/// Loads a blitter for a 128 by 128 canvas into a heap slot, the flags and the
/// mode packed into one word: mode << 4 | flags.
s32 BattleEffect_LoadBlitterFar(s32 slot, s32 modeAndFlags);

#endif
