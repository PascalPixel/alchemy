/// @file
/// Frost.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"
#include "macros.h"

#define CHIPS ((struct Particle *)Ram_MapCellBuffer)

#define TIMING(field) gFrost_Timings[args->variant * 3 + (field)]

// Motes swirl up around the targets while ice pillars rise one by one;
// then everything bursts.

/// Motes of ice spiral down and in while mounds of ice spring up; at the burst
/// the motes spread and rise, chips fly up and sparkles rise.
///
/// Abilities: Frost, Tundra, Glacier.
void BattleEffect_RunFrost(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *dots;
  DrawRectangle *draw;
  s32 count;
  s32 ymin;
  s32 ymax;
  s32 xyz[3];
  struct ScreenPoint screen;
  s32 point[3];
  DrawRectangle pair[2];
  struct Particle *mote;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  dots = Ram_HeapSlots->battleSheet;
  EFFECT_START_NARROW_CANVAS(1);
  Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
  BattleEffect_FetchRectangleBlitters(0, draw = pair);

  // A mote: x holds its angle, z its radius, y its height.
  for (i = 0; i != 64; i++) {
    struct Particle *seed = &work->particles[i];

    seed->x = Random16() & 0xffff;
    seed->z = (Random16() & 63) + 56;
    seed->y = px((Random16() & 31) - 64);
  }

  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
  if (args->side == EFFECT_SIDE_PARTY)
    REG_BG2X = -0x7000;

  for (frame = 0; frame != TIMING(RISE_FRAME) + 50; frame++) {
    ymin = px(120);
    ymax = 0;
    if (frame == TIMING(RISE_FRAME) + 11)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);
    point[0] = 0;
    point[1] = 0;
    point[2] = px(512);
    Transform_Reset();
    SceneTransform_ApplyPosition(point);
    if (frame >= 36 && frame < 64 && (frame & 3) == 0)
      Sound_PlayFar(SE_ICE);
    if (frame == 85)
      Sound_PlayFar(SE_WHOOSH);
    for (i = 0; i != args->targetCount; i++) {
      if (frame == i * 4 + 40)
        BattleEffect_TintBattler(args->targets[i], SPRITE_COLOR_BLUE_TINT,
                                 POSE_DAMAGE, TINT_KEEP, 0);
    }

    // The motes: before the burst they close in and climb to 48 pixels as
    // they turn; after it they spread and fall, and their heights bound the
    // smoke.
    if (frame < TIMING(RISE_FRAME))
      count = TIMING(RISE_MOTES);
    else
      count = 16;
    if (frame < TIMING(RISE_FRAME) + 35) {
      for (i = 0; i != count; i++) {
        if (frame > i) {
          s32 cel = i % 8;

          mote = &work->particles[i];
          if (mote->y < px(48 - i / 2) && mote->y > -px(48)) {
            struct ScreenPoint *pos;
            u32 width;
            u32 height;

            xyz[0] = mote->z * Trig_Sin(mote->x);
            xyz[1] = mote->y;
            xyz[2] = mote->z * Trig_Cos(mote->x);
            EffectPosition_ProjectPoint(xyz, pos = &screen);
            pos->x = (pos->x >> 17) + 64;
            pos->y = Q_16_16_UPPER_HALF(pos->y) + 60;
            draw[1](canvas, work->sheet + gFrost_MoteCells[cel],
                    pos->x - ((width = gFrost_MoteWidths[cel]) >> 1),
                    pos->y - ((height = gFrost_MoteHeights[cel]) >> 1), width,
                    height);
          }
          if (frame < TIMING(RISE_FRAME)) {
            if (frame > i + 16) {
              if (mote->z > 4)
                mote->z -= 2;
              if (mote->y < px(48))
                mote->y += px(5);
              mote->x += 0x200;
            }
          } else {
            mote->z += 8;
            mote->y -= px(i % 5 + 2);
            if (ymin > mote->y)
              ymin = mote->y;
            if (ymax < mote->y)
              ymax = mote->y;
          }
        }
      }
    }
    ymin += px(64);
    ymax += px(64);

    // The pillars, 32 pixels wide: one more every three frames from frame
    // 36, drawn MAX for the last seven frames before the burst.
    if (frame < TIMING(RISE_FRAME)) {
      for (i = 0; i != TIMING(RISE_COLUMNS); i++) {
        if (i < (frame - 36) / 3) {
          s32 cel = i % 3;
          u32 height;

          if (frame >= TIMING(RISE_FRAME) - 7) {
            draw[1](canvas, work->sheet + gFrost_MoundPictures[cel],
                    gFrost_MoundSpots[i * 2],
                    gFrost_MoundSpots[i * 2 + 1] -
                        (height = gFrost_MoundHeights[cel]),
                    32, height);
          } else {
            pair[0](canvas, work->sheet + gFrost_MoundPictures[cel],
                    gFrost_MoundSpots[i * 2],
                    gFrost_MoundSpots[i * 2 + 1] -
                        (height = gFrost_MoundHeights[cel]),
                    32, height);
          }
        }
      }
    }

    // The chips: at the burst 32 leap up from below (24 are drawn).
    if (frame == TIMING(RISE_FRAME)) {
      for (i = 0; i != 32; i++) {
        struct Particle *chip = &CHIPS[i];

        SET_POSITION(chip, px(Random16() & 127), px((Random16() & 15) + 80),
                     ((Random16() & 63) - 32) << 12);
        chip->velocityY = ((-Random16() & 15) - 16) << 13;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (frame >= TIMING(RISE_FRAME)) {
      for (i = 0; i != 24; i++) {
        struct Particle *chip = &CHIPS[i];

        if (chip->variant >= 0) {
          s32 cel = i % 8;

          draw[1](canvas, work->sheet + gFrost_ChipCells[cel],
                  Q_16_16_UPPER_HALF(chip->x), Q_16_16_UPPER_HALF(chip->y),
                  gFrost_ChipWidths[cel], gFrost_ChipHeights[cel]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->variant--;
        }
        if (ymin > chip->y)
          ymin = chip->y;
        if (ymax < chip->y)
          ymax = chip->y;
      }
    }
    ymin >>= 16;
    ymax >>= 16;
    if (ymax <= ymin)
      ymax = ymin + 1;

    // The smoke: from the burst it fills the height of the motes and chips
    // and rises half a pixel a frame.
    if (frame == TIMING(RISE_FRAME)) {
      for (i = 0; i != 32; i++) {
        struct Particle *puff = &work->particles[i];

        puff->velocityX = px(Random16() & 127);
        if (ymax == ymin)
          puff->velocityY = px(ymin);
        else
          puff->velocityY = px(Random16() % (ymax - ymin) + ymin);
        puff->variant = (Random16() & 15) + 20;
      }
    }
    if (frame >= TIMING(RISE_FRAME)) {
      s32 rise = (frame - TIMING(RISE_FRAME)) / 2;

      for (i = 0; i != 32; i++) {
        struct Particle *puff = &work->particles[i];

        if (puff->variant >= 0 && puff->variant <= 17) {
          s32 cel = (17 - puff->variant) / 2;
          u32 size;

          draw[1](canvas, dots + gBattleEffect_SparklePictures[cel],
                  Q_16_16_UPPER_HALF(puff->velocityX) -
                      ((size = gBattleEffect_SparkleSizes[cel]) >> 1),
                  Q_16_16_UPPER_HALF(puff->velocityY) - (size >> 1) - rise,
                  size, size);
        }
        puff->variant--;
        if ((puff->variant == -1 || puff->variant == 17) &&
            frame < TIMING(RISE_FRAME) + 35) {
          puff->variant = 17;
          puff->velocityX = px(Random16() & 127);
          puff->velocityY = px(Random16() % (ymax - ymin) + ymin);
        }
      }
    }
    EFFECT_PRESENT_CANVAS(work);
  }

  EFFECT_RELEASE_BLITTERS();
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
