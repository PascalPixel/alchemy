#ifndef ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H
#define ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H

/// @file
/// What games/tla/src/battle_effect/ declares for the files that use it,
/// written once.

#include "types.h"
#include "battle/battle_effect.h"

struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;
struct FxGroupEntry;
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
struct CameraWork {
  u8 unk00[0x36];
  u16 scroll;
};

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
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase,
                   s32 bluePhase); // Graphics_UpdatePhasePalette

extern u16 Data_08197424[]; // flare sizes 1 to 5: cell offsets

/// The canvas layer and its helpers; the comments give The Broken Seal's
/// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_CountDownBattlerTints(void);

s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter

void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler

void Func_0815b434(u8 *map, s32 size, s32 scale);

void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);

void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);

void Func_0818caa8(void *, s32, s32, s32);

struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);

void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // ☀️ SpawnObjects

void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets

void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);

void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar

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

extern u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells: dot sizes 1 to 10,
                             // cell offsets in the sheet
extern u16 gData_08197486[]; // ☀️ gBattleEffect_GlintCells
extern u8 gData_08197492[];  // ☀️ gBattleEffect_GlintCellWidths
extern u8 gData_08197498[];  // ☀️ gBattleEffect_GlintCellHeights
extern const u8 gData_081991b0[];
extern const u8 Data_081991c0[];
extern const u8 gData_08199210[]; // a quad's four corners, x y z for each
extern const u8 Data_08199220[];
extern const u8 gData_08199340[]; // the quad's face

void Func_08179e6c(void);

void Func_0813baec(void);

void VramBlock_Reset93Far(void);

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);

void Func_08020388(void);

void Func_08164abc(s32 r, s32 g, s32 b);
void Func_08164b2c(s32 red, s32 green, s32 blue);

void Func_081885f0(struct MeshVertex *vertices, struct MeshTriangle *triangles,
                   s32 segments);

void Graphics_LoadGreyRamp(void); // a grey ramp from color 1

void Func_0818de3c(struct MeshTriangle *triangles, s32 columns, s32 rows,
                   s32 cellWidth, s32 cellHeight);

// The models: the rings' quad (Data_08199220), its corners, the crystal and its
// corners.
extern const u8 gData_081990d0[];
extern const u8 gData_08199090[];

extern char ResourceId_Data0DA;

void Func_08164a4c(s32 red, s32 green, s32 blue); // Palette_BrightenBgEntries

void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);

void BattleEffect_ConfigureEffectDisplay(void);

void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);

void BattleEffect_ClearActorTimers(void);
void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color);
void Func_0814355c(void); // the BG2 canvas's registers

extern u8 gData_02014000[];

extern const u8 Data_081991e0[];
extern const u8 gData_08199364[];

void Func_0815b510(u8 *map, s32 size, s32 scale);

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
extern u8 Data_0819745e[];  // ☀️ gBattleEffect_PuffSizes
extern u16 Data_0819744c[]; // ☀️ gBattleEffect_PuffCells

void Func_08144b14(struct BattleEffectArgument *effect, s32 mode);
void Func_0814512c(struct BattleEffectArgument *effect, s32 mode);

void SummonEffect_RunCanvasMode1(void *action);
void SummonEffect_RunCanvasMode3(void *action);

void Func_08144628(struct BattleEffectArgument *effect, s32 mode);

void BattleEffect_ArmBg1HBlankDma(void); // arms the BG1 scroll's HBlank DMA

void Func_0815b634(void); // mode 3 only

extern s32 gData_08197834[]; // ☀️ gPrism_ShardOffsets
extern u8 gData_0819781a[];  // ☀️ gPrism_ShardWidths
extern u8 gData_08197826[];  // ☀️ gPrism_ShardHeights
extern u8 gData_081977f8[];  // ☀️ gPrism_ShardStarts: x, y for each shard

void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay

void Func_08147aec(struct BattleEffectArgument *effect, s32 mode);

void Func_0816442c(s32 formation, s32 x, s32 y); // ..PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x, s32 y); // ..RunImpactBurst

void Func_0813c5f4(struct BattleEffectArgument *effect, s32 mode);
void Func_0813cb34(struct BattleEffectArgument *effect, s32 mode);

extern char ResourceId_SheepCardSheet; // directory row 0xe3

void Func_0815e22c(struct EffectPosition *midpoint);

void Func_0814b9e8(struct BattleEffectArgument *effect, s32 mode);

void Func_0813d098(struct BattleEffectArgument *effect, s32 mode);

void BattleEffect_SetApproachMotion(s32 actor, s32 target, s32 frames,
                                    s32 percent);

extern char ResourceId_StarCrossSheet;
extern char ResourceId_CrossedDaggersSheet;

