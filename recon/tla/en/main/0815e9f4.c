// Draft of Func_0815e9f4, the battle effect BattleEffect_DispatchByIdRange
// runs for kinds 201 to 209 and 211 up: a ring of sparks gathers, then a
// burst of sparks at the target.

// On screen, while the battle view swings to the midpoint between the
// caster and its target: for kinds above 199 (all that reach it) the caster
// stops moving, the background fades, and 64 sparks spin in towards it on
// random axes over 32 frames, then the caster moves on as before. Then a
// four-cel flash covers the first target for eight frames (frames 0-7),
// the target recoils at frame 4, the battle resumes with hit cue 134 at
// frame 5, and 64 sparks fly out of the target in pairs, shrinking as
// they go, for the rest of 32 frames. Last, the background palette steps
// back to full over seven frames and the canvas layer closes.

// Setup: the transition step between each sheet load (the view keeps
// moving while the sheets decode), the canvas tile map, BG1 on the canvas,
// two blitters for the caster's side (additive and max, flipped for
// side 1), the crescent and star sheets, the Mars Djinn's palette when the
// caster is an enemy (actor 8 and up), the spark sheet, and the transfer
// callback in copy mode.

// Port of The Broken Seal's BattleEffect_RunRingAndSparkScene, with The Lost
// Age's heap slots, blitter modes (3 minus ☀️'s) and task priorities (the
// transfer at 0xc80, the fade at 0x480), no work->effect store, and these
// changes: the palette goes by DMA, the canvases are cleared 0x3c00 bytes
// at a time, the flash's middle cels are 128 pixels square from the map
// cell buffer drawn 4 pixels to the left, and the background's id is the
// battle work's halfword at 0x670.

// Listing 676 instructions, draft 676, differ 74 (2026-10-15). Left: rows
// one place apart through the whole function (heap and stack loads, the
// draw pair's stores, constants built around a pool or stack load), in
// both directions: the prologue issues the canvas load after the effect
// moves to fp, the loops mostly issue loads one slot earlier.

// Tried:
// ☀️'s body with The Lost Age's names and changes            96
// the blitter modes as 3 minus ☀️'s (additive and max)    96 -> 88
// the clear size as a local set at the top, as ☀️'s words:
// the native builds the constant at each call            88 -> 74
// declaration order, a greedy sweep of 18 locals           74

// 2026-10-26, under -mthumb-slow-loads: differ 30 of 676, the instructions
// and registers otherwise the native's. What remains is single rows where
// the native issues a spill or argument store before a load (the known
// store-then-load scheduling limit); barriers and one-pass blocks at each
// statement, statement moves and each local scoped to a block gain only
// the prologue's row or rows of that kind.

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "battle_action.h"
#include "battle_work.h"
#include "bg_scroll.h"
#include "dma.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; BattleEffect_LoadBlitter takes mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define SPARK_COUNT 64

typedef DrawRectangle BlitterPair[2];

extern char ResourceId_StarCrescentSheet;
extern char ResourceId_GreyCrescentSheet;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_ParticleSpritesD;
// Byte offsets of the spark cels in the spark sheet, by size - 1 (☀️'s
// gLuckyWheels_SparkCellOffsets).
extern const u16 Data_08197424[];

void wait(s32 frames);
void Sound_PlayFar(s32 cue);
u32 Random16(void);
void *Resource_GetTableEntry(s32 id);
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skipPalette,
                                s32 copyPalette);
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void BattleCamera_SetViewAtPairMidpointFar(s32 actor, s32 target,
                                                 s32 frames);
void BattleBackground_SetPaletteLevelFar(s32 background, s32 level);
s32 BattleUnit_GetEffectHeightFar(s32 actor);
void Actor_SetAnimationSpeedFar(struct MotionObject *object,
                                            s32 value);
void EffectPosition_ProjectScaledBattler(s32 actor,
                                        struct EffectPosition *position);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
