#ifndef ALCHEMY_BATTLE_EFFECT_BATTLE_EFFECT_H
#define ALCHEMY_BATTLE_EFFECT_BATTLE_EFFECT_H

/// @file
/// The battle effects as the rest of the battle calls them: a runner for each
/// Psynergy, Djinni, summon and monster ability, and the canvas, palette,
/// particle and model helpers they share.

#include "fixed_math.h"
#include "types.h"
#include "resource_ids.h"
#include "field/particle.h"
#include "battle/battle_effect.h"
#include "battle/battle_unit.h"
#include "battle_effect/battle_effect_ids.h"

struct BattleEffectParams;
struct ScreenPoint;
struct Particle;
struct MeshTriangle;
struct MeshVertex;
struct PoolEntry;
struct StripQuad;

/// The summons' sprite formations (BattleEffect_PlaceFormationObjects and
/// BattleEffect_RunImpactBurst), each named for the summon that keeps it.
enum SummonFormation {
  SUMMON_FORMATION_THOR,    ///< nine in a 3x3 grid at 32-pixel spacing
  SUMMON_FORMATION_NEREID,  ///< twelve in rows of 1, 3, 4 and 4
  SUMMON_FORMATION_CYBELE,  ///< eight in rows of 3, 2 and 3
  SUMMON_FORMATION_TIAMAT,  ///< eight drawn at 1.25 times their size
  SUMMON_FORMATION_MEGAERA, ///< eleven from signed columns
  SUMMON_FORMATION_MOLOCH,  ///< twelve in a 4x3 grid
  /// Tiamat's eight when the first target's animation kind is 2.
  SUMMON_FORMATION_TIAMAT_ALTERNATE
};

typedef DrawRectangle BlitterPair[2];

/// A texture as the render jobs take it: 1 << logWidth by 1 << logHeight.
struct Texture {
  u8 logWidth;
  u8 logHeight;
  u16 padding;
  void *pixels;
};

/// A point or a velocity, 16.16 pixels.
struct Point2D {
  FIXED x;
  FIXED y;
};

/// A sprite's scale across and down, 16.16, as an effect keeps it in a local
/// (struct SpriteScale is the same pair as a union with an array).
struct Scale {
  FIXED x;
  FIXED y;
};

/// An effect's variant (BattleEffectParams.variant): abilities of one family
/// that share an effect, such as Quake, Earthquake and Quake Sphere, and a
/// double or triple blow adds one or two. A runner that sets its own variants
/// names them beside it.
enum EffectVariant {
  EFFECT_VARIANT_BASIC,    ///< Quake, Douse, Bolt
  EFFECT_VARIANT_STRONGER, ///< Earthquake, Drench, Flash Bolt
  EFFECT_VARIANT_STRONGEST ///< Quake Sphere, Deluge, Blue Bolt
};

/// BattleMotion_ApplyKnockback's knockbacks, the columns of the
/// gBattleKnockback_* tables. Each is named by how far and how high it sends
/// the unit, as percent of its spot's distance from the centre and its hop in
/// pixels a frame; all but KNOCKBACK_HURL and KNOCKBACK_HURL_FAR accelerate by
/// 1 pixel a frame to at most 4.
enum BattleKnockback {
  KNOCKBACK_NUDGE,  ///< 140 percent, a hop of 3
  KNOCKBACK_HOP,    ///< 140 percent, a hop of 6
  KNOCKBACK_LEAP,   ///< 150 percent, a hop of 7
  KNOCKBACK_LAUNCH, ///< 200 percent, a hop of 10
  /// The hardest: 200 percent at 8 pixels a frame more each frame, to at
  /// most 20, with a hop of 5.
  KNOCKBACK_HURL,
  /// 150 percent, a hop of 2. From here on a unit already in the air takes
  /// the new hop too.
  KNOCKBACK_SKID,
  KNOCKBACK_BOUNCE, ///< the spot itself, a hop of 3
  KNOCKBACK_JOLT,   ///< the spot itself, a hop of 5.5
  /// Unused: the spot itself, a hop of 10; no knockback names it.
  KNOCKBACK_LIFT,
  /// Unused: 300 percent, as KNOCKBACK_HURL otherwise; no knockback names it.
  KNOCKBACK_HURL_FAR
};

/// A battle background's id, as BATTLE_BACKGROUND_ID reads it in the summons;
/// nothing reads the word or the bytes.
// XXX: is the union there for anything but the order of the summons' stores
// beside the read, so a FAKEMATCH(signature) to tag; reading the id as a plain
// u16 and comparing the editions would settle it.
union BattleBackgroundId {
  u16 id;
  s32 word;
  u8 bytes[6];
};

