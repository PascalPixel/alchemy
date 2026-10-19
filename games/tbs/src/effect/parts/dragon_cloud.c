/// @file
/// Dragon Cloud.
/// Part of effect.c, the battle effect object, compiled only through it.

#include "../effect_internal.h"

#define DRAGON_CLOUD_FRAMES 150
#define DRAGON_CLOUD_STRIKE_FRAME 83 ///< the flame strikes the first target
#define DRAGON_CLOUD_TRAIL_CELL_BYTES 936 ///< bytes in each trail copy
#define DRAGON_CLOUD_FIRST_EMBER 8      ///< the first ember in work->particles
#define DRAGON_CLOUD_FIRST_FLASH 48     ///< the first flash in work->particles
#define DRAGON_CLOUD_TRAIL_CELLS 0x1680 ///< the trail picture in the sheet
#define DRAGON_CLOUD_PUFF_CELLS 0x3c00  ///< the puff pictures in the sheet
#define DRAGON_CLOUD_PUFFS 8            ///< puffs, particles 0 to 7
#define DRAGON_CLOUD_EMBERS 40          ///< embers, from FIRST_EMBER
#define DRAGON_CLOUD_FLASHES 16         ///< flashes, from FIRST_FLASH

/// Draws puff picture cell of sheet on canvas at (x, y) with blit, sized by
/// the puff tables.
#define DRAGON_CLOUD_DRAW_PUFF(blit, canvas, sheet, cell, x, y)                \
  blit(canvas,                                                                 \
       &(sheet)[DRAGON_CLOUD_PUFF_CELLS] + gBattleFx_PuffArcCells[cell], x, y, \
       gBattleFx_PuffArcCellWidths[cell], gBattleFx_PuffArcCellHeights[cell])

