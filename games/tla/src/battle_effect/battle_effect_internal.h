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
#include "module/battle_effect/battle_effect.h"
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
#include "lib.h"
#include "sound/sound.h"
#include "battle/battle_effect_argument.h"
#include "actor.h"
#include "macros.h"

#include "field/effect_step.h"
#include "battle/battle_effect.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "menu/menu.h"
#include "field_event.h"
#include "battle/b5_context.h"
#include "battle/battle_action.h"
#include "field/sprite.h"
#include "graphics/render_placement.h"
#include "battle/battle_unit.h"
#include "system/irq.h"
#include "system/scene.h"
#include "owner_state.h"
#include "version.h"
#include "field_sprite.h"
#include "system/vector3.h"
#include "system/ram_code.h"

struct EffectSprite;
void *EffectSprite_Acquire(s32 width, s32 height, u32 shape, s32 mode);
void *EffectSprite_CreateWithTiles(s32 width, s32 height, s32 attributes,
                                   s32 flags);

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

struct FxUnit {
  u8 unk00[56];
  s16 hp;
};

struct FxBgScroll {
  u8 unk00[6];
  u16 y;
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

extern const struct Scale gEclipse_Scales;

extern const u8 gEclipse_GlintFlips[];

extern const u8 gEclipse_TileObjects[];

extern const u8 gEclipse_FillFrames[];

extern const u32 gEclipse_FillLevels[];

extern const u8 gEclipse_ObjectX[];

extern const u8 gEclipse_ObjectZ[];

extern const u8 gEclipse_RingGrowth[];

extern const s8 gEclipse_RingHeights[];

// The four crystals: the frame each starts after 48, how fast it grows and
// how tall.
extern u8 gFormChange_WaveStarts[];

extern u8 gFormChange_WaveGrowth[];

extern u8 gFormChange_WaveStretch[];

extern char ResourceId_EffectPalette03;

extern u8 gCondemn_PictureX[]; // each panel's x, by side (7 a side)

extern u8 gCondemn_PictureY[]; // each panel's y

// The 33 pieces: each one's size, place in the sheet and starting point.
extern u8 gDeathPlunge_PieceWidths[];

extern u8 gDeathPlunge_PieceHeights[];

extern u16 gDeathPlunge_PiecePictures[];

extern u8 gDeathPlunge_PieceStartX[];

extern u8 gDeathPlunge_PieceStartY[];

// The four corner sparks: their flips and their place around the target.
extern u8 gElementBurst_GlintFlips[];

extern s8 gDeathPlunge_X[];

extern s8 gDeathPlunge_Y[];

extern const struct Scale gCruelRuin_UnitScale;

extern const u16 gCruelRuin_CirclePictures[];

extern const u8 gCruelRuin_StreakCorners[];

extern const u16 gCruelRuin_DiscPitches[];

extern const u16 gCruelRuin_DiscYaws[];

extern const u8 gCruelRuin_PillarSways[];

extern const u8 gCruelRuin_PillarWidths[];

extern const u8 gCruelRuin_BlastFrames[];

extern const u8 gCruelRuin_BlastSizes[];

extern const u8 gCruelRuin_BlastX[];

extern const u8 gCruelRuin_BlastY[];

extern const u8 gCruelRuin_StreakStarts[];

extern const u8 gCruelRuin_StreakSizes[];

extern const u8 gCruelRuin_StreakX[];

extern const u8 gCruelRuin_StreakStepX[];

extern const u8 gCruelRuin_StreakStepY[];

extern const u8 gCruelRuin_StreakEndY[];

extern const s32 gCruelRuin_StreakRolls[];

extern void (*gBattleEffect_Handlers[])(struct BattleEffectArgument *args);

extern char ResourceId_Data0D0; // directory row 0xd0

extern char ResourceId_FeatherSheet; // directory row 0xd1

extern char ResourceId_CrescentRowSheet;

extern const struct Scale gZagan_UnitScale;

extern const u16 gZagan_ChargePoints[][2]; // the charging Zagan's parts

extern const u16 gZagan_GallopPoints[][2]; // the galloping Zagan's

extern const u8 gZagan_RingStarts[]; // each ring's start

extern const u8 gZagan_RingGrowth[]; // growth

extern const s8 gZagan_RingHeights[]; // height

// Shards and frames for each variant.
extern u8 gFalling_IceCounts[];

// Four bytes for each variant.
extern u8 gFalling_Counts[];

extern u8
    gDjinnBeams_MotePictures[]; // the mote's picture for each step of its sway

extern u16 gMirroredWall_Pictures[]; // the beam's head: three pictures, offsets

extern u8 gDjinnBeams_HeadWidths[]; // ... their widths

extern u8 gDjinnBeams_HeadHeights[]; // ... their heights

// Each mode's record: whether its shots burst into shards (else into smoke),
// the shots' blitter, shots per target, and frames before the targets'
// share.
extern s8 gLobbedShells_Styles[];

extern u8 gLobbedShells_ShardWidths[]; // shard widths

extern u8 gLobbedShells_ShardHeights[]; // shard heights

extern u16 gLobbedShells_ShardPictures[]; // shard cell offsets

extern u8 gNereid_Columns[]; // the three columns' feet, x and y

extern s32 gNereid_Gravity[]; // a falling spark's gravity, by spark

// Per mode: how many motes each target gets, and the effect's frame count
// before the targets' share.
extern u8 gGatheringLights_Counts[];

extern u8 gPlasma_Counts[];

extern u8 gPlasma_Columns[]; // 7 for each side

extern u16 gPlasma_Sparks[]; // sparks, shake

// Five halfwords for each variant: sparks in each
// group, groups, and each group's x offset.
extern u16 gPlasma_Shapes[][5];

extern u8 gPlasma_FlashPictures[];

// Crystals, frames and the frame that begins phase 134, for each variant.
extern u8 gPrism_Counts[];

extern struct Scale gProcne_UnitScale; // the comets' unit scale

extern u8 gProcne_GridX[]; // the nine comets' grid: x

extern u8 gProcne_GridY[]; // ... and depth

extern u16 gProcne_Frames[]; // the four comet strikes' frames

extern u8 gProcne_PictureWidths[]; // a strike's three pictures: widths

extern u8 gProcne_PictureHeights[]; // ... heights

extern u8 gProcne_PictureBiasY[]; // ... raise above the mark

extern u16 gProcne_Pictures[]; // ... cell offsets

extern u8 gTiamat_EmberSpeeds[]; // a grain's added speed, by grain

extern u8 gTiamat_SparkPixels[]; // a spark's two pixels

extern u8 gTiamat_EmberSizes[]; // a grain's size, by grain

extern u8 gTiamat_FlameWidths[]; // the streak's six pictures: widths

extern u8 gTiamat_FlameHeights[]; // ... heights

extern u8 gTiamat_FlameDrops[]; // ... drops below the streak

extern u16 gTiamat_FlamePictures[]; // ... cell offsets

extern s32 gStar_Points[]; // the two ring points, alternating

extern u8 gStar_Radii[]; // ring radii, alternating: 32, 16

// Four signed bytes for each mode.
extern s8 gStreaks_Styles[];

// The struck target's sideways tremble, in pixels (gStreaks_Tremble).
extern s8 gStreaks_Tremble[];

extern u8 gTargetMotes_GlintFlips[];
// Seven bytes for each mode.
extern u8 gTargetStrike_Styles[][7];

// The strike pictures (gTargetStrike_Widths, Heights, Offsets, Reach)
// and the column stages (gTargetStrike_BiteStages).
extern u8 gTargetStrike_Widths[];

extern u8 gTargetStrike_Heights[];

extern u16 gTargetStrike_Pictures[];

extern u8 gTargetStrike_Reach[];

extern u8 gTargetStrike_BiteStages[];

extern u8 gTargetStrike_GlintDrawFlags[];

extern u8 gVolcano_Counts[]; // for each variant

extern s8 gVolcano_Depths[][4];

extern u8 gVolcano_StartFrames[];

// Motes, then frames, for each variant.
extern u8 gVortices_Counts[];

// Funnels, sway radius and shake, for each variant.
extern u8 gVortices_Shapes[];

/// The battle screen's display mode while an effect plays: background mode 1
/// with BG0 to BG2, sprites and both windows on.
#define EFFECT_DISPCNT                                                         \
  (DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |     \
   DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON)

/// The whole pixels of a 16.16 value.
#define Q_16_16_TO_INT(v) ((s32)(v) >> 16)

/// The whole pixels of a stored 16.16 value, read as its upper halfword.
// UB: the s32 is read through an s16.
#ifdef UBFIX
#define Q_16_16_UPPER_HALF(v) ((s32)(v) >> 16)
#else
#define Q_16_16_UPPER_HALF(v) (((s16 *)&(v))[1])
#endif

// The canvas life cycle, shared by every canvas effect.

/// The heap slots an effect is lent, as HeapSlot_Alloc and HeapSlot_Release
/// take a slot: byte offsets into Ram_HeapSlots. The canvas is
/// HEAP_SLOT_CANVAS.
#define HEAP_SLOT_BATTLE_EFFECT 92 // Ram_HeapSlots->battleFxWork[0]
#define HEAP_SLOT_BATTLE_SHEET 100 // Ram_HeapSlots->unk64[0]

/// The bytes an effect's work block is lent: more than struct
/// BattleEffectWork holds.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

/// The bytes of one set of the ten mote pictures: 2 * (1 + 4 + ... + 100).
#define MOTE_SET_BYTES 770

/// The heap slots the effects build their blitters in, as
/// BattleEffect_LoadBlitter and HeapSlot_Release take a slot: a byte offset
/// into Ram_HeapSlots. The second is released before the first.
#define HEAP_SLOT_RAM_CODE 104   // Ram_HeapSlots->unk64[1]
#define HEAP_SLOT_RAM_CODE_2 188 // Ram_HeapSlots->unkb8[1]

/// Clips overflow on all four sides.
#define EFFECT_CLIP_BOUNDS (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

/// Builds a blitter for the canvas in heap slot `slot`, for one clip, mirror
/// and blend mode (BLIT_ADDITIVE to BLIT_COPY).
#define EFFECT_LOAD_BLITTER(slot, flags, mode)                                 \
  BattleEffect_LoadBlitter(slot, (mode) << 4 | (flags))

/// How the canvas transfer task treats the canvas after each copy
/// (work->transferMode); work->transferValue is its argument.
enum CanvasTransferMode {
  CANVAS_COPY,             ///< copy as drawn
  CANVAS_COPY_AND_CLEAR,   ///< copy, then fill the canvas with transferValue (0
                           ///< clears it)
  CANVAS_COPY_AND_FADE,    ///< copy, keeping half of each pixel's level for the
                           ///< next frame when transferValue is
                           ///< CANVAS_FADE_HALF, and three quarters for any
                           ///< other value
  CANVAS_COPY_AND_DARKEN,  ///< copy, then lower each level by a byte of
                           ///< transferValue, stopping at 0
  CANVAS_COPY_AND_BRIGHTEN ///< copy, then raise each level by a byte of
                           ///< transferValue
};

/// CANVAS_COPY_AND_FADE's transferValue: the percentage of each pixel's level
/// kept for the next frame.
enum CanvasFadeShare { CANVAS_FADE_HALF = 50, CANVAS_FADE_THREE_QUARTERS = 75 };

/// Sets what the canvas transfer leaves in the canvas after each copy.
#define EFFECT_SET_TRANSFER(work, mode, value)                                 \
  ((work)->transferMode = (mode), (work)->transferValue = (value))

/// Sets the transfer, then starts copying the canvas to VRAM once a frame.
#define EFFECT_START_TRANSFER(work, mode, value)                               \
  (EFFECT_SET_TRANSFER(work, mode, value),                                     \
   Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME))

