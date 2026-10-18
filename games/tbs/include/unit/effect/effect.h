#ifndef ALCHEMY_UNIT_EFFECT_EFFECT_H
#define ALCHEMY_UNIT_EFFECT_EFFECT_H

#include "types.h"
#include "render_placement.h"
#include "battle_effect_work.h"
#include "battle_effect.h"

struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;

// The large effects BattleFx_RunSceneEffect runs, by kind: the summons, and
// Meteor's scene (kind 0 runs it too).
enum SceneEffectKind {
  SCENE_EFFECT_RAMSES = 1,
  SCENE_EFFECT_NEREID,
  SCENE_EFFECT_KIRIN,
  SCENE_EFFECT_ATALANTA,
  SCENE_EFFECT_CYBELE,
  SCENE_EFFECT_NEPTUNE,
  SCENE_EFFECT_TIAMAT,
  SCENE_EFFECT_PROCNE,
  SCENE_EFFECT_JUDGMENT,
  SCENE_EFFECT_BOREAS,
  SCENE_EFFECT_METEOR,
  SCENE_EFFECT_THOR
};

typedef void (*BattleFxHandler)(struct BattleEffectArgument *effect);
extern BattleFxHandler gBattleFx_EffectHandlers[];

typedef struct {
  s32 unk_00[3];
  s32 currentValue;
  s32 indexRadius;
  s32 entryLimit;
  s32 unk_18[3];
  s16 values[1];
} BattleEffectIndexWindow;

// Particle storage: reuse Ram_MapCellBuffer as an array of struct EffectStep.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

#define EFFECT_CLIP_BOUNDS                                                     \
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

// Effect renderer pair: BattleFx_BuildBlitter creates [0] in
// HEAP_SLOT_BLITTER and [1] in HEAP_SLOT_BLITTER_ALTERNATE.
// Many effects keep this pair in their stack frame.
typedef DrawRectangle BlitterPair[2];

#define gBlitters ((DrawRectangle *)gWorkSlot)

// The functions the parts of effect.c define, part by part in include order.

// display.c
void BattleFx_ConfigureEffectDisplay(void);
void BattleFx_AdvanceScrollOnInterval(void);
void Camera_AdvanceBg2Reference(void);
void BattleFx_ArmWin0HBlankDma(void);

// falling.c
void BattleFx_RunIce(struct BattleEffectArgument *effect);
void BattleFx_RunDouse(struct BattleEffectArgument *effect);

