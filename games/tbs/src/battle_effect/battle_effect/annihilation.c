/// @file
/// Annihilation.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

#define ANNIHILATION_FRAMES 64

/// The user closes on the first target and fire bursts from it in a shower of
/// shards.
///
/// Abilities: Annihilation (BATTLE_EFFECT_ANNIHILATION).
void BattleEffect_RunAnnihilation(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
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

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  particleSheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  burstPixels = Ram_MapGridCellBuffer;
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burstPixels, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  BattleMotion_ApproachTargetFar(work->args->user, work->args->targets[0], 4,
                                 0);
  wait(1);
  targetActor = GetBattleObjectSlotFar(work->args->targets[0])->actor;

  for (i = 0; i != 64; i++) {
    // 64 shards start at the target, flying across towards the middle of the
    // field.
    struct Particle *seed = &work->particles[i];

    SET_POSITION(seed, targetActor->x.fixed, targetActor->y.fixed,
                 targetActor->z.fixed);
    SET_VELOCITY(seed, (Random16() & 255) << 11,
                 ((Random16() & 255) - 127) << 12,
                 ((Random16() & 255) - 127) << 12);
    if (seed->x > 0)
      seed->velocityX = -seed->velocityX;
    seed->variant = i / 4 * 2 + 16;
  }

  Projection_ScaledUnit(work->args->targets[0], &origin);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != ANNIHILATION_FRAMES; frame++) {
    if (frame == 8)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (work->args->variant != EFFECT_VARIANT_BASIC && frame == 8)
      Sound_PlayFar(SE_POWER_UP);
    Projection_ScaledUnit(work->args->user, &position);
    // The fire streak, one picture a frame in front of the user, drawn
    // additively and mirrored for EFFECT_SIDE_PARTY with a blitter made for
    // this one drawing.
    if (frame >= 6 && frame <= 11) {
      if (work->args->side == EFFECT_SIDE_ENEMIES)
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                            COMPOSITE_ADDITIVE);
      else
        EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                            EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                            COMPOSITE_ADDITIVE);
      blitters[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
      if (work->args->side == EFFECT_SIDE_ENEMIES)
        blitters[0](canvas, work->sheet + (frame - 6) * (48 * 72),
                    position.x / 2 - 24, position.y - 24, 48, 72);
      else
        blitters[0](canvas, work->sheet + (frame - 6) * (48 * 72),
                    position.x / 2, position.y - 24, 48, 72);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }
    // The fire burst over the target grows a step every two frames up to the
    // third (frame 20) and holds it to frame 47.
    if (frame >= 16 && frame <= 47) {
      s32 step = (frame - 16) / 2;
      s32 strip;

      EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                          COMPOSITE_LIGHTEN);
      blitters[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
      if (step > 2)
        step = 2;
      // Each strip holds three 40x80 pictures.
      if (work->args->variant == EFFECT_VARIANT_BASIC)
        strip = 0;
      else
        strip = 3 * (40 * 80);
      blitters[0](canvas, Ram_MapGridCellBuffer + strip + step * (40 * 80),
                  origin.x / 2 - 20, origin.y - 48, 40, 80);
      // A busy delay before the blitter is freed (BattleEffect_CallDoNothing).
      // Nothing here depends on the time it takes.
      // XXX (no evidence): why it waits; the delay changes nothing the game
      // draws or computes, so only the author's intent would say.
      BattleEffect_CallDoNothing(10000);
      HeapSlot_Release(HEAP_SLOT_RAM_CODE);
    }
    // The strike fills the canvas at its brightest, a flash that the transfer
    // then fades.
    if (frame == 8) {
      Dma_Fill32(CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST), canvas,
                 CANVAS_BYTES);
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    // Shards shrink as their lives run down, from mote size 7 to 2, and fall at
    // 1/64 pixel a frame per frame.
    if (frame > 3) {
      BattleEffect_FetchRectangleBlitters(work->args->side, blitters);
      for (i = 0; i != 64; i++) {
        s32 life;

        shard = &work->particles[i];
        life = shard->variant;
        if (life > 0) {
          Projection_Point(shard, &out);
          out.x >>= 1;
          out.y = out.y + origin.y - 112;
          size = (life >> 3) + 2;
          EFFECT_DRAW_MOTE(blitters[(i / 2) & 1], canvas, particleSheet, out.x,
                           out.y, size);
          Particle_AdvanceWithGravity3D(shard, 60, -px(1) / 64);
          shard->variant--;
        }
      }
      EFFECT_RELEASE_BLITTERS();
    }
    // The strike throws the target back with KNOCKBACK_HURL and shakes the
    // screen 4 frames.
    if (frame == 8) {
      BattleMotion_ApplyKnockbackFar(work->args->targets[0], KNOCKBACK_HURL);
      work->shakeFrames = 4;
    }
    // The target glows red on frames 6 and 14.
    if (frame == 6)
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_RED_TINT,
                               TINT_KEEP, TINT_KEEP, 0);
    if (frame == 14)
      BattleEffect_TintBattler(work->args->targets[0], SPRITE_COLOR_RED_TINT,
                               TINT_KEEP, TINT_KEEP, 0);
    BattleCamera_Shake(16, 16);
    EFFECT_END_FRAME(work);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef ANNIHILATION_FRAMES
