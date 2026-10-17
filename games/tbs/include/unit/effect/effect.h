#ifndef ALCHEMY_UNIT_EFFECT_EFFECT_H
#define ALCHEMY_UNIT_EFFECT_EFFECT_H

#include "types.h"
#include "render_placement.h"
#include "battle_effect_work.h"
#include "battle_effect.h"

struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;

typedef void (*BattleFxHandler)(struct BattleEffectArgument *effect);
extern BattleFxHandler gBattleFx_EffectHandlers[];

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

// Particle storage: reuse Ram_MapCellBuffer as an array of struct EffectStep.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

#define CLIP                                                                   \
  (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y) // Clip overflow on all four sides.

// Extract signed 16.16 pixels without reading an s32 through an s16 alias.
// The plain shift adds four bytes to native English effect.text overall.
#ifdef UBFIX
#define HI(v) ((s32)(v) >> 16)
#else
#define HI(v) (((s16 *)&(v))[1])
#endif

// Two-dimensional {x, y} pair for position or velocity.
struct Point2D {
  s32 x;
  s32 y;
};

// Effect renderer pair: BattleEffect_LoadWork creates [0] in
// HEAP_SLOT_BLITTER and [1] in HEAP_SLOT_BLITTER_ALTERNATE.
// Many effects keep this pair in their stack frame.
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
void BattleFx_RunOrbitingSpirals(struct BattleEffectArgument *effect);
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
void BattleFx_RunSideEmbers(struct BattleEffectArgument *effect);
void BattleFx_RunFormationEmbers(struct BattleEffectArgument *effect);
void BattleEffect_RunPaletteParticles(struct BattleEffectArgument *effect,
                                      s32 mode);
void BattleEffect_RunEmberColumns(struct BattleEffectArgument *effect);
void BattleFx_RunVortexMotes(struct BattleEffectArgument *effect);
void BattleFx_RunTornado(struct BattleEffectArgument *effect);
void BattleFx_RunFallingCrystals(struct BattleEffectArgument *effect);
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase,
                                 s32 blue_phase);
