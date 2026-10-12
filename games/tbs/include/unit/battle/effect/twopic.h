#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_TWOPIC_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_TWOPIC_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/fxdirect.h"

struct BattleEffectArgument;

void BattleFx_RunTwoResourceMode0(struct BattleEffectArgument *efx);
void BattleFx_RunTwoResourceMode1(struct BattleEffectArgument *efx);
void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode);
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *efx);

#endif
