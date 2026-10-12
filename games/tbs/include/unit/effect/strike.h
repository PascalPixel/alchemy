#ifndef ALCHEMY_UNIT_EFFECT_STRIKE_H
#define ALCHEMY_UNIT_EFFECT_STRIKE_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_efx.h"
#include "unit/effect/effect.h"
#include "unit/effect/fxdirect.h"
#include "unit/effect/twopic.h"

struct BattleEffectArgument;

void BattleFx_RunLeapingStrike(struct BattleEffectArgument *effect);
void BattleFx_RunClosingSkulls(struct BattleEffectArgument *effect);
void BattleFx_RunMemberOrbit(void *effect);
void BattleFx_RunMemberBurstMode0(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBurstMode1(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBurstMode2(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBurst(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_FetchRectangleBlitters(s32 flip, BattleEffectDrawRectangle *out);
void BattleFx_RunFortyEightFrameMode1(struct BattleEffectArgument *effect);
void BattleFx_RunFortyEightFrameMode0(struct BattleEffectArgument *effect);
void BattleFx_RunFortyEightFrameMode2(struct BattleEffectArgument *effect);
void BattleFx_RunFortyEightFrameEffect(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunTwoModeAMode0(struct BattleEffectArgument *effect);
void BattleFx_RunTwoModeAMode1(struct BattleEffectArgument *effect);
void BattleFx_RunMemberBeam(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunSevenModeMode4(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode5(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode1(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode6(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunSevenModeMode3(struct BattleEffectArgument *effect);
void BattleFx_RunSevenMode(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunCounterReveal(struct BattleEffectArgument *object);
void Graphics_ConvertBackgroundToBlueRamp(void);
void BattleFx_RunSpinningTriangle(struct BattleEffectArgument *effect);
void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect);
void BattleFx_RunRingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunSpinningStars(struct BattleEffectArgument *effect);
void BattleFx_RunWhirlwindComets(struct BattleEffectArgument *argument);
void BattleFx_RenderAnimationMode1(s32 effect);
void BattleFx_RenderAnimationMode0(s32 effect);
void BattleEffect_RunPaletteParticles(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect);
void BattleFx_RunVortexMotes(struct BattleEffectArgument *effect);
void BattleFx_RunTornado(struct BattleEffectArgument *effect);
void BattleFx_RunFallingCrystals(struct BattleEffectArgument *effect);
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase);
void BattleFx_RunLightningPillars(struct BattleEffectArgument *effect);
void BattleFx_RunTwoModeBMode0(s32 effect);
void BattleFx_RunTwoModeBMode1(s32 effect);
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 kind);
void BattleFx_RunFirePillars(struct BattleEffectArgument *effect);
void BattleFx_RenderMode0Or1(struct BattleEffectArgument *effect);
void BattleFx_RenderMode1(struct BattleEffectArgument *effect);
void BattleFx_RenderMode2(struct BattleEffectArgument *effect);
void BattleFx_RenderMode3(struct BattleEffectArgument *effect);
void BattleFx_RenderMode4(struct BattleEffectArgument *effect);
void BattleFx_RenderMode5(struct BattleEffectArgument *effect);
void BattleFx_RenderMode7(struct BattleEffectArgument *effect);
void BattleFx_RenderMode6(struct BattleEffectArgument *effect);
void BattleFx_RenderMode(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunBouncingSheep(void *object);
void BattleFx_RunTargetMarkers(struct BattleEffectArgument *efx);
void BattleFx_RunSparkTravel(struct BattleEffectArgument *effect);

#endif
