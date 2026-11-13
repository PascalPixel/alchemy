#ifndef ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H
#define ALCHEMY_MODULE_BATTLE_EFFECT_BATTLE_EFFECT_H

/// @file
/// What games/tla/src/battle_effect/ declares for the files that use it,
/// written once.

#include "field/effect_step.h"
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

void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);

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

void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattlePlayback_StartFar(s32 phase);

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

void BattleEffect_AdvanceScrollOnInterval(void);

void VramBlock_Reset93Far(void);

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);

void VramBlock_Load93Far(void);

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

void Palette_BrightenBgEntries(s32 red, s32 green, s32 blue);

void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);

void BattleEffect_ConfigureEffectDisplay(void);

void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);

void BattleEffect_ClearActorTimers(void);
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color);
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

void BattleEffect_SetupBlendedDisplay(void);

void Func_08147aec(struct BattleEffectArgument *effect, s32 mode);

void BattleEffect_PlaceFormationObjects(s32 formation, s32 x,
                                        s32 y); // ..PlaceFormationObjects
void BattleEffect_RunImpactBurst(s32 formation, s32 x,
                                 s32 y); // ..RunImpactBurst

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

void BattleEffect_StepPaletteToResource(s32);

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

struct Texture;

struct EclipseObject {
  u8 unk0[9];
  u8 unk09 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[6];
  u8 slot;
};

struct EclipseVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 pad;
};

// The three kinds the scenes ask for.
enum { KIND_RIGHT = 78, KIND_LEFT = 79, KIND_FINALE = 80 };

/// The battle effect work, as far as the canvas transfer and the canvas layer
/// read it.
struct CanvasWork {
  u8 unk0000[0x7780];
  s32 transferMode;
  s32 transferValue;
  u8 unk7788[0x18];
  s32 savedBg1X;
  s32 savedBg1Y;
  s32 shakeFrames;
  u8 unk77ac[8];
  s32 fadeFrames;
  s32 fadeStep;
  u8 unk77bc[0x128];
  s32 vblanksSinceTransfer;
  s32 transferPending;
};

struct BattleFxFadeWork {
  s32 unk00;
  s32 halvePending;
  s32 framesLeft;
  s32 step;
};

struct StreakWork {
  s32 frame;
  s32 transferPending;
};

struct Streak {
  s32 head;
  s32 tail;
  s32 unused08;
  s32 pitch;
  s32 yaw;
  s32 roll;
  s32 unused18;
};

struct StreakPoint {
  s32 x;
  s32 y;
  s32 z;
};

// The backdrop's work, in place of the battle effect work.
struct StreakSceneWork {
  s32 frame;        // the streaks' frame count (BattleEffect_DrawStreaks)
  s32 halvePending; // the streaks are drawn: halve the canvas into VRAM
  s32 fadeFrames;   // frames of the background's fade left
                    // (BattleEffect_DimBackgroundStep)
  s32 fadeStep;
};

