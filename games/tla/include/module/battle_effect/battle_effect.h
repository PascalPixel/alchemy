#ifndef ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H
#define ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H

/// @file
/// What the battle_effect object defines for the code outside it.

#include "types.h"
#include "field/effect_step.h"
#include "battle/battle_effect.h"

struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;
struct FxPartyList;
struct MeshTriangle;
struct MeshVertex;
struct PoolEntry;
struct StripQuad;
struct SummonArgument;

typedef DrawRectangle BlitterPair[2];
struct WordPair {
  s32 a;
  s32 b;
};

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

// The camera work, as far as this effect reads it.
struct Point2D {
  s32 x;
  s32 y;
};

struct Scale {
  s32 x;
  s32 y;
};

// The battle background's id, read through a union view of the bytes that
// begin with it. A union's member has the union's alias set, which conflicts
// with every store, so the read waits for view->secondMode's store as well
// as the scroll's and the scheduler keeps the two stores in source order; as
// *(u16 *)(battle)->unk670, 4 of 376 instructions differ in tla-en.
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
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Canvas_BlitAtFoot(void *, s32, s32, s32);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // ☀️ SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
extern u8 Data_02011000[];
extern u8
    Data_02012000[]; // the crystal texture and the flashes, after the map cells
extern u16 gBattleEffect_MoteCells[]; // ☀️ gBattleEffect_MoteCells: dot sizes 1
                                      // to 10, cell offsets in the sheet
