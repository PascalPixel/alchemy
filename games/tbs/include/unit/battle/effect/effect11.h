#ifndef ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT11_H
#define ALCHEMY_UNIT_BATTLE_EFFECT_EFFECT11_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect5.h"
#include "unit/battle/effect/effect6.h"
#include "unit/battle/effect/effect7.h"
#include "unit/battle/effect/effect8.h"
#include "unit/battle/effect/effect9.h"
#include "unit/battle/effect/effect10.h"

struct BattleEffectArgument;

void BattleFx_RunRipplingDisc(struct BattleEffectArgument *effect);
void ObjectGroup_StoreObjectAndRunStep7(s32 a0, s32 a1, s32 a2);
void BattleFx_RunFireSwirlCharge(struct BattleEffectArgument *effect);
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y);
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta);
void BattleFx_ArmPaletteHBlankDma(void);
void BattleFx_InitializeDefaultMode(struct BattleEffectArgument *efx);
void BattleFx_InitializeMode1(struct BattleEffectArgument *efx);
void BattleEffect_RunParticleStreams(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunCirclingFallingScene(struct BattleEffectArgument *effect);
void BattleEffect_RunDualParticleStream(struct BattleEffectArgument *effect);
void BattleFx_RunFireBurstShards(struct BattleEffectArgument *effect);
void BattleFx_RunFlameBlade(struct BattleEffectArgument *effect);
void BattleFx_RunFallingSword(struct BattleEffectArgument *effect);
void BattleEffect_RunShockwaveScene(struct BattleEffectArgument *effect);
void BattleFx_RunObjectRow(struct BattleEffectArgument *effect);
void Graphics_ResetBg2Pa(void);
void Graphics_SetBg2AffineScaleHalf(void);
void BattleFx_InitializeMode6(struct BattleEffectArgument *efx);
void BattleFx_RunSparkApproachVariant4(struct BattleEffectArgument *effect);
void BattleFx_RunSparkApproachVariant3(struct BattleEffectArgument *effect);
void BattleFx_RunRevealColumnMode1(s32 arg);
void BattleFx_RunRevealColumnMode2(s32 arg);
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant);
void BattleFx_RunRisingBurst(struct BattleEffectArgument *effect);
s32 BattleEffect_LoadWork(s32 slot, s32 width_shift, s32 height_shift, s32 flags, u32 mode);

#endif
