// Draft, differ 6 of 354: BattleFx_RunTorch, effect kind 214, The Broken Seal's
// BattleFx_RunTorch.

// Twin: ☀️ BattleFx_RunTorch (games/tbs/src/effect/effect.c), ported
// with The Lost Age's changes: the canvas set up through Func_0815585c
// (kind | side << 4 | anchor << 5), 80 frames where ☀️ has 96 (the fade out
// from frame 72), and a hit at frame 48 (results phase, eight frames of shake,
// the target's knock-back motion) that ☀️'s disc does not have.

// Devices: the empty asm at the start stores the argument before 0x03000000 is
// started (differ 8 without it); the BG2CNT halfword pinned to r3 in a
// one-pass block loads from the pool before the register address is started.
// Without the pin the halfword loads after movs r2, #128 and the call pair
// below matches (differ 6 again); the pin alone, without its block, loads first
// and keeps the call pair, but the pool entry lands 4 bytes off (differ 6);
// the pin behind an empty asm holding r2 (12), an unpinned u16 behind an empty
// asm (30), the store alone in a one-pass block (28), an unpinned u16 set in a
// one-pass block (40).

// Left, two reordered pairs:
// - the native starts the third argument of Func_0815585c (add r2, sp, #44)
// between the side's shift and its orrs;
// - in the I/O queue push it increments the count before it scales the entry
// offset, and stores the count before it adds the queue's base.

// Both are ties in sched2 that agscc breaks the other way. In the push,
// the increment and the offset's second shift are both ready at the same
// cycle with priority 10; the shift is data-dependent on the last insn and
// the increment only anti-dependent, so the shift wins. A pooled load costing
// 3 cycles instead of 2 would give the increment priority 11 (it leads to
// the pooled value), which is the native order (the 08164b2c.c header's
// finding). At the call, the orrs has one more dependent (add r3 reuses
// r3), so it wins its tie with add r2.

// Tried (differ): the flags written 70 | side << 4, with 0x46, (side << 4) ^
// 70,
// side * 16 | 70, or in a local first (6); &screenX through a local (64); the
// screens as struct EffectPosition (6); the push written out with the count
// stored first (6, the increment then takes r3), ++count (6), the count store
// in a one-pass block (8); alchemy permute 6 minutes, 2 jobs, on this draft
// (no candidate beat it). screenX declared first takes sp+44, as the
// native's.
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

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_0815585c(struct BattleEffectArgument *effect, s32 flags, s32 *screenX,
                   s32 *screenY); // BattleFx_PrepareCanvasEffect
void BattleFx_FetchRectangleBlitters(
    s32 side,
    DrawRectangle *draw); // BattleFx_FetchRectangleBlitters
void BattleFx_ProcessPendingGraphicsTransfer(void); // BattleFx_ProcessPendingGraphicsTransfer
void BattleFx_ArmBg2AffineHBlankDma(void); // BattleFx_ArmBg2AffineHBlankDma
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
void Func_08118088(s32 actor,
                   s32 variant);  // BattleMotion_ApplyVariantMotionFar
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void ObjectGroup_TickMemberTimers(void);         // ObjectGroup_TickMemberTimers
void BattleFx_EndCanvasLayer(void);         // BattleFx_EndCanvasLayer

// A disc of rings rises on BG2 over the target's side, its colour ramp
// cycling outward one ring a frame while each scanline sways on a sine wave.
//
// Setup: open the canvas, draw the distance map into the sheet and a warm
// ramp into BG palette 1-63, blit the disc to the canvas, and arm the
// per-scanline BG2 offsets. Each frame: fade in over the first 8 frames,
// rebuild the 160 sway offsets, rotate the ramp by one colour, shake the
// camera. At frame 48 the target is struck; from frame 72 the disc fades.
void BattleFx_RunTorch(struct BattleEffectArgument *effect) {
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

  /* FAKEMATCH: stores the argument before the 0x03000000 base is started; the
   * native's first instruction after sub sp is str r0 (the base first without
   * it, differ 8). */
  __asm__("");
  work = Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Open the canvas on BG2 over the target's side.
  BattleFx_BeginCanvasLayer(0x2000);
  Func_0815585c(effect, 6 | effect->side << 4 | 2 << 5, screenX, screenY);
  {
    /* FAKEMATCH: the native loads the BG2CNT halfword from the pool before it
     * starts the register's address; pinned to r3 in a one-pass block it does
     * (as a plain store it loads after the address's first half). */
    register u16 value __asm__("r3");

    do {
      value = 0x2784;
    } while (0);
    REG_BG2CNT = value;
  }
  REG_BLDALPHA = 0x1000;
  REG_BG2PA = 0xaa;
  BattleFx_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);

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
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0xc80);

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
    line = work->bg2X;
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

  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleFx_EndCanvasLayer();
}
