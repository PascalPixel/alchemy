/// @file
/// Quake.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"

#define PILLAR_W 40        ///< pixels
#define ROCK_OFFSET 0x1e00 ///< the rock picture in the sheet, after the band

/// The byte of pixel (x, y) in a 32-tile-wide 8bpp character block.
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
void BattleEffect_RunQuake(struct BattleEffectParams *args) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 column;
  DrawRectangle draw;
  struct BattleCamera *camera;
  s32 xOffset;
  struct RockWallHeights heights;
  s32 point[3];
  struct ScreenPoint screen;
  s32 member;
  s32 row;
  s32 col;
  s32 y;

  work = Ram_HeapSlots->battleEffect;
  canvas = Ram_HeapSlots->battleCanvas;
  camera = Ram_HeapSlots->cameraWork;
  EFFECT_START_NARROW_CANVAS(BGCNT_PRIORITY(1)); // no horizontal stretch
  REG_BLDCNT = 0;                                // no blending
  Resource_LoadAndDecompress(RESOURCE_ROCK_WALL_SHEET,
                             work->sheet + ROCK_OFFSET, 1, 1);

  // Copy the background's characters and cut the band out of them.
  Iwram_CopyWords(Ram_MapCellBuffer, (void *)BG_CHAR_BLOCK(2), 0x8000);
  for (row = 0; row != 16; row++) {
    y = row + 96;
    for (col = 0; col != PILLAR_W; col++) {
      s32 pixel = BG_DOT(col + 32, y);

      work->sheet[row * PILLAR_W + col] = Ram_MapCellBuffer[pixel];
    }
  }

  // On the far side the canvas moves 112 pixels right, and the targets'
  // screen positions with it.
  if (args->side == EFFECT_SIDE_PARTY) {
    REG_BG2X = -(NARROW_CANVAS_PARTY_X << 8);
    xOffset = -NARROW_CANVAS_PARTY_X;
  } else {
    xOffset = 0;
  }
  EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
                      BLITTER_TRANSPARENT);
  draw = (DrawRectangle)Ram_HeapSlots->ramCode;
  for (member = 0; member != args->targetCount; member++)
    work->particles[member].variant = 0; // 1 once landed
  work->transferMode =
      CANVAS_COPY_AND_CLEAR; // clear the canvas on each transfer
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  heights = gRockWall_Heights;
  work->shakeFrames = 128;
  Sound_PlayFar(SE_RUMBLE);

  for (frame = 0; frame != gQuake_Timings[args->variant][TIMING_FRAMES];
       frame++) {
    Matrix_Initialize();
    Matrix_SetLookAt(camera->eye, camera->focus);
    // The results begin 16 frames before the end.
    if (frame == gQuake_Timings[args->variant][TIMING_FRAMES] - 16)
      EFFECT_SHOW_DAMAGE(SE_IMPACT);

    // The pillars rise from frames 16, 20 and 24.
    for (column = 0; column != 3; column++) {
      s32 start = column * 4 + 16;

      if ((frame & 31) + 32 == start) // never true: the left side is >= 32
        heights.column[column] += 32;
      if (frame >= start &&
          frame < start + gQuake_Timings[args->variant][TIMING_RISE_FRAMES]) {
        s32 height;
        s32 top;

        // Up and down along a sine, 1/64 of a turn a frame.
        height = FIXEDtoINT(heights.column[column] *
                            Trig_Sin((frame - start) << 10));
        if (height < 0)
          height = -height;
        top = 112 - height;
        draw(canvas, work->sheet + ROCK_OFFSET, column * PILLAR_W + 8, top,
             PILLAR_W, height);
        draw(canvas, work->sheet, column * PILLAR_W + 8, 96 - height, PILLAR_W,
             16);

        // Launch the targets standing on the pillar.
        for (member = 0; member != args->targetCount; member++) {
          struct MotionObject *object;

          object = BattleObject_GetFar(args->targets[member])->object;
          point[0] = object->x;
          point[1] = object->y;
          point[2] = object->z;
          EffectPosition_ProjectPoint(point, &screen);
          screen.x += xOffset;
          if (screen.x >= column * PILLAR_W + 8 &&
              screen.x <= column * PILLAR_W + 8 + PILLAR_W && screen.y >= top) {
            object->velocityY = toFIXED(12); // 12 pixels a frame upward
            object->verticalMotionStrength = 0xab85;
          }
          if (object->y < 0)
            BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_NORMAL,
                                     POSE_DAMAGE, TINT_KEEP, 0);
        }
      }
    }

    // A target that lands flashes in palette 7 for 5 frames.
    for (member = 0; member != args->targetCount; member++) {
      struct MotionObject *object;
      struct Particle *state;

      object = BattleObject_GetFar(args->targets[member])->object;
      state = &work->particles[member];
      if (state->variant == 0 && object->y <= 0 && object->velocityY < 0) {
        state->variant = 1;
        BattleEffect_TintBattler(args->targets[member], SPRITE_COLOR_WHITE,
                                 POSE_DAMAGE, member, 5);
      }
    }
    BattleCamera_Shake(gQuake_Timings[args->variant][TIMING_SHAKE],
                       gQuake_Timings[args->variant][TIMING_SHAKE]);
    EFFECT_END_FRAME(work);
  }

  EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
