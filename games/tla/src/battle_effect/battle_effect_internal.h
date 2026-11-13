#ifndef ALCHEMY_BATTLE_EFFECT_INTERNAL_H
#define ALCHEMY_BATTLE_EFFECT_INTERNAL_H

/// @file
/// What every part of the battle_effect object shares: its includes.
/// battle_effect.c includes it before the parts, and each part includes it too.

#include "types.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "bg_scroll.h"
#include "system/dma.h"
#include "battle/battle_effect.h"
#include "battle_effect_work.h"
#include "field/effect_step.h"
#include "battle_display.h"
#include "system/task.h"
#include "battle/motobj.h"
#include "system/iwram_call.h"
#include "fixed_math.h"
#include "graphics/graphics.h"
#include "system/transform.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "system/input.h"
#include "battle_work.h"
#include "system/irq_lock.h"
#include "battle/render_model.h"
#include "battle/battle.h"
#include "graphics/vram_tab.h"
#include "item_icon.h"
#include "battle_fx_work.h"
#include "lib.h"
#include "sound/sound.h"
#include "battle/battle_effect_argument.h"
#include "actor.h"
#include "macros.h"

#include "field/effect_step.h"
#include "battle/battle_effect.h"

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

extern s32 gBind_Vertex[]; // the triangle's first corner: (0, 24, 0)

extern s32
    gBind_TriangleStrikeVertex[]; // the triangle's first corner: (0, 24, 0)

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

extern u8 gCondemn_PictureX[]; // each panel's x, by side (7 a side)

extern u8 gCondemn_PanelY[]; // each panel's y

// The 33 pieces: each one's size, place in the sheet and starting point
// (The Broken Seal's table of this name, Height, Offset, StartX and
// StartY).
extern u8 gDeathPlunge_PieceWidths[];

extern u8 gDeathPlunge_PieceHeights[];

extern u16 gDeathPlunge_PiecePictures[];

extern u8 gDeathPlunge_PieceStartX[];

extern u8 gDeathPlunge_PieceStartY[];

// The four corner sparks: their flips and their place around the target
// (The Broken Seal's gDeathPlunge_DrawFlags, _X and _Y).
extern u8 gSpiderWeb_PictureHeights[];

extern s8 gDeathPlunge_X[];

extern s8 gDeathPlunge_Y[];

extern const struct WordPair Data_08196dd8;

extern const u16 Data_08197438[];

extern const u8 Data_081976e0[];

extern const u16 Data_081976f0[];

extern const u16 gSpiderWeb_PictureBiasY[];

extern const u8 Data_08197700[];

extern const u8 Data_08197702[];

extern const u8 gGatheringLights_Counts[];

extern const u8 gMirroredWall_Pictures[];

extern const u8 gDjinnBeams_HeadPictures[];

extern const u8 gProcne_Pictures[];

extern const u8 gTiamat_EmberSpeeds[];

extern const u8 gTiamat_EmberSizes[];

extern const u8 Data_08197724[];

extern const u8 Data_08197728[];

extern const u8 Data_0819772c[];

extern const u8 Data_08197730[];

extern const s32 Data_08197734[];

extern void (*gData_08197aa0[])(s32 *script);

extern char ResourceId_Data0D0; // directory row 0xd0

extern char ResourceId_FeatherSheet; // directory row 0xd1

extern char ResourceId_CrescentRowSheet;

extern const struct RequestPair Data_08196e0c;

extern const u16 Data_08197a38[][2]; // the charging Zagan's parts

extern const u16 Data_08197a64[][2]; // the galloping Zagan's

extern const u8 Data_08197a94[]; // each ring's start

extern const u8 Data_08197a98[]; // growth

extern const s8 Data_08197a9c[]; // height

// Shards and frames for each variant (The Broken Seal's
// gFalling_IceCounts).
extern u8 gFalling_IceCounts[];

// Four bytes for each variant (as The Broken Seal has it).
extern u8 gFalling_Counts[];

extern u8
    gDjinnBeams_MotePictures[]; // the mote's picture for each step of its sway

extern u16 gMirroredWall_Cells[]; // the beam's head: three pictures, offsets