/// Presents the canvas and waits for the next frame.
#define EFFECT_PRESENT_CANVAS(work) ((work)->transferPending = 1, wait(1))

/// The usual end of an effect's frame: counts down the targets' flash timers,
/// then presents the canvas.
#define EFFECT_END_FRAME(work)                                                 \
  (BattleEffect_CountDownBattlerTints(), EFFECT_PRESENT_CANVAS(work))

/// Frees the two blitters, the second first.
#define EFFECT_RELEASE_BLITTERS()                                              \
  (HeapSlot_Release(HEAP_SLOT_RAM_CODE_2), HeapSlot_Release(HEAP_SLOT_RAM_CODE))

/// The usual end of a canvas effect: the transfer, the blitters, then the
/// canvas layer.
#define EFFECT_FINISH_CANVAS()                                                 \
  (Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer),                   \
   EFFECT_RELEASE_BLITTERS(), BattleEffect_EndCanvasLayer())

/// The same for an effect that drew through one blitter.
#define EFFECT_FINISH_CANVAS_ONE_BLITTER()                                     \
  (Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer),                   \
   HeapSlot_Release(HEAP_SLOT_RAM_CODE), BattleEffect_EndCanvasLayer())

/// Draws mote picture `size` (1 to 10: size pixels wide, twice that tall),
/// centred across x and standing on y.
#define EFFECT_DRAW_MOTE(draw, canvas, sheet, x, y, size)                      \
  (draw)(canvas, (sheet) + gBattleEffect_MotePictures[(size) - 1],             \
         (x) - (size) / 2, (y) - (size), size, (size) * 2)

/// Marks particles[0] to particles[count - 1] free: a particle whose variant
/// is -1 is not drawn or moved until an effect gives it a life.
#define EFFECT_FREE_PARTICLES(i, particles, count)                             \
  for ((i) = 0; (i) != (count); (i)++)                                         \
  (particles)[i].variant = -1

/// Shows the canvas stretched to twice its width, `top` sixteenths of its
/// colour added over what lies under it.
#define EFFECT_SHOW_WIDE_CANVAS(top)                                           \
  (REG_DISPCNT = EFFECT_DISPCNT, REG_BG2PA = Q_8_8(0.5),                       \
   REG_BLDALPHA = BLEND_ALPHA(top, 16),                                        \
   REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)

/// Which way BattleEffect_WipeCanvas sweeps.
enum CanvasWipeDirection {
  CANVAS_WIPE_DOWN,  ///< top to bottom
  CANVAS_WIPE_ACROSS ///< left to right
};
/// What BattleEffect_WipeCanvas leaves behind it.
enum CanvasWipeFill {
  CANVAS_WIPE_FILL, ///< colour 1, which its callers make black first
  CANVAS_WIPE_CLEAR ///< colour 0: the canvas cleared
};

/// Lets the battle show the damage, playing hitSound (0 keeps the one queued)
/// as the targets react; a second use changes nothing.
#define EFFECT_SHOW_DAMAGE(hitSound) BattlePlayback_StartFar(hitSound)

/// Passed to BattleEffect_TintBattler as its palette, pose or timer slot: leave
/// that one as it is.
#define TINT_KEEP (-1)

// The declarations the parts share.

struct Vec2;

struct AnimationEntry;
struct BattleEffectArgument;
struct EffectPosition;
struct EffectStep;
struct FxPartyList;
struct MeshTriangle;
struct PoolEntry;
struct SpriteScale;
struct ScaledSprite;
struct SpriteScale;
struct StripQuad;

// The parts' own routines, by the part that defines them.

// display.c
void BattleEffect_AdvanceScrollOnInterval(void);
// Configures the battle display for an effect.
void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_AdvanceBg2OnInterval(void);