void BattleEffect_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                     s32 bluePhase);
/// The canvas layer and its helpers.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);
void BattleEffect_DrawShadedCircles(u8 *map, s32 size, FIXED scale);
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectParams *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Canvas_BlitAtFoot(void *source, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void EffectPosition_ProjectBattler(s32 id, struct ScreenPoint *position);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void BattleEffect_SelectLivingTargets(struct BattleEffectParams *party);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
/// Moves a particle one frame on the screen: its velocity scaled by `damping`
/// sixty-fourths, then `gravity` added to its y speed.
void Particle_AdvanceWithGravity2D(struct Particle *step, s32 damping,
                                   s32 gravity);
void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
/// Moves a particle one frame in three dimensions: its velocity scaled by
/// `damping` sixty-fourths, then `gravity` added to its y speed.
void Particle_AdvanceWithGravity3D(struct Particle *step, s32 damping,
                                   s32 gravity);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void RagingHeat_FillGradientCanvas(void);
void BattleEffect_AdvanceScrollOnInterval(void);
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);
void Palette_BrightenBattleBg(s32 redDelta, s32 greenDelta, s32 blueDelta);
void Palette_BrightenSavedBattleBg(s32 redDelta, s32 greenDelta, s32 blueDelta);
void Mesh_BuildCylinderBand(struct MeshVertex *vertices,
                            struct MeshTriangle *triangles, s32 segments);
/// Loads a grey ramp into the background colours from colour 1.
void Graphics_LoadGreyRamp(void);
void Mesh_BuildGrid(struct MeshTriangle *triangles, s32 columns, s32 rows,
                    s32 cellWidth, s32 cellHeight);
