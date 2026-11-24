/// @file
/// Annihilation: a fire streak from the user, then a burst of fire and shards
/// on the target.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

/// The shards the burst throws.
#define SHARD_COUNT 64
/// The frames the first four shards live; each four after live two longer.
#define SHARD_LIFE 16
/// The stronger variant's three burst pictures, after the basic one's.
#define BURST_STRIP_BYTES (3 * PICTURE_BYTES(FIRE_BURST))

/// The effect's length and the frames of its strike, flash and burst.
#define ANNIHILATION_FRAMES 64
#define STREAK_FRAME 6 ///< the user's fire streak, six frames
#define HIT_FRAME 8
#define FLASH_FRAME 14
#define BURST_FRAME 16 ///< the burst on the target, until BURST_LAST_FRAME
#define BURST_LAST_FRAME 47

/// The user closes on the first target and fire bursts from it in a shower of
/// shards.
///
/// Abilities: Annihilation.
void BattleEffect_RunAnnihilation(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  s32 frame;
  u8 *particleSheet;
  struct BattleCamera *camera;
  struct MotionObject *targetActor;
  struct Particle *shard;
  struct ScreenPoint position;
  struct ScreenPoint origin;
  struct ScreenPoint out;
  DrawRectangle blitters[2];
  s32 i;
  s32 size;
  u8 *burstPixels;

  state = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  particleSheet = Ram_HeapSlots->battleSheet;
  camera = Ram_HeapSlots->cameraState;
  burstPixels = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_STREAK_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FIRE_BURST_SHEET, burstPixels, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, particleSheet, 0, 0);
  BattleMotion_ApproachTargetFar(args->user, args->targets[0], 4, 0);
  wait(1);
  targetActor = BattleObject_GetFar(args->targets[0])->object;

  // Shards burst from the target.
  for (i = 0; i != SHARD_COUNT; i++) {
    struct Particle *seed = &state->particles[i];

    SET_POSITION(seed, targetActor->x, targetActor->y, targetActor->z);
    seed->velocityX = (Random16() & 255) << 11;
    seed->velocityY = ((Random16() & 255) - 127) << 12;
    seed->velocityZ = ((Random16() & 255) - 127) << 12;
    if (seed->x > 0)
      seed->velocityX = -seed->velocityX;
    seed->variant = i / 4 * 2 + SHARD_LIFE;
  }

  EffectPosition_ProjectScaledBattler(args->targets[0], &origin);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != ANNIHILATION_FRAMES; frame++) {
    if (frame == HIT_FRAME)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (args->variant != EFFECT_VARIANT_BASIC && frame == HIT_FRAME)
      Sound_PlayFar(SE_POWER_UP);
    EffectPosition_ProjectScaledBattler(args->user, &position);
    // The fire streak in front of the user.
    if (frame >= STREAK_FRAME && frame <= STREAK_FRAME + 5) {
      if (args->side == EFFECT_SIDE_ENEMIES)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                            BLITTER_ADDITIVE);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            BLITTER_ADDITIVE);
      blitters[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
      if (args->side == EFFECT_SIDE_ENEMIES)
        blitters[0](canvas,
                    state->sheet +
                        (frame - STREAK_FRAME) * PICTURE_BYTES(FIRE_STREAK),
                    position.x / 2 - FIRE_STREAK_WIDTH / 2, position.y - 24,
                    FIRE_STREAK_WIDTH, FIRE_STREAK_HEIGHT);
      else
        blitters[0](canvas,
                    state->sheet +
                        (frame - STREAK_FRAME) * PICTURE_BYTES(FIRE_STREAK),
                    position.x / 2, position.y - 24, FIRE_STREAK_WIDTH,
                    FIRE_STREAK_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }
    // The fire burst over the target grows.
    if (frame >= BURST_FRAME && frame <= BURST_LAST_FRAME) {
      s32 step = (frame - BURST_FRAME) / 2;
      s32 strip;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
      blitters[0] = (DrawRectangle)Ram_HeapSlots->ramCode;
      if (step > 2)
        step = 2;
      if (args->variant == EFFECT_VARIANT_BASIC)
        strip = 0;
      else
        strip = BURST_STRIP_BYTES;
      blitters[0](canvas,
                  Ram_MapCellBuffer + strip + step * PICTURE_BYTES(FIRE_BURST),
                  origin.x / 2 - FIRE_BURST_WIDTH / 2, origin.y - 48,
                  FIRE_BURST_WIDTH, FIRE_BURST_HEIGHT);
      // A busy delay before the blitter is freed.
      // XXX (no evidence): why it waits; the delay changes nothing the game
      // draws or computes, so only the author's intent would say.
      BattleEffect_CallDoNothing(10000);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }
    // The strike flashes the canvas.
    if (frame == HIT_FRAME)
      Iwram_FillWordsEntry(canvas, CANVAS_BYTES,
                           CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
    EFFECT_VIEW_FROM_CAMERA(camera);
    // Shards shrink and fall.
    if (frame > 3) {
      BattleEffect_FetchRectangleBlitters(args->side, blitters);
      for (i = 0; i != SHARD_COUNT; i++) {
        s32 life;

        shard = &state->particles[i];
        life = shard->variant;
        if (life > 0) {
          EffectPosition_ProjectPoint(shard, &out);
          out.x >>= 1;
          out.y = out.y + origin.y - 112;
          size = (life >> 3) + 2;
          EFFECT_DRAW_MOTE(blitters[(i / 2) & 1], canvas, particleSheet, out.x,
                           out.y, size);
          Particle_AdvanceWithGravity3D(shard, 60, -toFIXED(1) / 64);
          shard->variant--;
        }
      }
      EFFECT_RELEASE_BLITTERS();
    }
    // The strike throws the target back.
    if (frame == HIT_FRAME) {
      BattleMotion_ApplyKnockbackFar(args->targets[0], KNOCKBACK_HURL);
      state->shakeFrames = 4;
    }
    if (frame == STREAK_FRAME)
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_RED_TINT,
                               TINT_KEEP, TINT_KEEP, 0);
    if (frame == FLASH_FRAME)
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_RED_TINT,
                               TINT_KEEP, TINT_KEEP, 0);
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef SHARD_COUNT
#undef SHARD_LIFE
#undef BURST_STRIP_BYTES
#undef ANNIHILATION_FRAMES
#undef STREAK_FRAME
#undef HIT_FRAME
#undef FLASH_FRAME
#undef BURST_FRAME
#undef BURST_LAST_FRAME