extern u16 gBattleEffect_GlintCells[];
extern u8 gData_08197492[]; // ☀️ gBattleEffect_GlintCellWidths
extern u8 gData_08197498[]; // ☀️ gBattleEffect_GlintCellHeights
extern const u8 gData_081991b0[];
extern const u8 Data_081991c0[];
extern const u8 gData_08199210[]; // a quad's four corners, x y z for each
extern const u8 Data_08199220[];
extern const u8 gData_08199340[]; // the quad's face
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
// The models: the rings' quad (Data_08199220), its corners, the crystal and its
// corners.
extern const u8 gData_081990d0[];
extern const u8 gData_08199090[];
extern char ResourceId_Data0DA;
void Palette_BrightenBgEntries(s32 red, s32 green, s32 blue);
void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_ClearActorTimers(void);
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color);
void BattleEffect_SetupCanvasWindows(void); // the BG2 canvas's registers
extern u8 gData_02014000[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199364[];
void BattleEffect_DrawShadedCirclesMirrored(u8 *map, s32 size, s32 scale);
void BattleEffect_RunRamses(struct BattleEffectArgument *effect);
void BattleEffect_RunNereid(struct BattleEffectArgument *effect);
void BattleEffect_RunKirin(struct BattleEffectArgument *effect);
void BattleEffect_RunAtalanta(struct BattleEffectArgument *effect);
void BattleEffect_RunNeptune(struct BattleEffectArgument *effect);
void BattleEffect_RunProcne(struct BattleEffectArgument *effect);
void BattleEffect_RunJudgment(struct BattleEffectArgument *effect);
void BattleEffect_RunBoreas(struct BattleEffectArgument *effect);
void BattleEffect_RunThor(struct BattleEffectArgument *effect);
void BattleEffect_RunMegaera(struct BattleEffectArgument *effect);
void BattleEffect_RunFlora(struct BattleEffectArgument *effect);
void BattleEffect_RunMoloch(struct BattleEffectArgument *effect);
void BattleEffect_RunUlysses(struct BattleEffectArgument *effect);
void BattleEffect_RunHaures(struct BattleEffectArgument *effect);
void BattleEffect_RunEclipse(struct BattleEffectArgument *effect);
void BattleEffect_RunCoatlicue(struct BattleEffectArgument *effect);
void BattleEffect_RunDaedalus(struct SummonArgument *arg);
void BattleEffect_RunAzul(struct BattleEffectArgument *effect);
void BattleEffect_RunCatastrophe(struct BattleEffectArgument *effect);
void BattleEffect_RunCharon(struct BattleEffectArgument *arg);
void BattleEffect_RunIris(struct BattleEffectArgument *effect);
void BattleEffect_RunDaedalusAlt(struct BattleEffectArgument *effect);
void BattleEffect_RunTrident(struct BattleEffectArgument *effect);
void BattleEffect_RunCruelRuin(struct BattleEffectArgument *effect);
void BattleEffect_RunMeteorBlow(struct BattleEffectArgument *effect);
extern char ResourceId_Data134; // The Broken Seal's ParticleSpritesA
extern char ResourceId_PetalSheet;
extern char ResourceId_Data0C1;
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
// The sparkle puff's size and cell for each of its nine pictures
// (gBattleEffect_PuffSizes and gBattleEffect_PuffCells).
extern u8 gBattleEffect_PuffSizes[];
extern u16 gBattleEffect_PuffCells[];
void BattleEffect_RunDjinnBeams(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunLobbedShells(struct BattleEffectArgument *effect,
                                  s32 mode);
void SummonEffect_RunCanvasMode1(void *action);
void SummonEffect_RunCanvasMode3(void *action);
void BattleEffect_RunGatheringLights(struct BattleEffectArgument *effect,
                                     s32 mode);
void BattleEffect_ArmBg1HBlankDma(void); // arms the BG1 scroll's HBlank DMA
void Palette_MirrorBgColors(void);       // mode 3 only
extern s32 gData_08197834[];             // ☀️ gPrism_ShardOffsets
extern u8 gPrism_ShardWidths[];
extern u8 gData_08197826[];     // ☀️ gPrism_ShardHeights
extern u8 gPrism_ShardStarts[]; // ☀️ gPrism_ShardStarts: x, y for each shard
void BattleEffect_SetupBlendedDisplay(void);
void BattleEffect_RunDragonFire(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x,
                                        s32 y); // ..PlaceFormationObjects
void BattleEffect_RunImpactBurst(s32 formation, s32 x,
                                 s32 y); // ..RunImpactBurst
void BattleEffect_RunStreakVolley(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleEffect_RunMoteFountain(struct BattleEffectArgument *effect,
                                  s32 mode);
extern char ResourceId_SheepCardSheet; // directory row 0xe3
void BattleEffect_GetTargetsMidpoint(struct EffectPosition *midpoint);
void BattleEffect_RunTumblingQuads(struct BattleEffectArgument *effect,
                                   s32 mode);
void BattleEffect_RunTargetStrike(struct BattleEffectArgument *effect,
                                  s32 mode);
void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);
extern char ResourceId_StarCrossSheet;
extern char ResourceId_CrossedDaggersSheet;
void BattleEffect_RunCloudSwirl(struct BattleEffectArgument *effect);
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
// BattleEffect_RunDjinniEntrance's packed argument: the Djinni's element, the
// side, and whether the Djinni stands at the anchor.
enum { DJINNI_VENUS, DJINNI_MERCURY, DJINNI_MARS, DJINNI_JUPITER };

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

// Places in the slash sheet.
enum SlashSheetCell {
  SLASH_FLASH_1 = 0x0000,
  SLASH_FLASH_2 = 0x0188,
  SLASH_FLASH_3_TOP = 0x057c,
  SLASH_FLASH_3_BOTTOM = 0x07d4,
  SLASH_FLASH_4_TOP = 0x0aaa,
  SLASH_FLASH_4_MIDDLE = 0x0c90,
  SLASH_FLASH_4_BOTTOM = 0x0e74,
  SLASH_FLASH_5_TOP = 0x1088,
  SLASH_FLASH_5_MIDDLE = 0x11f8,
  SLASH_FLASH_5_BOTTOM = 0x147c,
  SLASH_TOP = 0x16ac,
  SLASH_MIDDLE = 0x17fc,
  SLASH_BOTTOM = 0x1bf3
};

// A growing column, laid over struct EffectStep in work->particles.
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
extern u8 gData_08197467[];
extern u8 gBattleEffect_PuffArcCellHeights[];
extern u8 gBattleEffect_PuffArcCellBiasY[];
extern u16 gBattleEffect_PuffArcCells[];
extern u8 gSpire_ShardWidths[];
extern u8 gData_081974ad[]; // the shard heights
extern u16 gSpire_ShardCells[];
void BattleEffect_StepPaletteToResource(s32);
extern char ResourceId_Data0C9;
extern char ResourceId_VioletPaletteC;
extern char ResourceId_Data0D7;
void BattleEffect_WipeCanvas(s32, s32);
void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position);
// The quad meshes and corners: the card, the pictures and their spread.
extern u8 Data_081992b0[];
// The dagger's shards, ☀️'s palette ramp shards.
extern u16 gStatChange_ShardCells[];
extern u8 gStatChange_ShardWidths[];
extern u8 gStatChange_ShardHeights[];
void BattleEffect_BuildWindowEdgeTable(void);
extern char ResourceId_Data0CB;
extern const u8 Data_08199eb4[];
extern const u8 Data_08199ebc[];
extern const u8 Data_08199ea4[];
extern const u8 Data_08199ec5[];
void BattleEffect_RunDrainMotes(struct BattleEffectArgument *effect, s32 mode);
extern u8 gCybele_ObjectPoses[];
extern u16 gCybele_ThornPictures[]; // ☀️ gCybele_ThornOffsets
extern u8 Data_0819820c[];          // ☀️ gCybele_ThornWidths
extern u8 Data_08198211[];          // ☀️ gCybele_ThornHeights
extern u16 gCybele_LeafOffsets[];
extern u8 gCybele_LeafSizes[];
extern u16 gCybele_ChipPictures[]; // ☀️ the second gCybele_ThornOffsets
extern u8 gCybele_ChipWidths[];    // ☀️ the second gCybele_ThornWidths
extern u8 gCybele_ChipHeights[];   // ☀️ the second gCybele_ThornHeights
extern u16 gCybele_SparkOffsets[];
extern u8 gCybele_SparkSizes[];
extern u16 gCybele_EmberOffsets[];
extern u16 gCybele_BlastSizes[]; // ☀️ gCybele_EmberSizes
extern u8 gBoreas_Points[][2];
extern s8 gBoreas_ShakeOffsets[];
extern u16 gBoreas_CrystalPictures[]; // ☀️ gBoreas_RockCells
extern u8 gBoreas_CrystalWidths[];    // ☀️ gBoreas_RockWidths
extern u8 gBoreas_CrystalHeights[];   // ☀️ gBoreas_RockHeights
extern u8 gBoreas_Animations[];
extern struct Scale Data_08196e14; // ☀️ gBattleFx10_UnitScale
extern u16 gBoreas_DebrisCells[];
extern u8 gBoreas_DebrisWidths[];
extern u8 gBoreas_DebrisHeights[];
extern u16 gBoreas_SprayCells[];
extern u8 Data_081982c0[];                  // ☀️ gBoreas_SprayWidths
extern u8 Data_081982c3[];                  // ☀️ gBoreas_SprayHeights
extern u16 gBoreas_RisingCrystalPictures[]; // ☀️ gBoreas_BoulderCells
extern u8 gBoreas_RisingCrystalWidths[];    // ☀️ gBoreas_BoulderWidths
extern u8 gBoreas_RisingCrystalHeights[];   // ☀️ gBoreas_BoulderHeights
extern u16 gBoreas_FallPictures[];          // ☀️ gBoreas_FallCells
extern u8 Data_081982d8[];                  // ☀️ gBoreas_FallWidths
extern u8 Data_081982db[];                  // ☀️ gBoreas_FallHeights
extern s32 gDrain_Gravities[];
extern u8 Data_081982f4[]; // the mote size by mode
void BattleEffect_RunStatBoost(struct BattleEffectArgument *effect, u32 mode);
extern u8 gQuake_Timings[][3];
extern struct RockWallHeights gRockWall_Heights;
extern u8 gFire_Counts[];
extern u16 gFire_FlareCells[];
extern u8 gFire_FlareWidths[];
extern u8 gFire_FlareBiasY[];
extern u8 gFire_FlareHeights[];
extern u8 gFrost_Timings[];
extern u8 gFrost_ColumnSpots[];
extern u16 gFrost_MoteCells[];
extern u8 gFrost_MoteWidths[];
extern u8 gFrost_MoteHeights[];
extern u16 gFrost_ColumnCells[];
extern u8 gFrost_ColumnHeights[];
extern u16 gFrost_ChipCells[];
extern u8 gFrost_ChipWidths[];
extern u8 gFrost_ChipHeights[];
extern u8 gRay_Sparks[][2];
void BattleEffect_RunEruption(struct BattleEffectArgument *effect, s32 mode);
extern u8 gEruption_Counts[][3];
extern u16 gEruption_PuffCells[];
extern u16 gEruption_PuffSizes[];
extern u8 gTargetStrike_GlintDrawFlags[];
extern char ResourceId_LeafWarriorTiles;
extern s32 gAtalanta_UnitScale[];
void BattleEffect_RunGrowingColumns(struct BattleEffectArgument *effect,
                                    s32 mode);
// The Broken Seal's gGrowth_* tables, in effect_table.s.
extern u8 gGrowth_ColumnCounts[];  // _ColumnCounts: columns by strength
extern u8 gGrowth_ColumnX[];       // _ColumnX: each column's root
extern u8 gGrowth_HeightLimits[];  // _HeightLimits: by i % 3
extern u16 gGrowth_VineCells[];    // _VineCells
extern u8 gGrowth_VineWidths[];    // _VineWidths
extern s8 gGrowth_PillarHeights[]; // _PillarHeights: by i & 7
extern u16 gGrowth_PillarCells[];  // _PillarCells
extern u8 gGrowth_PillarWidths[];  // _PillarWidths
extern u16 gGrowth_PuffCells[];    // _PuffCells
extern s8 gGrowth_PuffWidths[];    // _PuffWidths
extern u8 gGrowth_PuffHeights[];   // _PuffHeights
// The Broken Seal's table of this name: each vine's x about the
// middle of the targets.
extern const s8 gGrowth_ColumnOffsets[];
// The Broken Seal's table of this name: thorns by strength.
extern u8 gGrowth_Counts[];
// The Broken Seal's table of this name: each thorn's root.
extern s8 gGrowth_X[];
// The Broken Seal's table of this name: 0-1 large, 2-3 small; odd ones
// grow to the left.
extern u8 gGrowth_Kinds[];
// The Broken Seal's table of this name, _PuffCells, _PuffWidths:
// the seven dust cels.
extern u8 gGrowth_ThornPuffHeights[];
extern u16 gGrowth_ThornPuffCells[];
extern s8 gGrowth_ThornPuffWidths[];
// The Broken Seal's table of this name: four bytes for each strength,
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
