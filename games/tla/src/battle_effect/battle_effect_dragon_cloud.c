/// @file
/// Dragon Cloud.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "system/io_write.h"

#define DRAGON_CLOUD_FRAMES 150

#define DRAGON_CLOUD_STRIKE_FRAME 83 ///< the flame strikes the first target
#define DRAGON_CLOUD_TRAIL_HEIGHT 39
/// FirePillarSheetB: five flames, then the trail picture.
#define DRAGON_CLOUD_FLAME_WIDTH 24
#define DRAGON_CLOUD_FLAME_HEIGHT 48
#define DRAGON_CLOUD_TRAIL_PICTURE (5 * PICTURE_BYTES(DRAGON_CLOUD_FLAME))
#define DRAGON_CLOUD_PUFF_PICTURES 15360 ///< the puff pictures in the sheet
#define DRAGON_CLOUD_PUFFS 8             ///< puffs, particles 0 to 7

#define DRAGON_CLOUD_TRAIL_WIDTH 24
#define DRAGON_CLOUD_TRAIL_PICTURE_BYTES                                       \
  PICTURE_BYTES(DRAGON_CLOUD_TRAIL) ///< bytes in each trail copy
#define DRAGON_CLOUD_FIRST_EMBER 8  ///< the first ember in work->particles
#define DRAGON_CLOUD_EMBERS 40      ///< embers, from DRAGON_CLOUD_FIRST_EMBER
#define DRAGON_CLOUD_FLASHES 16     ///< flashes, after the embers

