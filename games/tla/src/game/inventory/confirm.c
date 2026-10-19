#include "types.h"
#include "uiwindow.h"
#include "party_state.h"
#include "ram_buffer.h"

s16 *Scene_FindActorPlacement(s32 action);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(s32 count);
s32 Menu_RunConfirmSelectionFar(s32 useBottom, s32 name, s32 width, s32 third);
void Object_SetModeById(u32 id, s32 mode);
void UiWork_FinalizeEntityMatchingLocalizedIdFar(s32 sprite);
void UiWork_FinalizePendingCoreFar(void);
void Object_WaitUntilChildValueDiffers(s32 actor, s32 value);

/// The event command "choose yes or no", as ☀️'s
/// Event_AskYesNoWithActorAnimation (games/tbs/src/object/psynergy_effects.c):
/// waits for the message, asks yes or no below it or above it when the message
/// windows reach past row 15, and the speaker nods (animation 3) at yes and
/// shakes its head (4) at no. ⚓️ drops the wait for released keys, waits
/// (2 - event mode) * 3 frames, passes the name and its width and a third value
/// to the menu, and skips the animation wait in event mode 2.
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 forceBottom) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 sprite = *Scene_FindActorPlacement(*(s32 *)(work + 0x1e0));
  struct UiWindow *window = *(struct UiWindow **)(work + 0x1e4);
  struct UiWindow *side = *(struct UiWindow **)(work + 0x1e8);
  s32 useBottom = 1;
  s32 answer;

  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);

  WaitFrames((2 - gPartyState.eventMode) * 3);

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
    Object_SetModeById(actor, 4);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
    UiWork_FinalizePendingCoreFar();
    if (gPartyState.eventMode != 2)
      Object_WaitUntilChildValueDiffers(actor, 4);
  } else {
    Object_SetModeById(actor, 3);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
    UiWork_FinalizePendingCoreFar();
    if (gPartyState.eventMode != 2)
      Object_WaitUntilChildValueDiffers(actor, 3);
  }

  return answer;
}

s32 Func_080d295c(s32 a) { return Menu_RunConfirmSelectionFar(a, 0, 0, 0); }
