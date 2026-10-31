// Near miss, differ 2 of 92 (tla-en, 2026-10-31): sched2's tie-break after
// the second call. There the ready list holds the width shift (asrs r0) and
// the next part's multiply at equal priority (118 each under
// -mthumb-slow-loads); the class rule prefers the multiply, independent of
// the store just issued, over the shift, anti-dependent on it (it rewrites
// r0). Both reach the third call by paths of equal cost, so no source form
// with these instructions moves it.
// 2026-10-31: the plain form of the Broken Seal twin (Window_DrawZoomBorder in
// window/window/borders.c) now leaves only this, with the tree's struct Window
// (uiwindow.h) and prototypes in place of the draft's own struct: its fields
// no longer alias the scratch, so sched2 itself loads each next operand
// before the scratch store, and the forced temporaries and the pointer to
// the duration are gone. Measured devices: a one-pass block from the width
// shift through the third part's stores, with the height loaded first
// (differ 4: the block takes over stage's later uses as dependants, and the
// duration load then wins the entry tie over the frame load); that plus a
// one-pass block around the frame load, or one block from the entry to the
// shift (differ 2: the window copy and save_position's stack store swap at
// entry, sched2 then ranking them by RTL order); the copy to the width under
// a test whose two arms are the same (differ 87 to 102: the frame grows by
// a spill); an empty asm with operands reading the width (differ 46: the
// width takes r6); the frame read through a union (differ 4). Permuter: did
// not run (it found no listing for the copied draft).
// Earlier, with the draft's own struct (2026-10-12 to 2026-10-30): there the
// twin's form differed in 10; a barrier after the shift, the width copied
// after the part, size as u16 (differ 9), function-level temporaries (differ
// 44 and 46), single and paired empty asm statements or one-pass blocks, and
// a 4-minute permuter run never got below 2.
#include "types.h"
#include "iwram_call.h"
#include "uiwindow.h"

/// Scratch values for a stage of a window's zoom, as in The Broken Seal: the
/// scaled part of a side, the scaled duration and their ratio.
struct WindowZoomScratch {
  s32 scaledPart;
  s32 scaledDuration;
  s32 result;
};

void Window_DrawBorder(s32 x, s32 y, u32 width, u32 height);

void Func_08039f94(struct Window *window, s32 save_position) {
  struct WindowZoomScratch scratch;
  s32 stage;
  s32 duration;
  s32 remaining;
  s32 x;
  s32 y;
  s32 width;
  s32 height;

  stage = (s16)window->frame;
  duration = window->duration;
  remaining = duration - stage;
  // Iwram_RatioMulQ14(d, v) is v / d in 16.16 fixed point, as in The Broken
  // Seal: the left edge moves in by stage / duration of half the width
  // (result >> 16), and the width is (duration - stage) / duration of the
  // whole (result >> 15); the same for the top and the height.
  scratch.scaledPart = (stage * window->width) << 16;
  scratch.scaledDuration = duration << 17;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  x = (scratch.result >> 16) + (s16)window->x;

  scratch.scaledPart = (remaining * window->width) << 16;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  width = scratch.result >> 15;

  scratch.scaledPart = (stage * window->height) << 16;
  scratch.scaledDuration = window->duration << 17;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  y = (scratch.result >> 16) + (s16)window->y;

  scratch.scaledPart = (remaining * window->height) << 16;
  scratch.result =
      Iwram_RatioMulQ14(scratch.scaledDuration, scratch.scaledPart);
  height = scratch.result >> 15;

  Window_DrawBorder(x, y, width, height);
  if (save_position != 0) {
    window->previousX = x;
    window->previousY = y;
    window->previousWidth = width;
    window->previousHeight = height;
  }
}
