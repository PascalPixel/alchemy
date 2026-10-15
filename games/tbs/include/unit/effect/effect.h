#ifndef ALCHEMY_UNIT_EFFECT_EFFECT_H
#define ALCHEMY_UNIT_EFFECT_EFFECT_H

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"

struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;

typedef struct {
  s32 reserved00[3];
  s32 currentValue;
  s32 indexRadius;
  s32 entryLimit;
  s32 reserved18[3];
  s16 values[1];
} BattleEffectIndexWindow;

void BattlePres_ConfigureEffectDisplay(void);
void BattleFx_AdvanceScrollOnInterval(void);
void Camera_AdvanceBg2Reference(void);
void BattleFx_ArmWin0HBlankDma(void);
void BattleEffect_RunFallingParticles(struct BattleEffectArgument *effect);
void BattleEffect_RunFallingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunEmberStreaks(struct BattleEffectArgument *effect);
void BattleFx_RunLimeStreaks(struct BattleEffectArgument *effect);
void BattleFx_RunVioletStreaks(struct BattleEffectArgument *effect);
void BattleFx_RunCyanStreaks(struct BattleEffectArgument *effect);
void BattleFx_RunRedStreaks(struct BattleEffectArgument *effect);
void BattleFx_RunLimeStreaksAgain(struct BattleEffectArgument *effect);
void BattleFx_RunStreakVolley(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunDriftingMoteFountain(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMoteFountain(struct BattleEffectArgument *effect);
void BattleFx_RunMoteFountain(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunLightningGlintSlash(struct BattleEffectArgument *effect);
void BattleFx_RunEmberDrainColumns(struct BattleEffectArgument *effect);
void BattleFx_RunLimeSmokeColumn(struct BattleEffectArgument *effect);
void BattleFx_RunMarsSmokeColumn(struct BattleEffectArgument *effect);
void BattleFx_RunLightningGlintColumns(struct BattleEffectArgument *effect);
void BattleFx_RunLimeSmokeSlash(struct BattleEffectArgument *effect);
void BattleFx_RunMarsDrainColumns(struct BattleEffectArgument *effect);
void BattleFx_RunBlueSparkColumns(struct BattleEffectArgument *effect);
void BattleFx_RunHeavyMarsSlash(struct BattleEffectArgument *effect);
void BattleFx_RunLightningSparkSlashes(struct BattleEffectArgument *effect);
void BattleFx_RunPinkSmokeColumns(struct BattleEffectArgument *effect);
void BattleFx_RunLargeDrainColumns(struct BattleEffectArgument *effect);
void BattleFx_RunTargetStrike(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunFlippingBurst(struct BattleEffectArgument *effect);
void BattleFx_RunBladeRain(struct BattleEffectArgument *effect);

// 粒の置き場：Ram_MapCellBuffer を struct EffectStep の並びとして使う
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y) // 上下左右ではみ出しを切る

#define HI(v) (((s16 *)&(v))[1])

struct Placement {
  s32 x;
  s32 y;
  s32 z;
  s32 unk0c;
};

// 2 次元の組 {x, y}（位置や速さ）
struct Point2D {
  s32 x;
  s32 y;
};

// 演出の描画ルーチンの組：[0] は HEAP_SLOT_BLITTER、[1] は
// HEAP_SLOT_BLITTER_ALTERNATE に BattleEffect_LoadWork
// が作る。演出の多くがこの組をフレームに置く
typedef DrawRectangle BlitterPair[2];

#define gBlitters ((DrawRectangle *)gWorkSlot)

void BattleFx_RunIceFleckBurst(struct BattleEffectArgument *effect);
void BattleEffect_RunTileAndPaletteAnimation(
    struct BattleEffectArgument *effect);
void BattleFx_RunGatheringBurst(struct BattleEffectArgument *effect);
void BattlePresentation_DrawStreaks(void);
void BattlePresentation_PrepareScene(s32 kind);
void BattleFx_ScheduleCallbacksAndReleaseBlocks(void);

void BattleFx_RunLimeSwipe(struct BattleEffectArgument *effect);
void BattleFx_RunLaunchingSwipe(struct BattleEffectArgument *effect);
void BattleFx_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *effect);

void BattleEffect_WipeCanvas(s32 mode, s32 value);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_FlushPendingGraphicsTransfer(void);
void Graphics_ApplyWindowBlendRegisters(void);
void Display_ApplyBg2Reference(void);
void Palette_StepFadeTransfer(void);
void BattleFx_ClearActorTimers(void);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_BeginCanvasLayer(s32 bg_control);
void BattleFx_OpenCanvasLayer(s32 bg_control);
void BattleFx_BeginTiledCanvas(s32 bg_control);
void BattleFx_EndCanvasLayer(void);
void BattleFx_SetTransitionFlagAndDisplay(void);
void BattleFx_SetupCanvasTileMap(void);
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

void BattleFx_RunLeapingStrike(struct BattleEffectArgument *effect);
void BattleFx_RunClosingSkulls(struct BattleEffectArgument *effect);
void BattleFx_RunOrbitingSpirals(void *effect);
void BattleFx_RunPinkGatheringLights(struct BattleEffectArgument *effect);
void BattleFx_RunMarsGatheringLights(struct BattleEffectArgument *effect);
void BattleFx_RunMercuryGatheringLights(struct BattleEffectArgument *effect);
void BattleFx_RunGatheringLights(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_FetchRectangleBlitters(s32 flip, BlitterPair out);
void BattleFx_RunWideWall(struct BattleEffectArgument *effect);
void BattleFx_RunShakingWall(struct BattleEffectArgument *effect);
void BattleFx_RunWallOfFire(struct BattleEffectArgument *effect);
void BattleFx_RunMirroredWall(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunVenusDjinnBeams(struct BattleEffectArgument *effect);
void BattleFx_RunMercuryDjinnBeams(struct BattleEffectArgument *effect);
void BattleFx_RunDjinnBeams(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunGoldShardShells(struct BattleEffectArgument *effect);
void BattleFx_RunGreenShells(struct BattleEffectArgument *effect);
void BattleFx_RunBlueShells(struct BattleEffectArgument *effect);
void BattleFx_RunWebShells(struct BattleEffectArgument *effect);
void BattleFx_RunMarsShells(struct BattleEffectArgument *effect);
void BattleFx_RunLimeShells(struct BattleEffectArgument *effect);
void BattleFx_RunGreenShardShells(struct BattleEffectArgument *effect);
void BattleFx_RunLobbedShells(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunShatteringPanel(struct BattleEffectArgument *object);
void Graphics_ConvertBackgroundToBlueRamp(void);
void BattleFx_RunSpinningTriangle(struct BattleEffectArgument *effect);
void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect);
void BattleFx_RunRingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunSpinningStars(struct BattleEffectArgument *effect);
void BattleFx_RunWhirlwindComets(struct BattleEffectArgument *argument);
void BattleFx_RunSideEmbers(s32 effect);
void BattleFx_RunFormationEmbers(s32 effect);
void BattleEffect_RunPaletteParticles(struct BattleEffectArgument *effect,
                                      s32 mode);
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect);
void BattleFx_RunVortexMotes(struct BattleEffectArgument *effect);
void BattleFx_RunTornado(struct BattleEffectArgument *effect);
void BattleFx_RunFallingCrystals(struct BattleEffectArgument *effect);
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase,
                                 s32 blue_phase);
void BattleFx_RunLightningPillars(struct BattleEffectArgument *effect);
void BattleFx_RunSparkBursts(s32 effect);
void BattleFx_RunOrangeSparkBursts(s32 effect);
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
void BattleFx_RunTargetMarkers(struct BattleEffectArgument *effect);
void BattleFx_RunDjinnHeal(struct BattleEffectArgument *effect);

void Camera_ApplyPhasedDelta(void);
void BattleFx_RunNoEffectFrames(s32 cnt);
void BattleFx_InitializeMode(s32 *arg);
void BattleFx_DispatchMode(s32 *arg);
void BattleFx_BuildWindowEdgeTable(void);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *arg);
void BattleEffect_SetupBlendedDisplay(void);
void ObjectGroup_UpdateMembers(s32 set_id, s32 object_value, s32 group_value,
                               s32 state_slot, s32 state_value);
void BattleFx_InitTilemapAndFlushQueue(void);

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
void BattleFx_RunPuffArc(struct BattleEffectArgument *effect);
s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *win, s16 *out);
void BattleFx_RunRockWall(struct BattleEffectArgument *effect);
void BattleFx_RunHomingEmbers(struct BattleEffectArgument *effect);
void BattleFx_RunRisingMotes(struct BattleEffectArgument *effect);
void BattleFx_RunClosingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldMode1(struct BattleEffectArgument *effect);
void BattleFx_RunParticleFieldMode0(struct BattleEffectArgument *effect);
void BattleFx_RunParticleField(struct BattleEffectArgument *effect, s32 mode);

void BattleFx_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void BattleFx_RunNoEffect(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_RunSeriesIMode0(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode2(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode6(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode3(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode5(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode7(struct BattleEffectArgument *effect);
void BattleFx_RunSeriesIMode4(struct BattleEffectArgument *effect);
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);
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
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind,
                                  s32 side, s32 anchor, s32 *out_x, s32 *out_y);
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
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleFx_InitializeRisingObject(s32 id);
void BattleFx_SetApproachMotion(s32 id, s32 targetId, s32 frames);
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 unused);
void BattleFx_RunParticleFieldVariant0(s32 effect);
void BattleFx_RunParticleFieldVariant1(s32 effect);
void BattleFx_RunParticleFieldVariant2(s32 effect);
void BattleFx_RunParticleFieldVariant3(s32 effect);
void BattleFx_RunParticleFieldVariant(struct BattleEffectArgument *effect,
                                      s32 variant);
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h);
void BattleFx_RunMercuryDjinnFlames(struct BattleEffectArgument *effect);
void BattleFx_RunJupiterDjinnFall(struct BattleEffectArgument *effect);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal);
void BattleFx_RunVenusDjinnColumns(struct BattleEffectArgument *effect);
void BattleFx_RunMarsDjinnPuffs(struct BattleEffectArgument *effect);
void BattleFx_RunParticleReveal(struct BattleEffectArgument *effect);
void BattleFx_RunVenusDjinnRockToss(struct BattleEffectArgument *effect);
void Camera_ApplyShake(s32 random_mask, u32 shake_range);
void BattleFx_InitializeMode12(struct BattleEffectArgument *effect);
void BattleFx_RunShatteringRocks(struct BattleEffectArgument *object);
void EffectStep_RunAnimationMode6Or7Or8(struct EffectStep *step);
void EffectStep_RunAnimationMode0(struct EffectStep *step);
void EffectStep_RunAnimationMode0Alternate(struct EffectStep *step);
void EffectStep_RunAnimationMode1(struct EffectStep *step);
void EffectStep_RunAnimationMode2(struct EffectStep *step);
void EffectStep_RunAnimationMode3Or4Or5(struct EffectStep *step);
void BattlePres_RunBurstScene(struct BattleEffectArgument *effect, s32 variant);
void BattlePres_RunRisingWall(struct BattleEffectArgument *effect);

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping,
                                     s32 gravity);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping,
                                     s32 gravity);