/// Abilities: Dragon Cloud (kind 49).
///
/// A flame leaves the user along a stored path with a fading trail of 19
/// copies behind it, then strikes the first target on
/// DRAGON_CLOUD_STRIKE_FRAME and bursts into embers and flashes. The trail's
/// last nine copies are darker versions of the trail picture, each
/// DRAGON_CLOUD_TRAIL_CELL_BYTES long. The camera turns over the first 64 of
/// the DRAGON_CLOUD_FRAMES frames:
///
///   0 to 17    a pair of puffs, one mirrored, bursts in front of the user
///   18 to 58   the flame follows its stored path
///   50         the first target flashes, the screen shakes and eight puffs
///              rise in an arc
///   78 to 83   the flame dives in again
///   83         the target is struck; embers and flashes burst from it
void BattleFx_RunDragonCloud(struct BattleEffectArgument *effect) {
  struct Point2D trail[20];
  struct EffectPosition position;
  DrawRectangle draw[2];
  void **slots;
  void **cursor;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 drift;
  s8 *path;
  s32 x;
  s32 y;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 i;
  s32 j;

  EFFECT_GET_SLOTS(slots, cursor, work, canvas);
  drift = 0;
  x = 0;
  y = 0;
  sheet = EFFECT_SLOT(slots, HEAP_SLOT_BATTLE_SHEET);
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = BLEND_ALPHA(16, 16);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetB, work->sheet, 1,
                             1);
  // Make nine darker copies after the trail picture, for trail copies 11 to
  // 19, each four shades darker than the last.
  for (i = 1; i != 20; i++) {
    for (j = 0; j != DRAGON_CLOUD_TRAIL_CELL_BYTES; j++) {
      s32 shade = work->sheet[DRAGON_CLOUD_TRAIL_CELLS + j];

      if (i > 10) {
        shade = shade - i * 4 + 40;
        if (shade < 0)
          shade = 0;
        work->sheet[DRAGON_CLOUD_TRAIL_CELLS +
                    (i - 10) * DRAGON_CLOUD_TRAIL_CELL_BYTES + j] = shade;
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             &work->sheet[DRAGON_CLOUD_PUFF_CELLS], 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             Ram_MapCellBuffer, 1, 0);
  BattleFx_FetchRectangleBlitters(work->effect->side, draw);
  EFFECT_START_TRANSFER(work, CANVAS_COPY_AND_FADE, 75);

  // Seed the embers (particles 8 to 47) and flashes (48 to 63) thrown at the
  // strike, and the eight puffs (0 to 7) on an arc, each starting two frames
  // after the last.
  for (j = 0; j != DRAGON_CLOUD_EMBERS; j++) {
    struct EffectStep *ember = &work->particles[DRAGON_CLOUD_FIRST_EMBER + j];

    if (work->effect->side == 0)
      ember->x = -0x380000;
    else
      ember->x = 0x380000;
    ember->y = 0;
    ember->z = 0;
    SET_VELOCITY(ember, ((Random16() & 63) - 32) << 14, (Random16() & 63) << 13,
                 ((Random16() & 63) - 32) << 14);
    ember->variant = 1;
  }
  for (j = 0; j != DRAGON_CLOUD_FLASHES; j++) {
    struct EffectStep *flash = &work->particles[DRAGON_CLOUD_FIRST_FLASH + j];

    if (work->effect->side == 0)
      flash->x = -0x380000;
    else
      flash->x = 0x380000;
    flash->y = 0x140000;
    flash->z = 0;
    SET_VELOCITY(flash, ((Random16() & 63) - 32) << 14, (Random16() & 63) << 12,
                 ((Random16() & 63) - 32) << 14);
    flash->variant = 0;
  }
  for (i = 0, j = -0x4000; i != DRAGON_CLOUD_PUFFS; i++, j += 0x1000) {
    struct EffectStep *puff = &work->particles[i];

    if (work->effect->side == 1)
      puff->x = (Trig_Sin(j) * 24 >> 16) + 88;
    else
      puff->x = (-(Trig_Sin(j) * 24) >> 16) + 16;
    puff->y = (Trig_Cos(j) * 16 >> 16) + 40;
    puff->variant = -(i * 2);
  }
  path = Resource_GetTableEntry((s32)&ResourceId_DualStreamPath);

  for (frame = 0; frame != DRAGON_CLOUD_FRAMES; frame++) {
    camera = gCameraWork;
    if (frame == DRAGON_CLOUD_STRIKE_FRAME)
      EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
    if (frame == 0)
      AudioCommand_PlayFar(SE_136);
    if (frame == 50)
      AudioCommand_PlayFar(SE_136);
    if (work->effect->side == 0) {
      if (frame <= 63)
        camera->yaw -= 0x100;
    } else {
      if (frame <= 63)
        camera->yaw += 0x100;
    }
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    if (frame <= 17) {
      s32 cell = frame / 3;

      DRAGON_CLOUD_DRAW_PUFF(draw[0], canvas, work->sheet, cell, 48,
                             gBattleFx_PuffArcCellBiasY[cell] + 60);
      DRAGON_CLOUD_DRAW_PUFF(draw[1], canvas, work->sheet, cell, 56,
                             gBattleFx_PuffArcCellBiasY[cell] + 60);
    }
    // The path gives two 16-bit coordinates on frame 18, then a signed byte
    // step for each axis every frame through frame 58. From frame 78 the
    // flame starts again at (-56, 48) and dives 16 pixels a frame.
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
      if (frame == 78) {
        x = -56;
        y = 48;
      } else {
        y -= 16;
      }
    }
    // Shift the trail back by one and draw it: copy j follows the flame from
    // frame j + 19 to frame j + 83, and copies past the tenth are darker.
    for (j = 19; j != 0; j--) {
      if (frame > j + 18 && frame <= j + 83) {
        trail[j].x = trail[j - 1].x;
        trail[j].y = trail[j - 1].y;
        if (j > 10)
          draw[0](canvas,
                  &work->sheet[DRAGON_CLOUD_TRAIL_CELLS +
                               (j - 10) * DRAGON_CLOUD_TRAIL_CELL_BYTES],
                  trail[j].x, trail[j].y, 24, 39);
        else
          draw[0](canvas, &work->sheet[DRAGON_CLOUD_TRAIL_CELLS], trail[j].x,
                  trail[j].y, 24, 39);
      }
    }
    EFFECT_VIEW_FROM_CAMERA(camera);
    // The flame leans with its smoothed vertical motion (drift, -8 to 8):
    // one of five cells, from rising to falling.
    if (frame >= 18 && frame <= DRAGON_CLOUD_STRIKE_FRAME) {
      s32 lift;

      if (work->effect->side == 1)
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
      trail[0].x = position.x - 12;
      trail[0].y = position.y - 20;
      draw[0](canvas, &work->sheet[lift * 0x480], trail[0].x - 6,
              trail[0].y - 2, 24, 48);
    }
    if (frame == DRAGON_CLOUD_STRIKE_FRAME) {
      work->shakeFrames = 8;
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
      BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
    }
    if (frame > DRAGON_CLOUD_STRIKE_FRAME) {
      // Embers are motes sized by depth. Only records 56 to 63 play the
      // six flash pictures, two frames each; the first eight flashes, at 48
      // to 55, are drawn as motes.
      for (i = 0; i != 56; i++) {
        struct EffectStep *ember =
            &work->particles[DRAGON_CLOUD_FIRST_EMBER + i];

        if (ember->y >= 0) {
          s32 size;

          EffectPosition_ProjectPoint(&ember->x, &position);
          position.x >>= 1;
          if (position.depth <= 159)
            position.depth = 160;
          if (position.depth > 799)
            position.depth = 799;
          size = 9 - (position.depth - 160) / 64;
          if (i > 47) {
            if (ember->variant <= 11) {
              draw[0](canvas, Ram_MapCellBuffer + ember->variant / 2 * 0x800,
                      position.x - 16, position.y - 32, 32, 64);
              ember->variant++;
            }
          } else {
            EFFECT_DRAW_MOTE(draw[0], canvas, sheet, position.x, position.y,
                             size);
          }
          ember->x += ember->velocityX;
          ember->y += ember->velocityY;
          ember->z += ember->velocityZ;
          ember->velocityY -= 0x2000;
        }
      }
    }
    if (frame == 50) {
      work->shakeFrames = 12;
      ObjectGroup_UpdateMembers(work->effect->actors[0], SPRITE_COLOR_WHITE,
                                POSE_DAMAGE, 0, 8);
    }
    // The puffs, mirrored, two frames a cell; each starts when its variant
    // counts up to zero.
    if (frame > 49) {
      for (i = 0; i != DRAGON_CLOUD_PUFFS; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (puff->variant >= 0 && puff->variant <= 11) {
          s32 cell = puff->variant / 2;

          DRAGON_CLOUD_DRAW_PUFF(draw[1], canvas, work->sheet, cell,
                                 puff->x -
                                     gBattleFx_PuffArcCellWidths[cell] / 2,
                                 puff->y + gBattleFx_PuffArcCellBiasY[cell]);
        }
        puff->variant++;
      }
    }
    Camera_ApplyShake(8, 8);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS();
}

#undef DRAGON_CLOUD_FRAMES
#undef DRAGON_CLOUD_STRIKE_FRAME
#undef DRAGON_CLOUD_TRAIL_CELL_BYTES
#undef DRAGON_CLOUD_FIRST_EMBER
#undef DRAGON_CLOUD_FIRST_FLASH
#undef DRAGON_CLOUD_TRAIL_CELLS
#undef DRAGON_CLOUD_PUFF_CELLS
#undef DRAGON_CLOUD_PUFFS
#undef DRAGON_CLOUD_EMBERS
#undef DRAGON_CLOUD_FLASHES
#undef DRAGON_CLOUD_DRAW_PUFF
