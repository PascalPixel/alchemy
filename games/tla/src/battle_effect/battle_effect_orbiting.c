/// @file
/// Swirling stars and target bursts, two effects no table names and nothing
/// in the game reaches.
// Part of battle_effect.c.

#include "battle_effect_internal.h"

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

/// Unused: nothing calls it. Stars spin about a point above the field, each on
/// its own axis, then are drawn back towards it.
UNUSED void BattleEffect_RunSwirlingStars(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  FIXED center[3];
  struct ScreenPoint screen;
  DrawRectangle draw[2];
  struct Particle *star;
  s32 i;
  s32 frame;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_STAR_DOT_SHEET, work->sheet, 1, 1);
  BattleEffect_FetchRectangleBlitters(args->side ^ 1, draw);
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
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    EFFECT_VIEW_FROM_CAMERA(camera);
    Matrix_TranslateVector(center);
    for (i = 0; i != SWIRLING_STARS_COUNT; i++) {
      star = &PARTICLES[i];
      if (frame > i / 4 && star->variant == 0) {
        s32 size;

        Matrix_Push();
        switch (i & 3) {
        case 0:
          Matrix_RotateY(frame * SWIRLING_STAR_SPIN(i));
          break;
        case 1:
          Matrix_RotateX(-frame * SWIRLING_STAR_SPIN(i));
          break;
        case 2:
          Matrix_RotateZ(-frame * SWIRLING_STAR_SPIN(i));
          break;
        case 3:
          Matrix_RotateX(-frame * SWIRLING_STAR_SPIN(i));
          Matrix_RotateZ(-frame * SWIRLING_STAR_SPIN(i));
          break;
        }
        EffectPosition_ProjectPoint(star, &screen);
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

/// The burst's six pictures in the sheet, after the smoke's six.
#define TARGET_BURST_PICTURES (6 * PICTURE_BYTES(SMOKE_PUFF))

#define TARGET_BURSTS_FRAMES 96

/// Unused: nothing calls it. On each target in turn a burst flares while puffs
/// of smoke scatter around it.
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
  FIXED point[3];

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress(RESOURCE_SMOKE_SHEET, work->sheet, 1, 1);
  Resource_LoadAndDecompress(RESOURCE_TARGET_BURST_IMAGE,
                             work->sheet + TARGET_BURST_PICTURES, 0, 0);
  palette = Resource_GetTableEntry(RESOURCE_PINK_BURST_SHEET);
  Iwram_CopyWordsEntry((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
  drawBurst = (DrawRectangle)Ram_HeapSlots->ramCode;
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
                      BLITTER_MAX);
  drawSmoke = (DrawRectangle)Ram_HeapSlots->ramCodePaired;

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
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    EFFECT_FOR_EACH_TARGET(member, args) {
      struct MotionObject *target;

      target = BattleObject_GetFar(args->targets[member])->object;
      EFFECT_VIEW_FROM_CAMERA(camera);
      SET_VECTOR(point, target->x, toFIXED(20), target->z);
      Matrix_TranslateVector(point);
      if (frame >= member * 8 && frame < member * 8 + 40) {
        for (i = 0; i != 6; i++) {
          s32 picture;

          // A picture every six frames, holding the last.
          puff = &work->particles[member * 6 + i];
          picture = puff->variant / 6;
          if (picture > 5)
            picture = 5;
          EffectPosition_ProjectPoint(puff, &screen);
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
#undef SWIRLING_STAR_SPIN
#undef TARGET_BURST_PICTURES
#undef TARGET_BURSTS_FRAMES