// The roll step of the scene transform.
void Func_080150e4(s32 angle);
void Transform_SetLookAt(void *camera, void *eye);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
// Sets up the canvas tile map on BG1.
void Func_08143d80(void);
// Marks the transition done and shows the battle view again.
void BattleEffect_HideBg1AndRestoreBackdrop(void);
// Copies the canvas to VRAM when a transfer is pending.
void BattleEffect_ProcessPendingGraphicsTransfer(void);
// Steps the background palette fade.
void Palette_StepFadeTransfer(void);
// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
// BattlePlayback_Start through its far call.
void Func_081180e8(s32 hitCue);
// An actor's recoil motion, through a far call.
void Func_08118088(s32 actor, s32 motion);

void Func_0815e9f4(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  struct EffectPosition anchor;
  struct EffectPosition point;
  struct EffectPosition spot;
  struct BattleEffectWork *work;
  void *canvas;
  s32 shake;
  u8 *sheet;
  s32 *camera;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 acceleration;
  s32 strength;
  BlitterPair draw;
  struct MotionObject *object;
  struct MotionObject *target;
  u16 *background;
  s32 words = 0x3c00;
  s32 big;
  s32 half;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  big = 1;
  if (effect->kind <= 199)
    big = 0;

  // Setup, one step of the view's swing between each load.
  BattleCamera_SetViewAtPairMidpointFar(effect->actor,
                                              effect->firstTarget, 130);
  wait(1);
  Func_08143d80();
  REG_BG1CNT = 0x1f80;
  if (effect->side == 0) {
    BattleEffect_LoadBlitter(HEAP_BLITTER,
                  BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  } else {
    BattleEffect_LoadBlitter(HEAP_BLITTER,
                  BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                             COMPOSITE_FLIP_X));
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                  BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                        COMPOSITE_FLIP_X));
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  BattleCamera_SetViewAtPairMidpointFar(effect->actor,
                                              effect->firstTarget, 130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarCrescentSheet, work->sheet, 1,
                             0);
  BattleCamera_SetViewAtPairMidpointFar(effect->actor,
                                              effect->firstTarget, 130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_GreyCrescentSheet,
                             Ram_MapCellBuffer, 1, 1);
  // An enemy caster's sparks take the Mars Djinn's colours.
  if (effect->actor > 7)
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
            (void *)BG_PLTT, 0x84000020, REG_DMA3);
  BattleCamera_SetViewAtPairMidpointFar(effect->actor,
                                              effect->firstTarget, 130);
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  BattleCamera_SetViewAtPairMidpointFar(effect->actor,
                                              effect->firstTarget, 130);
  wait(1);
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The gathering ring, for the larger kinds: the caster holds still while
  // 64 sparks spin in towards it on random axes.
  if (big == 1) {
    object = GetBattleObjectSlotFar(effect->actor)->object;
    for (i = 0; i != SPARK_COUNT; i++) {
      struct EffectStep *mote = &work->particles[i];

      mote->x = (Random16() & 63) + 16;
      mote->y = 0;
      mote->z = 0;
      mote->velocityX = Random16() & 0xffff;
      mote->velocityY = Random16() & 0xffff;
      mote->velocityZ = Random16() & 0xffff;
    }
    Actor_SetAnimationSpeedFar(object, 0);
    velocityX = object->velocityX;
    velocityY = object->velocityY;
    velocityZ = object->velocityZ;
    strength = object->verticalMotionStrength;
    acceleration = object->acceleration;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    object->acceleration = 0;
    object->verticalMotionStrength = 0;
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    shake = 64 - pos.x;
    gBgScroll[1].x = shake;
    gBgScroll[1].y = 80;
    work->fadeFrames = 24;
    work->fadeStep = 0;
    Task_Add(Palette_StepFadeTransfer, 0x480);
    Sound_PlayFar(212);
    for (frame = 0; frame != 32; frame++) {
      BattleCamera_SetViewAtPairMidpointFar(effect->actor,
                                                  effect->firstTarget, 130);
      // Spark i starts at frame i / 4, 16 to 79 pixels out, and moves in
      // 4 pixels a frame until it reaches the caster.
      for (i = 0; i != SPARK_COUNT; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->x >= 0 && frame >= i / 4) {
          s32 size = (i & 1) + 5;

          Render_ResetTransformState();
          Func_080150e4(mote->velocityZ);
          SceneTransform_ApplyPitch(mote->velocityX);
          SceneTransform_ApplyYaw(mote->velocityY);
          EffectPosition_ProjectPoint(&mote->x, &point);
          point.x += 64;
          point.y = point.y + pos.y + 24;
          if (point.depth < -60)
            point.depth = -60;
          if (point.depth > 60)
            point.depth = 60;
          point.depth += 60;
          draw[0](canvas, sheet + Data_08197424[size - 1], point.x - size,
                  point.y - size, size * 2, size * 2);
          mote->x -= 4;
        }
      }
      work->transferPending = 1;
      wait(1);
    }
    Task_Remove(Palette_StepFadeTransfer);
    Actor_SetAnimationSpeedFar(object, 16);
    object->velocityX = velocityX;
    object->velocityY = velocityY;
    object->velocityZ = velocityZ;
    object->acceleration = acceleration;
    object->verticalMotionStrength = strength;
  }

  // The burst: clear both canvases, blend, and centre the layer on the
  // first target, kept within 128 pixels.
  Iwram_ClearWords(canvas, words);
  Iwram_ClearWords((void *)0x06004000, words);
  work->transferMode = 2;
  work->transferValue = 75;
  REG_BG1CNT = 0x1f81;
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  if (effect->side == 0)
    shake = 32 - anchor.x;
  else
    shake = 96 - anchor.x;
  if (shake > 0)
    shake = 0;
  if (shake < -128)
    shake = -128;
  anchor.x += shake;
  gBgScroll[1].x = shake;
  gBgScroll[1].y = 80;

  // The sparks leave the middle of the target, away from the caster, each
  // living 16 frames more than the last.
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  half = BattleUnit_GetEffectHeightFar(effect->actors[0]) / 2;
  for (i = 0; i != SPARK_COUNT; i++) {
    struct EffectStep *spark = &work->particles[i];

    spark->x = target->x;
    spark->y = target->y + half;
    spark->z = target->z;
    spark->velocityX = (Random16() & 255) << 10;
    spark->velocityY = (Random16() & 255) << 10;
    spark->velocityZ = ((Random16() & 255) - 127) << 10;
    if (spark->x > 0)
      spark->velocityX = -spark->velocityX;
    spark->variant = i + 16;
  }

  for (frame = 0; frame != 32; frame++) {
    // The battle resumes, hit cue 134; the target recoils a frame before.
    if (frame == 5)
      Func_081180e8(134);
    if (frame == 4)
      Func_08118088(effect->actors[0], 0);
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    pos.y += 16;
    // The flash: four cels, two frames each.
    if (frame <= 1)
      draw[0](canvas, work->sheet, 0, 0, 120, 120);
    else if (frame <= 3)
      draw[0](canvas, Ram_MapCellBuffer, -4, 0, 128, 128);
    else if (frame <= 5)
      draw[0](canvas, Ram_MapCellBuffer + 0x4000, -4, 0, 128, 128);
    else if (frame <= 7)
      draw[0](canvas, work->sheet + 0x3840, 0, 0, 120, 120);
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    // From frame 4 the sparks fly, two to each particle, through the two
    // blitters in turn.
    if (frame >= 4 && frame <= 31) {
      for (i = 0; i != SPARK_COUNT; i++) {
        s32 pair = i / 2;
        struct EffectStep *spark = &work->particles[pair];
        s32 size = spark->variant;

        if (size > 0) {
          EffectPosition_ProjectPoint(&spark->x, &spot);
          spot.x += shake;
          size >>= 3;
          size += 2;
          spot.y += 16;
          draw[pair & 1](canvas, sheet + Data_08197424[size - 1], spot.x - size,
                         spot.y - size, size * 2, size * 2);
          BattleEffectKernels_IntegrateVector3(spark, 60, -0x400);
          spark->variant--;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  // Teardown: the background palette steps back over seven frames.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  gBgScroll[1].y = 32;
  for (frame = 0, background = (u16 *)Ram_HeapSlots->battleWork->unk670;
       frame != 7; frame++) {
    BattleBackground_SetPaletteLevelFar(*background, 6 - frame);
    wait(1);
  }
  BattleEffect_HideBg1AndRestoreBackdrop();
}