void Func_08191530(struct BattleEffectArgument *effect);

void Camera_ApplyPhasedDelta(void);

struct EffectPosition;

typedef s32 (*CopyWordsFn)(void *destination, const void *source, s32 size);

typedef struct {
  s32 reserved00[3];
  s32 currentValue;
  s32 indexRadius;
  s32 entryLimit;
  s32 reserved18[3];
  s16 values[1];
} BattleEffectIndexWindow;

struct EffectCamera {
  s32 eye[3];
  s32 pos[3];
};

// Func_0815585c's packed argument: the Djinni's element, the side, and
// whether the Djinni stands at the anchor.
enum { DJINNI_VENUS, DJINNI_MERCURY, DJINNI_MARS, DJINNI_JUPITER };

// Places in the sheet.
enum SeriesBSheet {
  SERIES_B_FLARES = 0x0000,  // the gathering lights
  SERIES_B_BURST = 0x060e,   // six 40x40 bursts
  SERIES_B_PICTURE = 0x2b8e, // the mode's picture
  SERIES_B_DJINNI = 0x65c0   // the 40x40 Djinni
};

// A row of Data_081982f7, one for each variant.
enum {
  TIMING_FRAMES,      // the whole effect
  TIMING_RISE_FRAMES, // how long each pillar stands
  TIMING_SHAKE        // the camera shake
};

struct RockWallHeights {
  s32 column[4]; // each pillar's height in pixels
};

// A row of Data_08198348, one for each variant.
enum {
  RISE_MOTES,   // the rising motes
  RISE_COLUMNS, // the pillars
  RISE_FRAME    // the frame they burst
};

// A row of Data_081983a2, one for each variant.
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
extern u8 gData_0819746d[];  // gBattleEffect_PuffArcCellHeights
extern u8 gData_08197473[];  // gBattleEffect_PuffArcCellBiasY
extern u16 gData_0819747a[]; // gBattleEffect_PuffArcCells
extern u8 gData_0819749e[];
extern u8 gData_081974ad[]; // the shard heights
extern u16 gData_081974bc[];

void Func_0815f0a0(s32);

extern char ResourceId_Data0C9;

extern char ResourceId_VioletPaletteC;

extern char ResourceId_Data0D7;

void BattleEffect_WipeCanvas(s32, s32);

void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position);

// The quad meshes and corners: the card, the pictures and their spread.
extern u8 Data_081992b0[];
// The dagger's shards, ☀️'s palette ramp shards.
extern u16 Data_081974dc[]; // gStatChange_ShardCells
extern u8 Data_081974f4[];  // gStatChange_ShardWidths
extern u8 Data_08197500[];  // gStatChange_ShardHeights

void BattleEffect_BuildWindowEdgeTable(void);

extern char ResourceId_Data0CB;

extern const u8 Data_08199eb4[];
extern const u8 Data_08199ebc[];
extern const u8 Data_08199ea4[];
extern const u8 Data_08199ec5[];

void Func_0814ea58(struct BattleEffectArgument *effect, s32 mode);

extern u8 Data_081981fc[];    // ☀️ gCybele_ObjectPoses
extern u16 Data_08198202[];   // ☀️ gCybele_ThornOffsets
extern u8 Data_0819820c[];    // ☀️ gCybele_ThornWidths
extern u8 Data_08198211[];    // ☀️ gCybele_ThornHeights
extern u16 Data_08198216[];   // ☀️ gCybele_LeafOffsets
extern u8 Data_0819821c[];    // ☀️ gCybele_LeafSizes
extern u16 Data_08198220[];   // ☀️ the second gCybele_ThornOffsets
extern u8 Data_0819822a[];    // ☀️ the second gCybele_ThornWidths
extern u8 Data_0819822f[];    // ☀️ the second gCybele_ThornHeights
extern u16 Data_08198234[];   // ☀️ gCybele_SparkOffsets
extern u8 Data_0819823e[];    // ☀️ gCybele_SparkSizes
extern u16 Data_08198244[];   // ☀️ gCybele_EmberOffsets
extern u16 Data_08198252[];   // ☀️ gCybele_EmberSizes
extern u8 Data_08198260[][2]; // ☀️ gBoreas_Points
extern s8 Data_08198280[];    // ☀️ gBoreas_ShakeOffsets
extern u16 Data_08198284[];   // ☀️ gBoreas_RockCells
extern u8 Data_0819828a[];    // ☀️ gBoreas_RockWidths
extern u8 Data_0819828d[];    // ☀️ gBoreas_RockHeights
extern u8 Data_08198290[];    // ☀️ gBoreas_Animations

extern struct Scale Data_08196e14; // ☀️ gBattleFx10_UnitScale