// canvas.c
void BattleEffect_StartCanvasLayer(s32 bgControl);
/// Closes the canvas layer.
void BattleEffect_EndCanvasLayer(void);
/// Counts down the actors' flash timers.
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_StartTiledCanvas(s32 bg_control);
void BattleEffect_StartTiledCanvasFilled(s32 bg_control);
void BattleEffect_ProcessPendingBlock2Transfer(void);
void Display_ApplyBg2Affine(void);
// Sets up the canvas tile map on BG1.
void BattleEffect_SetupCanvasTileMap(void);
// Marks the transition done and shows the battle view again.
void BattleEffect_HideBg1AndRestoreBackdrop(void);
void BattleEffect_FlushPendingGraphicsTransfer(void);
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color);
void BattleEffect_BuildCanvasTilemap(void);
void BattleEffect_ProcessPendingStampTransfer(void);

// mirrored_wall.c
/// Fetches the max and additive blitters, flipped or not.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);

// prism.c
void BattleEffect_UpdatePhasePalette(s32 frame, s32 redPhase, s32 greenPhase,
                                     s32 bluePhase);

// djinn_heal.c
void BattleCamera_ApplyPhasedDelta(void);

// dispatch.c
void BattleEffect_SelectLivingTargets(struct FxPartyList *party);
void BattleEffect_CallDoNothing(s32 cnt);
void BattleEffect_BuildWindowEdgeTable(void);

// battler_tint.c
/// Flashes an actor in a palette for some frames.
void BattleEffect_TintBattler(s32 setId, s32 objectValue, s32 groupValue,
                              s32 stateSlot, s32 stateValue);
void BattleEffect_SetupBlendedDisplay(void);

// stat_change.c
void BattleEffect_RunImpair(void *action);
void BattleEffect_RunDull(void *action);
void BattleEffect_RunWeaken(void *action);

// eruption.c
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant);

// projectiles.c
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
// BattleEffect_BuildBlitter itself.
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world);
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames, s32 rate);

// djinn_attacks.c
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal);

// hail.c
/// Shakes the camera while work->shakeFrames lasts.
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void Camera_ApplyVerticalShake(s32 range);

// gaia.c
struct AnimationEntry *AnimationObject_AllocateEmpty(void);

// spider_web.c
// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffect_RunSwirlSwipe(struct BattleEffectArgument *effect, s32 mode);
void Palette_MirrorBgColors(void);
void Graphics_LoadGreyRamp(void);

// azul.c
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);
// The middle between the first and the last target.
void BattleEffect_GetTargetsMidpoint(struct EffectPosition *position);
void Mesh_BuildGrid(struct MeshTriangle *triangles, s32 columns, s32 rows,
                    s32 cellWidth, s32 cellHeight);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position);

// positions.c
void BattleEffect_RunCastingImpact(struct BattleEffectArgument *action,
                                   s32 kind);
void BattleEffect_RunCriticalHit(struct BattleEffectArgument *effect);
void BattleEffect_RunAttackSlash(struct BattleEffectArgument *effect);

// casting_impact.c
// Steps the background colours one shade toward a resource's palette.
void BattleEffect_StepPaletteToResource(s32 resource);

// canvas_routine.c
void BattleEffect_WipeCanvas(s32 direction, s32 fill);

// particle_streams.c
void Palette_BrightenBattleBg(s32 red_delta, s32 green_delta, s32 blue_delta);
void BattleEffect_PlaceFormationObjects(s32 formation, s32 x, s32 z);
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y);
void BattleEffect_ArmPaletteHBlankDma(void);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);
void Palette_BrightenSavedBattleBg(s32 redDelta, s32 greenDelta, s32 blueDelta);
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta);

// ramses.c
// An actor's recoil motion, through a far call.
void BattleMotion_ApplyKnockbackFar(s32 actor, s32 motion);

// ragnarok.c
void BattleEffect_RunFallingSword(struct BattleEffectArgument *effect,
                                  s32 mode);

// thrash.c
void BattleEffect_RunFlashingStrike(struct BattleEffectArgument *effect,
                                    s32 variant);

// undead_sword.c
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunWaterSheet(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunWindFunnel(struct BattleEffectArgument *effect, s32 mode);

// heavy_divide.c
void BattleEffect_RunHeavyBlow(struct BattleEffectArgument *effect, s32 mode);

// thunder_mine.c
void BattleEffect_RunBoltTrail(struct BattleEffectArgument *effect, s32 mode);

// whorl.c
void BattleEffect_RunWaterColumn(struct BattleEffectArgument *effect, u32 mode);
void BattleEffect_RunDjinniCharge(struct BattleEffectArgument *effect,
                                  s32 mode);

// rime.c
void BattleEffect_RunDjinniSign(struct BattleEffectArgument *effect, s32 mode);

// gasp.c
void BattleEffect_RunGaspFamily(struct BattleEffectArgument *effect, s32 mode);

// raging_heat.c
void RagingHeat_FillGradientCanvas(void);

// rushing_strike.c
void BattleEffect_RunRushingStrike(struct BattleEffectArgument *effect,
                                   s32 mode);
void BattleEffect_BuildRepeatingTilemap(void);

// glowing_lunge.c
void BattleEffect_RunMirroredWall(struct BattleEffectArgument *effect,
                                  s32 mode);

// vengeance.c
void BattleEffect_RunGeyser(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunStarSwarm(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunBubbleColumn(struct BattleEffectArgument *effect);

// helm_splitter.c
void BattleMotion_ApproachTargetFar(s32 actorId, s32 targetId,
                                    s32 travelDivisor, s32 initialVelocityY);

// legend.c
void BattleEffect_RunElementCards(struct BattleEffectArgument *effect,
                                  s32 mode);

// cards.c
void BattleEffect_RunCards(struct BattleEffectArgument *effect, s32 mode);

// breaths.c
void Canvas_BlitAtFoot(void *source, s32 x, s32 y, s32 size);
void BattleEffect_RunBreaths(struct BattleEffectArgument *effect, s32 mode);
void BattleEffect_RunCrescentSlash(struct BattleEffectArgument *effect,
                                   s32 mode);

// trident.c
void BattleEffect_RunLeapingStrike(struct BattleEffectArgument *effect,
                                   s32 mode);
void BattleEffect_ArmMeteorBlowPaletteDma(void);

// liquifier.c
void BattleEffect_RunCanvasModeByCount(struct BattleEffectArgument *effect);

// odyssey.c
// The five-argument blitter builder's far call, which nothing calls; it serves
// only the never-taken call below.
s32 BattleEffect_BuildBlitterFar(s32 slot, s32 widthShift, s32 heightShift,
                                 s32 flags, u32 mode);
void Iris_WipeScreen(u8 side, u8 speed, void (*midpoint)(void));
void Iris_RestoreBattle(void);
void BattleEffect_BuildSteepShadeRampWave(u8 *table, u32 count, u32 repeats,
                                          s32 scale);

// moloch.c
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);

// Routines of other banks the parts call.
void Animation_ApplyChildArgumentFar(void *object, s32 value);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void BattleBackground_LoadFar(s32 layer, s32 resource, s32 mode);
void BattleBackground_SetModeFar(s32 mode, s32 background, s32 speed);
void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);
void BattleCamera_SetViewAtPairMidpointFar(s32 actor, s32 target, s32 frames);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);
void BattleEffect_RunMysticFlame(struct BattleEffectArgument *);
void BattleObject_PutFar(void);
void BattleObject_SpawnObjectsForListFar(s16 *actors, s32 a, s32 b);
/// BattlePlayback_Start through its far call.
void BattlePlayback_StartFar(s32 phase);
s32 BattleSprite_HasHeavyImpactFar(s32 enemyId);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void *GetMotionRecordFar(void *object, s32 index);
u32 Heap_GetRemainingIwram(void);
s32 Inn_RoomPrice(s32 innId);
void Palette_LoadBasicSpriteColors(void);
u32 Random16(void);
void ReleaseBattleObjectRecordsFar(s32 actor);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void SceneTransform_ApplyPosition(const s32 *pos);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyScale(s32 *scale);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyUniformScale(s32 scale);
void Shop_InitializeWork(void);
void Shop_ReleaseWork(void);
void Sprite_RemoveAnimationFar(void *record, s32 entry);
void Transform_ApplyPitch(s32 angle);
// The roll step of the scene transform.
void Transform_ApplyRoll(s32 angle);
void Transform_ApplyYaw(s32 angle);
void Transform_Pop(void);
void Transform_Push(void);
void Transform_ResetMatrix(void);
void Transform_SetLookAt(s32 *eye, s32 *target);
s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void VramBlock_Load93Far(void);
void VramBlock_Reset93Far(void);
void wait(s32 frames);