// streaks.c
void BattleFx_RunSonicWave(struct BattleEffectArgument *effect);
void BattleFx_RunShriek(struct BattleEffectArgument *effect);
void BattleFx_RunBansheeHowl(struct BattleEffectArgument *effect);
void BattleFx_RunWarCry(struct BattleEffectArgument *effect);
void BattleFx_RunWickedHowl(struct BattleEffectArgument *effect);
void BattleFx_RunLimeStreaks(struct BattleEffectArgument *effect);
void BattleFx_RunStreakVolley(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunSleepStar(struct BattleEffectArgument *effect);
void BattleFx_RunSoothingStar(struct BattleEffectArgument *effect);
void BattleFx_RunMoteFountain(struct BattleEffectArgument *effect, s32 mode);

// target_strike.c
void BattleFx_RunNumbingSting(struct BattleEffectArgument *effect);
void BattleFx_RunCannibalFang(struct BattleEffectArgument *effect);
void BattleFx_RunPoisonousBite(struct BattleEffectArgument *effect);
void BattleFx_RunRabidFang(struct BattleEffectArgument *effect);
void BattleFx_RunElectricBite(struct BattleEffectArgument *effect);
void BattleFx_RunPoisonTail(struct BattleEffectArgument *effect);
void BattleFx_RunVampiricFang(struct BattleEffectArgument *effect);
void BattleFx_RunDoubleFang(struct BattleEffectArgument *effect);
void BattleFx_RunMortalBlow(struct BattleEffectArgument *effect);
void BattleFx_RunTwinBeaks(struct BattleEffectArgument *effect);
void BattleFx_RunAcidBite(struct BattleEffectArgument *effect);
void BattleFx_RunDrainFang(struct BattleEffectArgument *effect);
void BattleFx_RunTargetStrike(struct BattleEffectArgument *effect, s32 mode);

// death_plunge.c
void BattleFx_RunDeathPlunge(struct BattleEffectArgument *effect);
void BattleFx_RunBladeRain(struct BattleEffectArgument *effect);
void BattleFx_RunIceFleckBurst(struct BattleEffectArgument *effect);
void BattleFx_RunTileAndPaletteAnimation(struct BattleEffectArgument *effect);
void BattleFx_RunGatheringBurst(struct BattleEffectArgument *effect);
void BattleFx_DrawStreaks(void);
void BattleFx_PrepareScene(s32 kind);
void BattleFx_ScheduleCallbacksAndReleaseBlocks(void);

// spider_web.c
void BattleFx_RunPoisonBeat(struct BattleEffectArgument *effect);
void BattleFx_RunSpinningBeat(struct BattleEffectArgument *effect);
void BattleFx_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunSpiderWeb(struct BattleEffectArgument *effect);

// canvas.c
void BattleFx_WipeCanvas(s32 mode, s32 value);
void BattleFx_ProcessPendingGraphicsTransfer(void);
void BattleFx_FlushPendingGraphicsTransfer(void);
void Graphics_ApplyWindowBlendRegisters(void);
void Display_ApplyBg2Reference(void);
void Palette_StepFadeTransfer(void);
void BattleFx_ClearActorTimers(void);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_OpenCanvasLayer(s32 bgControl);
void BattleFx_BeginTiledCanvas(s32 bgControl);
void BattleFx_EndCanvasLayer(void);
void BattleFx_HideBg1AndRestoreBackdrop(void);
void BattleFx_SetupCanvasTileMap(void);
void BattleFx_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);

// planet_diver.c
void BattleFx_RunPlanetDiver(struct BattleEffectArgument *effect);
void BattleFx_RunHaunt(struct BattleEffectArgument *effect);
void BattleFx_RunOrbitingSpirals(struct BattleEffectArgument *effect);

// gathering_lights.c
void BattleFx_RunDemonNight(struct BattleEffectArgument *effect);
void BattleFx_RunRumble(struct BattleEffectArgument *effect);
void BattleFx_RunHaunting(struct BattleEffectArgument *effect);
void BattleFx_RunGatheringLights(struct BattleEffectArgument *effect, s32 mode);

// mirrored_wall.c
void BattleFx_FetchRectangleBlitters(s32 flip, BlitterPair out);
void BattleFx_RunBerserk(struct BattleEffectArgument *effect);
void BattleFx_RunSpasm(struct BattleEffectArgument *effect);
void BattleFx_RunRecovery(struct BattleEffectArgument *effect);
void BattleFx_RunMirroredWall(struct BattleEffectArgument *effect, s32 mode);

// djinn_beams.c
void BattleFx_RunRevive(struct BattleEffectArgument *effect);
void BattleFx_RunDew(struct BattleEffectArgument *effect);
void BattleFx_RunDjinnBeams(struct BattleEffectArgument *effect, s32 mode);

// lobbed_shells.c
void BattleFx_RunMadSpatter(struct BattleEffectArgument *effect);
void BattleFx_RunDecompose(struct BattleEffectArgument *effect);
void BattleFx_RunBoneChiller(struct BattleEffectArgument *effect);
void BattleFx_RunStickyGoo(struct BattleEffectArgument *effect);
void BattleFx_RunPoisonInk(struct BattleEffectArgument *effect);
void BattleFx_RunStickyPoison(struct BattleEffectArgument *effect);
void BattleFx_RunRottenBlood(struct BattleEffectArgument *effect);
void BattleFx_RunLobbedShells(struct BattleEffectArgument *effect, s32 mode);

// condemn.c
void BattleFx_RunCondemn(struct BattleEffectArgument *object);

// bind.c
void Graphics_ConvertBackgroundToBlueRamp(void);
void BattleFx_RunBind(struct BattleEffectArgument *effect);
void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect);

