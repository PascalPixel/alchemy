/// @file
/// Field: finish the speaker display.

#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "input.h"

s32 UiWork_FinalizeEntityMatchingLocalizedIdFar();
s32 ObjectTable_ReadActiveValue();
s32 Func_080d1eac(s32 sprite);
s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 packed);
void Func_080380f8(s32 sprite, s32 zero, s32 x, s32 y);
s32 UiWork_IsCompleteFar(void);
s32 UiWork_IsIdleFar(s32 window);
void UiWork_FinalizePendingCoreFar(void);
void WaitFrames(s32 frames);

/// If the actor has graphics, use its graphics ID as speaker and end the window.
/// id: actor ID.
void ObjectTable_RunIfActive(s32 id) {
  s32 no;

  no = ObjectTable_ReadActiveValue(id);
  if (no != -1)
    UiWork_FinalizeEntityMatchingLocalizedIdFar(no);
}

/// The event command "show two messages and wait": two actors speak at once,
/// each message at its own (x, y) with its portrait at (portraitX,
/// portraitY), all in tiles. When delay is nonzero, the second message waits
/// for the first to finish, checking every delay frames. Waits for A, B, L or
/// R, then closes both.
// As ☀️'s function of the same name, which has no delay.
void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 firstX, s32 firstY,
                                      s32 firstPortraitX, s32 firstPortraitY,
                                      s32 second, s32 secondX, s32 secondY,
                                      s32 secondPortraitX, s32 secondPortraitY,
                                      s32 delay) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 firstSprite = ObjectTable_ReadActiveValue(first);
  s32 secondSprite = ObjectTable_ReadActiveValue(second);
  s32 firstWindow;
  s32 secondWindow;

  firstWindow =
      UiText_OpenMessageWindowFar((s16)event->messageId++, firstX, firstY,
                                  Func_080d1eac(firstSprite) << 16);
  Func_080380f8(firstSprite, 0, firstPortraitX, firstPortraitY);
  if (delay != 0) {
    while (!UiWork_IsCompleteFar())
      WaitFrames(delay);
  }

  secondWindow =
      UiText_OpenMessageWindowFar((s16)event->messageId++, secondX, secondY,
                                  Func_080d1eac(secondSprite) << 16);
  Func_080380f8(secondSprite, 0, secondPortraitX, secondPortraitY);

  while (!UiWork_IsCompleteFar())
    WaitFrames(1);

  WaitFrames(1);
  while (!(gInput.pressed & 0x303))
    WaitFrames(1);

  WaitFrames(1);
  UiWork_FinalizeEntityMatchingLocalizedIdFar(firstSprite);
  UiWork_FinalizeEntityMatchingLocalizedIdFar(secondSprite);
  UiWork_FinalizePendingCoreFar();
  WaitFrames(1);

  while (!UiWork_IsIdleFar(firstWindow))
    WaitFrames(1);
  while (!UiWork_IsIdleFar(secondWindow))
    WaitFrames(1);

  WaitFrames(1);
}