s32 EffectPosition_ApplyBaseAndYOffset(s32 *point,
                                       struct EffectPosition *position);
void EffectPosition_ApplyAnimationAndYOffset(s32 id,
                                             struct EffectPosition *position);
void EffectPosition_ApplyStepAndYOffset(s32 id,
                                        struct EffectPosition *position);
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position);
s32 BattleUnit_ProjectToScreen(s32 unit, s32 *screen);
void BattleFx_ProbeKeysForExtendedActor(void);

void BattleFx_DispatchByIdRange(struct BattleEffectArgument *effect);
void BattlePres_RunBeamSequence(struct BattleEffectArgument *effect);
void BattlePres_RunRingAndSparkScene(struct BattleEffectArgument *effect);
void BattleFx_StepPaletteToResource(s32 resourceId);
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind);

void BattleFx_RunRipplingDisc(struct BattleEffectArgument *effect);
void ObjectGroup_StoreObjectAndRunStep7(s32 a0, s32 a1, s32 a2);
void BattleFx_RunFireSwirlCharge(struct BattleEffectArgument *effect);
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y);
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta);
void BattleFx_ArmPaletteHBlankDma(void);
void BattleFx_InitializeDefaultMode(struct BattleEffectArgument *effect);
void BattleFx_InitializeMode1(struct BattleEffectArgument *effect);
void BattleEffect_RunParticleStreams(struct BattleEffectArgument *effect,
                                     s32 mode);
