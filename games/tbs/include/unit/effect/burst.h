#ifndef ALCHEMY_UNIT_EFFECT_BURST_H
#define ALCHEMY_UNIT_EFFECT_BURST_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/effect/effect.h"
#include "unit/effect/fxdirect.h"
#include "unit/effect/twopic.h"
#include "unit/effect/strike.h"
#include "unit/effect/dispatch.h"
#include "unit/effect/scene.h"

struct BattleEffectArgument;
struct EffectStep;

void BattleFx_SpawnObjects(s32 entry_count, s32 kind, u32 variant);
void BattleFx_RunNoEffect(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_RunSeriesIMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode2(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode6(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode3(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode5(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode7(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode4(struct BattleEffectArgument *effect);
void BattleEffect_RunBurstShower(struct BattleEffectArgument *efx, s32 mode);
void BattleFx_RunSwirlingStars(void *object);
void BattleFx_RunOrbitingSparks(void);
void BattleFx_RunTargetBursts(void *object);
void BattleEffect_RunStagedParticles(struct BattleEffectArgument *effect);
void BattleFx_RunDualTableMode0(struct BattleEffectArgument *effect);
void BattleFx_RunDualTableMode1(struct BattleEffectArgument *effect);
void BattleFx_RunDualTable(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunRisingColumns(struct BattleEffectArgument *effect);
void BattleFx_RaisePillars(struct BattleEffectArgument *effect);
void BattleFx_RunLightningBolts(struct BattleEffectArgument *effect);
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind, s32 side, s32 anchor, s32 *out_x, s32 *out_y);
void BattleFx_RunMode0(void *effect);
void BattleFx_RunMode1(void *effect);
void BattleFx_RunMode2(void *effect);
void BattleFx_RunMode3(void *effect);
void BattleFx_RunMode11(void *effect);
void BattleFx_RunMode4(void *effect);
void BattleEffect_RunMode5WithAction(void *effect);
void BattleFx_RunMode6WithAction(void *effect);
void BattleFx_RunMode7(void *effect);
void BattleFx_RunMode10(void *effect);
void BattleFx_RunMode9WithAction(void *effect);
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_InitializeRisingObject(s32 id);
void BattleFx_SetApproachMotion(s32 id, s32 tgt_id, s32 frames);
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 unused);
void BattleFx_RunParticleFieldVariant0(s32 effect);
void BattleFx_RunParticleFieldVariant1(s32 effect);
void BattleFx_RunParticleFieldVariant2(s32 effect);
void BattleFx_RunParticleFieldVariant3(s32 effect);
void BattleFx_RunParticleFieldVariant(struct BattleEffectArgument *effect, s32 variant);
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h);
void BattleFx_RunMercuryDjinnFlames(struct BattleEffectArgument *effect);
void BattleFx_RunJupiterDjinnFall(struct BattleEffectArgument *effect);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal);
void BattleFx_RunVenusDjinnColumns(struct BattleEffectArgument *effect);
void BattleFx_RunMarsDjinnPuffs(struct BattleEffectArgument *effect);
void BattleFx_RunParticleReveal(struct BattleEffectArgument *effect);
void BattleFx_RunVenusDjinnRockToss(struct BattleEffectArgument *effect);
void Camera_ApplyShake(s32 random_mask, u32 shake_range);
void BattleFx_InitializeMode12(struct BattleEffectArgument *efx);
void BattleFx_RunShatteringRocks(struct BattleEffectArgument *object);
void EffectStep_RunAnimationMode6Or7Or8(struct EffectStep *step);
void EffectStep_RunAnimationMode0(struct EffectStep *step);
void EffectStep_RunAnimationMode0Alternate(struct EffectStep *step);
void EffectStep_RunAnimationMode1(struct EffectStep *step);
void EffectStep_RunAnimationMode2(struct EffectStep *step);
void EffectStep_RunAnimationMode3Or4Or5(struct EffectStep *step);
void BattlePres_RunBurstScene(struct BattleEffectArgument *effect, s32 variant);
void BattlePres_RunRisingWall(struct BattleEffectArgument *effect);

#endif
