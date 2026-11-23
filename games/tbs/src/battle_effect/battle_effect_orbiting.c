/// @file
/// Swirling stars, orbiting skulls and target bursts.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

// None of the three effects here is called or named by any table: nothing in
// the game reaches them.

#define SWIRLING_STARS_FRAMES 160
#define SWIRLING_STARS_COUNT 64   ///< stars that swirl
#define SWIRLING_STARS_PLACED 256 ///< stars given a place, the first to swirl
/// A star is drawn SWIRLING_STAR_LARGEST pixels wide (and twice as tall) at
/// the nearest depth, a pixel narrower for every SWIRLING_STAR_DEPTH_STEP
/// farther, its depth clamped from SWIRLING_STAR_DEPTH_NEAR to
/// SWIRLING_STAR_DEPTH_FAR.
#define SWIRLING_STAR_LARGEST 9
#define SWIRLING_STAR_DEPTH_NEAR 250
#define SWIRLING_STAR_DEPTH_FAR 634
#define SWIRLING_STAR_DEPTH_STEP 64
/// The angle star `i` turns each frame about its axis: each later star faster.
#define SWIRLING_STAR_SPIN(i) (((i) + 32) * 8)

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
  for (i = 0; i != SWIRLING_STARS_PLACED; i++) {
    star = &PARTICLES[i];
    SET_POSITION(star, INTtoFIXED((Random16() & 0xff) - 127),
                 INTtoFIXED((Random16() & 0xff) - 127),
                 INTtoFIXED((Random16() & 0xff) - 127));
    SET_VELOCITY(star, 0, 0, 0);
    star->variant = 0;
  }
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  SET_VECTOR(center, 0, toFIXED(80), 0);
  for (frame = 0; frame != SWIRLING_STARS_FRAMES; frame++) {
    struct BattleCamera *camera = gBattleCamera;

    EFFECT_VIEW_FROM_CAMERA(camera);
    Matrix_TranslateVector(center);
    for (i = 0; i != SWIRLING_STARS_COUNT; i++) {
      star = &PARTICLES[i];
      if (frame > i / 4 && star->variant == 0) {
        s32 size;

        Matrix_Push();
        switch (i & 3) {
        case SPIN_YAW:
          Matrix_RotateY(frame * SWIRLING_STAR_SPIN(i));
          break;
        case SPIN_PITCH:
          Matrix_RotateX(-frame * SWIRLING_STAR_SPIN(i));
          break;
        case SPIN_ROLL:
          Matrix_RotateZ(-frame * SWIRLING_STAR_SPIN(i));
          break;
        case SPIN_PITCH_AND_ROLL:
          Matrix_RotateX(-frame * SWIRLING_STAR_SPIN(i));
          Matrix_RotateZ(-frame * SWIRLING_STAR_SPIN(i));
          break;
        }
        Projection_Point(star, &screen);
        screen.x >>= 1;
        Matrix_Pop();
        CLAMP(screen.depth, SWIRLING_STAR_DEPTH_NEAR, SWIRLING_STAR_DEPTH_FAR);
        size =
            SWIRLING_STAR_LARGEST - (screen.depth - SWIRLING_STAR_DEPTH_NEAR) /
                                        SWIRLING_STAR_DEPTH_STEP;
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
  /* LEFTOVER(frame): draw[1], the idle half of the pair. */
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
  REG_BLDALPHA = EFFECT_BLEND_ADD;
  skulls = Resource_GetTableEntry(RESOURCE_SKULL_SHEET);
  Iwram_CopyWords(BG_PLTT, skulls, SHEET_PALETTE_BYTES);
  Decompress_TaggedLz(skulls + SHEET_PALETTE_BYTES, work->sheet);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_CLEAR, 0);
  EFFECT_SET_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
  draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_VBLANK);
  if (work->args->side == EFFECT_SIDE_ENEMIES)
    xOffset = 0;
  else
    xOffset = -NARROW_CANVAS_PARTY_X;
  /* FAKEMATCH(dead): the scroll's store in a once-only block. */
  do {
    REG_BG2X = xOffset << 8;
  } while (0);
  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
    struct Particle *skull = &work->particles[i];

    skull->x = 0;
    skull->y = 0;
    skull->velocityX = 0;
    skull->z = 4;
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
        /* FAKEMATCH(temporary): rec, a pointer standing in for &point. */
        s32 *rec = NULL;

        EFFECT_VIEW_FROM_CAMERA(camera);
        rec = point;
        SET_VECTOR(rec, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        Projection_Point(rec, &center);
        center.x += xOffset;
        /* FAKEMATCH(dead): an empty once-only loop. */
        do {
        } while (0);
        for (i = 0; i != 3; i++) {
          // A skull's velocityX is its angle round the target.
          struct Particle *skull = &work->particles[member * 3 + i];
          s32 x;
          s32 y;

          x = screen.x +
              FIXEDtoINT(
                  (Trig_Sin(skull->velocityX + i * (DEGtoANG(120))) * 8));
          y = screen.y +
              FIXEDtoINT(
                  (Trig_Cos(skull->velocityX + i * (DEGtoANG(120))) * 8));
          skull->velocityX += TRIG_TURN / 128;
          draw[0](canvas, work->sheet + i * PICTURE_BYTES(SKULL),
                  x - SKULL_WIDTH / 2, y - 28, SKULL_WIDTH, SKULL_HEIGHT);
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

/// The burst's six pictures in the sprite sheet, after the smoke's six.
#define TARGET_BURST_PICTURES (6 * PICTURE_BYTES(SMOKE_PUFF))

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
  Iwram_CopyWords(BG_PLTT, palette, SHEET_PALETTE_BYTES);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_ADDITIVE);
  drawBurst = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      COMPOSITE_LIGHTEN);
  drawSmoke = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_PAIRED);

  for (i = 0; i != ARRAY_COUNT(work->particles); i++) {
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
      struct Actor *actor;

      actor = GetBattleObjectSlotFar(work->args->targets[member])->actor;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, actor->x.fixed, toFIXED(20), actor->z.fixed);
      Matrix_TranslateVector(point);
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
            Particle_AdvanceWithGravity3D(puff, 62, toFIXED(1) / 32);
            drawBurst(canvas,
                      work->sheet + picture * PICTURE_BYTES(SMOKE_PUFF) +
                          TARGET_BURST_PICTURES,
                      screen.x - SMOKE_PUFF_WIDTH / 2, screen.y - 36,
                      SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
          } else {
            Particle_AdvanceWithGravity3D(puff, 60, toFIXED(1) / 128);
            drawSmoke(canvas, work->sheet + picture * PICTURE_BYTES(SMOKE_PUFF),
                      screen.x - SMOKE_PUFF_WIDTH / 2, screen.y - 36,
                      SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
          }
          puff->variant++;
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }
  EFFECT_FINISH_CANVAS();
}

#undef SWIRLING_STARS_FRAMES
#undef SWIRLING_STARS_COUNT
#undef SWIRLING_STARS_PLACED
#undef SWIRLING_STAR_LARGEST
#undef SWIRLING_STAR_DEPTH_NEAR
#undef SWIRLING_STAR_DEPTH_FAR
#undef SWIRLING_STAR_DEPTH_STEP
#undef TARGET_BURST_PICTURES
#undef TARGET_BURSTS_FRAMES