struct ModelTextureB {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

struct RuinObject {
  u8 unk0[8];
  union {
    struct {
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
    } bits;
    u16 all;
  } attr;
  u8 unk0a[4];
  u8 resource;
};

struct FxRunWork {
  u8 unk0000[0x78e4];
  s32 active;
  u8 unk78e8[8];
  s32 *script;
};

struct FxPartyList {
  u8 unk00[20];
  s32 count;
  u8 unk18[12];
  s16 ids[8];
};

struct FxUnit {
  u8 unk00[56];
  s16 hp;
};

struct FxBgScroll {
  u8 unk00[6];
  u16 y;
};

struct ScrollStepWork {
  u8 unk0000[0x7790];
  s32 counter;
  s32 period;
  s32 stepX;
  s32 stepY;
};

struct AffineStepWork {
  u8 unk0000[0x7790];
  s32 counter;
  s32 period;
  s32 stepX;
  s32 stepY;
  u8 unk77a0[0x30];
  s32 x;
  s32 y;
};

// The kinds this body draws, chosen by the effect's entry.
enum {
  QUADS_MERCURY, // over a prepared canvas, with the Mercury Djinn
  QUADS_MARS,    // over a prepared canvas, with the Mars Djinn
  QUADS_RISING   // rising from the first target
};

// A render job (the first of Render_AllocateModelList's entries).
struct RenderJob {
  s32 kind;
  s32 next;
  const void *faces;
  void *vertices;
  struct Texture *texture;
  s32 fade;
  s32 tail;
};

struct RequestPair {
  s32 x, y;
};

struct FxGroupEntry {
  u8 unk00[5];
  u8 param;
  u8 unk06[16];
  u8 frame;
};

struct FxTimerWork {
  u8 unk0000[0x78dc];
  u8 actorTimers[8];
};

// The map cell buffer holds WIN0's line table and then the splashes.
struct DropSplashBuffer {
  u16 lines[160];
  struct EffectStep splashes[512];
};

// The kinds of spark group, chosen by the effect's entry.
enum {
  SPARKS_FIXED,     // from a fixed point, canvas at priority 1
  SPARKS_ORANGE,    // from a lower point, orange, with a palette fade
  SPARKS_FROM_ACTOR // from the caster, lightning colours, sparks rising
};

// The camera's heading, as the camera work holds it.
struct CameraView {
  u8 unk00[0x36];
  u16 yaw;
};

enum { MODE_SOOTHING_STAR, MODE_SLEEP_STAR, MODE_SLAVER };

// Where mode 3 unpacks the tornado's glints in the sheet.
enum { RENDER_MODE_TORNADO = 0x2580 };

// The star's texture, 64x64 pixels.
struct StarTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

// One model for the IWRAM renderer.
struct ModelDraw {
  s32 kind;
  s32 next;
  const void *model;
  void *corners;
  struct StarTexture *texture;
  s32 unk14;
  s32 tail;
};

extern s32 Data_0819790c[]; // the triangle's first corner: (0, 24, 0)
extern s32 Data_08197918[]; // the triangle's first corner: (0, 24, 0)

extern u8 Data_02010400[];
extern u8 Data_02010800[];
extern u8 Data_02012800[];

extern const struct WordPair Data_08196de0;

extern const u8 Data_08197744[];
extern const u8 Data_08197748[];
extern const u8 Data_0819774f[];
extern const u32 Data_08197760[];
extern const u8 Data_081977a4[];
extern const u8 Data_081977c4[];
extern const u8 Data_081977e4[];
extern const s8 Data_081977e8[];
// The four crystals: the frame each starts after 48, how fast it grows and
// how tall.
extern u8 Data_081977ec[];
extern u8 Data_081977f0[];
extern u8 Data_081977f4[];

extern char ResourceId_EffectPalette03;

void Func_08118150(s32 kind);

void ColorBuffer_Halve(void *source, void *destination, s32 size);
void ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_Darken(void *source, s32 amount, void *destination, s32 size);
void ColorBuffer_Brighten(void *source, s32 amount, void *destination,
                          s32 size);

extern u8 Data_081978f0[]; // each panel's x, by side (7 a side)
extern u8 Data_081978fe[]; // each panel's y
// The 33 pieces: each one's size, place in the sheet and starting point
// (The Broken Seal's gDeathPlunge_PieceWidths, Height, Offset, StartX and
// StartY).
extern u8 Data_08197609[];
extern u8 Data_0819762a[];
extern u16 Data_0819764c[];
extern u8 Data_08197690[];
extern u8 Data_081976b1[];
// The four corner sparks: their flips and their place around the target
// (The Broken Seal's gDeathPlunge_DrawFlags, _X and _Y).
extern u8 Data_081976da[];
extern s8 Data_081976d2[];
extern s8 Data_081976d6[];

extern const struct WordPair Data_08196dd8;

extern const u16 Data_08197438[];
extern const u8 Data_081976e0[];
extern const u16 Data_081976f0[];
extern const u16 Data_081976f8[];
extern const u8 Data_08197700[];
extern const u8 Data_08197702[];
extern const u8 Data_08197704[];
extern const u8 Data_0819770a[];
extern const u8 Data_08197710[];
extern const u8 Data_08197716[];
extern const u8 Data_0819771c[];
extern const u8 Data_08197720[];
extern const u8 Data_08197724[];
extern const u8 Data_08197728[];
extern const u8 Data_0819772c[];
extern const u8 Data_08197730[];
extern const s32 Data_08197734[];
extern void (*gData_08197aa0[])(s32 *script);

extern char ResourceId_Data0D0;      // directory row 0xd0
extern char ResourceId_FeatherSheet; // directory row 0xd1
extern char ResourceId_CrescentRowSheet;

extern const struct RequestPair Data_08196e0c;

extern const u16 Data_08197a38[][2]; // the charging Zagan's parts
extern const u16 Data_08197a64[][2]; // the galloping Zagan's
extern const u8 Data_08197a94[];     // each ring's start
extern const u8 Data_08197a98[];     // growth
extern const s8 Data_08197a9c[];     // height

s32 BattleMotion_GetSlotPaletteFar(s32 id);

// Shards and frames for each variant (The Broken Seal's
// gFalling_IceCounts).
extern u8 Data_08197521[];
// Four bytes for each variant (The Broken Seal's gFalling_Counts).
extern u8 Data_08197527[];
extern u8 Data_0819786c[];  // the mote's picture for each step of its sway
extern u16 Data_08197874[]; // the beam's head: three pictures, offsets
extern u8 Data_0819787a[];  // ... their widths
extern u8 Data_0819787d[];  // ... their heights
// Each mode's record: whether its shots burst into shards (else into smoke),
// the shots' blitter, shots per target, and frames before the targets'
// share.
extern s8 Data_08197880[];
extern u8 Data_081978a8[];  // shard widths
extern u8 Data_081978ba[];  // shard heights
extern u16 Data_081978cc[]; // shard cell offsets
extern u8 Data_08197990[];  // the three columns' feet, x and y
extern s32 Data_08197998[]; // a falling spark's gravity, by spark
// Per mode: how many motes each target gets, and the effect's frame count
// before the targets' share.
extern u8 Data_08197864[];
extern u8 Data_081979da[];  // ☀️ gPlasma_Counts
extern u8 Data_081979cc[];  // ☀️ gPlasma_Columns, 7 for each side
extern u16 Data_081979c0[]; // ☀️ gPlasma_Sparks: sparks, shake
// ☀️ gPlasma_Shapes, five halfwords for each variant: sparks in each
// group, groups, and each group's x offset.
extern u16 Data_081979de[][5];
extern u8 Data_08197a1a[]; // ☀️ gPlasma_FlashCells
// ☀️ gPrism_Counts, with a third byte: crystals, frames and the frame
// that begins phase 134, for each variant.
extern u8 Data_081979b7[];

s32 Math_UnsignedMulHighAdd(s32 value, u32 magic, s32 multiplier);

extern struct Scale Data_08196df8; // the comets' unit scale

extern u8 Data_0819793e[];   // the nine comets' grid: x
extern u8 Data_08197947[];   // ... and depth
extern u16 Data_08197950[];  // the four comet strikes' frames
extern u8 Data_08197958[];   // a strike's three pictures: widths
extern u8 Data_0819795b[];   // ... heights
extern u8 Data_0819795e[];   // ... raise above the mark
extern u16 Data_08197962[];  // ... cell offsets
extern u8 Data_08197968[];   // a grain's added speed, by grain
extern u8 Data_0819796c[];   // a spark's two pixels
extern u8 Data_0819796e[];   // a grain's size, by grain
extern u8 Data_08197972[];   // the streak's six pictures: widths
extern u8 Data_08197978[];   // ... heights
extern u8 Data_0819797e[];   // ... drops below the streak
extern u16 Data_08197984[];  // ... cell offsets
extern s32 gData_08197924[]; // the two ring points, alternating
extern u8 gData_0819793c[];  // ring radii, alternating: 32, 16
// Four signed bytes for each mode (The Broken Seal's gStreaks_Styles).
extern s8 Data_08197533[];
// The struck target's sideways tremble, in pixels (gStreaks_Tremble).
extern s8 Data_08197547[];
extern u8 Data_08197a34[]; // ☀️ gTargetMotes_GlintFlips
// Seven bytes for each mode (The Broken Seal's gTargetStrike_Styles, grown
// from 12 modes to 21).
extern u8 Data_0819754f[][7];
// The strike pictures (gTargetStrike_Widths, Heights, Offsets, Reach)
// and the column stages (gTargetStrike_ColumnStages).
extern u8 Data_081975e2[];
extern u8 Data_081975e8[];
extern u16 Data_081975ee[];
extern u8 Data_081975fa[];
extern u8 Data_08197600[];
extern u8 Data_08197605[];
extern u8 Data_08197a20[];    // ☀️ gVolcano_Counts, for each variant
extern s8 Data_08197a23[][4]; // ☀️ gVolcano_Depths
extern u8 Data_08197a2f[];    // ☀️ gVolcano_StartFrames
// ☀️ gVortices_Counts: motes, then frames, for each variant.
extern u8 gData_081979a8[];
// ☀️ gVortices_Shapes: funnels, sway radius and shake, for each variant.
extern u8 gData_081979ae[];

#endif
