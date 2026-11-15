#ifndef ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H
#define ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H

/// @file
/// What the battle_effect object defines for the code outside it.

#include "types.h"
#include "field/particle.h"
#include "battle/battle_effect.h"
#include "battle/battle_unit.h"

struct BattleEffectParams;
struct ScreenPoint;
struct Particle;
struct FxPartyList;
struct MeshTriangle;
struct MeshVertex;
struct PoolEntry;
struct StripQuad;
struct SummonArgument;

/// The large effects BattleEffect_RunSummon runs, by kind: The Broken
/// Seal's twelve summons, The Lost Age's twelve, and the scenes of Daedalus's
/// second strike, Trident, Cruel Ruin and Meteor Blow.
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

enum SummonEffectId {
  SUMMON_EFFECT_NONE,         ///< runs Meteor's effect
  SUMMON_EFFECT_RAMSES,       ///< BattleEffect_RunRamses (ramses.c)
  SUMMON_EFFECT_NEREID,       ///< BattleEffect_RunNereid (nereid.c)
  SUMMON_EFFECT_KIRIN,        ///< BattleEffect_RunKirin (kirin.c)
  SUMMON_EFFECT_ATALANTA,     ///< BattleEffect_RunAtalanta (atalanta.c)
  SUMMON_EFFECT_CYBELE,       ///< BattleEffect_RunCybele (cybele.c)
  SUMMON_EFFECT_NEPTUNE,      ///< BattleEffect_RunNeptune (neptune.c)
  SUMMON_EFFECT_TIAMAT,       ///< BattleEffect_RunTiamat (procne.c)
  SUMMON_EFFECT_PROCNE,       ///< BattleEffect_RunProcne (procne.c)
  SUMMON_EFFECT_JUDGMENT,     ///< BattleEffect_RunJudgment (judgment.c)
  SUMMON_EFFECT_BOREAS,       ///< BattleEffect_RunBoreas (boreas.c)
  SUMMON_EFFECT_METEOR,       ///< BattleEffect_RunMeteor (particle_streams.c)
  SUMMON_EFFECT_THOR,         ///< BattleEffect_RunThor (thor.c)
  SUMMON_EFFECT_ZAGAN,        ///< BattleEffect_RunZagan (djinn_heal.c)
  SUMMON_EFFECT_MEGAERA,      ///< BattleEffect_RunMegaera (thunder_mine.c)
  SUMMON_EFFECT_FLORA,        ///< BattleEffect_RunFlora (flora.c)
  SUMMON_EFFECT_MOLOCH,       ///< BattleEffect_RunMoloch (moloch.c)
  SUMMON_EFFECT_ULYSSES,      ///< BattleEffect_RunUlysses (spider_web.c)
  SUMMON_EFFECT_HAURES,       ///< BattleEffect_RunHaures (haures.c)
  SUMMON_EFFECT_ECLIPSE,      ///< BattleEffect_RunEclipse (canvas.c)
  SUMMON_EFFECT_COATLICUE,    ///< BattleEffect_RunCoatlicue (vengeance.c)
  SUMMON_EFFECT_DAEDALUS,     ///< BattleEffect_RunDaedalus (daedalus.c)
  SUMMON_EFFECT_AZUL,         ///< BattleEffect_RunAzul (azul.c)
  SUMMON_EFFECT_CATASTROPHE,  ///< BattleEffect_RunCatastrophe (catastrophe.c)
  SUMMON_EFFECT_CHARON,       ///< BattleEffect_RunCharon (charon.c)
  SUMMON_EFFECT_IRIS,         ///< BattleEffect_RunIris (moloch.c)
  SUMMON_EFFECT_DAEDALUS_ALT, ///< BattleEffect_RunDaedalusAlt (daedalus.c)
  SUMMON_EFFECT_TRIDENT,      ///< BattleEffect_RunTrident (trident.c)
  SUMMON_EFFECT_CRUEL_RUIN,   ///< BattleEffect_RunCruelRuin (cruel_ruin.c)
  SUMMON_EFFECT_METEOR_BLOW   ///< BattleEffect_RunMeteorBlow (meteor_blow.c)
};

typedef DrawRectangle BlitterPair[2];

// A texture as the render jobs take it: 1 << logWidth by 1 << logHeight.
struct Texture {
  u8 logWidth;
  u8 logHeight;
  u16 unk02;
  void *pixels;
};

struct FxSlot {
  void *object;
  u8 unk04[28];
  struct FxGroupEntry *animationEntry;
  struct FxGroupEntry *effectEntry;
  u8 unk28[2];
  u8 fading;
};

