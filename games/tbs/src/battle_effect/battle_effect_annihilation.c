/// @file
/// Annihilation.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

#define ANNIHILATION_FRAMES 64
#define ANNIHILATION_SHARDS 64
/// The first four shards' life in frames; each four after live two frames
/// longer.
#define ANNIHILATION_SHARD_LIFE 16
/// The fire streak in front of the user, its pictures a frame each from here,
/// and the target's first red flash.
#define ANNIHILATION_STREAK_FRAME 6
#define ANNIHILATION_STREAK_PICTURES 6
/// The white flash, the damage and the knock-back.
#define ANNIHILATION_HIT_FRAME 8
#define ANNIHILATION_FLASH_FRAME 14 ///< the target's second red flash
/// The fire burst over the target, from ANNIHILATION_BURST_FRAME to
/// ANNIHILATION_BURST_LAST_FRAME: three pictures two frames each, the last
/// held.
#define ANNIHILATION_BURST_FRAME 16
#define ANNIHILATION_BURST_LAST_FRAME 47
#define ANNIHILATION_BURST_PICTURES 3

/// The user closes on the first target and fire bursts from it in a shower of
/// shards, the flames settling into the kanji for destroy over it (the
/// stronger variant, which no ability plays, draws the kanji for kill).
///
/// Abilities: Annihilation.
void BattleEffect_RunAnnihilation(struct BattleEffectParams *args) {
  struct BattleEffectState *state;
  void *canvas;
  u8 *particleSheet;
  struct BattleCamera *camera;
  struct Actor *targetActor;
  struct Particle *shard;
  struct ScreenPoint position;
  struct ScreenPoint origin;
  struct ScreenPoint out;
  DrawRectangle blitters[2];
  s32 i;
  s32 frame;
  s32 size;
  u8 *burstPixels;

  state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  particleSheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  burstPixels = Ram_MapGridCellBuffer;
  state->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_FIRE_STREAK_SHEET, state->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_FIRE_BURST_SHEET, burstPixels, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_PARTICLE_SPRITES_A, particleSheet, 0, 0);
  BattleMotion_ApproachTargetFar(state->args->user, state->args->targets[0], 4,
                                 0);
  wait(1);
  targetActor = BattleObject_GetFar(state->args->targets[0])->actor;

  for (i = 0; i != ANNIHILATION_SHARDS; i++) {
    // Shards burst from the target.
    struct Particle *seed = &state->particles[i];

    SET_POSITION(seed, targetActor->x.fixed, targetActor->y.fixed,
                 targetActor->z.fixed);
    SET_VELOCITY(seed, (Random16() & 255) << 11,
                 ((Random16() & 255) - 127) << 12,
                 ((Random16() & 255) - 127) << 12);
    if (seed->x > 0)
      seed->velocityX = -seed->velocityX;
    seed->variant = i / 4 * 2 + ANNIHILATION_SHARD_LIFE;
  }

  Projection_ScaledUnit(state->args->targets[0], &origin);
  Task_Add(BattleEffect_TransferCanvas, TASK_PASS_VBLANK);
  EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != ANNIHILATION_FRAMES; frame++) {
    if (frame == ANNIHILATION_HIT_FRAME)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (state->args->variant != EFFECT_VARIANT_BASIC &&
        frame == ANNIHILATION_HIT_FRAME)
      Sound_PlayFar(SE_POWER_UP);
    Projection_ScaledUnit(state->args->user, &position);
    // The fire streak in front of the user.
    if (frame >= ANNIHILATION_STREAK_FRAME &&
        frame < ANNIHILATION_STREAK_FRAME + ANNIHILATION_STREAK_PICTURES) {
      if (state->args->side == EFFECT_SIDE_ENEMIES)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_ADDITIVE);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            COMPOSITE_ADDITIVE);
      blitters[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
      if (state->args->side == EFFECT_SIDE_ENEMIES)
        blitters[0](canvas,
                    state->sheet + (frame - ANNIHILATION_STREAK_FRAME) *
                                       PICTURE_BYTES(FIRE_STREAK),
                    position.x / 2 - FIRE_STREAK_WIDTH / 2, position.y - 24,
                    FIRE_STREAK_WIDTH, FIRE_STREAK_HEIGHT);
      else
        blitters[0](canvas,
                    state->sheet + (frame - ANNIHILATION_STREAK_FRAME) *
                                       PICTURE_BYTES(FIRE_STREAK),
                    position.x / 2, position.y - 24, FIRE_STREAK_WIDTH,
                    FIRE_STREAK_HEIGHT);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }
    // The fire burst over the target grows.
    if (frame >= ANNIHILATION_BURST_FRAME &&
        frame <= ANNIHILATION_BURST_LAST_FRAME) {
      s32 step = (frame - ANNIHILATION_BURST_FRAME) / 2;
      s32 strip;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      blitters[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
      if (step > ANNIHILATION_BURST_PICTURES - 1)
        step = ANNIHILATION_BURST_PICTURES - 1;
      if (state->args->variant == EFFECT_VARIANT_BASIC)
        strip = 0;
      else
        strip = ANNIHILATION_BURST_PICTURES * PICTURE_BYTES(FIRE_BURST);
      blitters[0](canvas,
                  Ram_MapGridCellBuffer + strip +
                      step * PICTURE_BYTES(FIRE_BURST),
                  origin.x / 2 - FIRE_BURST_WIDTH / 2, origin.y - 48,
                  FIRE_BURST_WIDTH, FIRE_BURST_HEIGHT);
      // A busy delay before the blitter is freed.
      // XXX (no evidence): why it waits; the delay changes nothing the game
      // draws or computes, so only the author's intent would say.
      BattleEffect_CallDoNothing(10000);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }
    // The strike flashes the canvas.
    if (frame == ANNIHILATION_HIT_FRAME) {
      Dma_Fill32(CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST), canvas,
                 CANVAS_BYTES);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    // Shards shrink and fall.
    if (frame > 3) {
      BattleEffect_BuildCanvasBlitters(state->args->side, blitters);
      for (i = 0; i != ANNIHILATION_SHARDS; i++) {
        s32 life;

        shard = &state->particles[i];
        life = shard->variant;
        if (life > 0) {
          Projection_Point(shard, &out);
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
    if (frame == ANNIHILATION_HIT_FRAME) {
      BattleMotion_ApplyKnockbackFar(state->args->targets[0], KNOCKBACK_HURL);
      state->shakeFrames = 4;
    }
    if (frame == ANNIHILATION_STREAK_FRAME)
      BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_RED_TINT,
                               TINT_KEEP, TINT_KEEP, 0);
    if (frame == ANNIHILATION_FLASH_FRAME)
      BattleEffect_TintBattler(state->args->targets[0], SPRITE_COLOR_RED_TINT,
                               TINT_KEEP, TINT_KEEP, 0);
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(state);
  }
  Task_Remove(BattleEffect_TransferCanvas);
  BattleEffect_EndCanvasLayer();
}

#undef ANNIHILATION_FRAMES
#undef ANNIHILATION_SHARDS
#undef ANNIHILATION_SHARD_LIFE
#undef ANNIHILATION_STREAK_FRAME
#undef ANNIHILATION_STREAK_PICTURES
#undef ANNIHILATION_HIT_FRAME
#undef ANNIHILATION_FLASH_FRAME
#undef ANNIHILATION_BURST_FRAME
#undef ANNIHILATION_BURST_LAST_FRAME
#undef ANNIHILATION_BURST_PICTURES
