/// @file
/// Swirling stars, orbiting skulls and target bursts.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// None of the three effects here is called or named by any table: they are in
// the ROM but unreachable.

#define SWIRLING_STARS_FRAMES 160
#define SWIRLING_STARS_COUNT 64 ///< stars that swirl

/// Swirling stars: stars spin about a point above the field, each on its own
/// axis, then are drawn back towards it.
UNUSED void BattleEffect_RunSwirlingStars(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 center[3];
  struct ScreenPoint screen;
  BlitterPair draw;
  struct Particle *star;
  s32 i;
  s32 frame;

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_STAR_DOT_SHEET, work->sheet, 1, 1);
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
  SET_VECTOR(center, 0, 160 << 15, 0);
  for (frame = 0; frame != SWIRLING_STARS_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    EFFECT_VIEW_FROM_CAMERA(camera);
    Transform_ApplyPosition(center);
    star = PARTICLES;
    for (i = 0; i != SWIRLING_STARS_COUNT; i++, star++) {
      if (frame > i / 4 && star->variant == 0) {
        s32 size;

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
        CLAMP(screen.depth, 250, 634);
        size = 9 - (screen.depth - 250) / 64;
        draw[0](canvas,
                work->sheet + ((i & 3) * MOTE_SET_BYTES +
                               gBattleEffect_MotePictures[size - 1]),
                screen.x - size / 2, screen.y - size, size, size * 2);
        Particle_AdvanceWithGravity3D(star, 60, 0);
        // After 30 frames a star springs back towards the centre.
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

/// Orbiting skulls: three skulls circle each target in turn.
UNUSED void BattleEffect_RunOrbitingSkulls(void) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 point[3];
  struct ScreenPoint center;
  struct ScreenPoint screen;
  s32 frame;
  /* LEFTOVER(frame): draw[1], the idle half of the pair; the ROM's frame
   * reserves 88 bytes (sub sp, #88), 4 more than one blitter needs, and no
   * instruction addresses the word at sp+40; BlitterPair is the blitter set
   * every effect declares, and BattleEffect_FetchRectangleBlitters and 28
   * battle-effect files fill or call its second half; as one DrawRectangle
   * draw[1], 22 of 265 instructions differ in tbs-en. */
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
  skulls = Resource_GetTableEntry(RESOURCE_SKULL_SHEET);
  Iwram_CopyWords((void *)BG_PLTT, skulls, 128);
  Decompress_TaggedLz(skulls + 128, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  if (work->args->side == EFFECT_SIDE_ENEMIES)
    xOffset = 0;
  else
    xOffset = -112;
  /* FAKEMATCH(dead): the scroll's store in a once-only block; as the plain
   * store, 2 of 265 instructions differ in tbs-en. */
  do {
    REG_BG2X = xOffset << 8;
  } while (0);
  i = 0;
  for (i = 0; i != 64; i++) {
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
      if (memberFrame < 64) {
        // Default: NULL until it is pointed at the point below.
        s32 *rec = NULL;

        EFFECT_VIEW_FROM_CAMERA(camera);
        rec = point;
        SET_VECTOR(rec, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        Projection_Point(rec, &center);
        center.x += xOffset;
        /* FAKEMATCH(dead): an empty once-only loop; without it, 2 of 265
         * instructions differ in tbs-en. */
        do {
        } while (0);
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

/// The burst's six pictures in the sheet, after the smoke's six.
#define TARGET_BURST_PICTURES (6 * (24 * 48))

#define TARGET_BURSTS_FRAMES 96

/// Target bursts: on each target in turn a burst flares while puffs of smoke
/// scatter around it.
UNUSED void BattleEffect_RunTargetBursts(struct BattleEffectParams *args) {
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

  work = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
  canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
  work->args = args;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_TARGET_BURST_IMAGE,
                             work->sheet + TARGET_BURST_PICTURES, 0, 0);
  palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  drawBurst = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_2, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  drawSmoke = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_2);

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
    struct BattleCamera *camera = gBattleCamera;

    EFFECT_FOR_EACH_TARGET(member, work->args) {
      struct Actor *args;

      args = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, args->x.fixed, 160 << 13, args->z.fixed);
      Transform_ApplyPosition(point);
      if (frame >= member * 8 && frame < member * 8 + 40) {
        struct Particle *puff;

        for (i = 0; i != 6; i++) {
          s32 picture;

          puff = &work->particles[member * 6 + i];
          picture = puff->variant / 6;
          if (picture > 5)
            picture = 5;
          Projection_Point(puff, &screen);
          screen.x >>= 1;
          if (i == 5) {
            Particle_AdvanceWithGravity3D(puff, 62, px(1) / 32);
            drawBurst(canvas,
                      work->sheet + picture * (24 * 48) + TARGET_BURST_PICTURES,
                      screen.x - 12, screen.y - 36, 24, 48);
          } else {
            Particle_AdvanceWithGravity3D(puff, 60, px(1) / 128);
            drawSmoke(canvas, work->sheet + picture * (24 * 48), screen.x - 12,
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
#undef TARGET_BURST_PICTURES
#undef TARGET_BURSTS_FRAMES
