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
extern const u8 gData_08199210[];
extern const u8 Data_08199220[];
extern const u8 gData_08199340[];

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

void Graphics_LoadGreyRamp(void);

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

extern char ResourceId_Data134;
extern char ResourceId_PetalSheet;
extern char ResourceId_Data0C1;

extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
// The sparkle puff's size and cell for each of its nine pictures
// (gBattleEffect_PuffSizes and gBattleEffect_PuffCells).
extern u8 Data_0819745e[];
extern u16 Data_0819744c[];

void Func_08144b14(struct BattleEffectArgument *effect, s32 mode);
void Func_0814512c(struct BattleEffectArgument *effect, s32 mode);

void SummonEffect_RunCanvasMode1(void *action);
void SummonEffect_RunCanvasMode3(void *action);

void Func_08144628(struct BattleEffectArgument *effect, s32 mode);

void BattleEffect_ArmBg1HBlankDma(void);

void Func_0815b634(void);

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

#endif
