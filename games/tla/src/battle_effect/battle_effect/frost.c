/// @file
/// Frost.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "sounds.h"

#define CHIPS ((struct EffectStep *)Ram_MapCellBuffer)

#define TIMING(field) gFrost_Timings[effect->variant * 3 + (field)]

// Motes swirl up around the targets while ice pillars rise one by one;
// then everything bursts.

// Setup: each mote gets an angle, a radius of 56-119 and a height of -64 to
// -33 pixels. Frame loop: until the burst the motes close in and climb five
// pixels a frame while they turn, and from frame 36 a pillar rises every
// three frames. At the burst the motes spread and fall, 32 chips leap up
// from below, and smoke rises half a pixel a frame through the height the
// motes and chips cover. The variant chooses the number of motes and
// pillars and the frame of the burst.
void BattleEffect_RunFrost(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *dots;
  DrawRectangle *draw;
  s32 count;
  s32 ymin;
  s32 ymax;
  s32 xyz[3];
  struct EffectPosition screen;
  s32 point[3];
  DrawRectangle pair[2];
  struct EffectStep *mote;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  dots = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
  BattleEffect_FetchRectangleBlitters(0, draw = pair);

  // A mote: x holds its angle, z its radius, y its height.
  for (i = 0; i != 64; i++) {
    struct EffectStep *seed = &work->particles[i];

    seed->x = Random16() & 0xffff;
    seed->z = (Random16() & 63) + 56;
    seed->y = ((Random16() & 31) - 64) << 16;
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (effect->side == 1)
    REG_BG2X = -0x7000;

  for (frame = 0; frame != TIMING(RISE_FRAME) + 50; frame++) {
    ymin = 120 << 16;
    ymax = 0;
    if (frame == TIMING(RISE_FRAME) + 11)
      BattlePlayback_StartFar(133);
    point[0] = 0;
    point[1] = 0;
    point[2] = 512 << 16;
    Transform_Reset();
    SceneTransform_ApplyPosition(point);
    if (frame >= 36 && frame < 64 && (frame & 3) == 0)
      Sound_PlayFar(SE_ICE);
    if (frame == 85)
      Sound_PlayFar(SE_WHOOSH);
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 4 + 40)
        BattleEffect_TintBattler(effect->actors[i], 9, 5, -1, 0);
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
          if (mote->y < (48 - i / 2) << 16 && mote->y > -(48 << 16)) {
            struct EffectPosition *pos;
            u32 width;
            u32 height;

            xyz[0] = mote->z * Trig_Sin(mote->x);
            xyz[1] = mote->y;
            xyz[2] = mote->z * Trig_Cos(mote->x);
            EffectPosition_ProjectPoint(xyz, pos = &screen);
            pos->x = (pos->x >> 17) + 64;
            pos->y = HI(pos->y) + 60;
            draw[1](canvas, work->sheet + gFrost_MoteCells[cel],
                    pos->x - ((width = gFrost_MoteWidths[cel]) >> 1),
                    pos->y - ((height = gFrost_MoteHeights[cel]) >> 1), width,
                    height);
          }
          if (frame < TIMING(RISE_FRAME)) {
            if (frame > i + 16) {
              if (mote->z > 4)
                mote->z -= 2;
              if (mote->y < 48 << 16)
                mote->y += 5 << 16;
              mote->x += 0x200;
            }
          } else {
            mote->z += 8;
            mote->y -= (i % 5 + 2) << 16;
            if (ymin > mote->y)
              ymin = mote->y;
            if (ymax < mote->y)
              ymax = mote->y;
          }
        }
      }
    }
    ymin += 64 << 16;
    ymax += 64 << 16;

    // The pillars, 32 pixels wide: one more every three frames from frame
    // 36, drawn MAX for the last seven frames before the burst.
    if (frame < TIMING(RISE_FRAME)) {
      for (i = 0; i != TIMING(RISE_COLUMNS); i++) {
        if (i < (frame - 36) / 3) {
          s32 cel = i % 3;
          u32 height;

          if (frame >= TIMING(RISE_FRAME) - 7) {
            draw[1](canvas, work->sheet + gFrost_ColumnCells[cel],
                    gFrost_ColumnSpots[i * 2],
                    gFrost_ColumnSpots[i * 2 + 1] -
                        (height = gFrost_ColumnHeights[cel]),
                    32, height);
          } else {
            pair[0](canvas, work->sheet + gFrost_ColumnCells[cel],
                    gFrost_ColumnSpots[i * 2],
                    gFrost_ColumnSpots[i * 2 + 1] -
                        (height = gFrost_ColumnHeights[cel]),
                    32, height);
          }
        }
      }
    }

    // The chips: at the burst 32 leap up from below (24 are drawn).
    if (frame == TIMING(RISE_FRAME)) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *chip = &CHIPS[i];

        chip->x = (Random16() & 127) << 16;
        chip->y = ((Random16() & 15) + 80) << 16;
        chip->z = ((Random16() & 63) - 32) << 12;
        chip->velocityY = ((-Random16() & 15) - 16) << 13;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (frame >= TIMING(RISE_FRAME)) {
      for (i = 0; i != 24; i++) {
        struct EffectStep *chip = &CHIPS[i];

        if (chip->variant >= 0) {
          s32 cel = i % 8;

          draw[1](canvas, work->sheet + gFrost_ChipCells[cel], HI(chip->x),
                  HI(chip->y), gFrost_ChipWidths[cel], gFrost_ChipHeights[cel]);
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
        struct EffectStep *puff = &work->particles[i];

        puff->velocityX = (Random16() & 127) << 16;
        if (ymax == ymin)
          puff->velocityY = ymin << 16;
        else
          puff->velocityY = (Random16() % (ymax - ymin) + ymin) << 16;
        puff->variant = (Random16() & 15) + 20;
      }
    }
    if (frame >= TIMING(RISE_FRAME)) {
      s32 rise = (frame - TIMING(RISE_FRAME)) / 2;

      for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (puff->variant >= 0 && puff->variant <= 17) {
          s32 cel = (17 - puff->variant) / 2;
          u32 size;

          draw[1](canvas, dots + gBattleEffect_PuffCells[cel],
                  HI(puff->velocityX) -
                      ((size = gBattleEffect_PuffSizes[cel]) >> 1),
                  HI(puff->velocityY) - (size >> 1) - rise, size, size);
        }
        puff->variant--;
        if ((puff->variant == -1 || puff->variant == 17) &&
            frame < TIMING(RISE_FRAME) + 35) {
          puff->variant = 17;
          puff->velocityX = (Random16() & 127) << 16;
          puff->velocityY = (Random16() % (ymax - ymin) + ymin) << 16;
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
