/// @file
/// The debug menus' destination row.
// Part of window.c.

#include "../window_internal.h"
#include "gamepak/far_call/field_sequence.h"

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define SELROW_TEXT_X 100
#define SELROW_LABEL_X 110
#else
#define SELROW_TEXT_X 74
#define SELROW_LABEL_X 82
#endif

/// Draws the debug scene and entrance numbers with their scene code and area
/// name. The first line has the scene's code (MsgDebugSceneCode + scene)
/// and, beside it, the area its entrance leads to (MsgDebugAreaName + area);
/// the numbers, three digits each, go under them.
void DebugSceneSelect_DrawRow(void *window, s16 first, const s16 *second) {
  s16 selected = first;
  s32 label = Area_GetNameIndexFar(selected, *second) + (s32)MsgDebugAreaName;

  WindowSprite_PrepareForRedraw(window);
  Text_DrawNumber(selected, 3, window, 0, 14);
  Text_DrawNumber(*second, 3, window, SELROW_LABEL_X, 14);
  Text_DrawString(gMenu_ColonString, window, SELROW_TEXT_X, 0);
  Text_DrawMessage(selected + (s32)MsgDebugSceneCode, window, 0, 0);
  Text_DrawString(gMenu_ColonString, window, SELROW_TEXT_X, 14);
  Text_DrawMessage(label, window, SELROW_LABEL_X, 0);
}

struct Work;

/// Picks a destination (primary and secondary) in a row menu and travels
/// there; returns -1 after travelling and -2 when cancelled.
s16 DebugSceneSelect_Run(void) {
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
  DebugSceneSelect_DrawRow(work, primary, &secondary);
  MenuArrow_Create(&object, &resource);

  while (gInput.held != 0)
    wait(1);

  for (;;) {
    result =
        (s16)DebugSceneSelect_HandleRowInput(work, primary, &secondary, &mode);
    if (result == -1) {
      MenuArrow_Release(resource);
      Window_Close(work, 2);
      Event_SetDestinationFar(primary, secondary);
      return result;
    }
    if (result == -2) {
      MenuArrow_Release(resource);
      Window_Close(work, 2);
      return result;
    }

    MenuArrow_SetPosition(&object, MENU_TEXT_X, mode * 14 + MENU_TEXT_Y);
    primary = result;
    wait(1);
  }
}
#undef SELROW_TEXT_X
#undef SELROW_LABEL_X
