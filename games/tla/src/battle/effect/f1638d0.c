#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "system.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The disc: a 128 x 128 picture of each pixel's distance from the centre,
// which the 63-colour ramp turns into rings.
#define DISC_SIZE 128
#define DISC_RINGS 63

#define RIPPLE_FRAMES 80
#define RIPPLE_FADE_IN_FRAMES 8
#define RIPPLE_FADE_OUT_FRAME 72
#define RIPPLE_HIT_FRAME 48

#define PHASE_RESULTS 134

void BattleEffect_StartCanvasLayer(s32 bgControl);
void Func_0815585c(struct BattleEffectArgument *effect, s32 flags, s32 *screenX,
                   s32 *screenY); // BattleEffect_RunDjinniEntrance
void BattleEffect_FetchRectangleBlitters(s32 side, DrawRectangle *draw);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_ArmBg2AffineHBlankDma(void);
void Func_081180e8(s32 phase); // BattleEventRuntime_StartPhaseFar
void Func_08118088(s32 actor,
                   s32 variant);  // BattleMotion_ApplyVariantMotionFar
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void ObjectGroup_TickMemberTimers(void);
void BattleEffect_EndCanvasLayer(void);

/// A disc of rings rises on BG2 over the target's side, its colour ramp
/// cycling outward one ring a frame while each scanline sways on a sine wave.
/// Abilities: Torch (kind 214).
///
/// Setup: open the canvas, draw the distance map into the sheet and a warm
/// ramp into BG palette 1-63, blit the disc to the canvas, and arm the
/// per-scanline BG2 offsets. Each frame: fade in over the first 8 frames,
/// rebuild the 160 sway offsets, rotate the ramp by one colour, shake the
/// camera. At frame 48 the target is struck; from frame 72 the disc fades.
void BattleEffect_RunTorch(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 screenX[3];
  s32 screenY[3];
  DrawRectangle draw[2];
  s32 x;
  s32 y;
  s32 dx;
  s32 dy;
  s32 centerY;
  s32 distance;
  s32 i;
  s32 shade;
  s32 red;
  s32 green;
  s32 blue;
  s32 frame;
  s32 wave;
  s32 level;
  u16 *ramp;
  s32 angle;
  s32 *line;

  /* FAKEMATCH(barrier): the empty asm stores the argument before the RAM base
   * is started; as plain statements, 2 of 354 instructions differ in tla-en. */
  __asm__("");
  work = Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Open the canvas on BG2 over the target's side.
  BattleEffect_StartCanvasLayer(0x2000);
  Func_0815585c(effect, 6 | effect->side << 4 | 2 << 5, screenX, screenY);
  REG_BG2CNT = 0x2784;
  REG_BLDALPHA = 0x1000;
  REG_BG2PA = 0xaa;
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer,
                                0xc80);

  // The distance map, one quarter at a time mirrored into the other three,
  // the centre an eighth lower than the middle so the disc leans back.
  for (y = 0; y != DISC_SIZE / 2; y++) {
    for (x = 0; x != DISC_SIZE / 2; x++) {
      centerY = y / 8 + DISC_SIZE / 2;
      dy = y - centerY;
      dx = x - DISC_SIZE / 2;
      distance = Iwram_Sqrt(dx * dx + dy * dy);
      distance /= 2;
      if (distance == 0)
        distance = 1;
      if (distance > DISC_RINGS)
        distance = DISC_RINGS;
      work->sheet[y * DISC_SIZE + x] = distance;
      work->sheet[y * DISC_SIZE + DISC_SIZE - 1 - x] = distance;
      work->sheet[(DISC_SIZE - 1 - y) * DISC_SIZE + x] = distance;
      work->sheet[(DISC_SIZE - 1 - y) * DISC_SIZE + DISC_SIZE - 1 - x] =
          distance;
    }
  }

  // The ramp: dark red at the rim, rising to pale yellow at ring 31 and
  // back, kept in the map cell buffer and written to the BG palette.
  ramp = (u16 *)Ram_MapCellBuffer;
  for (i = 1; i != DISC_RINGS + 1; i++) {
    if (i > 31)
      shade = 64 - i;
    else
      shade = i;
    red = shade * 9;
    green = shade * 7 - 42;
    blue = shade * 7 - 56;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    if (red > 255)
      red = 255;
    if (green > 255)
      green = 255;
    if (blue > 250)
      blue = 250;
    red >>= 3;
    green >>= 3;
    blue >>= 3;
    ramp[i] = ((u16 *)BG_PLTT)[i] = blue << 10 | green << 5 | red;
  }

  // Blit the disc once; from here on only the palette and the scanline
  // offsets move it.
  draw[0](canvas, work->sheet, 0, 0, DISC_SIZE, DISC_SIZE);
  work->transferPending = 1;
  Scheduler_AddOrUpdateCallback(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);

  for (frame = 0; frame != RIPPLE_FRAMES; frame++) {
    // Fade in, and the sway grows with the frame.
    if (frame <= RIPPLE_FADE_IN_FRAMES) {
      level = frame * 2;
      wave = level;
      REG_BLDALPHA = level | 0x1000;
    } else {
      wave = frame * 2;
    }
    if (frame > RIPPLE_FADE_OUT_FRAME)
      REG_BLDALPHA = (RIPPLE_FRAMES * 2 - frame * 2) | 0x1000;

    // The hit: results phase, shake, and the target knocked back.
    if (frame == RIPPLE_HIT_FRAME) {
      Func_081180e8(PHASE_RESULTS);
      work->shakeFrames = 8;
      Func_08118088(effect->actors[0], 0);
    }

    // One BG2 X offset per scanline, a sine wave running down the screen.
    line = work->bg2xLines;
    for (i = 0, angle = -(wave << 9); i != 160; i++) {
      *line++ = ((i << 18) - (Trig_Sin(angle) << 7) + 0x40000) >> 10;
      angle += 0x200;
    }

    // Rotate the ramp outward by one ring: the last colour wraps to the
    // first, DMA shifts the rest up, and the queue writes it at VBlank.
    if (frame > 127) {
      work->transferPending = 1;
    } else {
      ramp[1] = ramp[DISC_RINGS];
      Dma_Set(ramp + DISC_RINGS - 1, ramp + DISC_RINGS, 0x80a0003e, REG_DMA3);
      IO_WRITE_QUEUE_PUSH((u32)&((u16 *)BG_PLTT)[1], (u32)(ramp + 1),
                          0x8000003f);
    }
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ArmBg2AffineHBlankDma);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
