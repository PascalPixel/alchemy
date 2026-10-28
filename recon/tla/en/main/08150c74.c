// Near miss of BattleEffect_RunQuake, effect kind 1: differ 42 of 444 (draft 444).

// Twin: The Broken Seal's BattleEffect_RunQuake (effect.c), call for call.
// The Lost Age reads the effect from its argument, takes its heap blocks as
// Ram_HeapSlots fields, loads the blitter through BattleEffect_LoadBlitter
// (BLIT_TRANSPARENT is 2 where COMPOSITE_TRANSPARENT is 1), orders the
// callback 0xc80 and copies the background through a local "cells".
// ResourceId_RockWallSheet is row 0x14e of directory.s (asset
// BattleEffect_RockWallSheet), not yet named there.

// Left (differ 42, 2026-10-26; the copy through a local inline forwarder,
// as f143d80.c makes it and as The Broken Seal's IWRAM call layer makes
// every IWRAM call, so that the map buffer is loaded again for it: 44 to
// 42): the native sets the buffer (r5) and both band-loop counters (r6,
// r7) before the first call; here they start after the copy. The rest is
// single-row scheduling (immediates and pool loads one slot apart).

// Tried (trials 0 and 1, 2026-10-15): the map buffer read directly (98),
// counters and buffer set at the top (382), the buffer at the top only
// (76), do-while with an explicit offset (382), alchemy permute 6 minutes
// (58). 2026-10-26: the buffer set at the top with the copy reading
// Ram_MapCellBuffer (48 to 58), and with the counters and a row offset
// set there too (364 to 374).

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "battle_session.h"
#include "task.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
#include "motobj.h"
#include "actor.h"
#include "ram_buffer.h"
#include "system.h"
#include "transform.h"

static __inline__ s32 CopyWords_(void *destination, const void *source,
                                 s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void); // BattleEffect_CountDownBattlerTints
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // Actor_UpdateMembers
void Func_081180e8(s32 phase);       // BattleEventRuntime_StartPhaseFar
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake

// The Lost Age's blitter modes, BattleEffect_LoadBlitter's high nibble: 3 minus The
// Broken Seal's COMPOSITE_ADDITIVE, COMPOSITE_MAX and COMPOSITE_TRANSPARENT.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))

// A row of Data_081982f7, one for each variant.
enum {
  TIMING_FRAMES,      // the whole effect
  TIMING_RISE_FRAMES, // how long each pillar stands
  TIMING_SHAKE        // the camera shake
};

struct RockWallHeights {
  s32 column[4]; // each pillar's height in pixels
};

#define PILLAR_W 40        // pixels
#define ROCK_OFFSET 0x1e00 // the rock picture in the sheet, after the band

// The byte of pixel (x, y) in a 32-tile-wide 8bpp character block.
#define BG_DOT(x, y)                                                           \
  (((x) & 7) + (((x) / 8) << 6) + (((y) & 7) << 3) + (((y) / 8) << 11))

extern char ResourceId_RockWallSheet;
extern u8 Data_081982f7[][3];                // gQuake_Timings
extern struct RockWallHeights Data_08196e1c; // gRockWall_Heights

// Three rock pillars rise from the ground, left to right four frames apart,
// and sink again; targets standing on one are thrown into the air.

// Setup: the band of background under the targets (x 32-71, y 96-111) is
// cut out of BG character memory to cap each pillar. Frame loop: each
// pillar's height follows a sine over its rise time, a target over a
// pillar is launched upward, and a target that lands flashes; the camera
// shakes all the while. The variant chooses the length, the rise time and
// the shake.
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
  u8 *cells;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100; // no horizontal stretch
  REG_BLDCNT = 0;    // no blending
  Resource_LoadAndDecompress((s32)&ResourceId_RockWallSheet,
                             work->sheet + ROCK_OFFSET, 1, 1);

  // Copy the background's characters and cut the band out of them.
  cells = Ram_MapCellBuffer;
  CopyWords_(cells, (void *)0x06008000, 0x8000);
  for (row = 0; row != 16; row++) {
    y = row + 96;
    for (col = 0; col != PILLAR_W; col++) {
      s32 pixel = BG_DOT(col + 32, y);

      work->sheet[row * PILLAR_W + col] = cells[pixel];
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
  heights = Data_08196e1c;
  work->shakeFrames = 128;
  Sound_PlayFar(141);

  for (frame = 0; frame != Data_081982f7[effect->variant][TIMING_FRAMES];
       frame++) {
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    // The results begin 16 frames before the end.
    if (frame == Data_081982f7[effect->variant][TIMING_FRAMES] - 16)
      Func_081180e8(133);

    // The pillars rise from frames 16, 20 and 24.
    for (column = 0; column != 3; column++) {
      s32 start = column * 4 + 16;

      if ((frame & 31) + 32 == start) // never true: the left side is >= 32
        heights.column[column] += 32;
      if (frame >= start &&
          frame < start + Data_081982f7[effect->variant][TIMING_RISE_FRAMES]) {
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

          object = GetBattleObjectSlotFar(effect->actors[member])->object;
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
            Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
        }
      }
    }

    // A target that lands flashes in palette 7 for 5 frames.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object;
      struct EffectStep *state;

      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      state = &work->particles[member];
      if (state->variant == 0 && object->y <= 0 && object->velocityY < 0) {
        state->variant = 1;
        Func_0814cd48(effect->actors[member], 7, 5, member, 5);
      }
    }
    Func_08158ce0(Data_081982f7[effect->variant][TIMING_SHAKE],
                  Data_081982f7[effect->variant][TIMING_SHAKE]);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
