/// @file
/// Quake.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

// Battle effect kind 1, the quake: rock pillars rise under the targets. The
// Broken Seal's BattleEffect_RunQuake, call for call.

#define PILLAR_W 40        // pixels
#define ROCK_OFFSET 0x1e00 // the rock picture in the sheet, after the band

// The byte of pixel (x, y) in a 32-tile-wide 8bpp character block.
#define BG_DOT(x, y)                                                           \
  (((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

/// Three rock pillars rise from the ground, left to right four frames apart,
/// and sink again; targets standing on one are thrown into the air.
///
/// Setup: the band of background under the targets (x 32-71, y 96-111) is
/// cut out of BG character memory to cap each pillar. Frame loop: each
/// pillar's height follows a sine over its rise time, a target over a
/// pillar is launched upward, and a target that lands flashes; the camera
/// shakes all the while. The variant chooses the length, the rise time and
/// the shake.
void BattleEffect_RunQuake(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 column;
  DrawRectangle draw;
  struct BattleCamera *camera;
  s32 xOffset;
  struct RockWallHeights heights;
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 row;
  s32 col;
  s32 y;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100; // no horizontal stretch
  REG_BLDCNT = 0;    // no blending
  Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet,
                             work->sheet + ROCK_OFFSET, 1, 1);

  // Copy the background's characters and cut the band out of them.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)0x06008000, 0x8000);
  for (row = 0; row != 16; row++) {
    y = row + 96;
    for (col = 0; col != PILLAR_W; col++) {
      s32 pixel = BG_DOT(col + 32, y);

      work->sheet[row * PILLAR_W + col] = Ram_MapCellBuffer[pixel];
    }
  }

  // On the far side the canvas moves 112 pixels right, and the targets'
  // screen positions with it.
  if (effect->side == 1) {
    REG_BG2X = -(112 << 8);
    xOffset = -112;
  } else {
    xOffset = 0;
  }
  BattleEffect_LoadBlitter(
      104, BLIT_MODE(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  for (member = 0; member != effect->count; member++)
    work->particles[member].variant = 0; // 1 once landed
  work->transferMode = 1;                // clear the canvas on each transfer
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  heights = gRockWall_Heights;
  work->shakeFrames = 128;
  Sound_PlayFar(141);

  for (frame = 0; frame != gQuake_Timings[effect->variant][TIMING_FRAMES];
       frame++) {
    Transform_Reset();
    Transform_SetLookAt(camera->eye, camera->pos);
    // The results begin 16 frames before the end.
    if (frame == gQuake_Timings[effect->variant][TIMING_FRAMES] - 16)
      BattlePlayback_StartFar(133);

    // The pillars rise from frames 16, 20 and 24.
    for (column = 0; column != 3; column++) {
      s32 start = column * 4 + 16;

      if ((frame & 31) + 32 == start) // never true: the left side is >= 32
        heights.column[column] += 32;
      if (frame >= start &&
          frame < start + gQuake_Timings[effect->variant][TIMING_RISE_FRAMES]) {
        s32 height;
        s32 top;

        // Up and down along a sine, 1/64 of a turn a frame.
        height =
            (heights.column[column] * Trig_Sin((frame - start) << 10)) >> 16;
        if (height < 0)
          height = -height;
        top = 112 - height;
        draw(canvas, work->sheet + ROCK_OFFSET, column * PILLAR_W + 8, top,
             PILLAR_W, height);
        draw(canvas, work->sheet, column * PILLAR_W + 8, 96 - height, PILLAR_W,
             16);

        // Launch the targets standing on the pillar.
        for (member = 0; member != effect->count; member++) {
          struct MotionObject *object;

          object = BattleObject_GetFar(effect->actors[member])->object;
          point[0] = object->x;
          point[1] = object->y;
          point[2] = object->z;
          EffectPosition_ProjectPoint(point, &screen);
          screen.x += xOffset;
          if (screen.x >= column * PILLAR_W + 8 &&
              screen.x <= column * PILLAR_W + 8 + PILLAR_W && screen.y >= top) {
            object->velocityY = 12 << 16; // 12 pixels a frame upward
            object->verticalMotionStrength = 0xab85;
          }
          if (object->y < 0)
            BattleEffect_TintBattler(effect->actors[member], 0, 5, -1, 0);
        }
      }
    }

    // A target that lands flashes in palette 7 for 5 frames.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object;
      struct EffectStep *state;

      object = BattleObject_GetFar(effect->actors[member])->object;
      state = &work->particles[member];
      if (state->variant == 0 && object->y <= 0 && object->velocityY < 0) {
        state->variant = 1;
        BattleEffect_TintBattler(effect->actors[member], 7, 5, member, 5);
      }
    }
    BattleCamera_Shake(gQuake_Timings[effect->variant][TIMING_SHAKE],
                       gQuake_Timings[effect->variant][TIMING_SHAKE]);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
