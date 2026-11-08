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
  {
    s32 part = stage * window->height;
    /* FAKEMATCH(barrier): an empty asm on the multiply's result, which issues
     * the width's shift first; as the plain store, 2 of 92 instructions differ
     * in tla-en. */
    asm("" : "=r"(part) : "0"(part));
    width = scratch.result >> 15;
    scratch.scaledPart = part << 16;
  }
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