// Tables and resources, by the first part that reads them.

// falling.c
// The cone's 64 vertices, stored at the buffer's constant address.
extern u8 gMapCellBuffer[];

// cruel_ruin.c
extern char ResourceId_Data0B4;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0C9;
extern char ResourceId_Data0CA;
extern char ResourceId_Data104;
extern char ResourceId_Data134; // the spark sprites
extern char ResourceId_CornerRuneASheet;
extern char ResourceId_ForestCanopyTiles;
extern char ResourceId_SunbeamCloudsBackdrop;
extern char ResourceId_VioletSkyBackdrop;

// canvas.c
// Rows of Resource_DirectoryTable; the directory has yet to name most of them.
extern char ResourceId_EarthHorizonBackdrop;
// Rows of Resource_DirectoryTable; the directory has yet to name most of them.
extern char ResourceId_AerialViewBackdrop;
extern char ResourceId_GoldenDragonTiles;
extern char ResourceId_GoldenDragonHeadTiles;
extern char ResourceId_BlueFlameSheet;
extern char ResourceId_DustBlobSheet;
extern char ResourceId_TornadoSheet;
extern char ResourceId_Data0C1;

// projectiles.c
extern char ResourceId_BlastSheet;
extern char ResourceId_MercuryDjinnSmallSheet;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_VenusDjinnSmallSheet;
// Each mote size's picture in the particle sheet.
extern u16 gBattleEffect_MotePictures[];
// Directory rows, named by their assets.
extern char ResourceId_CometSheetA;
extern char ResourceId_CometSheetB;
extern char ResourceId_CometSheetC;
extern char ResourceId_IceBlockSheet;
extern char ResourceId_WindStreakSheet;
extern char ResourceId_Data0FF;
extern char ResourceId_LimePalette;
extern char ResourceId_SmokeSheet;
extern char ResourceId_WaterSpraySheet;
extern char ResourceId_ShurikenSheet;
extern char ResourceId_ProjectileVolleyImage;
extern char ResourceId_YellowSparkSheet;
extern char ResourceId_FireballSheet;
extern char ResourceId_CrystalSheet;
extern char ResourceId_BlueBeamSheet;
extern char ResourceId_IceChipSheet;
extern u8 gProjectileVolley_CometHeights[]; // by kind
extern u8 gProjectileVolley_Modes[][5];     // five bytes for each mode
extern u8 gProjectileVolley_FlashWidths[];
extern u8 gProjectileVolley_FlashHeights[];
extern u8 gProjectileVolley_FlashSteps[];
extern u8 gProjectileVolley_FlashRises[];
extern u16 gProjectileVolley_FlashPictures[];
extern u16 gProjectileVolley_PelletPictures[];
extern u8 gProjectileVolley_GlintFlips[]; // the glints' palettes
extern u16 gBattleEffect_GlintPictures[]; // the flashes' pictures: offsets,
extern u8 gBattleEffect_GlintPictureWidths[];
extern u8 gBattleEffect_GlintPictureHeights[]; // and heights
// The crystal shards' pictures.
extern s32 gPrism_ShardPictures[];
extern u8 gPrism_ShardWidths[];
extern u8 gPrism_ShardHeights[]; // and heights

// djinn_attacks.c
/// The frames the Djinn throws its five flames on.
extern const u8 gDjinnAttacks_LaunchFrames[];
extern char ResourceId_FlameballSheet;
extern char ResourceId_GlowOrbSheet;
extern char ResourceId_EmberStreakSheet;
// The ember puff's six pictures: widths, heights, rise and offsets.
extern u8 gBattleEffect_PuffArcPictureWidths[];
extern u8 gBattleEffect_PuffArcPictureHeights[];
extern u8 gBattleEffect_PuffArcPictureBiasY[];
extern u16 gBattleEffect_PuffArcPictures[];

// hail.c
// The ice chip's four pictures: offsets, widths and heights.
extern u16 gHail_Pictures[];
extern u8 gHail_PictureWidths[];
extern u8 gHail_PictureHeights[];
extern char ResourceId_PortalSheet;
// The chips: their flips (a blitter's flags), and, by shape, their pictures,
// places and sizes.
extern const u8 gHail_CrackleFlips[];
extern const u16 gHail_PortalPictures[];
extern const u8 gHail_PortalX[];
extern const u8 gHail_PortalY[];
extern const u8 gHail_PortalWidths[];
extern const u8 gHail_PortalHeights[];

// thor.c
extern char ResourceId_Data135;
extern char ResourceId_DuskCloudsBackdrop;
extern char ResourceId_JupiterDjinnSheet;
extern char ResourceId_LightFanSheet;
extern char ResourceId_LightningPillarSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_ParticleSpritesC;
extern char ResourceId_VioletLightningSheetB;
extern char ResourceId_VioletMoteSheet;
extern const s16 gThor_Points[6][2]; // the three lines' end points
extern const u8 gThor_TargetPalettes[];
extern const u8 gThor_CrackleFlips[];

// spire.c
extern char ResourceId_BoulderSheet;
// By variant.
extern u8 gSpire_RockCounts[];
// x, y in the boulder for each shard.
extern u8 gSpire_ShardOffsets[];
// By boulder.
extern s8 gSpire_RockX[];
extern u8 gSpire_DropFrames[];
// The shards' pictures, widths and heights.
extern u16 gSpire_ShardPictures[];
extern u8 gSpire_ShardWidths[];
extern u8 gSpire_ShardHeights[];

// flurry.c
extern char ResourceId_YellowOrbSheet;
extern char ResourceId_BlueArcSheetA;
extern char ResourceId_BlueArcSheetB;
extern char ResourceId_WaveSheet;
// The variants' records, eight bytes each.
extern u8 gBurstScene_Styles[];
// A glint's four flips.
extern const u8 gBurstScene_GlintFlips[];

// gaia.c
extern char ResourceId_EarthWallSheet;
extern char ResourceId_OrangePaletteA;
// By side and variant.
extern s8 gGaia_Bg2Shifts[][3];
// The growing wall's nine rows.
extern u8 gGaia_RowWidths[];
extern u16 gGaia_RowPictures[];
// The flame's three pictures.
extern u8 gGaia_FlameWidths[];
extern u8 gGaia_FlameHeights[];
extern u16 gGaia_FlamePictures[];
extern u8 gGaia_LateRockColumns[];
extern struct Scale gGaia_UnitScale;

// spider_web.c
// A scale of one in 16.16 on both axes.
extern const struct SpriteScale gUlysses_ParticleScale;
extern char ResourceId_Data0B1;
extern char ResourceId_Data0B3;
extern char ResourceId_RuneCircleSheet;
extern char ResourceId_Data0C8;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_RuneSheet;
extern u8 Data_02010200[];
// Byte offsets of the flare pictures in the spark sheet, by size - 1.
extern u16 gBattleEffect_FlarePictures[];
extern const u16 gUlysses_Pictures[];
extern const u8 gUlysses_PictureWidths[];
extern const u8 gUlysses_PictureHeights[];
extern const s8 gUlysses_ParticleStarts[];
extern const u8 gUlysses_ObjectX[];
extern const u8 gUlysses_ObjectZ[];
// The burst quad: its face list and its four corners, 64x64 pixels standing
// on its base.
extern const u8 gBattleEffect_SquareMesh[];
// Triple Chomp's jaw quad: its mesh and its four corners.
extern const u8 gBattleEffect_LargeSquareMesh[];
extern const u8 gBattleEffect_GroundSquareCorners[]; // its four corners
extern char ResourceId_MagentaSwirlSheet;
extern char ResourceId_MagentaTailSheet;
// The swirl's six pictures: offsets, x (six for each side), widths, rise and
// heights.
extern const u16 gSpiderWeb_Pictures[];
extern const u8 gSpiderWeb_PictureX[];
extern const u8 gSpiderWeb_PictureWidths[];
extern const s8 gSpiderWeb_PictureBiasY[];
extern const u8 gSpiderWeb_PictureHeights[];
extern char ResourceId_SpiderWebSheet;

