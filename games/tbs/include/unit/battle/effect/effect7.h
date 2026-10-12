#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT7_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT7_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect2.h"
#include "unit/battle/effect/effect3.h"
#include "unit/battle/effect/effect5.h"
#include "unit/battle/effect/effect6.h"

struct BattleEffectArgument;

void BattleEffect_RunDitherDissolveScene(struct BattleEffectArgument *effect);
void BattleFx_InitializeMode10(struct BattleEffectArgument *effect);
void BattleFx_RunIceShardBursts(struct BattleEffectArgument *effect);
void BattleFx_RunGatheringMotes(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode2(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode3Or4(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode4(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode5(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesAMode6(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesA(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunSeriesBMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode2(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode3(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode5(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesBMode4(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesB(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunPaletteRampMode0(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteRampMode1(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteRampMode2(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteRampMode3(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteRamp(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunPuffArc(struct BattleEffectArgument *efx);
s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *win, s16 *out);
void BattleFx_RunRockWall(struct BattleEffectArgument *effect);
void BattleFx_RunHomingEmbers(struct BattleEffectArgument *effect);
void BattleFx_RunRisingMotes(struct BattleEffectArgument *effect);
void BattleFx_RunClosingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldMode1(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldMode0(struct BattleEffectArgument *effect);
void BattleFx_RunParticleField(struct BattleEffectArgument *effect, s32 mode);

#endif
