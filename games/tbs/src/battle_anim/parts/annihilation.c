/// @file
/// Annihilation.
// Part of battle_anim.c.

#include "../battle_anim_internal.h"

#define ANNIHILATION_FRAMES 64

/// The user closes on the first target and fire bursts from it in a shower of
/// shards.
///
/// Abilities: Annihilation (BATTLE_ANIM_ANNIHILATION).
///
/// Its frames play out like this:
///
///   4 on       64 shards scatter from the target and fall
///   6 to 11    a fire streak flares in front of the user
///   8          the target is struck; the canvas is filled
///   16 to 47   a fire burst grows over the target in three steps
///
/// work->particles 0 to 63 hold the shards and Ram_MapGridCellBuffer the
/// burst's pictures. Each drawing builds the blitters it needs and frees them
/// the same frame; the animation ends with the transfer callback removed and
/// the canvas layer closed.
///
/// effect: the user and the target; a nonzero variant gives the burst its
/// second picture and a sound.
void BattleAnim_RunAnnihilation(struct BattleAnimArgs *effect) {
  void **slots;
  void **cursor;
  struct BattleAnimWork *work;
  void *canvas;
  u8 *particleSheet;
  struct BattleCamera *camera;
  struct Actor *targetObject;
  struct Particle *shard;
  struct ScreenPoint position;
  struct ScreenPoint origin;
  struct ScreenPoint out;
  DrawRectangle blitters[2];
  s32 i;
  s32 frame;
  s32 size;
  u8 *burstPixels;

  slots = &gHeapSlots[HEAP_SLOT_BATTLE_ANIM];
  cursor = slots;
  ANIM_GET_WORK_AND_CANVAS(cursor, work, canvas);
  particleSheet = ANIM_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  camera = ANIM_SLOT(slots, HEAP_SLOT_CAMERA);
  burstPixels = Ram_MapGridCellBuffer;
  work->effect = effect;
  BattleAnim_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burstPixels, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, particleSheet,
                             0, 0);
  BattleMotion_ApproachTargetFar(work->effect->user, work->effect->targets[0],
                                 4, 0);
  WaitFrames(1);
  targetObject = GetBattleObjectSlotFar(work->effect->targets[0])->object;

  i = 0;
  do {
    // 64 shards start at the target, flying across towards the middle of
    // the field; each group of four lives two frames longer than the last.
    struct Particle *seed = &work->particles[i];

    SET_POSITION(seed, targetObject->x.fixed, targetObject->y.fixed,
                 targetObject->z.fixed);
    SET_VELOCITY(seed, (Random16() & 255) << 11,
                 ((Random16() & 255) - 127) << 12,
                 ((Random16() & 255) - 127) << 12);
    if (seed->x > 0)
      seed->velocityX = -seed->velocityX;
    seed->variant = i / 4 * 2 + 16;
    i++;
  } while (i != 64);

  Projection_ScaledUnit(work->effect->targets[0], &origin);
  Task_Add(BattleAnim_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  ANIM_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != ANNIHILATION_FRAMES; frame++) {
    if (frame == 8)
      ANIM_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (work->effect->variant != ANIM_VARIANT_BASIC && frame == 8)
      Sound_PlaySongFar(SE_POWER_UP);
    Projection_ScaledUnit(work->effect->user, &position);
    // The fire streak, one picture a frame in front of the user, drawn
    // additively and mirrored for ANIM_SIDE_PARTY with a blitter made for this
    // one drawing.
    if (frame >= 6 && frame <= 11) {
      if (work->effect->side == ANIM_SIDE_ENEMIES)
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                          COMPOSITE_ADDITIVE);
      else
        ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
                          ANIM_CLIP_BOUNDS | COMPOSITE_FLIP_X,
                          COMPOSITE_ADDITIVE);
      blitters[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
      if (work->effect->side == ANIM_SIDE_ENEMIES)
        blitters[0](canvas, work->sheet + (frame - 6) * (48 * 72),
                    position.x / 2 - 24, position.y - 24, 48, 72);
      else
        blitters[0](canvas, work->sheet + (frame - 6) * (48 * 72),
                    position.x / 2, position.y - 24, 48, 72);
      FreeHeapSlot(HEAP_SLOT_RAM_CODE);
    }
    // The fire burst over the target grows a step every two frames up to
    // the third (frame 20) and holds it to frame 47; a nonzero variant takes
    // the second strip of pictures.
    if (frame >= 16 && frame <= 47) {
      s32 step = (frame - 16) / 2;
      s32 strip;

      ANIM_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, ANIM_CLIP_BOUNDS,
                        COMPOSITE_LIGHTEN);
      blitters[0] = (DrawRectangle)gHeapSlots[HEAP_SLOT_RAM_CODE];
      if (step > 2)
        step = 2;
      // Each strip holds three 40x80 pictures.
      if (work->effect->variant == ANIM_VARIANT_BASIC)
        strip = 0;
      else
        strip = 3 * (40 * 80);
      blitters[0](canvas, Ram_MapGridCellBuffer + strip + step * (40 * 80),
                  origin.x / 2 - 20, origin.y - 48, 40, 80);
      // A busy delay before the blitter is freed (BattleAnim_CallDoNothing).
      // Nothing here depends on the time it takes.
      // XXX (no evidence): why it waits; the delay changes nothing the game
      // draws or computes, so only the author's intent would say.
      BattleAnim_CallDoNothing(10000);
      FreeHeapSlot(HEAP_SLOT_RAM_CODE);
    }
    // The strike fills the canvas with level 63, a flash that the transfer
    // then fades.
    if (frame == 8) {
      Dma_Fill32(CANVAS_FILL_WORD(63), canvas, CANVAS_BYTES);
    }
    ANIM_VIEW_FROM_CAMERA(camera);
    // Shards shrink as their lives run down, from mote size 7 to 2, and fall
    // at 1/64 pixel a frame per frame. Pairs of shards alternate between the
    // brightest-wins and the additive blitter, both mirrored on
    // ANIM_SIDE_PARTY.
    if (frame > 3) {
      BattleAnim_FetchRectangleBlitters(work->effect->side, blitters);
      for (i = 0; i != 64; i++) {
        s32 life;

        shard = &work->particles[i];
        life = shard->variant;
        if (life > 0) {
          Projection_Point(shard, &out);
          out.x >>= 1;
          out.y = out.y + origin.y - 112;
          size = (life >> 3) + 2;
          ANIM_DRAW_MOTE(blitters[(i / 2) & 1], canvas, particleSheet, out.x,
                         out.y, size);
          Particle_AdvanceWithGravity3D(shard, 60, -0x400);
          shard->variant--;
        }
      }
      ANIM_RELEASE_BLITTERS();
    }
    // The strike throws the target back with KNOCKBACK_HURL and shakes the
    // screen 4 frames.
    if (frame == 8) {
      BattleMotion_ApplyKnockbackFar(work->effect->targets[0], KNOCKBACK_HURL);
      work->shakeFrames = 4;
    }
    // The target glows red on frames 6 and 14.
    if (frame == 6)
      BattleAnim_TintBattler(work->effect->targets[0], SPRITE_COLOR_RED_TINT,
                             -1, -1, 0);
    if (frame == 14)
      BattleAnim_TintBattler(work->effect->targets[0], SPRITE_COLOR_RED_TINT,
                             -1, -1, 0);
    BattleCamera_Shake(16, 16);
    ANIM_END_FRAME(work);
  }
  Task_Remove(BattleAnim_ProcessPendingGraphicsTransfer);
  BattleAnim_EndCanvasLayer();
}

#undef ANNIHILATION_FRAMES