// azul.c
extern char ResourceId_WispSheet;
// The 64 sparks, past the map cells' first 16 KB.
extern u8 gData_02014000[];
extern const u8 gAzul_PartPoints[][2];
extern const u8 gAzul_UnitScale[];
extern const u8 gAzul_ObjectX[];
extern const u8 gAzul_ObjectZ[];
extern const u8 gAzul_LateUnitScale[];
extern const s8 gAzul_LateObjectX[];
extern const s8 gAzul_LateObjectZ[];
extern const u8 gBattleEffect_SquareCorners[]; // its corners

// positions.c
extern char ResourceId_BeamImage;
extern char ResourceId_StarCrescentSheet;
// The strike's six pictures: offsets, reach, widths and heights.
extern u16 gBeam_Pictures[];
extern u8 gBeam_PictureReaches[];
extern u8 gBeam_PictureWidths[];
extern u8 gBeam_PictureHeights[];
extern char ResourceId_GreyCrescentSheet;

// casting_impact.c
extern char ResourceId_EffectPalette01;
extern char ResourceId_ImpFaceSheet;
extern char ResourceId_Data0DA;
extern char ResourceId_Data0DD;
extern char ResourceId_BoulderPicture;
extern char ResourceId_Data0F0;
extern char ResourceId_Data0F1;
extern char ResourceId_CloudPuffSheet;
extern char ResourceId_FlameSheetA;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_RedPaletteB;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_FireStreakSheet;
extern char ResourceId_CyanPalette;
extern char ResourceId_TanPalette;
extern char ResourceId_EffectPalette05;
extern char ResourceId_SkullSheet;
extern char ResourceId_CounterRevealSheet;
extern char ResourceId_RedCrescentSheetA;
extern char ResourceId_BlueBeastSheet;
extern char ResourceId_RedCrescentSheetB;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_DustPuffSheet;
extern char ResourceId_RockSpireSheet;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_TwelveModeImage;
// The reaper's seven panels: x by side (seven each), then y.
extern u8 gCastingImpact_PictureX[];
extern u8 gCastingImpact_PictureY[];
extern u8 gPrism_ShardStarts[];
extern const u8 gCastingImpact_SquallGlintFlips[];
extern const u8 gCastingImpact_SapGlintFlips[];
extern const u8 gCastingImpact_BitePictures[];
extern const u16 gCastingImpact_RaidensWrathPictures[];
extern const u16 gCastingImpact_RaidensWrathWidths[];
extern const u16 gCastingImpact_RaidensWrathHeights[];
extern u16 gCastingImpact_RaidensWrathX[];
extern u16 gCastingImpact_RaidensWrathY[];
extern const u16 gCastingImpact_ApocalypsePictures[];
extern const u16 gCastingImpact_ApocalypseWidths[];
extern const u16 gCastingImpact_ApocalypseHeights[];
extern const u16 gCastingImpact_ApocalypseX[];
extern u16 gCastingImpact_ApocalypseY[];
extern const u16 gCastingImpact_ApocalypseBurstPictures[];
extern const u8 gCastingImpact_ApocalypseBurstWidths[];
extern const u8 gCastingImpact_ApocalypseBurstHeights[];
extern u8 gCastingImpact_ApocalypseBurstPoints[][2];
extern const u16 gCastingImpact_IceChipPictures[];
extern const u8 gCastingImpact_StunBoltGlintFlips[];
extern const u8 gCastingImpact_StunJipBandStarts[];
extern const u8 gCastingImpact_StunJipBandGrowth[];
extern const s8 gCastingImpact_StunJipBandHeights[];
extern const u8 gCastingImpact_ArmorCrushWaveStarts[];
extern const u8 gCastingImpact_ArmorCrushWaveGrowth[];
extern const u8 gCastingImpact_ArmorCrushWaveStretch[];
// The geyser: a mesh of sixteen corners and its faces.
extern const u8 gBattleEffect_OctagonBandCorners[];
extern const u8 gCastingImpact_BandMesh[];
extern const u8 gBattleEffect_RingCorners[];
extern const u8 gBattleEffect_RingMesh[];
extern const u8
    gBattleEffect_TaperedBeamCorners[];     // its corners (Thunder Mine)
extern const u8 gBattleEffect_ColumnMesh[]; // a bolt's mesh

// particle_streams.c
extern char ResourceId_FirePillarSheetA;
extern u8 gNereidCopy_Columns[]; // the pillars' feet, x and y
extern const s32 gNereidCopy_Gravity[];
extern const u8 gHelmSplitter_TwelveX[];
extern const u8 gHelmSplitter_TwelveZ[];
extern const u8 gHelmSplitter_RightEightX[];
extern const u8 gHelmSplitter_RightEightZ[];
extern const u8 gFormationObjects_X[];
extern const u8 gFormationObjects_Z[];
extern const u8 gHelmSplitter_SmallEightX[];
extern const u8 gHelmSplitter_SmallEightZ[];
extern const s8 gFormationObjects_WideX[];
extern const u8 gFormationObjects_WideZ[];
extern char ResourceId_FlareSheet;
extern char ResourceId_FlareImage;
extern u16 gHelmSplitter_ImpactBurstPictures[];
extern u8 gHelmSplitter_ImpactBurstPictureWidths[];
extern u8 gHelmSplitter_ImpactBurstPictureHeights[];
extern char ResourceId_FlameSheetB;
extern s16 gParticleStreams_OrbitPoints[][3];
extern s16 gParticleStreams_FireballPoints[][2];
extern u16 gParticleStreams_FlashPictures[];
extern u16 gParticleStreams_FlashSizes[];

// ramses.c
extern char ResourceId_DesertBackdrop;
extern u8 gRamses_StatueColumns[];
extern u8 gRamses_StatueRows[];
extern u16 gRamses_PuffPictures[];
extern u16 gRamses_PuffSizes[];

// annihilation.c
extern char ResourceId_FireBurstSheet;

// ragnarok.c
extern char ResourceId_FlameBladeSheet;
extern u8 gRagnarok_StrikeColumns[];
extern u8 gRagnarok_FlashPictures[];
extern char ResourceId_SwordSheet;
extern u8 gTitanBlade_FlashPictures[];
extern s32 gTitanBlade_DustGravity[]; // the dust's four gravities
extern char ResourceId_BeachBackdrop;
extern char ResourceId_ForestBackdrop;
extern char ResourceId_WingedKnightATiles;
extern char ResourceId_WingedKnightBTiles;
extern char ResourceId_WingedKnightCTiles;
extern char ResourceId_BlueCrescentSheet;
extern char ResourceId_ExplosionSheet;
extern char ResourceId_OverlayImageA;
extern const u8 gJudgment_BurstPictures[];
extern const u8 gJudgment_ShardWidths[];
extern const u8 gJudgment_ShardHeights[];
extern const u16 gJudgment_ShardPictures[];

// kirin.c
extern char ResourceId_FireBlobSheet;
extern u8 gKirin_Columns[]; // the row's x offsets
extern u8 gKirin_Rows[];    // the row's heights

// neptune.c
extern char ResourceId_SeaSerpentTiles;
extern struct Scale gNeptune_UnitScale; // the serpent's scale
extern s32 gNeptune_Gravity[];          // the spray's four gravities

// thrash.c
extern char ResourceId_ArcFragmentsSheet;
extern char ResourceId_RevealMaskA;
extern char ResourceId_RevealMaskB;
extern char ResourceId_RevealMaskC;
extern char ResourceId_RevealMaskD;
extern char ResourceId_RevealMaskE;
extern char ResourceId_RevealMaskF;

