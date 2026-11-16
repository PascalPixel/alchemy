/// @file
/// Annihilation.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#undef HIT_FRAME

#define SHARD_COUNT 64
#define STREAK_PICTURE_BYTES PICTURE_BYTES(FIRE_STREAK)
#define BURST_PICTURE_BYTES PICTURE_BYTES(FIRE_BURST)
#define BURST_STRIP_BYTES (3 * BURST_PICTURE_BYTES) ///< the stronger variant's

#define ANNIHILATION_FRAMES 64
#define STREAK_FRAME 6 ///< the user's fire streak, six frames
#define HIT_FRAME 8
#define FLASH_FRAME 14
#define BURST_FRAME 16 ///< the burst on the target, until frame 47

/// The user closes on the first target and fire bursts from it in a shower of
/// shards.
///
/// Abilities: Annihilation.
void BattleEffect_RunAnnihilation(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *cells;
  struct BattleCamera *view;
  struct MotionObject *target;
  struct Particle *shard;
  struct ScreenPoint user;
  struct ScreenPoint origin;
  struct ScreenPoint screen;
  DrawRectangle draw[2];
  s32 i;
  s32 size;
  u8 *burst;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  cells = Ram_HeapSlots->battleSheet;
  view = Ram_HeapSlots->cameraWork;
  burst = Ram_MapCellBuffer;

  // Open the canvas, load the pictures, and dash at the target.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_STREAK_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FIRE_BURST_SHEET, burst, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, cells, 0, 0);
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 4, 0);
  wait(1);
  target = BattleObject_GetFar(args->targets[0])->object;

  // The shards start inside the target; each group of four lives two frames
  // longer than the one before.
  i = 0;
  do {
    struct Particle *seed = &work->particles[i];

    SET_POSITION(seed, target->x, target->y, target->z);
    seed->velocityX = (Random16() & 255) << 11;
    seed->velocityY = ((Random16() & 255) - 127) << 12;
    seed->velocityZ = ((Random16() & 255) - 127) << 12;
    if (seed->x > 0)
      seed->velocityX = -seed->velocityX;
    seed->variant = i / 4 * 2 + 16;
    i++;
  } while (i != SHARD_COUNT);

  EffectPosition_ProjectScaledBattler(args->targets[0], &origin);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != ANNIHILATION_FRAMES; frame++) {
    if (frame == HIT_FRAME)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (args->variant != EFFECT_VARIANT_BASIC && frame == HIT_FRAME)
      Sound_PlayFar(SE_POWER_UP);

    // The streak, drawn from the user toward the target.
    EffectPosition_ProjectScaledBattler(args->user, &user);
    if (frame >= STREAK_FRAME && frame <= STREAK_FRAME + 5) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                            BLITTER_ADDITIVE);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            BLITTER_ADDITIVE);
      draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
      if (args->side == EFFECT_SIDE_ENEMIES)
        draw[0](canvas,
                work->sheet + (frame - STREAK_FRAME) * STREAK_PICTURE_BYTES,
                user.x / 2 - 24, user.y - 24, FIRE_STREAK_WIDTH,
                FIRE_STREAK_HEIGHT);
      else
        draw[0](canvas,
                work->sheet + (frame - STREAK_FRAME) * STREAK_PICTURE_BYTES,
                user.x / 2, user.y - 24, FIRE_STREAK_WIDTH, FIRE_STREAK_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }

    // The burst on the target; the stronger variant has its own pictures.
    if (frame >= BURST_FRAME && frame <= 47) {
      s32 step = (frame - BURST_FRAME) / 2;
      s32 strip;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
      draw[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
      if (step > 2)
        step = 2;
      if (args->variant == EFFECT_VARIANT_BASIC)
        strip = 0;
      else
        strip = BURST_STRIP_BYTES;
      draw[0](canvas, Ram_MapCellBuffer + strip + step * BURST_PICTURE_BYTES,
              origin.x / 2 - 20, origin.y - 48, FIRE_BURST_WIDTH,
              FIRE_BURST_HEIGHT);
      BattleEffect_CallDoNothing(10000);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }
    if (frame == HIT_FRAME)
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));

    // The shards, projected from the battle camera.
    Matrix_Initialize();
    Matrix_SetLookAt(view->eye, view->focus);
    if (frame > 3) {
      BattleEffect_FetchRectangleBlitters(args->side, draw);
      for (i = 0; i != SHARD_COUNT; i++) {
        s32 life;

        shard = &work->particles[i];
        life = shard->variant;
        if (life > 0) {
          EffectPosition_ProjectPoint((s32 *)shard, &screen);
          screen.x >>= 1;
          screen.y = screen.y + origin.y - 112;
          size = (life >> 3) + 2;
          draw[(i / 2) & 1](
              canvas, cells + gBattleEffect_MotePictures[size - 1],
              screen.x - size / 2, screen.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(shard, 60, -toFIXED(1) / 64);
          shard->variant--;
        }
      }
      EFFECT_RELEASE_BLITTERS();
    }

    // The hit's knock-back, and the target's flashes.
    if (frame == HIT_FRAME) {
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      work->shakeFrames = 4;
    }
    if (frame == STREAK_FRAME)
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_RED_TINT,
                               TINT_KEEP, TINT_KEEP, 0);
    if (frame == FLASH_FRAME)
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_RED_TINT,
                               TINT_KEEP, TINT_KEEP, 0);
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