void Palette_BrightenBgEntries(s32 blueDelta, s32 greenDelta, s32 redDelta);
void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_ClearActorTimers(void);
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color);
/// Sets the registers that show the canvas on BG2.
void BattleEffect_SetupCanvasWindows(void);
void BattleEffect_DrawShadedCirclesMirrored(u8 *map, s32 size, s32 scale);
void BattleEffect_RunRamses(struct BattleEffectParams *effect);
void BattleEffect_RunNereid(struct BattleEffectParams *effect);
void BattleEffect_RunKirin(struct BattleEffectParams *effect);
void BattleEffect_RunAtalanta(struct BattleEffectParams *effect);
void BattleEffect_RunNeptune(struct BattleEffectParams *effect);
void BattleEffect_RunProcne(struct BattleEffectParams *effect);
void BattleEffect_RunJudgment(struct BattleEffectParams *effect);
void BattleEffect_RunBoreas(struct BattleEffectParams *effect);
void BattleEffect_RunThor(struct BattleEffectParams *effect);
void BattleEffect_RunMegaera(struct BattleEffectParams *effect);
void BattleEffect_RunFlora(struct BattleEffectParams *effect);
void BattleEffect_RunMoloch(struct BattleEffectParams *effect);
void BattleEffect_RunUlysses(struct BattleEffectParams *effect);
void BattleEffect_RunHaures(struct BattleEffectParams *effect);
void BattleEffect_RunEclipse(struct BattleEffectParams *effect);
void BattleEffect_RunCoatlicue(struct BattleEffectParams *effect);
void BattleEffect_RunDaedalus(struct BattleEffectParams *args);
void BattleEffect_RunAzul(struct BattleEffectParams *effect);
void BattleEffect_RunCatastrophe(struct BattleEffectParams *effect);
void BattleEffect_RunCharon(struct BattleEffectParams *arg);
void BattleEffect_RunIris(struct BattleEffectParams *effect);
void BattleEffect_RunDaedalusStrike(struct BattleEffectParams *effect);
void BattleEffect_RunTrident(struct BattleEffectParams *effect);
void BattleEffect_RunCruelRuin(struct BattleEffectParams *effect);
void BattleEffect_RunMeteorBlow(struct BattleEffectParams *effect);
void BattleEffect_RunDjinnBeams(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunLobbedShells(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunImpair(struct BattleEffectParams *args);
void BattleEffect_RunDull(struct BattleEffectParams *args);
void BattleEffect_RunGatheringLights(struct BattleEffectParams *effect,
                                     s32 mode);
/// Arms the HBlank DMA that scrolls BG1 line by line.
void BattleEffect_ArmBg1HBlankDma(void);
/// Reverses the first 64 background colours, keeping the backdrop colour.
void Palette_MirrorBgColors(void);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEffect_RunDragonFire(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x, s32 y);
void BattleEffect_RunImpactBurst(s32 formation, s32 x, FIXED y);
void BattleEffect_RunStreakVolley(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunMoteFountain(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_GetTargetsMidpoint(struct ScreenPoint *midpoint);
void BattleEffect_RunTumblingQuads(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunTargetStrike(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);
void BattleEffect_RunCloudSwirl(struct BattleEffectParams *effect);
void BattleCamera_ApplyPhasedDelta(void);
typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);
/// The window BattleEffect_BuildCenteredIndexList builds a list of indices
/// around: its value, radius and limit, and the values it lists.
typedef struct {
  s32 reservedA[3];
  s32 currentValue;
  s32 indexRadius;
  s32 entryLimit;
  s32 reservedB[3];
  s16 values[1];
} BattleEffectIndexWindow;
/// What BattleEffect_RunDjinniEntrance takes added to its element; the element
/// alone plays the 55-frame entrance.
enum DjinniEntranceLength {
  /// The long entrance: 84 frames instead of 55.
  DJINNI_ENTRANCE_LONG = ELEMENT_COUNT
};

/// Where BattleEffect_RunDjinniEntrance's Djinni appears; the first five
/// anchors stand it 60 pixels up.
enum DjinniEntranceAnchor {
  DJINNI_ANCHOR_OVER_TARGET, ///< over the first target
  DJINNI_ANCHOR_TARGET_X,    ///< at the first target's x, at depth 0
  DJINNI_ANCHOR_OVER_USER,   ///< over the user
  DJINNI_ANCHOR_USER_X,      ///< at the user's x, at depth 0
  DJINNI_ANCHOR_CENTRE,      ///< in the centre of the field
  DJINNI_ANCHOR_HIGH_USER_X, ///< at the user's x, 80 pixels up
  /// In the centre: the entrance takes 40 frames with no fade, and the
  /// backdrop brightens from frame 24.
  DJINNI_ANCHOR_CENTRE_BRIGHTENING,
  DJINNI_ANCHOR_LOW_CENTRE, ///< in the centre, 20 pixels up
  /// Over the first target: the entrance takes 54 frames.
  DJINNI_ANCHOR_OVER_TARGET_SHORT,
  DJINNI_ANCHOR_HIGH_TARGET_X, ///< at the first target's x, 120 pixels up
  DJINNI_ANCHOR_LOW_USER_X     ///< at the user's x, 30 pixels up
};

/// BattleEffect_RunDjinniEntrance's packed argument: which Djinni (an element,
/// plus DJINNI_ENTRANCE_LONG for the longer entrance), the side whose way it
/// faces, and where it appears (enum DjinniEntranceAnchor).
#define DJINNI_ENTRANCE(element, side, anchor)                                 \
  ((element) | (side) << 4 | (anchor) << 5)
#define DJINNI_ENTRANCE_ELEMENT(packed) ((packed) & 7)
#define DJINNI_ENTRANCE_SIDE(packed) (((packed) >> 4) & 1)
#define DJINNI_ENTRANCE_ANCHOR(packed) (((packed) >> 5) & 0xf)

/// A row of gQuake_Timings, one for each variant.
enum {
  TIMING_FRAMES,      ///< the whole effect
  TIMING_RISE_FRAMES, ///< how long each pillar stands
  TIMING_SHAKE        ///< the camera shake
};

/// The four rock pillars' heights, in pixels.
struct RockWallHeights {
  s32 column[4]; ///< each pillar's height in pixels
};

/// The five widths a rising pillar takes.
struct FiveWidths {
  s32 widths[5];
};

/// The seven widths a widening pillar takes.
struct SevenWidths {
  s32 widths[7];
};

/// Where a strip's petals start across, by quarter of its rise: the left edge
/// and the width they spread over.
struct FourSpans {
  struct {
    s32 left;
    s32 width;
  } spans[4];
};

/// A row of gFrost_Timings, one for each variant.
enum {
  RISE_MOTES,   ///< the rising motes
  RISE_COLUMNS, ///< the pillars
  RISE_FRAME    ///< the frame they burst
};

/// A row of gEruption_Counts, one for each variant.
enum { FIELD_MOTES, FIELD_PUFFS, FIELD_FRAMES };

/// A growing column, laid over struct Particle in work->particles.
struct DualColumn {
  s32 x;                   ///< the root on the canvas
  s32 y;                   ///< the root on the canvas
  u8 unused[8];            ///< no code names it
  s32 height;              ///< the full length
  u8 unusedAfterHeight[4]; ///< no code names it
  s32 start;               ///< the frame it starts to grow
};

void BattleEffect_StepPaletteToResource(s32 resourceId);
void BattleEffect_WipeCanvas(s32 direction, s32 fill);
void EffectPosition_ProjectSprite(s32 id, struct ScreenPoint *position);
void BattleEffect_BuildWindowEdgeTable(void);
void BattleEffect_RunDrainMotes(struct BattleEffectParams *effect, s32 mode);
extern const struct Scale gBoreas_UnitScale;
void BattleEffect_RunStatBoost(struct BattleEffectParams *effect, u32 mode);
extern const struct RockWallHeights gRockWall_Heights;
void BattleEffect_RunEruption(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunGrowingColumns(struct BattleEffectParams *effect,
                                    s32 mode);
/// Copies `size` bytes of colour levels, halving each.
void ColorBuffer_Halve(void *source, void *destination, s32 size);
/// Copies `size` bytes of colour levels at three quarters of each.
void ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);
/// Copies `size` bytes of colour levels, each lowered by `amount`.
void ColorBuffer_Darken(void *source, s32 amount, void *destination, s32 size);
/// Copies `size` bytes of colour levels, each raised by `amount`.
void ColorBuffer_Brighten(void *source, s32 amount, void *destination,
                          s32 size);
/// The high word of value times magic, plus value times multiplier: a division
/// by a constant.
s32 Math_UnsignedMulHighAdd(s32 value, u32 magic, s32 multiplier);

// The effect handlers, in the order the handler table (gBattleEffect_Handlers)
// first lists them.

void BattleEffect_RunQuake(struct BattleEffectParams *args);
void BattleEffect_RunSpire(struct BattleEffectParams *args);
void BattleEffect_RunGaia(struct BattleEffectParams *args);
void BattleEffect_RunGrowth(struct BattleEffectParams *args);
void BattleEffect_RunThorn(struct BattleEffectParams *args);
void BattleEffect_RunFrost(struct BattleEffectParams *args);
void BattleEffect_RunIce(struct BattleEffectParams *args);
void BattleEffect_RunPrism(struct BattleEffectParams *args);
void BattleEffect_RunDouse(struct BattleEffectParams *args);
void BattleEffect_RunFroth(struct BattleEffectParams *args);
void BattleEffect_RunFlare(struct BattleEffectParams *args);
void BattleEffect_RunFire(struct BattleEffectParams *args);
void BattleEffect_RunVolcano(struct BattleEffectParams *args);
void BattleEffect_RunBlast(struct BattleEffectParams *args);
void BattleEffect_RunNova(struct BattleEffectParams *args);
void BattleEffect_RunBolt(struct BattleEffectParams *args);
void BattleEffect_RunRay(struct BattleEffectParams *args);
void BattleEffect_RunPlasma(struct BattleEffectParams *args);
void BattleEffect_RunSlash(struct BattleEffectParams *args);
void BattleEffect_RunWhirlwind(struct BattleEffectParams *args);
void BattleEffect_RunCure(struct BattleEffectParams *args);
void BattleEffect_RunPly(struct BattleEffectParams *args);
void BattleEffect_RunWish(struct BattleEffectParams *args);
void BattleEffect_RunCurePoison(struct BattleEffectParams *args);
void BattleEffect_RunRevive(struct BattleEffectParams *args);
void BattleEffect_RunImpact(struct BattleEffectParams *args);
void BattleEffect_RunDull(struct BattleEffectParams *args);
void BattleEffect_RunGuard(struct BattleEffectParams *args);
void BattleEffect_RunImpair(struct BattleEffectParams *args);
void BattleEffect_RunWard(struct BattleEffectParams *args);
void BattleEffect_RunWeaken(struct BattleEffectParams *args);
void BattleEffect_RunTaint(struct BattleEffectParams *args);
void BattleEffect_RunDelude(struct BattleEffectParams *args);
void BattleEffect_RunUnusedOrbitingSpirals(struct BattleEffectParams *args);
void BattleEffect_RunHeartrender(struct BattleEffectParams *args);
void BattleEffect_RunParalyze(struct BattleEffectParams *args);
void BattleEffect_RunSleep(struct BattleEffectParams *args);
void BattleEffect_RunBind(struct BattleEffectParams *args);
void BattleEffect_RunHaunt(struct BattleEffectParams *args);
void BattleEffect_RunCondemn(struct BattleEffectParams *args);
void BattleEffect_RunCurse(struct BattleEffectParams *args);
void BattleEffect_RunDrain(struct BattleEffectParams *args);
void BattleEffect_RunBreak(struct BattleEffectParams *args);
void BattleEffect_RunDragonCloud(struct BattleEffectParams *args);
void BattleEffect_RunDemonNight(struct BattleEffectParams *args);
void BattleEffect_RunHelmSplitter(struct BattleEffectParams *args);
void BattleEffect_RunQuickStrike(struct BattleEffectParams *args);
void BattleEffect_RunDeathPlunge(struct BattleEffectParams *args);
void BattleEffect_RunShuriken(struct BattleEffectParams *args);
void BattleEffect_RunAnnihilation(struct BattleEffectParams *args);
void BattleEffect_RunPunji(struct BattleEffectParams *args);
void BattleEffect_RunCuttingEdge(struct BattleEffectParams *args);
void BattleEffect_RunRagnarok(struct BattleEffectParams *args);
void BattleEffect_RunHeatWave(struct BattleEffectParams *args);
void BattleEffect_RunPlanetDiver(struct BattleEffectParams *args);
void BattleEffect_RunTitanBlade(struct BattleEffectParams *args);
void BattleEffect_RunShiningStar(struct BattleEffectParams *args);
void BattleEffect_RunBlizzard(struct BattleEffectParams *args);
void BattleEffect_RunDemonFire(struct BattleEffectParams *args);
void BattleEffect_RunVorpalSlash(struct BattleEffectParams *args);
void BattleEffect_RunDrown(struct BattleEffectParams *args);
void BattleEffect_RunUnusedTriangleStrike(struct BattleEffectParams *args);
void BattleEffect_RunAstralBlast(struct BattleEffectParams *args);
void BattleEffect_RunHeartbreak(struct BattleEffectParams *args);
void BattleEffect_RunAura(struct BattleEffectParams *args);
void BattleEffect_RunFume(struct BattleEffectParams *args);
void BattleEffect_RunBeam(struct BattleEffectParams *args);
void BattleEffect_RunRagingHeat(struct BattleEffectParams *args);
void BattleEffect_RunCool(struct BattleEffectParams *args);
void BattleEffect_RunSabreDance(struct BattleEffectParams *args);
void BattleEffect_RunBackstab(struct BattleEffectParams *args);
void BattleEffect_RunJuggle(struct BattleEffectParams *args);
void BattleEffect_RunFlameCard(struct BattleEffectParams *args);
void BattleEffect_RunWhiplash(struct BattleEffectParams *args);
void BattleEffect_RunPoisonFlow(struct BattleEffectParams *args);
void BattleEffect_RunFirePuppet(struct BattleEffectParams *args);
void BattleEffect_RunDiamondDust(struct BattleEffectParams *args);
void BattleEffect_RunDiamondBerg(struct BattleEffectParams *args);
void BattleEffect_RunPlumeEdge(struct BattleEffectParams *args);
void BattleEffect_RunThunderMine(struct BattleEffectParams *args);
void BattleEffect_RunFrostCard(struct BattleEffectParams *args);
void BattleEffect_RunBrambleCard(struct BattleEffectParams *args);
void BattleEffect_RunThunderCard(struct BattleEffectParams *args);
void BattleEffect_RunBaffleCard(struct BattleEffectParams *args);
void BattleEffect_RunSwordCard(struct BattleEffectParams *args);
void BattleEffect_RunSleepCard(struct BattleEffectParams *args);
void BattleEffect_RunDeathCard(struct BattleEffectParams *args);
void BattleEffect_RunStoneJustice(struct BattleEffectParams *args);
void BattleEffect_RunHeavyDivide(struct BattleEffectParams *args);
void BattleEffect_RunWyrdCurse(struct BattleEffectParams *args);
void BattleEffect_RunAcheronsGrief(struct BattleEffectParams *args);
void BattleEffect_RunHurricane(struct BattleEffectParams *args);
void BattleEffect_RunDreamtide(struct BattleEffectParams *args);
void BattleEffect_RunLetheAlbion(struct BattleEffectParams *args);
void BattleEffect_RunRisingDragon(struct BattleEffectParams *args);
void BattleEffect_RunBoostHack(struct BattleEffectParams *args);
void BattleEffect_RunRadiantFire(struct BattleEffectParams *args);
void BattleEffect_RunFireDance(struct BattleEffectParams *args);
void BattleEffect_RunBlazeRush(struct BattleEffectParams *args);
void BattleEffect_RunShred(struct BattleEffectParams *args);
void BattleEffect_RunPurgatory(struct BattleEffectParams *args);
void BattleEffect_RunSoulShatter(struct BattleEffectParams *args);
void BattleEffect_RunAgingGas(struct BattleEffectParams *args);
void BattleEffect_RunMadZephyr(struct BattleEffectParams *args);
void BattleEffect_RunLunarSlash(struct BattleEffectParams *args);
void BattleEffect_RunNirvana(struct BattleEffectParams *args);
void BattleEffect_RunLightSurge(struct BattleEffectParams *args);
void BattleEffect_RunVoidBeam(struct BattleEffectParams *args);
void BattleEffect_RunLegend(struct BattleEffectParams *args);
void BattleEffect_RunLiquifier(struct BattleEffectParams *args);
void BattleEffect_RunMegiddo(struct BattleEffectParams *args);
void BattleEffect_RunOdyssey(struct BattleEffectParams *args);
void BattleEffect_RunVengeance(struct BattleEffectParams *args);
void BattleEffect_RunThornyGrave(struct BattleEffectParams *args);
void BattleEffect_RunPlanetary(struct BattleEffectParams *args);
void BattleEffect_RunLifeShear(struct BattleEffectParams *args);
void BattleEffect_RunCrucibleFire(struct BattleEffectParams *args);
void BattleEffect_RunReverseStar(struct BattleEffectParams *args);
void BattleEffect_RunGranite(struct BattleEffectParams *args);
void BattleEffect_RunVine(struct BattleEffectParams *args);
void BattleEffect_RunGround(struct BattleEffectParams *args);
void BattleEffect_RunSleet(struct BattleEffectParams *args);
void BattleEffect_RunHail(struct BattleEffectParams *args);
void BattleEffect_RunTonic(struct BattleEffectParams *args);
void BattleEffect_RunDew(struct BattleEffectParams *args);
void BattleEffect_RunPsyCrystal(struct BattleEffectParams *args);
void BattleEffect_RunFlash(struct BattleEffectParams *args);
void BattleEffect_RunTorch(struct BattleEffectParams *args);
void BattleEffect_RunZephyr(struct BattleEffectParams *args);
void BattleEffect_RunKite(struct BattleEffectParams *args);
void BattleEffect_RunGust(struct BattleEffectParams *args);
void BattleEffect_RunIron(struct BattleEffectParams *args);
void BattleEffect_RunSteam(struct BattleEffectParams *args);
void BattleEffect_RunShade(struct BattleEffectParams *args);
void BattleEffect_RunCoal(struct BattleEffectParams *args);
void BattleEffect_RunSalt(struct BattleEffectParams *args);
void BattleEffect_RunSpring(struct BattleEffectParams *args);
void BattleEffect_RunBreath(struct BattleEffectParams *args);
void BattleEffect_RunEther(struct BattleEffectParams *args);
void BattleEffect_RunBalm(struct BattleEffectParams *args);
void BattleEffect_RunTinder(struct BattleEffectParams *args);
void BattleEffect_RunWhorl(struct BattleEffectParams *args);
void BattleEffect_RunGale(struct BattleEffectParams *args);
void BattleEffect_RunWaft(struct BattleEffectParams *args);
void BattleEffect_RunCore(struct BattleEffectParams *args);
void BattleEffect_RunMud(struct BattleEffectParams *args);
void BattleEffect_RunHaze(struct BattleEffectParams *args);
void BattleEffect_RunPetra(struct BattleEffectParams *args);
void BattleEffect_RunRime(struct BattleEffectParams *args);
void BattleEffect_RunLull(struct BattleEffectParams *args);
void BattleEffect_RunGasp(struct BattleEffectParams *args);
void BattleEffect_RunFugue(struct BattleEffectParams *args);
void BattleEffect_RunCrystal(struct BattleEffectParams *args);
void BattleEffect_RunEddy(struct BattleEffectParams *args);
void BattleEffect_RunCannon(struct BattleEffectParams *args);
void BattleEffect_RunAroma(struct BattleEffectParams *args);
void BattleEffect_RunReflux(struct BattleEffectParams *args);
void BattleEffect_RunDjinnCounter(struct BattleEffectParams *args);
void BattleEffect_RunMold(struct BattleEffectParams *args);
void BattleEffect_RunMeld(struct BattleEffectParams *args);
void BattleEffect_RunShine(struct BattleEffectParams *args);
void BattleEffect_RunGeode(struct BattleEffectParams *args);
void BattleEffect_RunSerac(struct BattleEffectParams *args);
void BattleEffect_RunSpark(struct BattleEffectParams *args);
void BattleEffect_RunFury(struct BattleEffectParams *args);
void BattleEffect_RunChar(struct BattleEffectParams *args);
void BattleEffect_RunVenus(struct BattleEffectParams *args);
void BattleEffect_RunMercury(struct BattleEffectParams *args);
void BattleEffect_RunMars(struct BattleEffectParams *args);
void BattleEffect_RunJupiter(struct BattleEffectParams *args);
void BattleEffect_RunSonicWave(struct BattleEffectParams *args);
void BattleEffect_RunShriek(struct BattleEffectParams *args);
void BattleEffect_RunBansheeHowl(struct BattleEffectParams *args);
void BattleEffect_RunCrazyVoice(struct BattleEffectParams *args);
void BattleEffect_RunWarCry(struct BattleEffectParams *args);
void BattleEffect_RunWickedHowl(struct BattleEffectParams *args);
void BattleEffect_RunWingBeat(struct BattleEffectParams *args);
void BattleEffect_RunWingFlutter(struct BattleEffectParams *args);
void BattleEffect_RunRumble(struct BattleEffectParams *args);
void BattleEffect_RunBoneChiller(struct BattleEffectParams *args);
void BattleEffect_RunBoneCharge(struct BattleEffectParams *args);
void BattleEffect_RunMysticFlame(struct BattleEffectParams *args);
void BattleEffect_RunNumbingSting(struct BattleEffectParams *args);
void BattleEffect_RunBruteForce(struct BattleEffectParams *args);
void BattleEffect_RunStickyGoo(struct BattleEffectParams *args);
void BattleEffect_RunCannibalFang(struct BattleEffectParams *args);
void BattleEffect_RunBearClaw(struct BattleEffectParams *args);
void BattleEffect_RunPoisonousBite(struct BattleEffectParams *args);
void BattleEffect_RunFlyingAttack(struct BattleEffectParams *args);
void BattleEffect_RunUndeadSword(struct BattleEffectParams *args);
void BattleEffect_RunRansack(struct BattleEffectParams *args);
void BattleEffect_RunStickyPoison(struct BattleEffectParams *args);
void BattleEffect_RunElectricBite(struct BattleEffectParams *args);
void BattleEffect_RunPoisonTail(struct BattleEffectParams *args);
void BattleEffect_RunOnslaught(struct BattleEffectParams *args);
void BattleEffect_RunVampiricFang(struct BattleEffectParams *args);
void BattleEffect_RunBacteriaRush(struct BattleEffectParams *args);
void BattleEffect_RunRottenBlood(struct BattleEffectParams *args);
void BattleEffect_RunDoubleFang(struct BattleEffectParams *args);
void BattleEffect_RunMortalBlow(struct BattleEffectParams *args);
void BattleEffect_RunFreebiteRush(struct BattleEffectParams *args);
void BattleEffect_RunTwinBeaks(struct BattleEffectParams *args);
void BattleEffect_RunRabidFang(struct BattleEffectParams *args);
void BattleEffect_RunAcidBite(struct BattleEffectParams *args);
void BattleEffect_RunDynamite(struct BattleEffectParams *args);
void BattleEffect_RunHeadbutt(struct BattleEffectParams *args);
void BattleEffect_RunPoisonInk(struct BattleEffectParams *args);
void BattleEffect_RunSoothingStar(struct BattleEffectParams *args);
void BattleEffect_RunSpiderWeb(struct BattleEffectParams *args);
void BattleEffect_RunMadSpatter(struct BattleEffectParams *args);
void BattleEffect_RunSpasm(struct BattleEffectParams *args);
void BattleEffect_RunSleepStar(struct BattleEffectParams *args);
void BattleEffect_RunDecompose(struct BattleEffectParams *args);
void BattleEffect_RunHaunting(struct BattleEffectParams *args);
void BattleEffect_RunBerserk(struct BattleEffectParams *args);
void BattleEffect_RunSmokeBomb(struct BattleEffectParams *args);
void BattleEffect_RunWingStroke(struct BattleEffectParams *args);
void BattleEffect_RunPoisonBeat(struct BattleEffectParams *args);
void BattleEffect_RunSpinningBeat(struct BattleEffectParams *args);
void BattleEffect_RunOuterSpace(struct BattleEffectParams *args);
void BattleEffect_RunDragonDriver(struct BattleEffectParams *args);
void BattleEffect_RunDrainFang(struct BattleEffectParams *args);
void BattleEffect_RunSevereBlow(struct BattleEffectParams *args);
void BattleEffect_RunThrash(struct BattleEffectParams *args);
void BattleEffect_RunRecovery(struct BattleEffectParams *args);
void BattleEffect_RunEchoCut(struct BattleEffectParams *args);
void BattleEffect_RunWateryGrave(struct BattleEffectParams *args);
void BattleEffect_RunRagingFlood(struct BattleEffectParams *args);
void BattleEffect_RunHydroSlash(struct BattleEffectParams *args);
void BattleEffect_RunMightyPress(struct BattleEffectParams *args);
void BattleEffect_RunSlaver(struct BattleEffectParams *args);
void BattleEffect_RunCounterRush(struct BattleEffectParams *args);
void BattleEffect_RunDjinnfest(struct BattleEffectParams *args);
void BattleEffect_RunHeatKiss(struct BattleEffectParams *args);
void BattleEffect_RunAngleSpike(struct BattleEffectParams *args);
void BattleEffect_RunFireBreath(struct BattleEffectParams *args);
void BattleEffect_RunWaterBreath(struct BattleEffectParams *args);
void BattleEffect_RunIceBlessing(struct BattleEffectParams *args);
void BattleEffect_RunDarkBreath(struct BattleEffectParams *args);
void BattleEffect_RunAcidBreath(struct BattleEffectParams *args);
void BattleEffect_RunStormBreath(struct BattleEffectParams *args);
void BattleEffect_RunEvilBlessing(struct BattleEffectParams *args);
void BattleEffect_RunDeadlyGas(struct BattleEffectParams *args);
void BattleEffect_RunSandBreath(struct BattleEffectParams *args);
void BattleEffect_RunDesertGasp(struct BattleEffectParams *args);
void BattleEffect_RunBlackIce(struct BattleEffectParams *args);
void BattleEffect_RunTyphoonBlow(struct BattleEffectParams *args);
void BattleEffect_RunToxicBreath(struct BattleEffectParams *args);
void BattleEffect_RunGravelBlow(struct BattleEffectParams *args);
void BattleEffect_RunBlastBreath(struct BattleEffectParams *args);
void BattleEffect_RunDarksolGasp(struct BattleEffectParams *args);
void BattleEffect_RunManeater(struct BattleEffectParams *args);
void BattleEffect_RunUrFlash(struct BattleEffectParams *args);
void BattleEffect_RunCrusherGrip(struct BattleEffectParams *args);
void BattleEffect_RunVanishClaw(struct BattleEffectParams *args);
void BattleEffect_RunTwinShear(struct BattleEffectParams *args);
void BattleEffect_RunTripleChomp(struct BattleEffectParams *args);
void BattleEffect_RunDoublestep(struct BattleEffectParams *args);
void BattleEffect_RunPoisonGel(struct BattleEffectParams *args);
void BattleEffect_RunRisingVenom(struct BattleEffectParams *args);
void BattleEffect_RunSack(struct BattleEffectParams *args);
void BattleEffect_RunFireDanceUnleash(struct BattleEffectParams *args);
void BattleEffect_RunStunMuscle(struct BattleEffectParams *args);
void BattleEffect_RunRollingFlame(struct BattleEffectParams *args);
void BattleEffect_RunCage(struct BattleEffectParams *args);
void BattleEffect_RunPhoenix(struct BattleEffectParams *args);
void BattleEffect_RunHumanHunt(struct BattleEffectParams *args);
void BattleEffect_RunUnusedHeavyBite(struct BattleEffectParams *args);
void BattleEffect_RunUnusedFlashBite(struct BattleEffectParams *args);
void BattleEffect_RunKillSting(struct BattleEffectParams *args);
void BattleEffect_RunBeastNeedle(struct BattleEffectParams *args);
void BattleEffect_RunPoisonSting(struct BattleEffectParams *args);
void BattleEffect_RunStunSting(struct BattleEffectParams *args);
void BattleEffect_RunDemonEye(struct BattleEffectParams *args);
void BattleEffect_RunEmu(struct BattleEffectParams *args);
void BattleEffect_RunOceanFist(struct BattleEffectParams *args);
void BattleEffect_RunCallDullahan(struct BattleEffectParams *args);
void BattleEffect_RunGhostSoldier(struct BattleEffectParams *args);
void BattleEffect_RunRisingDragonEnemy(struct BattleEffectParams *args);
void BattleEffect_RunTrueCollide(struct BattleEffectParams *args);
void BattleEffect_RunStarMine(struct BattleEffectParams *args);
void BattleEffect_RunFormicAcid(struct BattleEffectParams *args);
void BattleEffect_RunClawAttack(struct BattleEffectParams *args);
void BattleEffect_RunBeatDance(struct BattleEffectParams *args);
void BattleEffect_RunUnusedSparkBite(struct BattleEffectParams *args);
void BattleEffect_RunLivingArmor(struct BattleEffectParams *args);
void BattleEffect_RunHeatStun(struct BattleEffectParams *args);
void BattleEffect_RunEarthForce(struct BattleEffectParams *args);
void BattleEffect_RunGuardAura(struct BattleEffectParams *args);
void BattleEffect_RunDjinnBlast(struct BattleEffectParams *args);
void BattleEffect_RunDjinnStorm(struct BattleEffectParams *args);
void BattleEffect_RunMineBall(struct BattleEffectParams *args);
void BattleEffect_RunElementSwap(struct BattleEffectParams *args);
void BattleEffect_RunDjinnStun(struct BattleEffectParams *args);
void BattleEffect_RunMysticCall(struct BattleEffectParams *args);
void BattleEffect_RunClarionCry(struct BattleEffectParams *args);
void BattleEffect_RunPsyBoost(struct BattleEffectParams *args);
void BattleEffect_RunStrongHit(struct BattleEffectParams *args);
void BattleEffect_RunChillBreath(struct BattleEffectParams *args);
void BattleEffect_RunStormBreathStrong(struct BattleEffectParams *args);

#endif
