#include "types.h"
#include "system.h"
#include "tla_edition.h"
#include "main_routine_work.h"

struct InputState {
  u32 held;
  u32 pressed;
};

extern struct InputState gInput;

struct Work;

struct TextObject {
  u8 storage[12];
};

void Event_SetDestinationFar(s16 primary, s16 secondary);
void UiTextResource_Release(s32 resource);
void UiTextResource_Initialize(struct TextObject *object, s32 *resource);
void UiTextResource_SetPosition(struct TextObject *object, s32 x, s32 y);
struct Work *Window_Create(s32 kind, s32 x, s32 y, s32 width, s32 layer);
void Window_Close(struct Work *work, s32 release);
void Menu_DrawSelectionRow(struct Work *work, s16 first, const s16 *second);
s32 Menu_HandleSelectionRowInput(struct Work *work, s16 value, s16 *sub,
                                 s16 *mode);

/// Picks a destination (primary and secondary) in a row menu and travels
/// there; returns -1 after travelling and -2 when cancelled.
s16 Menu_RunSelection(void) {
  s32 resource;
  s16 mode;
  s16 secondary;
  struct TextObject object;
  s16 primary;
  struct Work *work;
  s16 result;

  work = 0;
  mode = 0;
  primary = gMainRoutineWork.primary;
  secondary = gMainRoutineWork.secondary;
  work = Window_Create(0, 7, 30, 5, 2);
  Menu_DrawSelectionRow(work, primary, &secondary);
  UiTextResource_Initialize(&object, &resource);

  while (gInput.held != 0)
    wait(1);

  for (;;) {
    result =
        (s16)Menu_HandleSelectionRowInput(work, primary, &secondary, &mode);
    if (result == -1) {
      UiTextResource_Release(resource);
      Window_Close(work, 2);
      Event_SetDestinationFar(primary, secondary);
      return result;
    }
    if (result == -2) {
      UiTextResource_Release(resource);
      Window_Close(work, 2);
      return result;
    }

    UiTextResource_SetPosition(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
    primary = result;
    wait(1);
  }
}
