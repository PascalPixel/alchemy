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
#include "module/battle_effect/battle_effect_table.h"
#include "battle_effect_work.h"
#include "field/particle.h"
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
#include "battle/battle_effect_params.h"
#include "actor.h"
#include "macros.h"

#include "field/particle.h"
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

/// The backdrop's work, in place of the battle effect work.
struct StreakSceneWork {
  s32 frame;        // the streaks' frame count (BattleEffect_DrawStreaks)
  s32 halvePending; // the streaks are drawn: halve the canvas into VRAM
  s32 fadeFrames;   // frames of the background's fade left
                    // (BattleEffect_DimBackgroundStep)
  s32 fadeStep;
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

// The kinds this body draws, chosen by the effect's entry.
enum {
  QUADS_MERCURY, // over a prepared canvas, with the Mercury Djinn
  QUADS_MARS,    // over a prepared canvas, with the Mars Djinn
  QUADS_RISING   // rising from the first target
};

/// A render job (the first of Render_AllocateModelList's entries).
struct RenderJob {
  s32 kind;
  s32 next;
  const void *faces;
  void *vertices;
  struct Texture *texture;
  s32 fade;
  s32 tail;
};

/// The map cell buffer holds WIN0's line table and then the splashes.
struct DropSplashBuffer {
  u16 lines[160];
  struct Particle splashes[512];
};

// The kinds of spark group, chosen by the effect's entry.
enum {
  SPARKS_FIXED,     // from a fixed point, canvas at priority 1
  SPARKS_ORANGE,    // from a lower point, orange, with a palette fade
  SPARKS_FROM_ACTOR // from the caster, lightning colours, sparks rising
};

enum { MODE_SOOTHING_STAR, MODE_SLEEP_STAR, MODE_SLAVER };

// Where mode 3 unpacks the tornado's glints in the sheet.
enum { RENDER_MODE_TORNADO = 0x2580 };

/// The star's texture, 64x64 pixels.
struct StarTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  u8 *pixels;
};

/// One model for the IWRAM renderer.
struct ModelDraw {
  s32 kind;
  s32 next;
  const void *model;
  void *corners;
  struct StarTexture *texture;
  s32 unk14;
  s32 tail;
};

extern u8 Data_02010400[];

extern u8 Data_02010800[];

extern u8 Data_02012800[];

extern const struct Scale gEclipse_Scales;

extern char ResourceId_EffectPalette03;

extern const struct Scale gCruelRuin_UnitScale;

/// One handler for each battle effect, by effect less one; 0 where none.
extern void (*const gBattleEffect_Handlers[])(struct BattleEffectParams *args);

extern char ResourceId_Data0D0; ///< directory row 0xd0

extern char ResourceId_FeatherSheet; ///< directory row 0xd1

extern char ResourceId_CrescentRowSheet;

extern const struct Scale gZagan_UnitScale;

extern struct Scale gProcne_UnitScale; ///< the comets' unit scale

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

/// The bytes an effect's work block is lent: more than struct
/// BattleEffectWork holds.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

/// The bytes of one set of the ten mote pictures: 2 * (1 + 4 + ... + 100).
#define MOTE_SET_BYTES 770

/// Clips overflow on all four sides.
/// The columns of gTargetStrike_Styles, one row a style.
enum TargetStrikeColumn {
  TARGET_STRIKE_KIND,           ///< STRIKE_KIND_SLASH or STRIKE_KIND_BITE
  TARGET_STRIKE_HITS,           ///< slashes or bites
  TARGET_STRIKE_FLAGS,          ///< the extras (STRIKE_SMOKE and on)
  TARGET_STRIKE_COLOR,          ///< the background's palette (STRIKE_COLOR_)
  TARGET_STRIKE_PICTURE_FRAMES, ///< frames each picture shows
  TARGET_STRIKE_COMPOSITE,      ///< the bites' blitter mode
  TARGET_STRIKE_FRAMES          ///< the effect's length
};

// The two kinds of target strike (gTargetStrike_Styles).
#define STRIKE_KIND_SLASH 0 ///< sideways slashes
/// Rows of fangs above and below the target that close on it.
#define STRIKE_KIND_BITE 1

// A target strike style's extras.
#define STRIKE_SMOKE 1  ///< each hit raises smoke puffs from the target's feet
#define STRIKE_ABSORB 2 ///< motes fly out of the target and back into the user
#define STRIKE_GLINTS 4 ///< glints sparkle around the target
#define STRIKE_SPARKS 8 ///< each hit scatters sparks
#define STRIKE_HEAVY 16 ///< one heavy hit: a big shake and the target recoils
#define STRIKE_FLASH 32 ///< as heavy, with flash pictures for the smoke
#define STRIKE_STAR 64  ///< a star swells on the target

// The palettes a target strike's background takes (TARGET_STRIKE_COLOR).
#define STRIKE_COLOR_BLUE 0      ///< BlueBeamSheet's
#define STRIKE_COLOR_LIME 1      ///< LimePalette
#define STRIKE_COLOR_PINK 2      ///< PinkPalette
#define STRIKE_COLOR_EMBER 3     ///< EmberStreakSheet's
#define STRIKE_COLOR_MARS 4      ///< MarsDjinnSheet's
#define STRIKE_COLOR_LIGHTNING 5 ///< LightningBoltSheet's
#define STRIKE_COLOR_ORANGE 6    ///< OrangePaletteB