extern u8 gDjinnBeams_HeadWidths[]; // ... their widths

extern u8 gDjinnBeams_HeadHeights[]; // ... their heights

// Each mode's record: whether its shots burst into shards (else into smoke),
// the shots' blitter, shots per target, and frames before the targets'
// share.
extern s8 Data_08197880[];

extern u8 gLobbedShells_ShardWidths[]; // shard widths

extern u8 gLobbedShells_ShardHeights[]; // shard heights

extern u16 gLobbedShells_ShardOffsets[]; // shard cell offsets

extern u8 gNereid_Columns[]; // the three columns' feet, x and y

extern s32 Data_08197998[]; // a falling spark's gravity, by spark

// Per mode: how many motes each target gets, and the effect's frame count
// before the targets' share.
extern u8 Data_08197864[];

extern u8 gPlasma_Counts[];

extern u8 gPlasma_Columns[]; // ☀️ gPlasma_Columns, 7 for each side

extern u16 gPlasma_Sparks[]; // ☀️ gPlasma_Sparks: sparks, shake

// ☀️ gPlasma_Shapes, five halfwords for each variant: sparks in each
// group, groups, and each group's x offset.
extern u16 Data_081979de[][5];

extern u8 gPlasma_FlashPictures[]; // ☀️ gPlasma_FlashCells

// ☀️ gPrism_Counts, with a third byte: crystals, frames and the frame
// that begins phase 134, for each variant.
extern u8 Data_081979b7[];

extern struct Scale Data_08196df8; // the comets' unit scale

extern u8 gProcne_GridX[]; // the nine comets' grid: x

extern u8 gProcne_GridY[]; // ... and depth

extern u16 gProcne_Frames[]; // the four comet strikes' frames

extern u8 gProcne_CellWidths[]; // a strike's three pictures: widths

extern u8 gProcne_CellHeights[]; // ... heights

extern u8 gProcne_CellBiasY[]; // ... raise above the mark

extern u16 gProcne_Cells[]; // ... cell offsets

extern u8 gTiamat_DropSpeeds[]; // a grain's added speed, by grain

extern u8 gTiamat_SparkPixels[]; // a spark's two pixels

extern u8 gTiamat_DropSizes[]; // a grain's size, by grain

extern u8 gTiamat_FlameWidths[]; // the streak's six pictures: widths

extern u8 gTiamat_FlameHeights[]; // ... heights

extern u8 gTiamat_StreakDrops[]; // ... drops below the streak

extern u16 gTiamat_FlamePictures[]; // ... cell offsets

extern s32 gStar_Points[]; // the two ring points, alternating

extern u8 gStar_Radii[]; // ring radii, alternating: 32, 16

// Four signed bytes for each mode (The Broken Seal's gStreaks_Styles).
extern s8 gStreaks_Records[];

// The struck target's sideways tremble, in pixels (gStreaks_Tremble).
extern s8 gStreaks_Tremble[];

extern u8 Data_08197a34[]; // ☀️ gTargetMotes_GlintFlips

// Seven bytes for each mode (The Broken Seal's gTargetStrike_Styles, grown
// from 12 modes to 21).
extern u8 Data_0819754f[][7];

// The strike pictures (gTargetStrike_Widths, Heights, Offsets, Reach)
// and the column stages (gTargetStrike_ColumnStages).
extern u8 gTargetStrike_Widths[];

extern u8 gTargetStrike_Heights[];

extern u16 gTargetStrike_Offsets[];

extern u8 gTargetStrike_Reach[];

extern u8 gTargetStrike_ColumnStages[];

extern u8 gDeathPlunge_SwordWidths[];

extern u8 gVolcano_Counts[]; // ☀️ gVolcano_Counts, for each variant

extern s8 gVolcano_Depths[][4];

extern u8 gVolcano_StartFrames[];

// ☀️ gVortices_Counts: motes, then frames, for each variant.
extern u8 gVortices_Counts[];

// ☀️ gVortices_Shapes: funnels, sway radius and shake, for each variant.
extern u8 gVortices_Shapes[];

#endif // ALCHEMY_BATTLE_EFFECT_INTERNAL_H