// undead_sword.c
extern char ResourceId_LimeColumnSheet;
extern const u16 gUndeadSword_SparkPictures[];
extern const u16 gUndeadSword_SparkSizes[];
extern char ResourceId_CrackedTextureSheet;
extern char ResourceId_Data0D3;
extern u8 Data_02010108[];
extern u8 Data_02010318[];
extern const u8 gWindFunnel_Counts[]; // the funnels per mode
extern char ResourceId_CrystalBitmapSheet;
extern u8 Data_02014e20[]; // gMapCellBuffer + 20000, the berg
extern const u8 gBattleEffect_CylinderMesh[];    // the ripple's mesh
extern const u8 gBattleEffect_CylinderCorners[]; // its corners
extern const u8 gDiamondBerg_RingStarts[4];      // per ripple: its start,
extern const u8 gDiamondBerg_RingGrowth[4];      // its growth
extern const s8 gDiamondBerg_RingHeights[4];     // and its height
extern const s16 gDiamondBerg_ShardRolls[8];     // per flash: its roll
extern const s16 gDiamondBerg_ShardPitches[8];   // and pitch

// crystal.c
extern char ResourceId_Data0FB;

// geode.c
extern const u8 gGeode_WaveStarts[]; // each ring's start
extern const u8 gGeode_WaveGrowth[];
extern const s8 gGeode_WaveHeights[];
extern const u8 gGeode_RingCorners[];

// nirvana.c
// The spark pictures by stage.
extern u16 gSoulShatter_SparkPictures[];
extern u16 gSoulShatter_SparkSizes[];
extern char ResourceId_Data0CF;
extern u8 Data_02012000[];
extern u8 Data_02012540[];            // the mesh's end, after 112 faces
extern const s8 gSerac_RowHeights[];  // the cone's ring heights
extern const s8 gSerac_RowRadii[];    // and radii
extern const u8 gSerac_RingStarts[];  // each ripple's delay
extern const u8 gSerac_RingGrowth[];  // its growth
extern const s8 gSerac_RingHeights[]; // its height
extern const u8 gBattleEffect_TriangleMesh[];
extern const u8 gBattleEffect_TriangleCorners[];

// heavy_divide.c
extern char ResourceId_GreatSwordSheet;
extern char ResourceId_AxeSheet;
extern const u8 gHeavyBlow_SwordMesh[];      // the sword's mesh
extern const u8 gBattleEffect_BeamCorners[]; // its corners
extern u8 gHeavyBlow_StrikeFrames[];         // per mode: the strike's frame
extern u8 gHeavyBlow_ChipCounts[];           // per mode: the chips
extern const u8 gHeavyBlow_ChipPixels[];     // a chip's two pixels
extern char ResourceId_Data0CE;
// The columns, by number: x, start frame, full height and lean.
extern s16 gCool_ColumnX[];
extern u8 gCool_ColumnStarts[];
extern u8 gCool_ColumnHeights[];
extern u8 gCool_ColumnLeans[];
// The spark pictures by stage.
extern u16 gCool_SparkPictures[];
extern u16 gCool_SparkSizes[];
extern char ResourceId_Data0D8;
extern const s8 gWyrdCurse_DropBounces[]; // the drops' bounce speeds
extern const u8 gBattleEffect_SmallSquareMeshReversed[]; // the glow's and the

// thunder_mine.c
extern char ResourceId_VioletPaletteC;
extern const u8 gBoltTrail_GlintFlips[4]; // their flips
extern char ResourceId_WhirlBeamSheet;
extern char ResourceId_SparkleDots;
extern u16 gBattleEffect_SparklePictures[]; // the sparkles' cells
extern u8 gBattleEffect_SparkleSizes[];     // and sizes
extern const u8 gMegaera_GlintFlips[];
extern const u8 gMegaera_GlintModes[];
extern const s8 gVoidBeam_Corners[]; // the hexagon's corners (z, y)

// light_surge.c
extern char ResourceId_Data0C4;
extern char ResourceId_EffectPalette02;
extern const s32 gLightSurge_PillarSizes[4]; // per pillar: its size,
extern const s8 gLightSurge_PillarShades[4]; // its shade
extern s8 gLightSurge_PillarOffsets[4];      // and its offset

// whorl.c
extern char ResourceId_Data0D2;

// djinni_charge.c
extern char ResourceId_PetalSheet;
extern const u8 gDjinniCharge_EntranceFlags[];   // each mode's flags for
extern const s32 gDjinniCharge_DebrisGravity[4]; // the debris's gravities

// rime.c
extern char ResourceId_StarCrossSheet;

// gasp.c
extern const s8 gGaspFamily_StreakSteps[];
extern const u8 gGaspFamily_StreakMesh[];
extern const u8 gGaspFamily_Counts[];
extern const u8 gBattleEffect_LongBeamCorners[];
extern char ResourceId_LightBurstSheet;
extern char ResourceId_CornerGlowSheet;
extern char ResourceId_Data0D7;
extern char ResourceId_SmokeBandSheet;
extern char ResourceId_VenusDjinnSheet;

// charon.c
extern char ResourceId_CrimsonBeastTiles;
extern char ResourceId_RedSlashTiles;
extern char ResourceId_RedCrescentTiles;
extern char ResourceId_WhiteArrowsTiles;
extern char ResourceId_WhiteWispsTiles;
extern char ResourceId_Data0B5;
extern char ResourceId_CharmRingSheet;
extern char ResourceId_EffectPalette00;
extern char ResourceId_BoltSheet;
extern char ResourceId_CurseSealSheet;
extern char ResourceId_Data0C3;
extern u8 gCharon_FlashFrames[];

// rising_dragon.c
extern char ResourceId_ClawSlashSheet;
extern u8 Data_02011000[];
extern char ResourceId_Data0DC;
// The swing's pictures: widths, heights and offsets; the fist's: heights,
// y offsets and offsets.
extern u8 gHeartbreak_SwingWidths[];
extern u8 gHeartbreak_SwingHeights[];
extern u16 gHeartbreak_SwingPictures[];
extern u8 gHeartbreak_FistHeights[];
extern u8 gHeartbreak_FistY[];
extern u16 gHeartbreak_FistPictures[];

// raging_heat.c
extern char ResourceId_CornerRuneBSheet;
// Per level (effect->variant): the sparks, their spread, the rings' size
// and the embers.
extern const u8 gRagingHeat_Styles[][4];
extern char ResourceId_SmallFlameSheet;
// The life of each spark in the map cell buffer: gMapCellBuffer's
// EffectSteps from their variant field on.
extern s32 Data_02010018[];
extern u8 gJuggle_Styles[][7];

// flora.c
extern const struct Scale gFlora_UnitScale;
extern const u8 gFlora_PetalSizes[];
extern const u16 gFlora_PetalPictures[];
extern const u16 gFlora_BloomFrames[];
extern const s16 gFlora_BloomRolls[];
extern const s16 gFlora_BloomPitches[];
extern const s8 gFlora_BloomX[];
extern const s8 gFlora_BloomY[];
extern char ResourceId_GreenHairedMaidenTiles;
extern char ResourceId_PinkPetalsTiles;
extern u8 Data_02010020[];
extern u8 Data_020100a0[];
extern u8 Data_020102a0[];
extern u16 Data_0201401e[];
extern u16 Data_02014002[];
extern u16 Data_02014260[];
extern char ResourceId_CrossedDaggersSheet;
// The dagger quad: its face list and its four corners, 32x80 pixels from
// (-16, -16) to (16, 64), the point down.
extern const u8 gSabreDance_DaggerMesh[];
extern const u8 gSabreDance_DaggerCorners[];
extern char ResourceId_PinkStarSheet;

// fire_puppet.c
extern char ResourceId_Data0F2;
extern char ResourceId_Data0F3;

// ocean_fist.c
extern const s32 gOceanFist_DropGravity[4]; // the early drops' drags

// rushing_strike.c
extern char ResourceId_RedHazeBackdrop;