void BattleFx_RunLightningPillars(struct BattleEffectArgument *effect);
void BattleFx_RunSparkBursts(struct BattleEffectArgument *effect);
void BattleFx_RunOrangeSparkBursts(struct BattleEffectArgument *effect);
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 kind);
void BattleFx_RunFirePillars(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotesMode0Or1(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotesMode1(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotesMode2(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotesMode3(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotesMode4(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotesMode5(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotesMode7(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotesMode6(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotes(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunBouncingSheep(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMarkers(struct BattleEffectArgument *effect);
void BattleFx_RunDjinnHeal(struct BattleEffectArgument *effect);

void Camera_ApplyPhasedDelta(void);
void BattleFx_RunNoEffectFrames(s32 cnt);
void BattleFx_RunSceneEffect(struct BattleEffectArgument *effect);
void BattleFx_RunTableEffect(struct BattleEffectArgument *effect);
void BattleFx_BuildWindowEdgeTable(void);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *arg);
void BattleEffect_SetupBlendedDisplay(void);
void ObjectGroup_UpdateMembers(s32 actorId, s32 palette, s32 animation,
                               s32 timerSlot, s32 timerFrames);
void BattleFx_InitTilemapAndFlushQueue(void);

void BattleEffect_RunDitherDissolveScene(struct BattleEffectArgument *effect);
void BattleFx_RunRockAndIceScene(struct BattleEffectArgument *effect);
void BattleFx_RunIceShardBursts(struct BattleEffectArgument *effect);
void BattleFx_RunGatheringMotes(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotesMode0(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotesMode1(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotesMode2(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotesMode3Or4(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotesMode4(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotesMode5(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotesMode6(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotes(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunStatBoostMode0(struct BattleEffectArgument *effect);
void BattleFx_RunStatBoostMode1(struct BattleEffectArgument *effect);
void BattleFx_RunStatBoostMode2(struct BattleEffectArgument *effect);
void BattleFx_RunStatBoostMode3(struct BattleEffectArgument *effect);
void BattleFx_RunStatBoostMode5(struct BattleEffectArgument *effect);
void BattleFx_RunStatBoostMode4(struct BattleEffectArgument *effect);
void BattleFx_RunStatBoost(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunStatReductionMode0(struct BattleEffectArgument *effect);
void BattleFx_RunStatReductionMode1(struct BattleEffectArgument *effect);
void BattleFx_RunStatReductionMode2(struct BattleEffectArgument *effect);
void BattleFx_RunStatReductionMode3(struct BattleEffectArgument *effect);
void BattleFx_RunStatReduction(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunPuffArc(struct BattleEffectArgument *effect);
s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                    s16 *output);
void BattleFx_RunRockWall(struct BattleEffectArgument *effect);
void BattleFx_RunHomingEmbers(struct BattleEffectArgument *effect);
void BattleFx_RunRisingMotes(struct BattleEffectArgument *effect);
void BattleFx_RunClosingBolts(struct BattleEffectArgument *effect);
void BattleFx_RunEruptionMode1(struct BattleEffectArgument *effect);
void BattleFx_RunEruptionMode0(struct BattleEffectArgument *effect);
void BattleFx_RunEruption(struct BattleEffectArgument *effect, s32 mode);

void BattleFx_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void BattleFx_RunNoEffect(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_RunBurstShowerMode0(struct BattleEffectArgument *effect);
void BattleFx_RunBurstShowerMode2(struct BattleEffectArgument *effect);
void BattleFx_RunBurstShowerMode6(struct BattleEffectArgument *effect);
void BattleFx_RunBurstShowerMode3(struct BattleEffectArgument *effect);
void BattleFx_RunBurstShowerMode5(struct BattleEffectArgument *effect);
void BattleFx_RunBurstShowerMode7(struct BattleEffectArgument *effect);
void BattleFx_RunBurstShowerMode4(struct BattleEffectArgument *effect);
void BattleEffect_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunSwirlingStars(void *object);
void BattleFx_RunOrbitingSparks(void);
void BattleFx_RunTargetBursts(void *object);
void BattleEffect_RunStagedParticles(struct BattleEffectArgument *effect);
void BattleFx_RunGrowingColumnsMode0(struct BattleEffectArgument *effect);
void BattleFx_RunGrowingColumnsMode1(struct BattleEffectArgument *effect);
void BattleFx_RunGrowingColumns(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunRisingColumns(struct BattleEffectArgument *effect);
void BattleFx_RaisePillars(struct BattleEffectArgument *effect);
void BattleFx_RunLightningBolts(struct BattleEffectArgument *effect);
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind,
                                  s32 side, s32 anchor, s32 *out_x, s32 *out_y);
void BattleFx_RunProjectileVolleyMode0(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode1(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode2(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode3(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode11(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode4(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode5WithAction(
    struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode6WithAction(
    struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode7(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode10(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolleyMode9WithAction(
    struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleFx_InitializeRisingObject(s32 id);
void BattleFx_SetApproachMotion(s32 id, s32 targetId, s32 frames);
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount);
void BattleFx_RunRushingStrikeMode0(struct BattleEffectArgument *effect);
void BattleFx_RunRushingStrikeMode1(struct BattleEffectArgument *effect);
void BattleFx_RunRushingStrikeMode2(struct BattleEffectArgument *effect);
void BattleFx_RunRushingStrikeMode3(struct BattleEffectArgument *effect);
void BattleFx_RunRushingStrike(struct BattleEffectArgument *effect,
                               s32 variant);
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h);
void BattleFx_RunMercuryDjinnFlames(struct BattleEffectArgument *effect);
void BattleFx_RunJupiterDjinnFall(struct BattleEffectArgument *effect);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
void BattleFx_RunVenusDjinnColumns(struct BattleEffectArgument *effect);
void BattleFx_RunMarsDjinnPuffs(struct BattleEffectArgument *effect);
void BattleFx_RunParticleReveal(struct BattleEffectArgument *effect);
void BattleFx_RunVenusDjinnRockToss(struct BattleEffectArgument *effect);
void Camera_ApplyShake(s32 random_mask, u32 shake_range);
void BattleFx_RunStormScene(struct BattleEffectArgument *effect);
void BattleFx_RunShatteringRocks(struct BattleEffectArgument *object);
void EffectStep_RunAnimationMode6Or7Or8(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode0(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode0Alternate(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode1(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode2(struct BattleEffectArgument *effect);
void EffectStep_RunAnimationMode3Or4Or5(struct BattleEffectArgument *effect);
void BattlePres_RunBurstScene(struct BattleEffectArgument *effect, s32 variant);
void BattlePres_RunRisingWall(struct BattleEffectArgument *effect);

void EffectStep_AdvanceWithGravity3D(struct EffectStep *step, s32 damping,
                                     s32 gravity);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping,
                                     s32 gravity);
s32 EffectPosition_ApplyBaseAndYOffset(const void *point,
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
void ObjectGroup_StoreObjectAndRunStep7(struct BattleEffectArgument *effect);
void BattleFx_RunFireSwirlCharge(struct BattleEffectArgument *effect);
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y);
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta);
void BattleFx_ArmPaletteHBlankDma(void);
void BattleFx_RunParticleStreamsMode0(struct BattleEffectArgument *effect);
void BattleFx_RunParticleStreamsMode1(struct BattleEffectArgument *effect);
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
void BattleFx_RunFormationSweep(struct BattleEffectArgument *effect);
void BattleFx_RunSparkApproachVariant4(struct BattleEffectArgument *effect);
void BattleFx_RunSparkApproachVariant3(struct BattleEffectArgument *effect);
void BattleFx_RunRevealColumnMode1(struct BattleEffectArgument *effect);
void BattleFx_RunRevealColumnMode2(struct BattleEffectArgument *effect);
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant);
void BattleFx_RunRisingBurst(struct BattleEffectArgument *effect);
s32 BattleEffect_LoadWork(s32 slot, s32 width_shift, s32 height_shift,
                          s32 flags, u32 mode);


#endif
