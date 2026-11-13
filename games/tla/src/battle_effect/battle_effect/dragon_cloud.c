/// @file
/// Dragon Cloud.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

// Dragon Cloud (effect kind 49): a puff arc rises over the user, then a
// dragon's head flies the path from frame 18, trailing fading copies of
// itself, while the camera turns; from frame 78 it dives onto the target.
// The puffs burst at frame 50 and the target flashes; at frame 83 the head
// strikes, the target reels, and 56 embers fly from it: sparks, then flashes.
// Variant 1, ⚓️'s own, loads orange colours, throws 128 sparks up from the
// user's side at the start, sheds four sparks a frame from the head and
// throws 128 more from the target with the embers.
void BattleEffect_RunDragonCloud(struct BattleEffectArgument *effect) {
  struct Point2D trail[20];
  struct EffectPosition target;
  struct EffectPosition position;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 drift;
  s8 *path;
  s32 x;
  s32 y;
  u8 *graphics;
  struct Actor *object;
  struct EffectStep *step;
  s32 i;
  s32 j;
  s32 angle;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  drift = 0;
  x = 0;
  y = 0;
  graphics = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;

  // The pillar sheet, and nine copies of its cell, each darker than the last,
  // for the trail.
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetB, work->sheet, 1,
                             1);
  for (i = 1; i != 20; i++) {
    for (j = 0; j != PILLAR_CELL_SIZE; j++) {
      s32 shade = work->sheet[0x1680 + j];

      if (i > 10) {
        shade = shade - i * 4 + 40;
        if (shade < 0)
          shade = 0;
        work->sheet[0x1680 + (i - 10) * PILLAR_CELL_SIZE + j] = shade;
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             &work->sheet[0x3c00], 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             Ram_MapCellBuffer, 1, 0);
  /* FAKEMATCH(wrapper): the copy routine fetched through Iwram_GetCopyWords. */
  if (effect->variant == 1)
    Iwram_GetCopyWords()(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The 56 embers wait at the target: sparks first, then flashes, which rise
  // more slowly.
  object = BattleObject_GetFar(effect->actors[0])->object;
  for (i = 0; i != 56; i++) {
    step = &work->particles[8 + i];
    SET_POSITION(step, object->x, 0, object->z);
    if (i >= Data_08199d5c[effect->variant]) {
      step->velocityX = ((Random16() & 63) - 32) << 14;
      step->velocityY = (Random16() & 63) << 12;
    } else {
      step->velocityX = ((Random16() & 63) - 32) << 14;
      step->velocityY = (Random16() & 63) << 13;
    }
    step->velocityZ = ((Random16() & 63) - 32) << 14;
    /* FAKEMATCH(dead): an empty once-only block. */
    do {
    } while (0);
    step->variant = 1;
  }

  // The sparks: all asleep but the first 128, at the target (only variant 1
  // draws them).
  for (i = 0; i != SPARK_COUNT; i++)
    SPARKS[i].variant = -1;
  for (i = 0; i != 128; i++) {
    step = &SPARKS[i];
    SET_POSITION(step, object->x, 0, object->z);
    step->velocityX = ((Random16() & 63) - 32) << 14;
    step->velocityY = (Random16() & 63) << 12;
    step->velocityZ = ((Random16() & 63) - 32) << 14;
    step->variant = (Random16() & 15) + 24;
  }

  // The eight puffs, on an arc over the user, each two frames after the last.
  for (i = 0, angle = -TRIG_TURN / 4; i != 8; angle += TRIG_TURN / 16, i++) {
    step = &work->particles[i];
    if (effect->side == 1)
      step->x = (Trig_Sin(angle) * 24 >> 16) + 88;
    else
      step->x = (-(Trig_Sin(angle) * 24) >> 16) + 16;
    step->y = (Trig_Cos(angle) * 16 >> 16) + 40;
    /* FAKEMATCH(dead): an empty once-only block. */
    do {
    } while (0);
    step->variant = -(i * 2);
  }
  path = Resource_GetTableEntry((s32)&ResourceId_DualStreamPath);

  for (frame = 0; frame != 150; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == 83) {
      if (effect->variant == 0)
        BattlePlayback_StartFar(134);
      else
        BattlePlayback_StartFar(145);
    }
    if (frame == 0)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 50)
      Sound_PlayFar(SE_WHOOSH);

    // For 64 frames the camera turns away from the user's side.
    if (effect->side == 0) {
      if (frame <= 63)
        camera->yaw -= 0x100;
    } else {
      if (frame <= 63)
        camera->yaw += 0x100;
    }
    BattleCamera_SetViewFar(0, 0, 0, 100);

    // Frames 0-17: the puff arc rises, one cell every three frames.
    if (frame <= 17) {
      s32 cell = frame / 3;
      s32 width;
      s32 offset;

      offset = gBattleEffect_PuffArcCells[cell];
      /* FAKEMATCH(dead): width, read from the puff table and never used. */
      width = gData_08197467[cell];
      draw[0](canvas, &work->sheet[offset + 0x3c00],
              56 - gData_08197467[cell] / 2,
              gBattleEffect_PuffArcCellBiasY[cell] + 60, gData_08197467[cell],
              gBattleEffect_PuffArcCellHeights[cell]);
      draw[1](canvas, &work->sheet[gBattleEffect_PuffArcCells[cell] + 0x3c00],
              64 - gData_08197467[cell] / 2,
              gBattleEffect_PuffArcCellBiasY[cell] + 60, gData_08197467[cell],
              gBattleEffect_PuffArcCellHeights[cell]);
    }

    // Variant 1: 128 sparks thrown up from the user's side at the start.
    if (effect->variant == 1) {
      if (frame == 0) {
        for (i = 0; i != 128; i++) {
          s32 angle;
          s32 force;

          step = &SPARKS[128 + i];
          step->x = px(60);
          step->y = px(108);
          angle = (Random16() & 0x7fff) + TRIG_TURN / 4;
          force = (Random16() & 255) + 127;
          step->velocityX = (Trig_Sin(angle) * force) >> 7;
          step->velocityY = (Trig_Cos(angle) * force) >> 5;
          step->variant = (Random16() & 15) + 24;
        }
      }
      for (i = 0; i != 64; i++) {
        step = &SPARKS[128 + i];
        if (step->variant >= 0 && frame >= i / 8) {
          s32 size = (step->variant >> 4) + 1;

          draw[i & 1](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                      HI(step->x) - size / 2, HI(step->y) - size, size,
                      size * 2);
          BattleEffectKernels_IntegrateVector2(step, 62, 0x4000);
          if (step->y > ((i & 3) + 108) << 16)
            step->velocityY = -step->velocityY / 2;
          step->variant--;
        }
      }
    }

    // Frames 18-58: the head flies the path, a 16-bit start and then
    // steps; from frame 78 it dives from above the target.
    if (frame >= 18 && frame <= 58) {
      if (frame == 18) {
        x = (path[0] << 8) + (u8)path[1];
        y = (path[2] << 8) + (u8)path[3] + 16;
        path += 4;
      } else {
        x += path[0];
        y += path[1];
        path += 2;
      }
    }
    if (frame >= 78 && frame <= 118) {
      EffectPosition_ProjectBattler(effect->actors[0], &target);
      if (frame == 78) {
        x = target.x / 2;
        y = 48;
      } else {
        y -= 16;
      }
    }

    // The trail: each copy takes the place of the one before, the older
    // ones darker.
    for (j = 19; j != 0; j--) {
      if (frame > j + 18 && frame <= j + 83) {
        trail[j].x = trail[j - 1].x;
        trail[j].y = trail[j - 1].y;
        if (j > 10)
          draw[0](canvas, &work->sheet[0x1680 + (j - 10) * PILLAR_CELL_SIZE],
                  trail[j].x - 12, trail[j].y, 24, 39);
        else
          draw[0](canvas, &work->sheet[0x1680], trail[j].x - 12, trail[j].y, 24,
                  39);
      }
    }
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);

    // Frames 18-83: the head, tilted by how fast it climbs or falls.
    if (frame >= 18 && frame <= 83) {
      s32 lift;

      if (frame >= 78 && frame <= 118)
        position.x = x;
      else if (effect->side == 1)
        position.x = 64 - x / 2;
      else
        position.x = x / 2 + 64;
      position.y = 60 - y;
      lift = (position.y - trail[0].y - 24) / 2;
      if (lift > 2)
        lift = 2;
      if (lift < -2)
        lift = -2;
      drift += lift;
      if (drift > 8)
        drift = 8;
      if (drift < -8)
        drift = -8;
      lift = drift / 4 + 2;
      trail[0].x = position.x;
      trail[0].y = position.y - 20;
      draw[0](canvas, &work->sheet[lift * 0x480], trail[0].x - 12,
              trail[0].y - 2, 24, 48);

      // The head sheds four sparks a frame (only variant 1 draws them).
      for (i = 0; i != 4; i++) {
        s32 angle;
        s32 force;
        s32 index;

        index = ((frame * 4 + i) & 127) + 256;
        step = &SPARKS[index];
        step->x = position.x << 16;
        step->y = position.y << 16;
        angle = Random16();
        force = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * force) >> 7;
        step->velocityY = (Trig_Cos(angle) * force) >> 5;
        step->variant = (Random16() & 15) + 24;
      }
    }

    // After the strike: the embers fly from the target, sized by depth;
    // the flashes play the flash sheet.
    if (frame > 82) {
      for (i = 0; i != 56; i++) {
        step = &work->particles[8 + i];
        if (step->y >= 0) {
          s32 size;

          EffectPosition_ProjectPoint(&step->x, &position);
          position.x >>= 1;
          if (position.depth <= 159)
            position.depth = 160;
          if (position.depth > 799)
            position.depth = 799;
          size = 9 - (position.depth - 160) / 64;
          if (i >= Data_08199d5c[effect->variant]) {
            s32 cell;

            if (effect->variant == 0)
              cell = step->variant / 2;
            else
              cell = step->variant / 4;
            if (cell <= 5) {
              draw[effect->variant](canvas, Ram_MapCellBuffer + (cell << 11),
                                    position.x - 16, position.y - 40, 32, 64);
              step->variant++;
              BattleEffectKernels_IntegrateVector3(step, 62, 0x2000);
            }
          } else {
            draw[0](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
            BattleEffectKernels_IntegrateVector3(step, 64, -0x2000);
          }
        }
      }

      // Variant 1's 128 sparks at the target.
      if (effect->variant == 1) {
        for (i = 0; i != 128; i++) {
          s32 size;

          step = &SPARKS[i];
          size = step->variant;
          if (size >= 0) {
            EffectPosition_ProjectPoint(&step->x, &position);
            size >>= 3;
            size += 1;
            position.x >>= 1;
            draw[i & 1](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                        position.x - size / 2, position.y - size, size,
                        size * 2);
            BattleEffectKernels_IntegrateVector3(step, 62, 0x2000);
            step->variant--;
          }
        }
      }
    }

    // Variant 1: the sparks the head shed.
    if (effect->variant == 1) {
      struct EffectStep *spark = &SPARKS[256];

      for (i = 0; i != 128; i++, spark++) {
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 4) + 1;

          draw[i & 1](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                      HI(spark->x) - size / 2, HI(spark->y) - size, size,
                      size * 2);
          BattleEffectKernels_IntegrateVector2(spark, 60, -0x4000);
          spark->variant--;
        }
      }
    }

    // Frame 83: the strike. Frame 50: the puffs burst.
    if (frame == 83) {
      work->shakeFrames = 8;
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 1);
    }
    if (frame == 50) {
      work->shakeFrames = 12;
      BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
    }
    if (frame > 49) {
      for (i = 0; i != 8; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && step->variant <= 11) {
          s32 cell = step->variant / 2;

          draw[1](canvas,
                  &work->sheet[gBattleEffect_PuffArcCells[cell] + 0x3c00],
                  step->x - gData_08197467[cell] / 2,
                  step->y + gBattleEffect_PuffArcCellBiasY[cell],
                  gData_08197467[cell], gBattleEffect_PuffArcCellHeights[cell]);
        }
        step->variant++;
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
#undef HEAP_SLOT_BLITTER
#undef BLITTER
#undef HI
#undef HEAP_SLOT_BLITTER_ALTERNATE
#undef SPARKS
#undef SPARK_COUNT