// glowing_lunge.c
extern char ResourceId_Data181;
extern char ResourceId_FirePillarSheetB;
// The streak quad's faces (32x64) and the flash quad's (128x128), and the
// corners of a 64x64 square around the origin, which both scale.
extern u8 gBattleEffect_TallMesh[];
// The streak's scale per mode, 16.16.
extern const s32 gRushingStrike_Scales[];

// daedalus.c
extern const u8 gBattleEffect_StandingSquareCorners[]; // its corners
// Per mode: the quad's scale (16.16, doubled when used) and its unk14.
extern const s32 gMirroredWall_BurstScales[];
extern const s32 gMirroredWall_BurstShades[];
extern char ResourceId_BronzeAutomatonTiles;
extern char ResourceId_MissileTiles;
extern char ResourceId_Data0BA;
extern char ResourceId_Data0BB;
extern struct Vec2 gDaedalus_WaveScale;
extern struct Vec2 gDaedalus_ShipScale;
extern struct Vec2 gDaedalus_ParticleScale;
extern s8 gDaedalus_ShakeOffsets[];
extern u8 gDaedalus_DriftPictures[];
extern u8 gDaedalus_DriftStarts[][2];
extern s32 gDaedalus_DriftSpeeds[][2];
extern u8 gDaedalus_DriftFrames[];
extern u16 gDaedalus_ClatterFrames[];
extern u16 gDaedalus_RingStarts[];
extern s8 gDaedalus_RingX[];
extern u8 gDaedalus_SpritePoints[][2];
extern u8 gDaedalus_SplashPoints[][6][2];
extern u16 gDaedalus_ParticleStarts[];
extern const u8 gDaedalusAlt_StrikeFrames[];     // the seven strike frames
extern const u8 gBattleEffect_SmallSquareMesh[]; // the rings' mesh

// vengeance.c
extern char ResourceId_Data0B0;
extern const u8 gVengeance_BladeStrikes[][7]; // per level: which blades strike
extern char ResourceId_Data0F5;
extern u8 Data_020112e8[];                  // the reaper's raised pose
extern u8 Data_0201276b[];                  // the blade's texture
extern u8 Data_0201476b[];                  // the handle's texture
extern u8 Data_0201576b[];                  // the flash's texture
extern const u8 gLifeShear_BladeMesh[];     // the blade's mesh
extern const u8 gLifeShear_HandleMesh[];    // the handle's mesh
extern const u8 gLifeShear_ScytheCorners[]; // their corners
extern char ResourceId_CrossTextureSheet;
// Per mode: the drops' gravity, and their smallest cel.
extern const s32 gGeyser_DropGravity[];
extern const s8 gGeyser_DropMinSizes[];
extern const u8 gGeyser_Faces[];
extern char ResourceId_Data0FD;
// 256 bubbles past the map cells. Each is an EffectStep with its picture in z
// and its life in variant (-1 once it has left the top of the canvas).
extern struct EffectStep gData_02015000[];
// The fifteen pictures of ResourceId_Data0FD, a glowing oval from 16x32 down to
// 3x4: their widths, heights and byte offsets into the sheet.
extern u8 gBubbleColumn_PictureWidths[];
extern u8 gBubbleColumn_PictureHeights[];
extern u16 gBubbleColumn_Pictures[];
extern u8 Data_02014400[];
extern u8 Data_02014800[];
extern const struct Scale gCoatlicue_UnitScale;
extern u8 gCoatlicue_PictureWidths[];
extern u8 gCoatlicue_PictureHeights[];
extern u16 gCoatlicue_Pictures[];
extern u16 gCoatlicue_BurstFrames[];
extern u8 gCoatlicue_BurstX[];
extern u8 gCoatlicue_BurstY[];
extern u8 gCoatlicue_BurstSpreadX[];
extern u8 gCoatlicue_BurstSpacingY[];
extern s8 gCoatlicue_BurstVelocityX[];
extern s8 gCoatlicue_BurstVelocityY[];
extern s8 gCoatlicue_BurstZ[];
extern s8 gCoatlicue_BurstVelocityZ[];
extern u8 gCoatlicue_PictureBias[];
extern u16 gCoatlicue_RingFrames[];
extern u16 gCoatlicue_FlashFrames[];
extern u16 gCoatlicue_GlowFrames[];
extern u8 gCoatlicue_CoilStarts[];
extern u16 gCoatlicue_CoilScales[];
extern u16 gCoatlicue_CoilRolls[];
extern u16 gCoatlicue_CoilGrowth[];
extern u16 gCoatlicue_CoilMaxScales[];
extern char ResourceId_BellMaidenTiles;
extern char ResourceId_Data0FC;

// catastrophe.c
extern char ResourceId_ChainedDemonTiles;
extern char ResourceId_Data0EE;
extern char ResourceId_Data0EF;
extern char ResourceId_Data0F8;
extern char ResourceId_Data0F9;
extern char ResourceId_GoldenScalesTiles;
extern char ResourceId_PurpleArmourTiles;
extern char ResourceId_WhiteSpikesTiles;
extern u8 Data_02016000[];
extern struct EffectStep Data_02016d00[];
extern const struct Scale gCatastrophe_UnitScale;
extern const u8 gCatastrophe_BurstMesh[];
extern const u16 gCatastrophe_FlashFrames[];
extern const u16 gCatastrophe_FlashLengths[];
extern const u8 gCatastrophe_DemonX[];
extern const u8 gCatastrophe_DemonZ[];
extern const u8 gCatastrophe_ArmourX[];
extern const u16 gCatastrophe_ArmourZ[];
extern const u8 gCatastrophe_MirroredDemonX[];
extern const u8 gCatastrophe_MirroredDemonZ[];
extern const u16 gCatastrophe_BurstStarts[];
extern const u8 gCatastrophe_BurstX[];
extern const u8 gCatastrophe_BurstY[];
extern const u8 gCatastrophe_BurstPictures[];
extern const u16 gCatastrophe_CanvasFlashFrames[];
extern const u8 gCatastrophe_StrikeX[];
extern const u8 gCatastrophe_StrikeY[];
extern const u16 gCatastrophe_StrikeFrames[];
extern const u8 gCatastrophe_StrikeSpeedsY[];
extern const s16 gCatastrophe_StrikeRolls[];
extern const s16 gCatastrophe_StrikeTurns[];
extern const s16 gCatastrophe_StrikePictures[];
extern const u8 gCatastrophe_GlintFlips[];
extern const s16 gCatastrophe_ShardRolls[];

// haures.c
extern u8 Data_02010ae8[];
extern u8 Data_02010be0[];
extern u8 Data_02010fa0[];
extern const struct Scale gHaures_ChargeScale;
extern const struct Scale gHaures_OrbitScale;
extern const struct Scale gHaures_OpeningScale;
extern const u8 gHaures_BandMesh[];
extern const u8 gHaures_BurstMesh[];
extern const u8 gHaures_ParticleX[];
extern s32 gHaures_UnitScale[];
extern const u8 gHaures_WingShapes[];
extern u8 gHaures_WingMorphSteps[];
extern const u8 gHaures_PartPoints[];
extern const u8 gHaures_BandStarts[];
// Resource ids, named after their places in the resource directory.
extern char ResourceId_Data0F7;
extern char ResourceId_MossCaveBackdrop;

// helm_splitter.c
extern char ResourceId_FireSwirlSheet;
extern u16 gHelmSplitter_Pictures[];
extern s8 gHelmSplitter_PictureBiasX[];
extern s8 gHelmSplitter_PictureBiasY[];
extern u8 gHelmSplitter_PictureWidths[];
extern u8 gHelmSplitter_PictureHeights[];
extern char ResourceId_DualStreamPath;
// How many of the 56 embers are sparks, by variant (48 and 40); the rest
// are flashes.
extern u8 gDragonCloud_SparkCounts[];

// legend.c
extern char ResourceId_BladeSheet;
extern char ResourceId_Data0E9;
extern s32 gLegend_WingCorners[];
extern char ResourceId_VineCardSheet;
extern char ResourceId_DragonCardSheet;
extern char ResourceId_SunCardSheet;
extern char ResourceId_BoltCardSheet;
// Each card's length in frames: 124, 140, 132 and 130.
extern u8 gElementCards_Frames[];