/// A point or a velocity, 16.16 pixels.
struct Point2D {
  s32 x;
  s32 y;
};

/// A sprite's scale across and down, 16.16, as an effect keeps it in a local
/// (struct SpriteScale is the same pair as a union with an array).
struct Scale {
  s32 x;
  s32 y;
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
  KNOCKBACK_JOLT    ///< the spot itself, a hop of 5.5
};

union BattleBackgroundId {
  u16 id;
  u8 bytes[6];
};

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: BattleEffect_BuildBlitter builds additive for 0, max for 1,
// transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

void BattleEffect_UpdatePhasePalette(
    s32 frame, s32 redPhase, s32 greenPhase,
    s32 bluePhase);                       // Graphics_UpdatePhasePalette
extern u16 gBattleEffect_FlarePictures[]; // flare sizes 1 to 5: cell offsets
/// The canvas layer and its helpers; the comments give The Broken Seal's
/// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);
void BattleEffect_DrawShadedCircles(u8 *map, s32 size, s32 scale);
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectParams *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Canvas_BlitAtFoot(void *, s32, s32, s32);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void EffectPosition_ProjectBattler(s32 id, struct ScreenPoint *position);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct Particle *step, s32 gravity,
                                          s32 drag);
void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct Particle *step, s32 damping,
                                          s32 gravity);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
extern u8 Data_02011000[];
extern u8
    Data_02012000[]; // the crystal texture and the flashes, after the map cells
extern u16 gBattleEffect_MotePictures[]; // dot sizes 1
                                         // to 10, cell offsets in the sheet
extern u16 gBattleEffect_GlintPictures[];
extern u8 gBattleEffect_GlintPictureWidths[];
extern u8 gBattleEffect_GlintPictureHeights[];
extern const u8 gBattleEffect_TaperedBeamCorners[];
extern const u8 gBattleEffect_BeamCorners[];
extern const u8 gBattleEffect_GroundSquareCorners[]; // a quad's four corners, x
                                                     // y z for each
extern const u16 gBattleEffect_SmallSquareMesh[];
extern const u16 gBattleEffect_SquareMesh[]; // the quad's face
void RagingHeat_FillGradientCanvas(void);
void BattleEffect_AdvanceScrollOnInterval(void);
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);
void Palette_BrightenBattleBg(s32 r, s32 g, s32 b);
void Palette_BrightenSavedBattleBg(s32 red, s32 green, s32 blue);
void Mesh_BuildCylinderBand(struct MeshVertex *vertices,
                            struct MeshTriangle *triangles, s32 segments);
void Graphics_LoadGreyRamp(void); // a grey ramp from color 1
void Mesh_BuildGrid(struct MeshTriangle *triangles, s32 columns, s32 rows,
                    s32 cellWidth, s32 cellHeight);