/// The target strike styles, rows of gTargetStrike_Styles: The Broken Seal's
/// twelve, then this game's own.
enum TargetStrikeStyle {
  TARGET_STRIKE_STYLE_BLUE_SPARK_BITES,
  TARGET_STRIKE_STYLE_LIME_SMOKE_BITE,
  TARGET_STRIKE_STYLE_PINK_SMOKE_BITES,
  TARGET_STRIKE_STYLE_EMBER_DRAIN_BITES,
  TARGET_STRIKE_STYLE_MARS_DRAIN_BITES,
  TARGET_STRIKE_STYLE_LIGHTNING_GLINT_BITES,
  TARGET_STRIKE_STYLE_LIGHTNING_GLINT_SLASH,
  TARGET_STRIKE_STYLE_LIGHTNING_SPARK_SLASHES,
  TARGET_STRIKE_STYLE_MARS_HEAVY_SLASH,
  TARGET_STRIKE_STYLE_LIME_SMOKE_SLASH,
  TARGET_STRIKE_STYLE_MARS_SMOKE_BITE,
  TARGET_STRIKE_STYLE_MARS_ADDITIVE_DRAIN_BITES,
  TARGET_STRIKE_STYLE_LIGHTNING_FOUR_SPARK_SLASHES,
  TARGET_STRIKE_STYLE_LIGHTNING_EIGHT_SPARK_SLASHES,
  TARGET_STRIKE_STYLE_ORANGE_FLASH_GLINT_SLASH,
  TARGET_STRIKE_STYLE_MANEATER, ///< the same row as MARS_ADDITIVE_DRAIN_BITES
  TARGET_STRIKE_STYLE_ORANGE_FLASH_SPARK_SLASH,
  TARGET_STRIKE_STYLE_MARS_HEAVY_BITE,
  TARGET_STRIKE_STYLE_ORANGE_FLASH_BITE,
  TARGET_STRIKE_STYLE_MARS_STAR_SLASH,
  TARGET_STRIKE_STYLE_BLUE_SPARK_BITE,
};

/// Window 0 shows the battle view: BG0 to BG2, the sprites and blending.
#define EFFECT_WIN0_LAYERS                                                     \
  (WININ_WIN0_BG0 | WININ_WIN0_BG1 | WININ_WIN0_BG2 | WININ_WIN0_OBJ |         \
   WININ_WIN0_CLR)

/// Opens the canvas layer at its own width: BG2 shows it one pixel a pixel.
#define EFFECT_START_NARROW_CANVAS(bgControl)                                  \
  (BattleEffect_StartCanvasLayer(bgControl), REG_BG2PA = Q_8_8(1))

/// Strikes target `i`: it flashes white in its damage pose and is knocked back
/// by `motion`.
#define EFFECT_STRIKE_TARGET(effect, i, frames, motion)                        \
  (BattleEffect_TintBattler((effect)->targets[i], SPRITE_COLOR_WHITE,          \
                            POSE_DAMAGE, i, frames),                           \
   BattleMotion_ApplyKnockbackFar((effect)->targets[i], motion))

#define EFFECT_CLIP_BOUNDS (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

/// Builds a blitter for the canvas in heap slot `slot`, for one clip, mirror
/// and blend mode (BLITTER_ADDITIVE to BLITTER_COPY).
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

struct AnimationEntry;
struct BattleEffectParams;
struct ScreenPoint;
struct Particle;
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
void BattleEffect_SelectLivingTargets(struct BattleEffectParams *party);
void BattleEffect_CallDoNothing(s32 cnt);
void BattleEffect_BuildWindowEdgeTable(void);

// battler_tint.c
/// Flashes an actor in a palette for some frames.
void BattleEffect_TintBattler(s32 setId, s32 objectValue, s32 groupValue,
                              s32 stateSlot, s32 stateValue);
void BattleEffect_SetupBlendedDisplay(void);

// stat_change.c
void BattleEffect_RunImpair(struct BattleEffectParams *args);
void BattleEffect_RunDull(struct BattleEffectParams *args);
void BattleEffect_RunWeaken(struct BattleEffectParams *args);

// eruption.c
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant);

// projectiles.c
void BattleEffectKernels_IntegrateVector3(struct Particle *step, s32 damping,
                                          s32 gravity);
void BattleEffectKernels_IntegrateVector2(struct Particle *step, s32 damping,
                                          s32 gravity);
