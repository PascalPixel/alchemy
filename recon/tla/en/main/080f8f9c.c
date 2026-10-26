// Near miss, differ 2 of 170 (2026-10-26): the twin of The Broken Seal's
// PagedList_HandleInput, the keys read from gInput.repeat. Left: on a step
// forward the native sets the result 0 straight after storing the cursor, before
// the page's load; here it comes after the multiply.
#include "types.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "input.h"

void Link_DrawShiftedTilePairFar(s32 addr);
void Audio_PlayCue(s32 cue);
void Input_ResetKeyRepeat(void);

s32 Menu_HandlePageInput(s32 horizontal, s32 count, s32 per_page, s32 *cursor,
                         s32 *page) {
  s32 pages;
  s32 next;
  s32 previous;
  s32 page_forward;
  s32 result;

  result = -1;
  if (count == 0)
    goto done;
  Link_DrawShiftedTilePairFar(0x06002500);
  pages = __divsi3(count, per_page);
  if (__modsi3(count, per_page) != 0)
    pages++;
  if (horizontal) {
    next = gInput.repeat & KEY_RIGHT;
    previous = gInput.repeat & KEY_LEFT;
    horizontal = gInput.repeat & KEY_UP;
    page_forward = gInput.repeat & KEY_DOWN;
  } else {
    next = gInput.repeat & KEY_DOWN;
    previous = gInput.repeat & KEY_UP;
    horizontal = gInput.repeat & KEY_LEFT;
    page_forward = gInput.repeat & KEY_RIGHT;
  }
  if (horizontal) {
    Audio_PlayCue(111);
    if (--*page < 0)
      *page = pages - 1;
    if (*cursor + *page * per_page > count - 1) {
      *cursor = count - *page * per_page - 1;
      if (*cursor > per_page - 1)
        *cursor = per_page - 1;
    }
    Input_ResetKeyRepeat();
    result = 1;
    goto done;
  }
  if (page_forward) {
    Audio_PlayCue(111);
    if (++*page > pages - 1)
      *page = 0;
    if (*cursor + *page * per_page > count - 1) {
      *cursor = count - *page * per_page - 1;
      if (*cursor > per_page - 1)
        *cursor = per_page - 1;
    }
    Input_ResetKeyRepeat();
    result = 1;
    goto done;
  }
  if (previous) {
    Audio_PlayCue(111);
    if (--*cursor < 0) {
      *cursor = per_page - 1;
      *cursor = count - *page * per_page - 1;
      if (*cursor > per_page - 1)
        *cursor = per_page - 1;
    }
    goto row_changed;
  }
  result = -1;
  if (next == 0)
    goto done;
  {
    Audio_PlayCue(111);
    result = 0;
    if (++*cursor == count - *page * per_page)
      *cursor = result;
    if (*cursor > per_page - 1)
      *cursor = result;
  }
row_changed:
  result = 0;
done:
  return result;
}
