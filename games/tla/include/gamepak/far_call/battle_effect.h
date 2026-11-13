#ifndef ALCHEMY_FAR_CALL_BATTLE_EFFECT_H
#define ALCHEMY_FAR_CALL_BATTLE_EFFECT_H

/// @file
/// The far calls into the battle effect bank, as the other banks' code calls
/// them.

#include "types.h"

struct BattleFxRequest;
struct BattlePresentationWork;
void BattleEffect_DispatchByIdRangeFar(struct BattlePresentationWork *work);
void BattleEffect_RunFromTableFar(struct BattlePresentationWork *work);
void BattleEffect_RunElementPillarFar(struct BattlePresentationWork *work);
void Func_08138050(struct BattleFxRequest *request, s32 value);

#endif