// stars.c
void BattleFx_RunAstralBlast(struct BattleEffectArgument *effect);
void BattleFx_RunShiningStar(struct BattleEffectArgument *effect);

// procne.c
void BattleFx_RunProcne(struct BattleEffectArgument *argument);

// tiamat.c
void BattleFx_RunDragonDriver(struct BattleEffectArgument *effect);
void BattleFx_RunTiamat(struct BattleEffectArgument *effect);
void BattleFx_RunPaletteParticles(struct BattleEffectArgument *effect,
                                  s32 mode);

// nereid.c
void BattleFx_RunNereid(struct BattleEffectArgument *effect);

// vortices.c
void BattleFx_RunFroth(struct BattleEffectArgument *effect);
void BattleFx_RunWhirlwind(struct BattleEffectArgument *effect);

// prism.c
void BattleFx_RunPrism(struct BattleEffectArgument *effect);
void Graphics_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                 s32 bluePhase);

// plasma.c
void BattleFx_RunPlasma(struct BattleEffectArgument *effect);
void BattleFx_RunNova(struct BattleEffectArgument *effect);
void BattleFx_RunDynamite(struct BattleEffectArgument *effect);
void BattleFx_RunSparkGroups(struct BattleEffectArgument *effect, s32 kind);

// volcano.c
void BattleFx_RunVolcano(struct BattleEffectArgument *effect);

