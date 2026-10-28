#include "types.h"
#include "uiwindow.h"
#include "adventure.h"
#include "ram_buffer.h"

s16 *Scene_FindActorPlacement(s32 action);
s32 Message_AllTypedFar(void);
void wait(s32 count);
s32 Menu_RunConfirmSelectionFar(s32 useBottom, s32 name, s32 width, s32 third);
void Actor_SetModeById(u32 id, s32 mode);
void Window_CloseMatchingIdFar(s32 sprite);
void Message_CloseFinishedWindowsFar(void);
void Actor_WaitForAnimationEnd(s32 actor, s32 value);

/// The event command "choose yes or no": waits for the message, asks yes or no
/// below it or above it when the message windows reach past row 15, and the
/// speaker nods (animation 3) at yes and shakes its head (4) at no. It waits
/// (2 - event mode) * 3 frames, passes the name and its width and a third value
/// to the menu, and skips the animation wait in event mode 2.
// As ☀️'s Event_AskYesNoWithActorAnimation
// (games/tbs/src/object/psynergy_effects.c), less its wait for released keys.
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 forceBottom) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 sprite = *Scene_FindActorPlacement(*(s32 *)(work + 0x1e0));
  struct Window *window = *(struct Window **)(work + 0x1e4);
  struct Window *side = *(struct Window **)(work + 0x1e8);
  s32 useBottom = 1;
  s32 answer;

  while (Message_AllTypedFar() == 0)
    wait(1);

  wait((2 - gAdventure.eventMode) * 3);

  if (forceBottom == 0) {
    s32 bottom = (s16)window->y + window->height;

    if (side != NULL) {
      s32 sideBottom = (s16)side->y + side->height;
      if (bottom < sideBottom)
        bottom = sideBottom;
    }

    if (bottom > 15)
      useBottom = 0;
  }

  answer = Menu_RunConfirmSelectionFar(useBottom, *(s16 *)(work + 0xcb0),
                                       *(s16 *)(work + 0xcb2),
                                       *(s16 *)(work + 0xcb4));
  if (answer != 0) {
    Actor_SetModeById(actor, 4);
    Window_CloseMatchingIdFar(sprite);
    Message_CloseFinishedWindowsFar();
    if (gAdventure.eventMode != 2)
      Actor_WaitForAnimationEnd(actor, 4);
  } else {
    Actor_SetModeById(actor, 3);
    Window_CloseMatchingIdFar(sprite);
    Message_CloseFinishedWindowsFar();
    if (gAdventure.eventMode != 2)
      Actor_WaitForAnimationEnd(actor, 3);
  }

  return answer;
}

s32 Func_080d295c(s32 a) { return Menu_RunConfirmSelectionFar(a, 0, 0, 0); }