void BattleEffect_RunCirclingFallingScene(struct BattleEffectArgument *effect);
void BattleEffect_RunDualParticleStream(struct BattleEffectArgument *effect);
void BattleFx_RunFireBurstShards(struct BattleEffectArgument *effect);
void BattleFx_RunFlameBlade(struct BattleEffectArgument *effect);
void BattleFx_RunFallingSword(struct BattleEffectArgument *effect);
void BattleEffect_RunShockwaveScene(struct BattleEffectArgument *effect);
void BattleFx_RunObjectRow(struct BattleEffectArgument *effect);
void Graphics_ResetBg2Pa(void);
void Graphics_SetBg2AffineScaleHalf(void);
void BattleFx_InitializeMode6(struct BattleEffectArgument *effect);
void BattleFx_RunSparkApproachVariant4(struct BattleEffectArgument *effect);
void BattleFx_RunSparkApproachVariant3(struct BattleEffectArgument *effect);
void BattleFx_RunRevealColumnMode1(s32 arg);
void BattleFx_RunRevealColumnMode2(s32 arg);
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant);
void BattleFx_RunRisingBurst(struct BattleEffectArgument *effect);
s32 BattleEffect_LoadWork(s32 slot, s32 width_shift, s32 height_shift,
                          s32 flags, u32 mode);

extern const s32 gRockWall_Heights[4];
extern const s32 gFormation_NormalScale[2];
extern const s32 gFormation_SmallScale[2];

#endif
