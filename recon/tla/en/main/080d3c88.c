// Under -mthumb-slow-loads (2026-10-26): differ 4 of 385 in tla-en, instruction
// order only. Setting handle and portrait before loading the event work changes
// nothing; no empty asm placement lowers it; permute (4 minutes) found nothing
// lower.
// Near miss, differ 18 of 385 (2026-10-24): The Broken Seal's
// FieldMessage_ShowAtObject (TBS_PATCH_1's portrait placement) on this game's
// heap slots and event work. Left: instruction order only, in the run up to
// the first two calls and after the second leader lookup: the native sets the
// shared zero before it loads the event work, stores the margin and the
// sprite and reloads the work from the frame before it starts the next
// offset (movs r2, #240 / #226 / #197); here the offsets come first. An empty
// volatile asm after the zero differs 22; a temporary for the heap slots behind
// a barrier frees a frame slot and reallocates the function (differ 373);
// three minutes of permuter on the opening statements' order found nothing.
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"

struct ObjectRuntime;

s32 ObjectTable_ReadActiveValue(s32 localId);
struct ObjectRuntime *ObjectTable_Get(s32 localId);
s32 Render_ProjectPoint(const void *point, s32 *screen);
s32 Func_080d5bec(s32 objectId, s32 *position);
void UiText_GetResourceDimensionsAltFar(s32 message, s32 *left, s32 *top,
                                        u32 *width, u32 *height);
void UiText_GetResourceDimensionsFar(s32 message, s32 *left, s32 *top,
                                     u32 *width, u32 *height);
s32 Localization_LookupEntryIdFar(s32 sprite);
s32 Func_080d1eac(s32 sprite);
s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 packed);
s32 Func_080380f8(s32 sprite, s32 zero, s32 x, s32 y);
s32 UiWork_IsCompleteFar(void);
void WaitTicks(s32 ticks);

#define SPEAKER_PORTRAIT_LEFT 0x1000
#define SPEAKER_PORTRAIT_RIGHT 0x2000
#define SPEAKER_WINDOW_ABOVE 0x4000
#define SPEAKER_WINDOW_BELOW 0x8000

s32 UiText_OpenMessageAtObject(s32 localId, s32 unused) {
  u8 *win;
  struct EventWork *work;
  s32 sprite;
  s32 extra;
  s32 portraitTop;
  s32 margin;
  u32 height;
  u32 width;
  s32 top;
  s32 left;
  s32 pos[3];
  s32 handle;
  s32 portrait;
  s32 flags;
  s32 message;
  struct ObjectRuntime *object;
  s32 x;
  s32 y;
  s32 found;
  s32 column;

  win = Ram_HeapSlots->windowTiles;
  work = Ram_HeapSlots->eventWork;
  handle = 0;
  portrait = 0;
  sprite = ObjectTable_ReadActiveValue(localId);
  extra = 0;
  portraitTop = 0;
  margin = 4;
  flags = localId & 0xf000;
  message = (s16)work->messageId;
  localId &= 0xfff;
  x = 0;
  object = ObjectTable_Get(localId);
  work->currentOwner = localId;
  y = 0;
  found = 0;
  if (work->waitMode == 0) {
    if (object != 0) {
      if (work->controlMode == 3) {
        Render_ProjectPoint((u8 *)object + 8, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
        y -= 2;
      } else {
        if (Func_080d5bec(localId, pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    } else if (localId <= 7) {
      sprite = localId;
      object = ObjectTable_Get(EventRuntime_GetControlledOwner());
      if (work->controlMode == 3) {
        Render_ProjectPoint((u8 *)object + 8, pos);
        x = pos[0] >> 3;
        y = pos[1] >> 3;
        found = 1;
      } else {
        if (Func_080d5bec(EventRuntime_GetControlledOwner(), pos) != -1)
          found = 1;
        else
          found = 0;
        x = pos[0] >> 3;
        y = pos[1] >> 3;
      }
    }

    if (!found) {
      left = 15;
      top = 10;
    } else {
      left = 0;
      top = 0;
      UiText_GetResourceDimensionsAltFar(message, &left, &top, &width,
                                         &height);
      left = x - (s32)width / 2;
      if (flags & SPEAKER_WINDOW_ABOVE)
        top = y - height - 1;
      else if (!(flags & SPEAKER_WINDOW_BELOW) && y <= 8)
        top = y - height - 1;
      else
        top = y + 4;
    }

    if (win[4] != 0)
      margin = 5;
    column = x;
    if (flags & SPEAKER_PORTRAIT_LEFT) {
      column = x - margin - 2;
      if (column < 0)
        column = 0;
    } else if (flags & SPEAKER_PORTRAIT_RIGHT) {
      column += 2;
      if (column + margin > 29)
        column = 29 - margin;
    } else if (column <= 15) {
      column = column - margin - 2;
      if (column < 0)
        column = x + 2;
    } else {
      column += 2;
      if (column + margin > 29)
        column = x - margin - 2;
    }

    found = Localization_LookupEntryIdFar(sprite);
    if (found != -1) {
      UiText_GetResourceDimensionsAltFar(message, &left, &top, &width,
                                         &height);
      message = -1;
      if (top > y)
        portraitTop = top - 5;
      else
        portraitTop = top + height;
      if (portraitTop < 0)
        portraitTop = top + height;
      else if (portraitTop + 5 > 19)
        portraitTop = top - 5;
      if (top < portraitTop) {
        s32 lines = height;

        UiText_GetResourceDimensionsFar(message, &left, &top, &width,
                                        &height);
        message = -1;
        extra = lines - height + 1;
      }
    } else if (top < y) {
      s32 lines = height;

      UiText_GetResourceDimensionsFar(message, &left, &top, &width, &height);
      extra = lines - height + 1;
      message = found;
    }
    if (column < 0)
      column = 0;
    else if (column + margin > 29)
      column = 29 - margin;

    if (win[4] != 0) {
      WaitTicks(8);
      if (extra != 0)
        handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1,
                                             18);
      else
        handle = UiText_OpenMessageWindowFar(message, left, top, 2);
    } else {
      s32 voice = Func_080d1eac(sprite);

      if (extra != 0)
        handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1,
                                             (voice << 16) | 17);
      else
        handle =
            UiText_OpenMessageWindowFar(message, left, top, (voice << 16) | 1);
    }
    if (win[4] != 0)
      portrait = Func_080380f8(sprite, 0, column, portraitTop);
    else
      portrait = Func_080380f8(sprite, 0, column, portraitTop);
    while (!UiWork_IsCompleteFar())
      WaitTicks(1);
  }
  work->objectStatus = handle;
  *(s32 *)((u8 *)work + 0x1e8) = portrait;
  work->messageId++;
  return handle;
}