/// A flame leaves the user along a stored path with a fading trail behind it,
/// strikes the first target and bursts into embers and flashes.
///
/// Abilities: Dragon Cloud, Epicenter.
void BattleEffect_RunDragonCloud(struct BattleEffectParams *args) {
  struct Point2D trail[20];
  struct ScreenPoint target;
  struct ScreenPoint position;
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
  struct Particle *step;
  s32 i;
  s32 j;
  s32 angle;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  drift = 0;
  x = 0;
  y = 0;
  graphics = Ram_HeapSlots->battleSheet;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = EFFECT_BLEND_ADD;

  // The pillar sheet, and darker copies of the trail picture, for the trail's
  // tail.
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetB, work->sheet, 1,
                             1);
  for (i = 1; i != 20; i++) {
    for (j = 0; j != DRAGON_CLOUD_TRAIL_PICTURE_BYTES; j++) {
      s32 shade = work->sheet[DRAGON_CLOUD_TRAIL_PICTURE + j];

      if (i > 10) {
        shade = shade - i * 4 + 40;
        if (shade < 0)
          shade = 0;
        work->sheet[DRAGON_CLOUD_TRAIL_PICTURE +
                    (i - 10) * DRAGON_CLOUD_TRAIL_PICTURE_BYTES + j] = shade;
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             &work->sheet[DRAGON_CLOUD_PUFF_PICTURES], 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             Ram_MapCellBuffer, 1, 0);
  /* FAKEMATCH(wrapper): the copy routine fetched through Iwram_GetCopyWords. */
  if (args->variant == EFFECT_VARIANT_STRONGER)
    Iwram_GetCopyWords()(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
        SHEET_PALETTE_BYTES);
  BattleEffect_FetchRectangleBlitters(args->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

  // The 56 embers wait at the target: sparks first, then flashes, which rise
  // more slowly.
  object = BattleObject_GetFar(args->targets[0])->object;
  for (i = 0; i != DRAGON_CLOUD_EMBERS + DRAGON_CLOUD_FLASHES; i++) {
    step = &work->particles[DRAGON_CLOUD_FIRST_EMBER + i];
    SET_POSITION(step, object->x.fixed, 0, object->z.fixed);
    if (i >= gDragonCloud_SparkCounts[args->variant]) {
      step->velocityX = ((Random16() & 63) - 32) << 14;
      step->velocityY = (Random16() & 63) << 12;
    } else {
      step->velocityX = ((Random16() & 63) - 32) << 14;
      step->velocityY = (Random16() & 63) << 13;
    }
    Io_Write32(((Random16() & 63) - 32) << 14, &step->velocityZ);
    step->variant = 1;
  }

  // The sparks: all asleep but the first 128, at the target (only variant 1
  // draws them).
  for (i = 0; i != SPARK_COUNT; i++)
    SPARKS[i].variant = -1;
  for (i = 0; i != 128; i++) {
    step = &SPARKS[i];
    SET_POSITION(step, object->x.fixed, 0, object->z.fixed);
    step->velocityX = ((Random16() & 63) - 32) << 14;
    step->velocityY = (Random16() & 63) << 12;
    step->velocityZ = ((Random16() & 63) - 32) << 14;
    step->variant = (Random16() & 15) + 24;
  }

  // The eight puffs, on an arc over the user, each two frames after the last.
  for (i = 0, angle = -DEGtoANG(90); i != DRAGON_CLOUD_PUFFS;
       angle += TRIG_TURN / 16, i++) {
    step = &work->particles[i];
    if (args->side == EFFECT_SIDE_PARTY)
      step->x = (FIXEDtoINT(Trig_Sin(angle) * 24)) + 88;
    else
      step->x = (FIXEDtoINT(-(Trig_Sin(angle) * 24))) + 16;
    Io_Write32((FIXEDtoINT(Trig_Cos(angle) * 16)) + 40, &step->y);
    step->variant = -(i * 2);
  }
  path = Resource_GetTableEntry((s32)&ResourceId_DualStreamPath);

  for (frame = 0; frame != DRAGON_CLOUD_FRAMES; frame++) {
    struct BattleCamera *camera = Ram_HeapSlots->cameraWork;

    if (frame == DRAGON_CLOUD_STRIKE_FRAME) {
      if (args->variant == EFFECT_VARIANT_BASIC)
        EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
      else
        EFFECT_SHOW_DAMAGE(SE_CRASH);
    }
    if (frame == 0)
      Sound_PlayFar(SE_WHOOSH);
    if (frame == 50)
      Sound_PlayFar(SE_WHOOSH);

    // For 64 frames the camera turns away from the user's side.
    if (args->side == EFFECT_SIDE_ENEMIES) {
      if (frame <= 63)
        camera->yaw -= 0x100;
    } else {
      if (frame <= 63)
        camera->yaw += 0x100;
    }
    BattleCamera_SetViewFar(0, 0, 0, BATTLE_ZOOM_NORMAL);

    // Frames 0-17: the puff arc rises, one cell every three frames.
    if (frame <= 17) {
      s32 cell = frame / 3;
      s32 width;
      s32 offset;

      offset = gBattleEffect_PuffArcPictures[cell];
      /* FAKEMATCH(dead): width, read from the puff table and never used. */
      width = gBattleEffect_PuffArcPictureWidths[cell];
      draw[0](canvas, &work->sheet[offset + DRAGON_CLOUD_PUFF_PICTURES],
              56 - gBattleEffect_PuffArcPictureWidths[cell] / 2,
              gBattleEffect_PuffArcPictureBiasY[cell] + 60,
              gBattleEffect_PuffArcPictureWidths[cell],
              gBattleEffect_PuffArcPictureHeights[cell]);
      draw[1](canvas,
              &work->sheet[gBattleEffect_PuffArcPictures[cell] +
                           DRAGON_CLOUD_PUFF_PICTURES],
              64 - gBattleEffect_PuffArcPictureWidths[cell] / 2,
              gBattleEffect_PuffArcPictureBiasY[cell] + 60,
              gBattleEffect_PuffArcPictureWidths[cell],
              gBattleEffect_PuffArcPictureHeights[cell]);
    }

    // Variant 1: 128 sparks thrown up from the user's side at the start.
    if (args->variant == EFFECT_VARIANT_STRONGER) {
      if (frame == 0) {
        for (i = 0; i != 128; i++) {
          s32 angle;
          s32 force;

          step = &SPARKS[128 + i];
          step->x = toFIXED(60);
          step->y = toFIXED(108);
          angle = (Random16() & 0x7fff) + DEGtoANG(90);
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

          draw[i & 1](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                      Q_16_16_UPPER_HALF(step->x) - size / 2,
                      Q_16_16_UPPER_HALF(step->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(step, 62, toFIXED(1) / 4);
          if (step->y > INTtoFIXED((i & 3) + 108))
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
      EffectPosition_ProjectBattler(args->targets[0], &target);
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
          draw[0](canvas,
                  &work->sheet[DRAGON_CLOUD_TRAIL_PICTURE +
                               (j - 10) * DRAGON_CLOUD_TRAIL_PICTURE_BYTES],
                  trail[j].x - 12, trail[j].y, DRAGON_CLOUD_TRAIL_WIDTH,
                  DRAGON_CLOUD_TRAIL_HEIGHT);
        else
          draw[0](canvas, &work->sheet[DRAGON_CLOUD_TRAIL_PICTURE],
                  trail[j].x - 12, trail[j].y, DRAGON_CLOUD_TRAIL_WIDTH,
                  DRAGON_CLOUD_TRAIL_HEIGHT);
      }
    }
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);

    // Frames 18-83: the head, tilted by how fast it climbs or falls.
    if (frame >= 18 && frame <= DRAGON_CLOUD_STRIKE_FRAME) {
      s32 lift;

      if (frame >= 78 && frame <= 118)
        position.x = x;
      else if (args->side == EFFECT_SIDE_PARTY)
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
      draw[0](canvas, &work->sheet[lift * PICTURE_BYTES(DRAGON_CLOUD_FLAME)],
              trail[0].x - 12, trail[0].y - 2, DRAGON_CLOUD_FLAME_WIDTH,
              DRAGON_CLOUD_FLAME_HEIGHT);

      // The head sheds four sparks a frame (only variant 1 draws them).
      for (i = 0; i != 4; i++) {
        s32 angle;
        s32 force;
        s32 index;

        index = ((frame * 4 + i) & 127) + 256;
        step = &SPARKS[index];
        step->x = INTtoFIXED(position.x);
        step->y = INTtoFIXED(position.y);
        angle = Random16();
        force = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * force) >> 7;
        step->velocityY = (Trig_Cos(angle) * force) >> 5;
        step->variant = (Random16() & 15) + 24;
      }
    }

    // After the strike: the embers fly from the target, sized by depth;
    // the flashes play the flash sheet.
    if (frame >= DRAGON_CLOUD_STRIKE_FRAME) {
      for (i = 0; i != DRAGON_CLOUD_EMBERS + DRAGON_CLOUD_FLASHES; i++) {
        step = &work->particles[DRAGON_CLOUD_FIRST_EMBER + i];
        if (step->y >= 0) {
          s32 size;

          EffectPosition_ProjectPoint(&step->x, &position);
          position.x >>= 1;
          if (position.depth <= 159)
            position.depth = 160;
          if (position.depth > 799)
            position.depth = 799;
          size = 9 - (position.depth - 160) / 64;
          if (i >= gDragonCloud_SparkCounts[args->variant]) {
            s32 cell;

            if (args->variant == EFFECT_VARIANT_BASIC)
              cell = step->variant / 2;
            else
              cell = step->variant / 4;
            if (cell <= 5) {
              draw[args->variant](canvas, Ram_MapCellBuffer + (cell << 11),
                                  position.x - 16, position.y - 40,
                                  FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
              step->variant++;
              BattleEffectKernels_IntegrateVector3(step, 62, toFIXED(1) / 8);
            }
          } else {
            draw[0](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
            BattleEffectKernels_IntegrateVector3(step, PARTICLE_DAMPING_NONE,
                                                 -toFIXED(1) / 8);
          }
        }
      }

      // Variant 1's 128 sparks at the target.
      if (args->variant == EFFECT_VARIANT_STRONGER) {
        for (i = 0; i != 128; i++) {
          s32 size;

          step = &SPARKS[i];
          size = step->variant;
          if (size >= 0) {
            EffectPosition_ProjectPoint(&step->x, &position);
            size >>= 3;
            size += 1;
            position.x >>= 1;
            draw[i & 1](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                        position.x - size / 2, position.y - size, size,
                        size * 2);
            BattleEffectKernels_IntegrateVector3(step, 62, toFIXED(1) / 8);
            step->variant--;
          }
        }
      }
    }

    // Variant 1: the sparks the head shed.
    if (args->variant == EFFECT_VARIANT_STRONGER) {
      struct Particle *spark = &SPARKS[256];

      for (i = 0; i != 128; i++, spark++) {
        if (spark->variant >= 0) {
          s32 size = (spark->variant >> 4) + 1;

          draw[i & 1](canvas, graphics + gBattleEffect_MotePictures[size - 1],
                      Q_16_16_UPPER_HALF(spark->x) - size / 2,
                      Q_16_16_UPPER_HALF(spark->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(spark, 60, -toFIXED(1) / 4);
          spark->variant--;
        }
      }
    }

    // Frame 83: the strike. Frame 50: the puffs burst.
    if (frame == DRAGON_CLOUD_STRIKE_FRAME) {
      work->shakeFrames = 8;
      EFFECT_STRIKE_TARGET(args, 0, 8, KNOCKBACK_HOP);
    }
    if (frame == 50) {
      work->shakeFrames = 12;
      BattleEffect_TintBattler(args->targets[0], SPRITE_COLOR_WHITE,
                               POSE_DAMAGE, 0, 8);
    }
    if (frame > 49) {
      for (i = 0; i != DRAGON_CLOUD_PUFFS; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && step->variant <= 11) {
          s32 cell = step->variant / 2;

          draw[1](canvas,
                  &work->sheet[gBattleEffect_PuffArcPictures[cell] +
                               DRAGON_CLOUD_PUFF_PICTURES],
                  step->x - gBattleEffect_PuffArcPictureWidths[cell] / 2,
                  step->y + gBattleEffect_PuffArcPictureBiasY[cell],
                  gBattleEffect_PuffArcPictureWidths[cell],
                  gBattleEffect_PuffArcPictureHeights[cell]);
        }
        step->variant++;
      }
    }
    BattleCamera_Shake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}
#undef BLITTER
#undef SPARKS
#undef SPARK_COUNT