extern u16 Data_081982aa[];  // ☀️ gBoreas_DebrisCells
extern u8 Data_08198294[];   // ☀️ gBoreas_DebrisWidths
extern u8 Data_0819829f[];   // ☀️ gBoreas_DebrisHeights
extern u16 Data_081982c6[];  // ☀️ gBoreas_SprayCells
extern u8 Data_081982c0[];   // ☀️ gBoreas_SprayWidths
extern u8 Data_081982c3[];   // ☀️ gBoreas_SprayHeights
extern u16 Data_081982cc[];  // ☀️ gBoreas_BoulderCells
extern u8 Data_081982d2[];   // ☀️ gBoreas_BoulderWidths
extern u8 Data_081982d5[];   // ☀️ gBoreas_BoulderHeights
extern u16 Data_081982de[];  // ☀️ gBoreas_FallCells
extern u8 Data_081982d8[];   // ☀️ gBoreas_FallWidths
extern u8 Data_081982db[];   // ☀️ gBoreas_FallHeights
extern s32 gData_081982e4[]; // gDrain_Gravities
extern u8 Data_081982f4[];   // the mote size by mode

void Func_0814fab8(struct BattleEffectArgument *effect, u32 mode);

extern u8 Data_081982f7[][3]; // gQuake_Timings

extern struct RockWallHeights Data_08196e1c; // gRockWall_Heights

extern u8 Data_08198300[];    // gFire_Counts
extern u16 Data_08198316[];   // gFire_FlareCells
extern u8 Data_08198303[];    // gFire_FlareWidths
extern u8 Data_0819830f[];    // gFire_FlareBiasY
extern u8 Data_08198309[];    // gFire_FlareHeights
extern u8 Data_08198348[];    // gFrost_Timings
extern u8 Data_08198322[];    // gFrost_ColumnSpots
extern u16 Data_08198362[];   // gFrost_MoteCells
extern u8 Data_08198351[];    // gFrost_MoteWidths
extern u8 Data_08198359[];    // gFrost_MoteHeights
extern u16 Data_08198372[];   // gFrost_ColumnCells
extern u8 Data_08198378[];    // gFrost_ColumnHeights
extern u16 Data_0819838c[];   // gFrost_ChipCells
extern u8 Data_0819837b[];    // gFrost_ChipWidths
extern u8 Data_08198383[];    // gFrost_ChipHeights
extern u8 Data_0819839c[][2]; // gRay_Sparks

void Func_08151ff0(struct BattleEffectArgument *effect, s32 mode);

extern u8 Data_081983a2[][3]; // gEruption_Counts
extern u16 Data_081983ac[];   // gEruption_PuffCells
extern u16 Data_081983ba[];   // gEruption_PuffSizes
extern u8 Data_081983c8[];    // gTargetStrike_GlintDrawFlags

extern char ResourceId_LeafWarriorTiles;

extern s32 Data_081983cc[]; // gAtalanta_UnitScale

void Func_081548d0(struct BattleEffectArgument *effect, s32 mode);

// The Broken Seal's gGrowth_* tables, in effect_table.s.
extern u8 Data_081983ea[];  // _ColumnCounts: columns by strength
extern u8 Data_081983ed[];  // _ColumnX: each column's root
extern u8 Data_081983d7[];  // _HeightLimits: by i % 3
extern u16 Data_081983da[]; // _VineCells
extern u8 Data_081983d4[];  // _VineWidths
extern s8 Data_081983fd[];  // _PillarHeights: by i & 7
extern u16 Data_081983e4[]; // _PillarCells
extern u8 Data_081983e0[];  // _PillarWidths
extern u16 Data_08198414[]; // _PuffCells
extern s8 Data_08198405[];  // _PuffWidths
extern u8 Data_0819840c[];  // _PuffHeights
// The Broken Seal's gGrowth_ColumnOffsets: each vine's x about the
// middle of the targets.
extern const s8 Data_08198422[];
// The Broken Seal's gGrowth_Counts: thorns by strength.
extern u8 Data_08198442[];
// The Broken Seal's gGrowth_X: each thorn's root.
extern s8 Data_0819843a[];
// The Broken Seal's gGrowth_Kinds: 0-1 large, 2-3 small; odd ones
// grow to the left.
extern u8 Data_08198432[];
// The Broken Seal's gGrowth_ThornPuffHeights, _PuffCells, _PuffWidths:
// the seven dust cels.
extern u8 Data_0819844c[];
extern u16 Data_08198454[];
extern s8 Data_08198445[];
// The Broken Seal's gBolt_Counts: four bytes for each strength,
// sparks, glints, glint spread and bolts.
extern u8 Data_08198462[];
// The Broken Seal's gBolt_GlintFlips and _GlintModes.
extern u8 Data_0819846e[];
extern u8 Data_08198472[];

#endif
