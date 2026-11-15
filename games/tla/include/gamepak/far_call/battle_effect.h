#ifndef ALCHEMY_FAR_CALL_BATTLE_EFFECT_H
#define ALCHEMY_FAR_CALL_BATTLE_EFFECT_H

/// @file
/// The far calls into the battle effect bank, as the other banks' code calls
/// them.

#include "types.h"

struct BattleFxRequest;
struct BattlePresentationWork;
void BattleEffect_RunByIdRangeFar(struct BattlePresentationWork *work);
/// Runs an ordinary battle effect from gBattleEffect_Handlers, with the sheet,
/// work and canvas it lends.
void BattleEffect_RunFromTableFar(struct BattlePresentationWork *work);
void BattleEffect_RunElementPillarFar(struct BattlePresentationWork *work);
/// A glow on the canvas map widens over the user for 56 frames while 16
/// spinning rings rise around it, four abreast.
void BattleEffect_RunFormChangeFar(struct BattleFxRequest *request, s32 value);

#endif
