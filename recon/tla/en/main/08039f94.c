// Near miss, differ 2 of 92 (tla-en, 2026-10-30): sched2's tie-break after
// the second call. There the ready list holds the width shift (asrs r0) and
// the next part's multiply at equal priority (118 each under
// -mthumb-slow-loads); the class rule prefers the multiply, independent of
// the store just issued, over the shift, anti-dependent on it (it rewrites
// r0). The instructions and their dependences are the native's, so no source
// form seen moves it. Measured 2026-10-30: a barrier after the shift with the
// width copied after the part (the copy is propagated away and the move to
// fp still precedes the multiply: differ 2); size as u16 (differ 9); the
// duration's address or the shift moved after the part (differ 2); permuter,
// 4 minutes: nothing better.
// Earlier (2026-10-26): no single empty asm, one-pass block, wrapping block
// or swap of neighbouring statements brings it closer, nor any pair of empty
// asm statements or one-pass blocks (device search).
// Near miss of Window_UpdateInterpolatedGeometry (The Broken Seal twin in
// window/window.c; first written 2026-10-12): differ 2 of 92. After
// each of three ratio calls the native loads the next operand (window->x,
// window->height, window->y) before it stores the result to the scratch
// (ldrsh; str r0, [r5, #8]); the twin's form stores first each time (differ
// 10). The ratio and the operand held in temporaries, loaded in the native's
// order, fix all three; the pointer to the duration (found by the permuter)
// keeps the third part's registers. Left: after the second call the native
// shifts the width out before it multiplies the next part; agscc multiplies
// first. Measured: function-level temporaries reused at the later sites
// (differ 44 and 46: the allocation changes), a barrier after the width
// (differ 6).
#include "types.h"
#include "iwram_call.h"

struct Window {
  s32 unk00;
  s32 unk04;
  u16 width;
  u16 height;
  s16 x;
  s16 y;
  u16 unk10;
  u16 unk12;
  u16 state;
  u16 flags;
  s16 frame;
  s16 duration;
  s16 previousX;
  s16 previousY;
  s16 previousWidth;
  s16 previousHeight;
};

void Window_DrawBorder(s32 x, s32 y, s32 width, s32 height);

// Draw the window partway through opening or closing.
void Window_UpdateInterpolatedGeometry(void *arg, s32 savePosition) {
  struct Window *window = arg;
  s32 scratch[3];
  s32 frame;
  s32 duration;
  s32 remaining;
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 r;
  s32 origin;
  s16 *durationField;

  frame = (s16)window->frame;
  duration = window->duration;
  remaining = duration - frame;
  scratch[0] = (frame * window->width) << 16;
  scratch[1] = duration << 17;
  /* FAKEMATCH: forced temporaries: the operand loads before the store. */
  r = Iwram_RatioMulQ14(scratch[1], scratch[0]);
  origin = window->x;
  scratch[2] = r;
  x = (r >> 16) + origin;

  scratch[0] = (remaining * window->width) << 16;
  /* FAKEMATCH: forced temporaries, as above, and the duration's address
   * taken early. */
  {
    s32 result = Iwram_RatioMulQ14(scratch[1], scratch[0]);
    s32 size = window->height;

    scratch[2] = result;
    width = result >> 15;
    durationField = &window->duration;

    scratch[0] = (frame * size) << 16;
  }
  scratch[1] = (*durationField) << 17;
  /* FAKEMATCH: forced temporaries, as above. */
  {
    s32 result = Iwram_RatioMulQ14(scratch[1], scratch[0]);
    s32 originY = window->y;

    scratch[2] = result;
    y = (result >> 16) + originY;
  }

  scratch[0] = (remaining * window->height) << 16;
  scratch[2] = Iwram_RatioMulQ14(scratch[1], scratch[0]);
  height = scratch[2] >> 15;

  Window_DrawBorder(x, y, width, height);
  if (savePosition != 0) {
    window->previousX = x;
    window->previousY = y;
    window->previousWidth = width;
    window->previousHeight = height;
  }
}