// cards.c
extern char ResourceId_SheepCardSheet;
extern char ResourceId_ButterflyCardSheet;
extern char ResourceId_SwordCardSheet;
extern char ResourceId_DaggerSheet;
extern char ResourceId_PaletteRampImage;
extern char ResourceId_SkeletonCardSheet;
// The dagger's shards.
extern u16 gStatChange_ShardPictures[];
extern u8 gStatChange_ShardWidths[];
extern u8 gStatChange_ShardHeights[];

// triple_chomp.c
// The three bites' frames: 0, 12 and 24.
extern u8 gTripleChomp_BiteFrames[];

// breaths.c
extern char ResourceId_DustCloudSheet;
// Each breath's length, the frame its burst starts, and the glow's widest
// size by variant.
extern u8 gBreaths_Frames[];
extern u8 gBreaths_BurstFrames[];
extern u8 gBreaths_MaxSizes[];
// The four jaws of variant 2, by breath: their start frames, their growth,
// and their x offsets from the mouth.
extern u8 gBreaths_RingStarts[];
extern u8 gBreaths_RingGrowth[];
extern s8 gBreaths_RingX[];
// The frame each breath hits the targets.
extern u8 gBreaths_FlashFrames[];

// trident.c
extern char ResourceId_Data0FA;
extern u8 Data_02011400[];
extern u8 Data_02012964[];             // 128 sparks
extern const u8 gTrident_RingStarts[]; // each ring's start
extern const u8 gTrident_RingGrowth[];
extern const s8 gTrident_RingHeights[];
extern const s32 gTrident_Spins[]; // each trident's yaw and turn
extern const u8 gTrident_ProngCorners[];
extern const u8 gTrident_GlintFlips[];
extern const u8 gLeapingStrike_WaveStarts[];
extern const u8 gLeapingStrike_WaveGrowth[];
extern const s8 gLeapingStrike_WaveHeights[];
extern const u8 gLeapingStrike_RingStarts[];
extern const u8 gLeapingStrike_RingGrowth[];
extern const s8 gLeapingStrike_RingHeights[];

// meteor_blow.c
extern char ResourceId_GlowOrbSetSheet;
extern const u8 gMeteorBlow_PictureWidths[];
extern const u8 gMeteorBlow_PictureHeights[];
extern const u16 gMeteorBlow_Pictures[];
extern const u8 gMeteorBlow_WaveStarts[];
extern const u8 gMeteorBlow_WaveGrowth[];
extern const u8 gMeteorBlow_WaveStretch[];
extern const u8 gMeteorBlow_RingStarts[];
extern const u8 gMeteorBlow_RingGrowth[];
extern const s8 gMeteorBlow_RingHeights[];

// megiddo.c
extern const u16 gMegiddo_FlashStarts[]; // the five rings' start frames
extern const s16 gMegiddo_FlashPitches[];
extern const s16 gMegiddo_FlashYaws[];
extern const s16 gMegiddo_RayPitches[];
extern const s16 gMegiddo_RayYaws[];
extern const s16 gMegiddo_RayRolls[];
extern const s8 gMegiddo_RayX[];
extern const s8 gMegiddo_RayY[];
extern char ResourceId_Data103;

// odyssey.c
extern char ResourceId_Data102;
extern char ResourceId_Data101;
extern const s16 gOdyssey_FlashPitches[];
extern const s16 gOdyssey_FlashYaws[];
extern char ResourceId_CloudSwirlSheet;

// moloch.c
extern const u8 gMoloch_MoteSizes[];
extern const u8 gMoloch_RingStarts[];
extern const u8 gMoloch_RingGrowth[];
extern const s8 gMoloch_RingHeights[];
extern const u16 gIris_PillarMesh[];
extern char ResourceId_SunsetBackdrop;
extern char ResourceId_StarfieldFlareBackdrop;
extern char ResourceId_RobedGoddessTiles;
extern u8 Data_02010578[];
extern const struct Scale gIris_UnitScale;
extern s32 gIris_SplashRadii[];
extern const u8 gIris_SplashOffsetsY[];
extern const u8 gIris_ParticleCorners[];
extern const u8 gIris_ParticleMesh[];
extern const u8 gIris_PsynergySoundFrames[];
extern const u16 gIris_DoorSoundFrames[];
extern const u16 gIris_PowerUpSoundFrames[];
extern const s8 gIris_StreakX[];
extern const u8 gIris_StreakY[];
extern const u16 gIris_StreakAngles[];
extern const s16 gIris_StreakSpeeds[];
extern const u8 gIris_LineShades[];
extern s32 gIris_MoteSizes[];
extern const u16 gIris_SplashStarts[];
extern const u16 gIris_SplashEnds[];
extern const u8 gIris_SplashX[];
extern const u8 gIris_SplashY[];
extern const u8 gIris_SplashSmall[];
extern const u8 gIris_DropSpeeds[];
extern const u8 gIris_DropFrames[];
extern const u8 gIris_DropPictureSteps[];
extern const u8 gIris_LeftStripCorners[];
extern const u8 gIris_RightStripCorners[];
extern const u8 gIris_StripMesh[];
extern const u8 gIris_ConeCorners[];
extern const u8 gIris_ConeMesh[];
extern const u32 BattleEffect_PatchTemplateCode[];
extern const u32 BattleEffect_Mask[];
extern const u16 Data_081973f0[];
extern const u32 BattleEffect_CopyReverse[];
extern const u32 BattleEffect_CopyForward[];
extern const u32 BattleEffect_Copy8Reverse[];
extern const u32 BattleEffect_Copy8Forward[];
extern const u32 BattleEffect_TransparentReverse[];
extern const u32 BattleEffect_TransparentForward[];
extern const u32 BattleEffect_Transparent8Reverse[];
extern const u32 BattleEffect_Transparent8Forward[];
extern const u32 BattleEffect_MaxReverse[];
extern const u32 BattleEffect_MaxForward[];
extern const u32 BattleEffect_Max4Reverse[];
extern const u32 BattleEffect_Max4Forward[];
extern const u32 BattleEffect_AdditiveReverse[];
extern const u32 BattleEffect_AdditiveForward[];
extern const u32 BattleEffect_ColorReverse[];
extern const u32 BattleEffect_ColorForward[];
extern const u8 Data_081380ac[];
extern u8 Data_081380acCodeSize[];
extern const u8 Data_0813b9a0[];
extern u8 Data_0813b9a0CodeSize[];
// The rasterizer kernels, ARM code that runs from IWRAM, with their sizes in
// bytes and, for three of them, in DMA words.
extern const u8 Data_08138fc8[];
extern const u8 Data_081391c8[];
extern const u8 Data_081393d8[];
extern const u8 Data_081395f8[];
extern const u8 Data_08139aac[];
extern const u8 Data_08139f70[];
extern const u8 Data_0813a444[];
extern const u8 Data_0813a924[];
extern const u8 Data_0813ae14[];
extern const u8 Data_0813b2ec[];
extern const u8 Data_0813b7a0[];
extern u8 Data_08138fc8CodeSize[];
extern u8 Data_081391c8CodeSize[];
extern u8 Data_081393d8CodeSize[];
extern u8 Data_081395f8CodeSize[];
extern u8 Data_08139aacCodeSize[];
extern u8 Data_08139f70CodeSize[];
extern u8 Data_0813a444CodeSize[];
extern u8 Data_0813a924CodeSize[];
extern u8 Data_0813ae14CodeSize[];
extern u8 Data_0813b2ecCodeSize[];
extern u8 Data_0813b7a0CodeSize[];
extern u8 Data_081395f8CodeWords[];
extern u8 Data_0813ae14CodeWords[];
extern u8 Data_0813b2ecCodeWords[];

#endif // ALCHEMY_BATTLE_EFFECT_INTERNAL_H
