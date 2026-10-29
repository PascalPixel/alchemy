/// @file
/// Swirling stars, orbiting skulls and target bursts.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// None of the three effects here is called, and no table or other data in
// the ROM holds the address of any of them: they are in the ROM but
// unreachable.

#define SWIRLING_STARS_FRAMES 160
#define SWIRLING_STARS_COUNT 64 ///< stars that swirl

/// Swirling stars: 64 stars scattered around a point 80 pixels up spin
/// about it, each on its own axis, appearing four a frame; 30 frames after
/// it appears, each star is drawn back towards the centre. Stars are drawn
/// smaller the further away they are. The effect runs for 160 frames. All
/// 256 stars of the pool are seeded, at rest in a cube 255 pixels across,
/// though only 64 are drawn.
///
/// Nothing calls it. Draws through the side's blitters mirrored (side ^ 1).
UNUSED void BattleEffect_RunSwirlingStars(struct BattleEffectParams *args) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 center[3];
  struct ScreenPoint screen;
  BlitterPair draw;
  struct Particle *star;
  s32 i;
  s32 frame;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(work->args->side ^ 1, draw);
  for (i = 0; i != 256; i++) {
    star = &PARTICLES[i];
    SET_POSITION(star, ((Random16() & 0xff) - 127) << 16,
                 ((Random16() & 0xff) - 127) << 16,
                 ((Random16() & 0xff) - 127) << 16);
    SET_VELOCITY(star, 0, 0, 0);
    star->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  // The stars turn about a point 80 pixels up.
  SET_VECTOR(center, 0, 160 << 15, 0);
  for (frame = 0; frame != SWIRLING_STARS_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = gBattleCamera;
    EFFECT_VIEW_FROM_CAMERA(camera);
    Transform_ApplyPosition(center);
    star = PARTICLES;
    // Star i appears after frame i / 4; variant stays 0, so none is retired.
    for (i = 0; i != SWIRLING_STARS_COUNT; i++, star++) {
      if (frame > i / 4 && star->variant == 0) {
        s32 size;

        // Turn the star about one of four axes, picked by its index, faster
        // for later stars.
        Transform_Push();
        switch (i & 3) {
        case SPIN_YAW:
          Transform_ApplyYaw(frame * (i + 32) * 8);
          break;
        case SPIN_PITCH:
          Transform_ApplyPitch(-frame * (i + 32) * 8);
          break;
        case SPIN_ROLL:
          Transform_ApplyRoll(-frame * (i + 32) * 8);
          break;
        case SPIN_PITCH_AND_ROLL:
          Transform_ApplyPitch(-frame * (i + 32) * 8);
          Transform_ApplyRoll(-frame * (i + 32) * 8);
          break;
        }
        Projection_Point(star, &screen);
        screen.x >>= 1;
        Transform_Pop();
        // Size 9 to 3 over depths 250 to 634, from the star's own mote set
        // (i & 3).
        CLAMP(screen.depth, 250, 634);
        size = 9 - (screen.depth - 250) / 64;
        draw[0](canvas,
                work->sheet + ((i & 3) * MOTE_SET_BYTES +
                               gBattleEffect_MoteCells[size - 1]),
                screen.x - size / 2, screen.y - size, size, size * 2);
        Particle_AdvanceWithGravity3D(star, 60, 0);
        // From 30 frames after it appears, it accelerates towards the centre
        // by 1/256 of its distance a frame, a spring that the 60/64 damping
        // above slows.
        if (frame > i / 4 + 30) {
          s32 dx = -star->x >> 8;
          s32 dy = -star->y >> 8;
          s32 dz = -star->z >> 8;

          star->velocityX += dx;
          star->velocityY += dy;
          star->velocityZ += dz;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS();
}

/// Orbiting skulls: three skulls circle each target in turn, 16 frames
/// apart, for 64 frames each, a third of a turn apart on a circle of 8
/// pixels around the target. Runs with the effect already in
/// gBattleEffectWork.
///
/// Nothing calls it. Copies the skull palette over the first 128 bytes of BG
/// palette RAM; writes REG_BG2PA, REG_BG2X and REG_BLDALPHA; keeps each
/// skull's angle in velocityX of work->particles, three to a target.
UNUSED void BattleEffect_RunOrbitingSkulls(void) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 point[3];
  struct ScreenPoint center;
  struct ScreenPoint screen;
  s32 frame;
  /* FAKEMATCH(dead): the blitter kept in a pair, whose unused second word
   * fills the frame; as one DrawRectangle, 11 of 265 instructions differ in
   * tbs-en. */
  BlitterPair draw;
  s32 member;
  struct BattleCamera *camera;
  u8 *skulls;
  s32 xOffset;
  s32 i;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1));
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  skulls = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
  Iwram_CopyWords((void *)BG_PLTT, skulls, 128);
  Decompress_TaggedLz(skulls + 128, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  // The first transfer mode is overwritten at once.
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  draw[0] = (DrawRectangle)HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  // Put the canvas on the targets' side of the screen: at one texel a pixel
  // it covers screen x 0 to 127 for enemy targets and 112 to 239 for the
  // party.
  if (work->args->side == EFFECT_SIDE_ENEMIES)
    xOffset = 0;
  else
    xOffset = -112;
  /* FAKEMATCH(one-pass): the scroll's store in a once-only block, so the
   * loop's i = 0 comes before the particles' address; as the plain store, 2
   * of 265 instructions differ in tbs-en. */
  do {
    REG_BG2X = xOffset << 8;
  } while (0);
  i = 0;
  // velocityX holds each skull's angle on its orbit; x, y and z are set but
  // never read.
  for (; i != 64; i++) {
    struct Particle *step = &work->particles[i];

    step->x = 0;
    step->y = 0;
    step->velocityX = 0;
    step->z = 4;
  }
  for (frame = 0; frame != work->args->targetCount * 16 + 64; frame++) {
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *actor;
      u32 memberFrame;

      actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      Projection_ScaledUnit(work->args->targets[member], &screen);
      memberFrame = frame - member * 16;
      screen.x += xOffset;
      // Unsigned, so the frames before this target's turn are skipped too.
      if (memberFrame < 64) {
        /* FAKEMATCH(dead): rec initialized to NULL, a second set loop.c
         * sees, so it keeps the point's address in rec instead of hoisting
         * it into fp; as s32 *rec, 57 of 265 instructions differ in tbs-en. */
        s32 *rec = NULL;

        // The actor's projection is computed, but nothing reads it.
        EFFECT_VIEW_FROM_CAMERA(camera);
        rec = point;
        SET_VECTOR(rec, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        Projection_Point(rec, &center);
        center.x += xOffset;
        /* FAKEMATCH(one-pass): an empty once-only loop keeps the store before
         * the spark loop's i = 0; as a plain store, 2 of 265 instructions
         * differ in tbs-en. */
        do {
        } while (0);
        // Three skulls a third of a turn apart, each turning 1/128 of a turn
        // a frame; skull i is the i-th 24x24 picture, drawn with its bottom
        // 4 pixels above its point on the circle.
        for (i = 0; i != 3; i++) {
          struct Particle *spark = &work->particles[member * 3 + i];
          s32 x;
          s32 y;

          x = screen.x +
              (((Trig_Sin(spark->velocityX + i * (TRIG_TURN / 3)) * 8)) >> 16);
          y = screen.y +
              (((Trig_Cos(spark->velocityX + i * (TRIG_TURN / 3)) * 8)) >> 16);
          spark->velocityX += TRIG_TURN / 128;
          draw[0](canvas, work->sheet + i * (24 * 24), x - 12, y - 28, 24, 24);
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// The burst picture in the sheet, after the smoke.
#define TARGET_BURSTS_OFFSETS (6 * (24 * 48))

#define TARGET_BURSTS_FRAMES 96

/// Target bursts: on each target in turn, 8 frames apart, a burst flares up
/// in the middle while five puffs of smoke scatter around it, for 40
/// frames.
///
/// Nothing calls it. Copies the pink burst palette over the first 128 bytes
/// of BG palette RAM, uses work->particles six to a target.
UNUSED void BattleEffect_RunTargetBursts(struct BattleEffectParams *args) {
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  void *palette;
  s32 frame;
  DrawRectangle drawSmoke;
  DrawRectangle drawBurst;
  struct Particle *puff;
  s32 i;
  struct ScreenPoint screen;
  s32 point[3];

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TargetBurstImage,
                             work->sheet + TARGET_BURSTS_OFFSETS, 0, 0);
  palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  drawBurst = EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  drawSmoke = EFFECT_SLOT(slots, HEAP_SLOT_RAM_CODE_2);

  // Six puffs for each target: five scatter at random, up to 2 pixels a
  // frame across and half a pixel vertically, and the sixth (the burst)
  // stays in the middle. The burst draws its two random numbers too, unused.
  for (i = 0; i != 64; i++) {
    s32 angle;
    s32 speed;

    puff = &work->particles[i];
    angle = Random16() & TRIG_ANGLE_MASK;
    speed = Random16() & 0xff;
    SET_POSITION(puff, 0, 0, 0);
    if (i % 6 == 5) {
      puff->velocityX = 0;
      puff->velocityY = 0;
    } else {
      puff->velocityX = (Trig_Sin(angle) * speed) >> 7;
      puff->velocityY = (Trig_Cos(angle) * speed) >> 9;
    }
    puff->velocityZ = 0;
    puff->variant = 0;
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  for (frame = 0; frame != TARGET_BURSTS_FRAMES; frame++) {
    s32 member;
    struct BattleCamera *camera;

    camera = gBattleCamera;
    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *args;

      args = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      EFFECT_VIEW_FROM_CAMERA(camera);
      // Centred 20 pixels above the target's feet.
      SET_VECTOR(point, args->x.fixed, 160 << 13, args->z.fixed);
      Transform_ApplyPosition(point);
      if (frame >= member * 8 && frame < member * 8 + 40) {
        struct Particle *puff;

        // The target's five puffs and its burst, each through six pictures
        // of six frames, holding the last; the burst rises faster.
        for (i = 0; i != 6; i++) {
          s32 cell;

          puff = &work->particles[member * 6 + i];
          cell = puff->variant / 6;
          if (cell > 5)
            cell = 5;
          Projection_Point(puff, &screen);
          screen.x >>= 1;
          if (i == 5) {
            Particle_AdvanceWithGravity3D(puff, 62, Q16_ONE / 32);
            drawBurst(canvas,
                      work->sheet + cell * (24 * 48) + TARGET_BURSTS_OFFSETS,
                      screen.x - 12, screen.y - 36, 24, 48);
          } else {
            Particle_AdvanceWithGravity3D(puff, 60, Q16_ONE / 128);
            drawSmoke(canvas, work->sheet + cell * (24 * 48), screen.x - 12,
                      screen.y - 36, 24, 48);
          }
          puff->variant += 1;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS();
}

#undef SWIRLING_STARS_FRAMES
#undef SWIRLING_STARS_COUNT
#undef TARGET_BURSTS_OFFSETS
#undef TARGET_BURSTS_FRAMES