// The models: the rings' quad (gBattleEffect_SmallSquareMesh), its corners, the
// crystal and its corners.
extern const u16 gBattleEffect_RingMesh[];
extern const u8 gBattleEffect_RingCorners[];
extern char ResourceId_Data0DA;
void Palette_BrightenBgEntries(s32 red, s32 green, s32 blue);
void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_ClearActorTimers(void);
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color);
void BattleEffect_SetupCanvasWindows(void); // the BG2 canvas's registers
extern u8 gData_02014000[];
extern const u8 gBattleEffect_SquareCorners[];
extern const u16 gBattleEffect_LargeSquareMesh[];
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
void BattleEffect_RunDaedalus(struct SummonArgument *arg);
void BattleEffect_RunAzul(struct BattleEffectParams *effect);
void BattleEffect_RunCatastrophe(struct BattleEffectParams *effect);
void BattleEffect_RunCharon(struct BattleEffectParams *arg);
void BattleEffect_RunIris(struct BattleEffectParams *effect);
void BattleEffect_RunDaedalusAlt(struct BattleEffectParams *effect);
void BattleEffect_RunTrident(struct BattleEffectParams *effect);
void BattleEffect_RunCruelRuin(struct BattleEffectParams *effect);
void BattleEffect_RunMeteorBlow(struct BattleEffectParams *effect);
extern char ResourceId_ParticleSpritesA; // The Broken Seal's ParticleSpritesA
extern char ResourceId_PetalSheet;
extern char ResourceId_Data0C1;
extern const u8 gBattleEffect_CylinderCorners[];
extern const u16 gBattleEffect_CylinderMesh[];
// The sparkle puff's size and cell for each of its nine pictures
// (gBattleEffect_SparkleSizes and gBattleEffect_SparklePictures).
extern u8 gBattleEffect_SparkleSizes[];
extern u16 gBattleEffect_SparklePictures[];
void BattleEffect_RunDjinnBeams(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunLobbedShells(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunImpair(void *action);
void BattleEffect_RunDull(void *action);
void BattleEffect_RunGatheringLights(struct BattleEffectParams *effect,
                                     s32 mode);
void BattleEffect_ArmBg1HBlankDma(void); // arms the BG1 scroll's HBlank DMA
void Palette_MirrorBgColors(void);       // mode 3 only
extern s32 gPrism_ShardPictures[];
extern u8 gPrism_ShardWidths[];
extern u8 gPrism_ShardHeights[];
extern u8 gPrism_ShardStarts[]; // x, y for each shard
void BattleEffect_SetupBlendedDisplay(void);
void BattleEffect_RunDragonFire(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x,
                                        s32 y); // ..PlaceFormationObjects
void BattleEffect_RunImpactBurst(s32 formation, s32 x,
                                 s32 y); // ..RunImpactBurst
void BattleEffect_RunStreakVolley(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunMoteFountain(struct BattleEffectParams *effect, s32 mode);
extern char ResourceId_SheepCardSheet; // directory row 0xe3
void BattleEffect_GetTargetsMidpoint(struct ScreenPoint *midpoint);
void BattleEffect_RunTumblingQuads(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunTargetStrike(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);
extern char ResourceId_StarCrossSheet;
extern char ResourceId_CrossedDaggersSheet;
void BattleEffect_RunCloudSwirl(struct BattleEffectParams *effect);
void BattleCamera_ApplyPhasedDelta(void);
typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);
typedef struct {
  s32 reserved00[3];
  s32 currentValue;
  s32 indexRadius;
  s32 entryLimit;
  s32 reserved18[3];
  s16 values[1];
} BattleEffectIndexWindow;
/// BattleEffect_RunDjinniEntrance's longer entrance, added to the Djinni's
/// element: 84 frames in place of 55.
enum { DJINNI_ENTRANCE_LONG = ELEMENT_COUNT };

/// Where BattleEffect_RunDjinniEntrance's Djinni appears: the first five as The
/// Broken Seal's, 60 pixels up, and this game's own after them.
enum DjinniEntranceAnchor {
  DJINNI_ANCHOR_OVER_TARGET, ///< over the first target
  DJINNI_ANCHOR_TARGET_X,    ///< at the first target's x, at depth 0
  DJINNI_ANCHOR_OVER_USER,
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
#define DJINNI_ENTRANCE_ANCHOR(packed) (((packed) >> 5) & 15)

// Places in the sheet.
enum SeriesBSheet {
  SERIES_B_FLARES = 0x0000,  // the gathering lights
  SERIES_B_BURST = 0x060e,   // six 40x40 bursts
  SERIES_B_PICTURE = 0x2b8e, // the mode's picture
  SERIES_B_DJINNI = 0x65c0   // the 40x40 Djinni
};

// A row of gQuake_Timings, one for each variant.
enum {
  TIMING_FRAMES,      // the whole effect
  TIMING_RISE_FRAMES, // how long each pillar stands
  TIMING_SHAKE        // the camera shake
};

struct RockWallHeights {
  s32 column[4]; // each pillar's height in pixels
};

// A row of gFrost_Timings, one for each variant.
enum {
  RISE_MOTES,   // the rising motes
  RISE_COLUMNS, // the pillars
  RISE_FRAME    // the frame they burst
};

// A row of gEruption_Counts, one for each variant.
enum { FIELD_MOTES, FIELD_PUFFS, FIELD_FRAMES };

// Places in the sheet.
enum VolleyBSheet {
  VOLLEY_B_SHOTS = 0x0000,   // the mode's own shot pictures
  VOLLEY_B_BEAM = 0x1000,    // the beam, doubled to 48 wide
  VOLLEY_B_GLOW = 0x1492,    // mode 20's glows
  VOLLEY_B_PICTURE = 0x0c56, // the mode's picture
  VOLLEY_B_DJINNI = 0x65c0   // the 20x40 Djinni
};

/// The pictures in the slash sheet: five flash pictures, then the shaft in
/// three parts, with the sizes the draws use.
enum SlashSheetPicture {
  /// 14x28
  ATALANTA_SHAFT_FLASH_1 = 0,
  /// 23x44
  ATALANTA_SHAFT_FLASH_2 = ATALANTA_SHAFT_FLASH_1 + 14 * 28,
  /// 20x30
  ATALANTA_SHAFT_FLASH_3_TOP = ATALANTA_SHAFT_FLASH_2 + 23 * 44,
  /// 22x33
  ATALANTA_SHAFT_FLASH_3_BOTTOM = ATALANTA_SHAFT_FLASH_3_TOP + 20 * 30,
  /// 18x27
  ATALANTA_SHAFT_FLASH_4_TOP = ATALANTA_SHAFT_FLASH_3_BOTTOM + 22 * 33,
  /// 22x22
  ATALANTA_SHAFT_FLASH_4_MIDDLE = ATALANTA_SHAFT_FLASH_4_TOP + 18 * 27,
  /// 19x28
  ATALANTA_SHAFT_FLASH_4_BOTTOM = ATALANTA_SHAFT_FLASH_4_MIDDLE + 22 * 22,
  /// 16x23
  ATALANTA_SHAFT_FLASH_5_TOP = ATALANTA_SHAFT_FLASH_4_BOTTOM + 19 * 28,
  /// 23x28
  ATALANTA_SHAFT_FLASH_5_MIDDLE = ATALANTA_SHAFT_FLASH_5_TOP + 16 * 23,
  /// 20x28
  ATALANTA_SHAFT_FLASH_5_BOTTOM = ATALANTA_SHAFT_FLASH_5_MIDDLE + 23 * 28,
  /// 16x21
  ATALANTA_SHAFT_TOP = ATALANTA_SHAFT_FLASH_5_BOTTOM + 20 * 28,
  /// 29x35
  ATALANTA_SHAFT_MIDDLE = ATALANTA_SHAFT_TOP + 16 * 21,
  /// 21x24
  ATALANTA_SHAFT_BOTTOM = ATALANTA_SHAFT_MIDDLE + 29 * 35
};

// A growing column, laid over struct Particle in work->particles.
struct DualColumn {
  s32 x; // root on the canvas
  s32 y;
  u8 unk08[8];
  s32 height; // full length
  u8 unk14[4];
  s32 start; // the frame it starts to grow
};

extern char ResourceId_EffectPalette01;
extern char ResourceId_Data0C2;
extern u8 gBattleEffect_PuffArcPictureWidths[];
extern u8 gBattleEffect_PuffArcPictureHeights[];
extern u8 gBattleEffect_PuffArcPictureBiasY[];
extern u16 gBattleEffect_PuffArcPictures[];
extern u8 gSpire_ShardWidths[];
extern u8 gSpire_ShardHeights[]; // the shard heights
extern u16 gSpire_ShardPictures[];
void BattleEffect_StepPaletteToResource(s32);
extern char ResourceId_Data0C9;
extern char ResourceId_VioletPaletteC;
extern char ResourceId_Data0D7;
void BattleEffect_WipeCanvas(s32, s32);
void EffectPosition_ProjectSprite(s32 id, struct ScreenPoint *position);
// The quad meshes and corners: the card, the pictures and their spread.
extern const u16 gBattleEffect_TallMesh[];
// The dagger's shards, ☀️'s palette ramp shards.
extern u16 gStatChange_ShardPictures[];
extern u8 gStatChange_ShardWidths[];
extern u8 gStatChange_ShardHeights[];
void BattleEffect_BuildWindowEdgeTable(void);
extern char ResourceId_Data0CB;
extern const u8 gLiquifier_SplashStarts[];
extern const u8 gLiquifier_SplashHeights[];
extern const u8 gLiquifier_SplashCorners[];
extern const u8 gCanvasBurst_FlamePictures[];
void BattleEffect_RunDrainMotes(struct BattleEffectParams *effect, s32 mode);
extern u8 gCybele_ObjectPoses[];
extern u16 gCybele_ThornPictures[];
extern u8 gCybele_ThornWidths[];
extern u8 gCybele_ThornHeights[];
extern u16 gCybele_FlowerPictures[];
extern u8 gCybele_FlowerSizes[];
extern u16 gCybele_ChipPictures[];
extern u8 gCybele_ChipWidths[];
extern u8 gCybele_ChipHeights[];
extern u16 gCybele_SparkPictures[];
extern u8 gCybele_SparkSizes[];
extern u16 gCybele_BlastPictures[];
extern u16 gCybele_BlastSizes[];
extern u8 gBoreas_Points[][2];
extern s8 gBoreas_ShakeOffsets[];
extern u16 gBoreas_CrystalPictures[];
extern u8 gBoreas_CrystalWidths[];
extern u8 gBoreas_CrystalHeights[];
extern u8 gBoreas_Animations[];
extern struct Scale gBoreas_UnitScale;
extern u16 gBoreas_DebrisPictures[];
extern u8 gBoreas_DebrisWidths[];
extern u8 gBoreas_DebrisHeights[];
extern u16 gBoreas_SprayPictures[];
extern u8 gBoreas_SprayWidths[];
extern u8 gBoreas_SprayHeights[];
extern u16 gBoreas_RisingCrystalPictures[];
extern u8 gBoreas_RisingCrystalWidths[];
extern u8 gBoreas_RisingCrystalHeights[];
extern u16 gBoreas_FallPictures[];
extern u8 gBoreas_FallWidths[];
extern u8 gBoreas_FallHeights[];
extern s32 gDrain_Gravities[];
extern u8 gDrainMotes_Sizes[]; // the mote size by mode
void BattleEffect_RunStatBoost(struct BattleEffectParams *effect, u32 mode);
extern u8 gQuake_Timings[][3];
extern struct RockWallHeights gRockWall_Heights;
extern u8 gFire_Counts[];
extern u16 gFire_FlarePictures[];
extern u8 gFire_FlareWidths[];
extern u8 gFire_FlareBiasY[];
extern u8 gFire_FlareHeights[];
extern u8 gFrost_Timings[];
extern u8 gFrost_MoundSpots[];
extern u16 gFrost_MoteCells[];
extern u8 gFrost_MoteWidths[];
extern u8 gFrost_MoteHeights[];
extern u16 gFrost_MoundPictures[];
extern u8 gFrost_MoundHeights[];
extern u16 gFrost_ChipCells[];
extern u8 gFrost_ChipWidths[];
extern u8 gFrost_ChipHeights[];
extern u8 gRay_Sparks[][2];
void BattleEffect_RunEruption(struct BattleEffectParams *effect, s32 mode);
extern u8 gEruption_Counts[][3];
extern u16 gEruption_PuffPictures[];
extern u16 gEruption_PuffSizes[];
extern u8 gProjectileVolleyB_GlintFlips[];
extern char ResourceId_LeafWarriorTiles;
extern s32 gAtalanta_UnitScale[];
void BattleEffect_RunGrowingColumns(struct BattleEffectParams *effect,
                                    s32 mode);
// The Broken Seal's gGrowth_* tables, in effect_table.s.
extern u8 gGrowth_ColumnCounts[];    // _ColumnCounts: columns by strength
extern u8 gGrowth_ColumnX[];         // _ColumnX: each column's root
extern u8 gGrowth_HeightLimits[];    // _HeightLimits: by i % 3
extern u16 gGrowth_VinePictures[];   // _VineCells
extern u8 gGrowth_VineWidths[];      // _VineWidths
extern s8 gGrowth_PillarHeights[];   // _PillarHeights: by i & 7
extern u16 gGrowth_PillarPictures[]; // _PillarCells
extern u8 gGrowth_PillarWidths[];    // _PillarWidths
extern u16 gGrowth_PuffPictures[];   // _PuffCells
extern s8 gGrowth_PuffWidths[];      // _PuffWidths
extern u8 gGrowth_PuffHeights[];     // _PuffHeights
// Each vine's x about the
// middle of the targets.
extern const s8 gGrowth_ColumnOffsets[];
// Thorns by strength.
extern u8 gGrowth_Counts[];
// Each thorn's root.
extern s8 gGrowth_X[];
// 0-1 large, 2-3 small; odd ones
// grow to the left.
extern u8 gGrowth_Kinds[];
// The Broken Seal's table of this name, _PuffCells, _PuffWidths:
// the seven dust cels.
extern u8 gGrowth_ThornPuffHeights[];
extern u16 gGrowth_ThornPuffPictures[];
extern s8 gGrowth_ThornPuffWidths[];
// Four bytes for each strength,
// sparks, glints, glint spread and bolts.
extern u8 gBolt_Counts[];
// The Broken Seal's gBolt_GlintFlips and _GlintModes.
extern u8 gBolt_GlintFlips[];
extern u8 gBolt_GlintModes[];
struct Streak {
  s32 head;
  s32 tail;
  s32 unused08;
  s32 pitch;
  s32 yaw;
  s32 roll;
  s32 unused18;
};

struct FxPartyList {
  u8 unk00[20];
  s32 count;
  u8 unk18[12];
  s16 ids[8];
};

struct FxGroupEntry {
  u8 unk00[5];
  u8 param;
  u8 unk06[16];
  u8 frame;
};

void ColorBuffer_Halve(void *source, void *destination, s32 size);
void ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_Darken(void *source, s32 amount, void *destination, s32 size);
void ColorBuffer_Brighten(void *source, s32 amount, void *destination,
                          s32 size);
s32 Math_UnsignedMulHighAdd(s32 value, u32 magic, s32 multiplier);

#endif