// BattleEffect_BuildBlitter itself.
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);
void BattleEffect_RunDjinniEntrance(struct BattleEffectParams *effect,
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
void BattleEffect_RunSwirlSwipe(struct BattleEffectParams *effect, s32 mode);
void Palette_MirrorBgColors(void);
void Graphics_LoadGreyRamp(void);

// azul.c
void EffectPosition_ProjectBattler(s32 id, struct ScreenPoint *position);
// The middle between the first and the last target.
void BattleEffect_GetTargetsMidpoint(struct ScreenPoint *position);
void Mesh_BuildGrid(struct MeshTriangle *triangles, s32 columns, s32 rows,
                    s32 cellWidth, s32 cellHeight);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void EffectPosition_ProjectSprite(s32 id, struct ScreenPoint *position);

// positions.c
void BattleEffect_RunCastingImpact(struct BattleEffectParams *action, s32 kind);
void BattleEffect_RunCriticalHit(struct BattleEffectParams *effect);
void BattleEffect_RunAttackSlash(struct BattleEffectParams *effect);

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
void BattleEffect_RunFallingSword(struct BattleEffectParams *effect, s32 mode);

// thrash.c
void BattleEffect_RunFlashingStrike(struct BattleEffectParams *effect,
                                    s32 variant);

// undead_sword.c
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunWaterSheet(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunWindFunnel(struct BattleEffectParams *effect, s32 mode);

// heavy_divide.c
void BattleEffect_RunHeavyBlow(struct BattleEffectParams *effect, s32 mode);

// thunder_mine.c
void BattleEffect_RunBoltTrail(struct BattleEffectParams *effect, s32 mode);

// whorl.c
void BattleEffect_RunWaterColumn(struct BattleEffectParams *effect, u32 mode);
void BattleEffect_RunDjinniCharge(struct BattleEffectParams *effect, s32 mode);

// rime.c
void BattleEffect_RunDjinniSign(struct BattleEffectParams *effect, s32 mode);

// gasp.c
void BattleEffect_RunGaspFamily(struct BattleEffectParams *effect, s32 mode);

// raging_heat.c
void RagingHeat_FillGradientCanvas(void);

// rushing_strike.c
void BattleEffect_RunRushingStrike(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_BuildRepeatingTilemap(void);

// glowing_lunge.c
void BattleEffect_RunMirroredWall(struct BattleEffectParams *effect, s32 mode);

// vengeance.c
void BattleEffect_RunGeyser(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunStarSwarm(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunBubbleColumn(struct BattleEffectParams *effect);

// helm_splitter.c
void BattleMotion_ApproachTargetFar(s32 actorId, s32 targetId,
                                    s32 travelDivisor, s32 initialVelocityY);

// legend.c
void BattleEffect_RunElementCards(struct BattleEffectParams *effect, s32 mode);

// cards.c
void BattleEffect_RunCards(struct BattleEffectParams *effect, s32 mode);

// breaths.c
void Canvas_BlitAtFoot(void *source, s32 x, s32 y, s32 size);
void BattleEffect_RunBreaths(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_RunCrescentSlash(struct BattleEffectParams *effect, s32 mode);

// trident.c
void BattleEffect_RunLeapingStrike(struct BattleEffectParams *effect, s32 mode);
void BattleEffect_ArmMeteorBlowPaletteDma(void);

// liquifier.c
void BattleEffect_RunCanvasModeByCount(struct BattleEffectParams *effect);

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
/// Turns the battle camera, over `frames` frames, to look at the point between
/// two units.
void BattleCamera_SetViewAtPairMidpointFar(s32 actor, s32 target, s32 frames);
void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);
/// Glowing orbs spiral in on each target.
///
/// Abilities: Mystic Flame.
void BattleEffect_RunMysticFlame(struct BattleEffectParams *);
void BattleObject_PutFar(void);
/// Spawns the battle objects of a list of units through its far call.
void BattleObject_SpawnObjectsForListFar(s16 *actors, s32 a, s32 b);
/// BattlePlayback_Start through its far call.
void BattlePlayback_StartFar(s32 phase);
/// Whether an enemy's sprite takes the heavy impact, through its far call.
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

// The tables of effect_table.c, in its order.

extern u16 gBattleEffect_MotePictures[];
extern u16 gBattleEffect_FlarePictures[];
extern const u16 gCruelRuin_CirclePictures[];
extern u16 gBattleEffect_SparklePictures[];
extern u8 gBattleEffect_SparkleSizes[];
extern u8 gBattleEffect_PuffArcPictureWidths[];
extern u8 gBattleEffect_PuffArcPictureHeights[];
extern u8 gBattleEffect_PuffArcPictureBiasY[];
extern u16 gBattleEffect_PuffArcPictures[];
extern u16 gBattleEffect_GlintPictures[];
extern u8 gBattleEffect_GlintPictureWidths[];
extern u8 gBattleEffect_GlintPictureHeights[];
extern u8 gSpire_ShardWidths[];
extern u8 gSpire_ShardHeights[];
extern u16 gSpire_ShardPictures[];
extern u16 gStatChange_ShardPictures[];
extern u8 gStatChange_ShardWidths[];
extern u8 gStatChange_ShardHeights[];
extern u8 gCastingImpact_PictureX[];
extern u8 gCastingImpact_PictureY[];
extern u8 gFalling_IceCounts[];
extern u8 gFalling_Counts[];
extern s8 gStreaks_Styles[];
extern s8 gStreaks_Tremble[];
extern u8 gTargetStrike_Styles[][7];
extern u8 gTargetStrike_Widths[];
extern u8 gTargetStrike_Heights[];
extern u16 gTargetStrike_Pictures[];
extern u8 gTargetStrike_Reach[];
extern u8 gTargetStrike_BiteStages[];
extern u8 gTargetStrike_GlintDrawFlags[];
extern u8 gDeathPlunge_PieceWidths[];
extern u8 gDeathPlunge_PieceHeights[];
extern u16 gDeathPlunge_PiecePictures[];
extern u8 gDeathPlunge_PieceStartX[];
extern u8 gDeathPlunge_PieceStartY[];
extern s8 gDeathPlunge_X[];
extern s8 gDeathPlunge_Y[];
extern u8 gElementBurst_GlintFlips[];
extern const struct ModelVertex gCruelRuin_StreakCorners[];
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
extern const u8 gEclipse_GlintFlips[];
extern const u8 gEclipse_TileObjects[];
extern const u8 gEclipse_FillFrames[];
extern const u32 gEclipse_FillLevels[];
extern const u8 gEclipse_ObjectX[];
extern const u8 gEclipse_ObjectZ[];
extern const u8 gEclipse_RingGrowth[];
extern const s8 gEclipse_RingHeights[];
extern u8 gFormChange_WaveStarts[];
extern u8 gFormChange_WaveGrowth[];
extern u8 gFormChange_WaveStretch[];
extern u8 gPrism_ShardStarts[];
extern u8 gPrism_ShardWidths[];
extern u8 gPrism_ShardHeights[];
extern s32 gPrism_ShardPictures[];
extern u8 gGatheringLights_Counts[];
extern u8 gDjinnBeams_MotePictures[];
extern u16 gMirroredWall_Pictures[];
extern u8 gDjinnBeams_HeadWidths[];
extern u8 gDjinnBeams_HeadHeights[];
extern s8 gLobbedShells_Styles[];
extern u8 gLobbedShells_ShardWidths[];
extern u8 gLobbedShells_ShardHeights[];
extern u16 gLobbedShells_ShardPictures[];
extern u8 gCondemn_PictureX[];
extern u8 gCondemn_PictureY[];
extern s32 gBind_Vertex[];
extern s32 gBind_TriangleStrikeVertex[];
extern s32 gStar_Points[];
extern u8 gStar_Radii[];
extern u8 gProcne_GridX[];
extern u8 gProcne_GridY[];
extern u16 gProcne_Frames[];
extern u8 gProcne_PictureWidths[];
extern u8 gProcne_PictureHeights[];
extern u8 gProcne_PictureBiasY[];
extern u16 gProcne_Pictures[];
extern u8 gTiamat_EmberSpeeds[];
extern u8 gTiamat_SparkPixels[];
extern u8 gTiamat_EmberSizes[];
extern u8 gTiamat_FlameWidths[];
extern u8 gTiamat_FlameHeights[];
extern u8 gTiamat_FlameDrops[];
extern u16 gTiamat_FlamePictures[];
extern u8 gNereid_Columns[];
extern s32 gNereid_Gravity[];
extern u8 gVortices_Counts[];
extern u8 gVortices_Shapes[];
extern u8 gPrism_Counts[];
extern u16 gPlasma_Sparks[];
extern u8 gPlasma_Columns[];
extern u8 gPlasma_Counts[];
extern u16 gPlasma_Shapes[][5];
extern u8 gPlasma_FlashPictures[];
extern u8 gVolcano_Counts[];
extern s8 gVolcano_Depths[][4];
extern u8 gVolcano_StartFrames[];
extern u8 gTargetMotes_GlintFlips[];
extern const u16 gZagan_ChargePoints[][2];
extern const u16 gZagan_GallopPoints[][2];
extern const u8 gZagan_RingStarts[];
extern const u8 gZagan_RingGrowth[];
extern const s8 gZagan_RingHeights[];
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
extern u8 gBoreas_DebrisWidths[];
extern u8 gBoreas_DebrisHeights[];
extern u16 gBoreas_DebrisPictures[];
extern u8 gBoreas_SprayWidths[];
extern u8 gBoreas_SprayHeights[];
extern u16 gBoreas_SprayPictures[];
extern u16 gBoreas_RisingCrystalPictures[];
extern u8 gBoreas_RisingCrystalWidths[];
extern u8 gBoreas_RisingCrystalHeights[];
extern u8 gBoreas_FallWidths[];
extern u8 gBoreas_FallHeights[];
extern u16 gBoreas_FallPictures[];
extern s32 gDrain_Gravities[];
extern u8 gDrainMotes_Sizes[];
extern u8 gQuake_Timings[][3];
extern u8 gFire_Counts[];
extern u8 gFire_FlareWidths[];
extern u8 gFire_FlareHeights[];
extern u8 gFire_FlareBiasY[];
extern u16 gFire_FlarePictures[];
extern u8 gFrost_MoundSpots[];
extern u8 gFrost_Timings[];
extern u8 gFrost_MoteWidths[];
extern u8 gFrost_MoteHeights[];
extern u16 gFrost_MoteCells[];
extern u16 gFrost_MoundPictures[];
extern u8 gFrost_MoundHeights[];
extern u8 gFrost_ChipWidths[];
extern u8 gFrost_ChipHeights[];
extern u16 gFrost_ChipCells[];
extern u8 gRay_Sparks[][2];
extern u8 gEruption_Counts[][3];
extern u16 gEruption_PuffPictures[];
extern u16 gEruption_PuffSizes[];
extern u8 gProjectileVolleyB_GlintFlips[];
extern s32 gAtalanta_UnitScale[];
extern u8 gGrowth_VineWidths[];
extern u8 gGrowth_HeightLimits[];
extern u16 gGrowth_VinePictures[];
extern u8 gGrowth_PillarWidths[];
extern u16 gGrowth_PillarPictures[];
extern u8 gGrowth_ColumnCounts[];
extern u8 gGrowth_ColumnX[];
extern s8 gGrowth_PillarHeights[];
extern s8 gGrowth_PuffWidths[];
extern u8 gGrowth_PuffHeights[];
extern u16 gGrowth_PuffPictures[];
extern const s8 gGrowth_ColumnOffsets[];
extern u8 gGrowth_Kinds[];
extern s8 gGrowth_X[];
extern u8 gGrowth_Counts[];
extern s8 gGrowth_ThornPuffWidths[];
extern u8 gGrowth_ThornPuffHeights[];
extern u16 gGrowth_ThornPuffPictures[];
extern u8 gBolt_Counts[];
extern u8 gBolt_GlintFlips[];
extern u8 gBolt_GlintModes[];
extern u8 gProjectileVolley_CometHeights[];
extern u8 gProjectileVolley_Modes[][5];
extern u8 gProjectileVolley_FlashWidths[];
extern u8 gProjectileVolley_FlashHeights[];
extern u8 gProjectileVolley_FlashSteps[];
extern u8 gProjectileVolley_FlashRises[];
extern u16 gProjectileVolley_FlashPictures[];
extern u16 gProjectileVolley_PelletPictures[];
extern u8 gProjectileVolley_GlintFlips[];
extern const u8 gDjinnAttacks_LaunchFrames[];
extern u8 gHail_PictureWidths[];
extern u8 gHail_PictureHeights[];
extern u16 gHail_Pictures[];
extern const u8 gHail_CrackleFlips[];
extern const u8 gHail_PortalWidths[];
extern const u8 gHail_PortalHeights[];
extern const u16 gHail_PortalPictures[];
extern const u8 gHail_PortalX[];
extern const u8 gHail_PortalY[];
extern const u8 gThor_TargetPalettes[];
extern const u8 gThor_CrackleFlips[];
extern u8 gSpire_ShardOffsets[];
extern s8 gSpire_RockX[];
extern u8 gSpire_DropFrames[];
extern u8 gSpire_RockCounts[];
extern u8 gFlurry_Styles[][8];
extern const u8 gFlurry_GlintFlips[];
extern u16 gGaia_RowPictures[];
extern u8 gGaia_RowWidths[];
extern u16 gGaia_FlamePictures[];
extern u8 gGaia_FlameWidths[];
extern u8 gGaia_FlameHeights[];
extern s8 gGaia_Bg2Shifts[][3];
extern u8 gGaia_LateRockColumns[];
extern const u8 gUlysses_ObjectX[];
extern const u8 gUlysses_ObjectZ[];
extern const u8 gUlysses_PictureWidths[];
extern const u8 gUlysses_PictureHeights[];
extern const u16 gUlysses_Pictures[];
extern const s8 gUlysses_ParticleStarts[];
extern const u8 gSpiderWeb_PictureWidths[];
extern const u8 gSpiderWeb_PictureHeights[];
extern const u16 gSpiderWeb_Pictures[];
extern const u8 gSpiderWeb_PictureX[];
extern const s8 gSpiderWeb_PictureBiasY[];
extern const u8 gAzul_PartPoints[][2];
extern const struct Scale gAzul_UnitScale;
extern const u8 gAzul_ObjectX[];
extern const u8 gAzul_ObjectZ[];
extern const struct Scale gAzul_LateUnitScale;
extern const s8 gAzul_LateObjectX[];
extern const s8 gAzul_LateObjectZ[];
extern u8 gBeam_PictureWidths[];
extern u8 gBeam_PictureHeights[];
extern u16 gBeam_Pictures[];
extern u8 gBeam_PictureReaches[];
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
extern u8 gNereidCopy_Columns[];
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
extern u8 gHelmSplitter_ImpactBurstPictureWidths[];
extern u8 gHelmSplitter_ImpactBurstPictureHeights[];
extern u16 gHelmSplitter_ImpactBurstPictures[];
extern s16 gParticleStreams_OrbitPoints[][3];
extern s16 gParticleStreams_FireballPoints[][2];
extern u16 gParticleStreams_FlashPictures[];
extern u16 gParticleStreams_FlashSizes[];
extern u8 gRamses_StatueColumns[];
extern u8 gRamses_StatueRows[];
extern u16 gRamses_PuffPictures[];
extern u16 gRamses_PuffSizes[];
extern u8 gRagnarok_StrikeColumns[];
extern u8 gRagnarok_FlashPictures[];
extern u8 gTitanBlade_FlashPictures[];
extern s32 gTitanBlade_DustGravity[];
extern const u8 gJudgment_BurstPictures[];
extern const u16 gJudgment_ShardPictures[];
extern const u8 gJudgment_ShardWidths[];
extern const u8 gJudgment_ShardHeights[];
extern u8 gKirin_Columns[];
extern u8 gKirin_Rows[];
extern s32 gNeptune_Gravity[];
extern const u16 gUndeadSword_SparkPictures[];
extern const u16 gUndeadSword_SparkSizes[];
extern const u8 gWindFunnel_Counts[];
extern const u8 gDiamondBerg_RingStarts[4];
extern const u8 gDiamondBerg_RingGrowth[4];
extern const s8 gDiamondBerg_RingHeights[4];
extern const s16 gDiamondBerg_ShardRolls[8];
extern const s16 gDiamondBerg_ShardPitches[8];
extern const u8 gGeode_WaveStarts[];
extern const u8 gGeode_WaveGrowth[];
extern const s8 gGeode_WaveHeights[];
extern u16 gSoulShatter_SparkPictures[];
extern u16 gSoulShatter_SparkSizes[];
extern const s8 gSerac_RowHeights[];
extern const s8 gSerac_RowRadii[];
extern const u8 gSerac_RingStarts[];
extern const u8 gSerac_RingGrowth[];
extern const s8 gSerac_RingHeights[];
extern u8 gHeavyBlow_StrikeFrames[];
extern u8 gHeavyBlow_ChipCounts[];
extern const u8 gHeavyBlow_ChipPixels[];
extern s16 gCool_ColumnX[];
extern u8 gCool_ColumnStarts[];
extern u8 gCool_ColumnHeights[];
extern u8 gCool_ColumnLeans[];
extern u16 gCool_SparkPictures[];
extern u16 gCool_SparkSizes[];
extern const s8 gWyrdCurse_DropBounces[];
extern const u8 gBoltTrail_GlintFlips[];
extern const u8 gMegaera_GlintFlips[];
extern const u8 gMegaera_GlintModes[];
extern const s32 gLightSurge_PillarSizes[4];
extern const s8 gLightSurge_PillarShades[4];
extern s8 gLightSurge_PillarOffsets[4];
extern const u8 gDjinniCharge_EntranceFlags[];
extern const s32 gDjinniCharge_DebrisGravity[4];
extern const s8 gGaspFamily_StreakSteps[];
extern const u16 gGaspFamily_StreakMesh[];
extern const u8 gGaspFamily_Counts[];
extern u8 gCharon_FlashFrames[];
extern const struct ModelVertex gBattleEffect_OctagonBandCorners[];
extern const struct ModelVertex gBattleEffect_CylinderCorners[];
extern const u16 gCastingImpact_BandMesh[];
extern const u16 gGeyser_Faces[];
extern const u16 gBattleEffect_CylinderMesh[];
extern const struct ModelVertex gGeode_RingCorners[];
extern const struct ModelVertex gBattleEffect_RingCorners[];
extern const u16 gBattleEffect_RingMesh[];
extern const u16 gBattleEffect_TriangleMesh[];
extern const struct ModelVertex gBattleEffect_TriangleCorners[];
extern const struct ModelVertex gBattleEffect_TaperedBeamCorners[];
extern const struct ModelVertex gBattleEffect_BeamCorners[];
extern const struct ModelVertex gBattleEffect_LongBeamCorners[];
extern const struct ModelVertex gBattleEffect_SquareCorners[];
extern const struct ModelVertex gBattleEffect_StandingSquareCorners[];
extern struct ModelVertex gLegend_WingCorners[];
extern const struct ModelVertex gBattleEffect_GroundSquareCorners[];
extern const u16 gBattleEffect_SmallSquareMesh[];
extern const u16 gBattleEffect_SmallSquareMeshReversed[];
extern const u16 gBattleEffect_ColumnMesh[];
extern const u16 gHeavyBlow_SwordMesh[];
extern const u16 gBattleEffect_TallMesh[];
extern const u16 gHaures_BandMesh[];
extern const u16 gCatastrophe_BurstMesh[];
extern const u16 gBattleEffect_SquareMesh[];
extern const u16 gBattleEffect_LargeSquareMesh[];
extern const u16 gHaures_BurstMesh[];
extern u8 gHeartbreak_SwingWidths[];
extern u8 gHeartbreak_SwingHeights[];
extern u16 gHeartbreak_SwingPictures[];
extern u8 gHeartbreak_FistHeights[];
extern u8 gHeartbreak_FistY[];
extern u16 gHeartbreak_FistPictures[];
extern const u8 gRagingHeat_Styles[][4];
extern u8 gJuggle_Styles[][7];
extern const u8 gFlora_PetalSizes[];
extern const u16 gFlora_PetalPictures[];
extern const u16 gFlora_BloomFrames[];
extern const s16 gFlora_BloomRolls[];
extern const s16 gFlora_BloomPitches[];
extern const s8 gFlora_BloomX[];
extern const s8 gFlora_BloomY[];
extern const u16 gSabreDance_DaggerMesh[];
extern const struct ModelVertex gSabreDance_DaggerCorners[];
extern const u16 gFume_Levels[][FUME_COLUMNS];
extern const s32 gOceanFist_DropGravity[4];
extern const s32 gRushingStrike_Scales[];
extern const s32 gMirroredWall_BurstScales[];
extern const s32 gMirroredWall_BurstShades[];
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
extern const u8 gDaedalusAlt_StrikeFrames[];
extern const u8 gVengeance_BladeStrikes[][7];
extern const u16 gLifeShear_BladeMesh[];
extern const u16 gLifeShear_HandleMesh[];
extern const struct ModelVertex gLifeShear_ScytheCorners[];
extern const s32 gGeyser_DropGravity[];
extern const s8 gGeyser_DropMinSizes[];
extern u8 gBubbleColumn_PictureWidths[];
extern u8 gBubbleColumn_PictureHeights[];
extern u16 gBubbleColumn_Pictures[];
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
extern const u8 gHaures_ParticleX[];
extern s32 gHaures_UnitScale[];
extern const u8 gHaures_WingShapes[];
extern u8 gHaures_WingMorphSteps[];
extern const u8 gHaures_PartPoints[];
extern const u8 gHaures_BandStarts[];
extern u8 gHelmSplitter_PictureWidths[];
extern u8 gHelmSplitter_PictureHeights[];
extern u16 gHelmSplitter_Pictures[];
extern s8 gHelmSplitter_PictureBiasX[];
extern s8 gHelmSplitter_PictureBiasY[];
extern u8 gDragonCloud_SparkCounts[];
extern u8 gElementCards_Frames[];
extern struct SparkMotion gElementCards_SparkMotions[];
extern u8 gTripleChomp_BiteFrames[];
extern u8 gBreaths_Frames[];
extern u8 gBreaths_BurstFrames[];
extern u8 gBreaths_MaxSizes[];
extern u8 gBreaths_RingStarts[];
extern u8 gBreaths_RingGrowth[];
extern s8 gBreaths_RingX[];
extern u8 gBreaths_FlashFrames[];
extern const u8 gTrident_RingStarts[];
extern const u8 gTrident_RingGrowth[];
extern const s8 gTrident_RingHeights[];
extern const s32 gTrident_Spins[];
extern const struct ModelVertex gTrident_ProngCorners[];
extern const u8 gTrident_GlintFlips[];
extern const u8 gLeapingStrike_WaveStarts[];
extern const u8 gLeapingStrike_WaveGrowth[];
extern const s8 gLeapingStrike_WaveHeights[];
extern const u8 gLeapingStrike_RingStarts[];
extern const u8 gLeapingStrike_RingGrowth[];
extern const s8 gLeapingStrike_RingHeights[];
extern const u8 gMeteorBlow_PictureWidths[];
extern const u8 gMeteorBlow_PictureHeights[];
extern const u16 gMeteorBlow_Pictures[];
extern const u8 gMeteorBlow_WaveStarts[];
extern const u8 gMeteorBlow_WaveGrowth[];
extern const u8 gMeteorBlow_WaveStretch[];
extern const u8 gMeteorBlow_RingStarts[];
extern const u8 gMeteorBlow_RingGrowth[];
extern const s8 gMeteorBlow_RingHeights[];
extern const struct ModelVertex gLiquifier_SplashCorners[];
extern const u8 gLiquifier_SplashStarts[];
extern const u8 gLiquifier_SplashHeights[];
extern const u8 gCanvasBurst_FlamePictures[];
extern const s16 gMegiddo_FlashPitches[];
extern const s16 gMegiddo_FlashYaws[];
extern const u16 gMegiddo_FlashStarts[];
extern const s16 gMegiddo_RayPitches[];
extern const s16 gMegiddo_RayYaws[];
extern const s16 gMegiddo_RayRolls[];
extern const s8 gMegiddo_RayX[];
extern const s8 gMegiddo_RayY[];
extern const s16 gOdyssey_FlashPitches[];
extern const s16 gOdyssey_FlashYaws[];
extern const u8 gMoloch_RingStarts[];
extern const u8 gMoloch_RingGrowth[];
extern const s8 gMoloch_RingHeights[];
extern s32 gIris_SplashRadii[];
extern const u8 gIris_SplashOffsetsY[];
extern const u16 gIris_PillarMesh[];
extern const struct ModelVertex gIris_ParticleCorners[];
extern const u16 gIris_ParticleMesh[];
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
extern const struct ModelVertex gIris_LeftStripCorners[];
extern const struct ModelVertex gIris_RightStripCorners[];
extern const u16 gIris_StripMesh[];
extern const struct ModelVertex gIris_ConeCorners[];
extern const u16 gIris_ConeMesh[];

// The resources, by the first part that reads them.

/// falling.c
/// The cone's 64 vertices, stored at the buffer's constant address.
extern u8 gMapGridCellBuffer[];

/// cruel_ruin.c
extern char ResourceId_Data0B4;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0C9;
extern char ResourceId_Data0CA;
extern char ResourceId_Data104;
extern char ResourceId_ParticleSpritesA; // the spark sprites
extern char ResourceId_CornerRuneASheet;
extern char ResourceId_ForestCanopyTiles;
extern char ResourceId_SunbeamCloudsBackdrop;
extern char ResourceId_VioletSkyBackdrop;

/// canvas.c
/// Rows of Resource_DirectoryTable; the directory has yet to name most of them.
extern char ResourceId_EarthHorizonBackdrop;
/// Rows of Resource_DirectoryTable; the directory has yet to name most of them.
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
/// Directory rows, named by their assets.
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

// djinn_attacks.c
extern char ResourceId_FlameballSheet;
extern char ResourceId_GlowOrbSheet;
extern char ResourceId_EmberStreakSheet;

/// hail.c
extern char ResourceId_PortalSheet;

/// thor.c
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
extern const s16 gThor_Points[6][2]; ///< the three lines' end points

// spire.c
extern char ResourceId_BoulderSheet;

// flurry.c
void BattleEffect_RunFlurry(struct BattleEffectParams *args, s32 mode);
extern char ResourceId_YellowOrbSheet;
extern char ResourceId_BlueArcSheetA;
extern char ResourceId_BlueArcSheetB;
extern char ResourceId_WaveSheet;
/// BattleEffect_RunFlurry's modes, by the abilities that play them: the rows
/// of gFlurry_Styles. Gust and Flying Attack take one, two or three strikes as
/// a double or triple blow raises their variant.
enum FlurryMode {
  FLURRY_BEAR_CLAW,            ///< five strikes; also Ransack
  FLURRY_BACTERIA_RUSH,        ///< seven strikes, in lime
  FLURRY_FREEBITE_RUSH,        ///< nine strikes, in blue
  FLURRY_FLYING_ATTACK,        ///< one strike, in embers
  FLURRY_FLYING_ATTACK_DOUBLE, ///< two
  FLURRY_FLYING_ATTACK_TRIPLE, ///< three
  FLURRY_GUST,                 ///< one strike, each filling the canvas
  FLURRY_GUST_DOUBLE,          ///< two
  FLURRY_GUST_TRIPLE,          ///< three
  FLURRY_UR_FLASH,             ///< three strikes, in the Mars Djinni's colours
  FLURRY_CRUSHER_GRIP,         ///< one strike, with glints
  FLURRY_VANISH_CLAW,          ///< one strike, with a ripple and smoke
  FLURRY_CLAW_ATTACK           ///< one strike after a longer lunge, in embers
};
/// A flurry's arcs, its FLURRY_ARC_SHEET.
enum FlurryArcs {
  FLURRY_ARCS_AND_WAVES, ///< blue arc A, with a wave for the third and
                         ///< fourth of every four strikes
  FLURRY_ARCS            ///< blue arc B for every strike
};
/// Whose palette a flurry takes, its FLURRY_PALETTE.
enum FlurryPalette {
  FLURRY_PALETTE_MARS_DJINNI, ///< the Mars Djinni sheet's
  FLURRY_PALETTE_LIME,
  FLURRY_PALETTE_BLUE_ARC,
  FLURRY_PALETTE_EMBER ///< the ember streak sheet's
};
/// What else a flurry plays around its target, its FLURRY_EXTRA.
enum FlurryExtra {
  FLURRY_EXTRA_NONE,
  FLURRY_EXTRA_GLINTS, ///< three glints a frame until frame 31
  FLURRY_EXTRA_RIPPLE  ///< BG2 swings on a sine wave and smoke rises
};

// gaia.c
extern char ResourceId_EarthWallSheet;
extern char ResourceId_OrangePaletteA;
extern struct Scale gGaia_UnitScale;

/// spider_web.c
/// A scale of one in 16.16 on both axes.
extern const struct SpriteScale gUlysses_ParticleScale;
extern char ResourceId_Data0B1;
extern char ResourceId_Data0B3;
extern char ResourceId_RuneCircleSheet;
extern char ResourceId_Data0C8;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_RuneSheet;
extern u8 Data_02010200[];
extern char ResourceId_MagentaSwirlSheet;
extern char ResourceId_MagentaTailSheet;
extern char ResourceId_SpiderWebSheet;

// azul.c
extern char ResourceId_WispSheet;
// The 64 sparks, past the map cells' first 16 KB.
extern u8 gData_02014000[];

// positions.c
extern char ResourceId_BeamImage;
extern char ResourceId_StarCrescentSheet;
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

// particle_streams.c
extern char ResourceId_FirePillarSheetA;
extern char ResourceId_FlareSheet;
extern char ResourceId_FlareImage;
extern char ResourceId_FlameSheetB;

// ramses.c
extern char ResourceId_DesertBackdrop;

/// annihilation.c
extern char ResourceId_FireBurstSheet;

/// ragnarok.c
extern char ResourceId_FlameBladeSheet;
extern char ResourceId_SwordSheet;
extern char ResourceId_BeachBackdrop;
extern char ResourceId_ForestBackdrop;
extern char ResourceId_WingedKnightATiles;
extern char ResourceId_WingedKnightBTiles;
extern char ResourceId_WingedKnightCTiles;
extern char ResourceId_BlueCrescentSheet;
extern char ResourceId_ExplosionSheet;
extern char ResourceId_OverlayImageA;

/// kirin.c
extern char ResourceId_FireBlobSheet;

/// neptune.c
extern char ResourceId_SeaSerpentTiles;
extern struct Scale gNeptune_UnitScale; ///< the serpent's scale

/// thrash.c
extern char ResourceId_ArcFragmentsSheet;
extern char ResourceId_RevealMaskA;
extern char ResourceId_RevealMaskB;
extern char ResourceId_RevealMaskC;
extern char ResourceId_RevealMaskD;
extern char ResourceId_RevealMaskE;
extern char ResourceId_RevealMaskF;

/// undead_sword.c
extern char ResourceId_LimeColumnSheet;
extern char ResourceId_CrackedTextureSheet;
extern char ResourceId_Data0D3;
extern u8 Data_02010108[];
extern u8 Data_02010318[];
extern char ResourceId_CrystalBitmapSheet;
extern u8 Data_02014e20[]; ///< gMapGridCellBuffer + 20000, the berg

/// crystal.c
extern char ResourceId_Data0FB;

// geode.c

/// nirvana.c
extern char ResourceId_Data0CF;
extern u8 Data_02012000[];
extern u8 Data_02012540[]; ///< the mesh's end, after 112 faces

/// heavy_divide.c
extern char ResourceId_GreatSwordSheet;
extern char ResourceId_AxeSheet;
extern char ResourceId_Data0CE;
extern char ResourceId_Data0D8;

// thunder_mine.c
extern char ResourceId_VioletPaletteC;
extern char ResourceId_WhirlBeamSheet;
extern char ResourceId_SparkleDots;
extern const s8 gVoidBeam_Corners[]; ///< the hexagon's corners (z, y)

/// light_surge.c
extern char ResourceId_Data0C4;
extern char ResourceId_EffectPalette02;

/// whorl.c
extern char ResourceId_Data0D2;

// djinni_charge.c
extern char ResourceId_PetalSheet;

// rime.c
extern char ResourceId_StarCrossSheet;

/// gasp.c
extern char ResourceId_LightBurstSheet;
extern char ResourceId_CornerGlowSheet;
extern char ResourceId_Data0D7;
extern char ResourceId_SmokeBandSheet;
extern char ResourceId_VenusDjinnSheet;

/// charon.c
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

/// rising_dragon.c
extern char ResourceId_ClawSlashSheet;
extern u8 Data_02011000[];
extern char ResourceId_Data0DC;

/// raging_heat.c
extern char ResourceId_CornerRuneBSheet;
extern char ResourceId_SmallFlameSheet;
/// The life of each spark in the map cell buffer: gMapGridCellBuffer's
/// Particles from their variant field on.
extern s32 Data_02010018[];

/// flora.c
extern const struct Scale gFlora_UnitScale;
extern char ResourceId_GreenHairedMaidenTiles;
extern char ResourceId_PinkPetalsTiles;
extern u8 Data_02010020[];
extern u8 Data_020100a0[];
extern u8 Data_020102a0[];
extern u16 Data_0201401e[];
extern u16 Data_02014002[];
extern u16 Data_02014260[];
extern char ResourceId_CrossedDaggersSheet;
extern char ResourceId_PinkStarSheet;

/// fire_puppet.c
extern char ResourceId_Data0F2;
extern char ResourceId_Data0F3;

// ocean_fist.c

/// rushing_strike.c
extern char ResourceId_RedHazeBackdrop;

/// glowing_lunge.c
extern char ResourceId_Data181;
extern char ResourceId_FirePillarSheetB;

// daedalus.c
extern char ResourceId_BronzeAutomatonTiles;
extern char ResourceId_MissileTiles;
extern char ResourceId_Data0BA;
extern char ResourceId_Data0BB;
extern struct Scale gDaedalus_WaveScale;
extern struct Scale gDaedalus_ShipScale;
extern struct Scale gDaedalus_ParticleScale;

/// vengeance.c
extern char ResourceId_Data0B0;
extern char ResourceId_Data0F5;
extern u8 Data_020112e8[]; ///< the reaper's raised pose
extern u8 Data_0201276b[]; ///< the blade's texture
extern u8 Data_0201476b[]; ///< the handle's texture
extern u8 Data_0201576b[]; ///< the flash's texture
extern char ResourceId_CrossTextureSheet;
extern char ResourceId_Data0FD;
/// 256 bubbles past the map cells. Each is an Particle with its picture in z
/// and its life in variant (-1 once it has left the top of the canvas).
extern struct Particle gData_02015000[];
extern u8 Data_02014400[];
extern u8 Data_02014800[];
extern const struct Scale gCoatlicue_UnitScale;
extern char ResourceId_BellMaidenTiles;
extern char ResourceId_Data0FC;

/// catastrophe.c
extern char ResourceId_ChainedDemonTiles;
extern char ResourceId_Data0EE;
extern char ResourceId_Data0EF;
extern char ResourceId_Data0F8;
extern char ResourceId_Data0F9;
extern char ResourceId_GoldenScalesTiles;
extern char ResourceId_PurpleArmourTiles;
extern char ResourceId_WhiteSpikesTiles;
extern u8 Data_02016000[];
extern struct Particle Data_02016d00[];
extern const struct Scale gCatastrophe_UnitScale;

/// haures.c
extern u8 Data_02010ae8[];
extern u8 Data_02010be0[];
extern u8 Data_02010fa0[];
extern const struct Scale gHaures_ChargeScale;
extern const struct Scale gHaures_OrbitScale;
extern const struct Scale gHaures_OpeningScale;
/// Resource ids, named after their places in the resource directory.
extern char ResourceId_Data0F7;
extern char ResourceId_MossCaveBackdrop;

/// helm_splitter.c
extern char ResourceId_FireSwirlSheet;
extern char ResourceId_DualStreamPath;

/// legend.c
extern char ResourceId_BladeSheet;
extern char ResourceId_Data0E9;
extern char ResourceId_VineCardSheet;
extern char ResourceId_DragonCardSheet;
extern char ResourceId_SunCardSheet;
extern char ResourceId_BoltCardSheet;

// cards.c
extern char ResourceId_SheepCardSheet;
extern char ResourceId_ButterflyCardSheet;
extern char ResourceId_SwordCardSheet;
extern char ResourceId_DaggerSheet;
extern char ResourceId_PaletteRampImage;
extern char ResourceId_SkeletonCardSheet;

/// triple_chomp.c

/// breaths.c
extern char ResourceId_DustCloudSheet;

/// trident.c
extern char ResourceId_Data0FA;
extern u8 Data_02011400[];
extern u8 Data_02012964[]; ///< 128 sparks

/// meteor_blow.c
extern char ResourceId_GlowOrbSetSheet;

// megiddo.c
extern char ResourceId_Data103;

/// odyssey.c
extern char ResourceId_Data102;
extern char ResourceId_Data101;
extern char ResourceId_CloudSwirlSheet;

/// moloch.c
extern const u8 gMoloch_MoteSizes[];
extern char ResourceId_SunsetBackdrop;
extern char ResourceId_StarfieldFlareBackdrop;
extern char ResourceId_RobedGoddessTiles;
extern u8 Data_02010578[];
extern const struct Scale gIris_UnitScale;
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
/// The rasterizer kernels, ARM code that runs from IWRAM, with their sizes in
/// bytes and, for three of them, in DMA words.
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