// target_motes.c
void BattleFx_RunTaint(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotesMode1(struct BattleEffectArgument *effect);
void BattleFx_RunDelude(struct BattleEffectArgument *effect);
void BattleFx_RunParalyze(struct BattleEffectArgument *effect);
void BattleFx_RunHeartrender(struct BattleEffectArgument *effect);
void BattleFx_RunMysticFlame(struct BattleEffectArgument *effect);
void BattleFx_RunDemonFire(struct BattleEffectArgument *effect);
void BattleFx_RunDrown(struct BattleEffectArgument *effect);
void BattleFx_RunTargetMotes(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunSleep(struct BattleEffectArgument *effect);
void BattleFx_RunCurse(struct BattleEffectArgument *effect);

// djinn_heal.c
void BattleFx_RunDjinnHeal(struct BattleEffectArgument *effect);
void Camera_ApplyPhasedDelta(void);

// dispatch.c
void BattleFx_CallNoEffect(s32 count);
void BattleFx_RunSceneEffect(struct BattleEffectArgument *effect);
void BattleFx_RunTableEffect(struct BattleEffectArgument *effect);
void BattleFx_BuildWindowEdgeTable(void);
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *arg);
void BattleFx_SetupBlendedDisplay(void);
void ObjectGroup_UpdateMembers(s32 actorId, s32 palette, s32 animation,
                               s32 timerSlot, s32 timerFrames);
void BattleFx_InitTilemapAndFlushQueue(void);

// cybele.c
void BattleFx_RunCybele(struct BattleEffectArgument *effect);

// boreas.c
void BattleFx_RunBoreas(struct BattleEffectArgument *effect);

// drain.c
void BattleFx_RunBreak(struct BattleEffectArgument *effect);
void BattleFx_RunDrain(struct BattleEffectArgument *effect);

// healing.c
void BattleFx_RunPly(struct BattleEffectArgument *effect);
void BattleFx_RunWish(struct BattleEffectArgument *effect);
void BattleFx_RunCure(struct BattleEffectArgument *effect);
void BattleFx_RunCurePoison(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotesMode4(struct BattleEffectArgument *effect);
void BattleFx_RunTonic(struct BattleEffectArgument *effect);
void BattleFx_RunPsyCrystal(struct BattleEffectArgument *effect);
void BattleFx_RunHealingMotes(struct BattleEffectArgument *effect, u32 mode);

// stat_change.c
void BattleFx_RunImpact(struct BattleEffectArgument *effect);
void BattleFx_RunGuard(struct BattleEffectArgument *effect);
void BattleFx_RunWard(struct BattleEffectArgument *effect);
void BattleFx_RunGranite(struct BattleEffectArgument *effect);
void BattleFx_RunFlash(struct BattleEffectArgument *effect);
void BattleFx_RunZephyr(struct BattleEffectArgument *effect);
void BattleFx_RunStatBoost(struct BattleEffectArgument *effect, u32 mode);
void BattleFx_RunDull(struct BattleEffectArgument *effect);
void BattleFx_RunImpair(struct BattleEffectArgument *effect);
void BattleFx_RunWeaken(struct BattleEffectArgument *effect);
void BattleFx_RunStatReductionMode3(struct BattleEffectArgument *effect);
void BattleFx_RunStatReduction(struct BattleEffectArgument *effect, s32 mode);

// flare.c
void BattleFx_RunFlare(struct BattleEffectArgument *effect);
s32 BattleFx_BuildCenteredIndexList(BattleEffectIndexWindow *window,
                                    s16 *output);

// quake.c
void BattleFx_RunQuake(struct BattleEffectArgument *effect);

// fire.c
void BattleFx_RunFire(struct BattleEffectArgument *effect);

// frost.c
void BattleFx_RunFrost(struct BattleEffectArgument *effect);

// ray.c
void BattleFx_RunRay(struct BattleEffectArgument *effect);

// eruption.c
void BattleFx_RunSmokeBomb(struct BattleEffectArgument *effect);
void BattleFx_RunBlast(struct BattleEffectArgument *effect);
void BattleFx_RunEruption(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void BattleFx_RunNoEffect(void);

// burst_shower.c
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_RunFireBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunWaterBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunDarkBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunAcidBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunStormBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunEvilBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunDeadlyGas(struct BattleEffectArgument *effect);
void BattleFx_RunBurstShower(struct BattleEffectArgument *effect, s32 mode);

// orbiting.c
void BattleFx_RunSwirlingStars(void *object);
void BattleFx_RunOrbitingSkulls(void);
void BattleFx_RunTargetBursts(void *object);

// atalanta.c
void BattleFx_RunAtalanta(struct BattleEffectArgument *effect);

// growth.c
void BattleFx_RunGrowth(struct BattleEffectArgument *effect);
void BattleFx_RunPunji(struct BattleEffectArgument *effect);
void BattleFx_RunGrowingColumns(struct BattleEffectArgument *effect, s32 mode);
void BattleFx_RunVine(struct BattleEffectArgument *effect);
void BattleFx_RunThorn(struct BattleEffectArgument *effect);

// bolt.c
void BattleFx_RunBolt(struct BattleEffectArgument *effect);

// projectiles.c
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind,
                                  s32 side, s32 anchor, s32 *outX, s32 *outY);
void BattleFx_RunSlash(struct BattleEffectArgument *effect);
void BattleFx_RunShuriken(struct BattleEffectArgument *effect);
void BattleFx_RunCrazyVoice(struct BattleEffectArgument *effect);
void BattleFx_RunWingFlutter(struct BattleEffectArgument *effect);
void BattleFx_RunWingStroke(struct BattleEffectArgument *effect);
void BattleFx_RunWingBeat(struct BattleEffectArgument *effect);
void BattleFx_RunCuttingEdge(struct BattleEffectArgument *effect);
void BattleFx_RunHeatWave(struct BattleEffectArgument *effect);
void BattleFx_RunSleet(struct BattleEffectArgument *effect);
void BattleFx_RunIceBlessing(struct BattleEffectArgument *effect);
void BattleFx_RunBlizzard(struct BattleEffectArgument *effect);
void BattleFx_RunProjectileVolley(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleFx_InitializeRisingObject(s32 id);
void BattleFx_SetApproachMotion(s32 id, s32 targetId, s32 frames);

// rushing_strike.c
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride, s32 rowCount);
void BattleFx_RunBoneCharge(struct BattleEffectArgument *effect);
void BattleFx_RunBruteForce(struct BattleEffectArgument *effect);
void BattleFx_RunOnslaught(struct BattleEffectArgument *effect);
void BattleFx_RunHeadbutt(struct BattleEffectArgument *effect);
void BattleFx_RunRushingStrike(struct BattleEffectArgument *effect,
                               s32 variant);

// djinn_attacks.c
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h);
void BattleFx_RunMercury(struct BattleEffectArgument *effect);
void BattleFx_RunJupiter(struct BattleEffectArgument *effect);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
void BattleFx_RunVenus(struct BattleEffectArgument *effect);
void BattleFx_RunMars(struct BattleEffectArgument *effect);

// hail.c
void BattleFx_RunHail(struct BattleEffectArgument *effect);
void BattleFx_RunGround(struct BattleEffectArgument *effect);
void Camera_ApplyShake(s32 randomMask, u32 shakeRange);

// thor.c
void BattleFx_RunThor(struct BattleEffectArgument *effect);

// spire.c
void BattleFx_RunSpire(struct BattleEffectArgument *object);

// burst_scene.c
void BattleFx_RunGust(struct BattleEffectArgument *effect);
void BattleFx_RunBearClaw(struct BattleEffectArgument *effect);
void BattleFx_RunRansack(struct BattleEffectArgument *effect);
void BattleFx_RunBacteriaRush(struct BattleEffectArgument *effect);
void BattleFx_RunFreebiteRush(struct BattleEffectArgument *effect);
void BattleFx_RunFlyingAttack(struct BattleEffectArgument *effect);
void BattleFx_RunBurstScene(struct BattleEffectArgument *effect, s32 variant);

// gaia.c
void BattleFx_RunGaia(struct BattleEffectArgument *effect);

// positions.c
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

// beams.c
void BattleFx_RunBeamSequence(struct BattleEffectArgument *effect);
void BattleFx_RunRingAndSparkScene(struct BattleEffectArgument *effect);

// casting_impact.c
void BattleFx_StepPaletteToResource(s32 resourceId);
void BattleFx_RunCastingImpact(struct BattleEffectArgument *command, s32 kind);

// torch.c
void BattleFx_RunTorch(struct BattleEffectArgument *effect);
void BattleFx_RunKite(struct BattleEffectArgument *effect);

// helm_splitter.c
void BattleFx_RunHelmSplitter(struct BattleEffectArgument *effect);
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleFx_RunImpactBurst(s32 formation, s32 x, s32 y);

// particle_streams.c
void Palette_BrightenBgEntries(s32 blueStep, s32 greenStep, s32 redStep);
void BattleFx_ArmPaletteHBlankDma(void);
void BattleFx_RunMeteor(struct BattleEffectArgument *effect);
void BattleFx_RunOuterSpace(struct BattleEffectArgument *effect);
void BattleFx_RunParticleStreams(struct BattleEffectArgument *effect, s32 mode);

// ramses.c
void BattleFx_RunRamses(struct BattleEffectArgument *effect);

// dragon_cloud.c
void BattleFx_RunDragonCloud(struct BattleEffectArgument *effect);

// annihilation.c
void BattleFx_RunAnnihilation(struct BattleEffectArgument *effect);

// ragnarok.c
void BattleFx_RunRagnarok(struct BattleEffectArgument *effect);

// titan_blade.c
void BattleFx_RunTitanBlade(struct BattleEffectArgument *effect);

// judgment.c
void BattleFx_RunJudgment(struct BattleEffectArgument *effect);

// kirin.c
void BattleFx_RunKirin(struct BattleEffectArgument *effect);

// neptune.c
void Graphics_ResetBg2Pa(void);
void Graphics_SetBg2AffineScaleHalf(void);
void BattleFx_RunNeptune(struct BattleEffectArgument *effect);

// thrash.c
void BattleFx_RunThrash(struct BattleEffectArgument *effect);
void BattleFx_RunSparkApproachVariant3(struct BattleEffectArgument *effect);
void BattleFx_RunQuickStrike(struct BattleEffectArgument *effect);
void BattleFx_RunVorpalSlash(struct BattleEffectArgument *effect);
void BattleFx_RunFlashingStrike(struct BattleEffectArgument *effect, s32 variant);

// undead_sword.c
void BattleFx_RunUndeadSword(struct BattleEffectArgument *effect);

// blitter.c
s32 BattleFx_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                      u32 mode);

#endif
