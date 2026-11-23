/// @file
/// The debug menus' destination row.
// Part of screens.c.

#include "window_internal.h"
#include "gamepak/far_call/field_sequence.h"

/// Where the debug destination row's colons and its second column (the
/// entrance number and the area name) start, in pixels; further right in
/// Japanese.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define DEBUG_ROW_COLON_X 100
#define DEBUG_ROW_SECOND_X 110
#else
#define DEBUG_ROW_COLON_X 74
#define DEBUG_ROW_SECOND_X 82
#endif

/// Draws the debug scene and entrance numbers with their scene code and area
/// name. The first line has the scene's code (MsgDebugSceneCode + scene)
/// and, beside it, the area its entrance leads to (MsgDebugAreaName + area);
/// the numbers, three digits each, go under them.
void DebugSceneSelect_DrawRow(void *window, s16 scene, const s16 *entrance) {
  s16 selected = scene;
  s32 label = Area_GetNameIndexFar(selected, *entrance) + (s32)MsgDebugAreaName;

  WindowSprite_PrepareForRedraw(window);
  Text_DrawNumber(selected, 3, window, 0, 14);
  Text_DrawNumber(*entrance, 3, window, DEBUG_ROW_SECOND_X, 14);
  Text_DrawString(gMenu_ColonString, window, DEBUG_ROW_COLON_X, 0);
  Text_DrawMessage(selected + (s32)MsgDebugSceneCode, window, 0, 0);
  Text_DrawString(gMenu_ColonString, window, DEBUG_ROW_COLON_X, 14);
  Text_DrawMessage(label, window, DEBUG_ROW_SECOND_X, 0);
}

/// The debug destination menu: picks a scene and an entrance, starting from
/// the scene the party came from, and sends the party there with A; returns
/// -1 after setting the destination and -2 when B cancels.
s16 DebugSceneSelect_Run(void) {
  s32 arrowSlot;
  s16 field;
  s16 entrance;
  struct MenuSprite arrow;
  s16 scene;
  struct Window *window;
  s16 result;

  window = 0;
  field = 0;
  scene = gMainRoutineWork.returnScene;
  entrance = gMainRoutineWork.returnEntrance;
  window = Window_Create(0, 7, 30, 5, WINDOW_FLAG_INSTANT);
  DebugSceneSelect_DrawRow(window, scene, &entrance);
  MenuArrow_Create(&arrow, &arrowSlot);

  while (gInput.held != 0)
    wait(1);

  for (;;) {
    result =
        (s16)DebugSceneSelect_HandleRowInput(window, scene, &entrance, &field);
    if (result == -1) {
      MenuArrow_Release(arrowSlot);
      Window_Close(window, 2);
      Event_SetDestinationFar(scene, entrance);
      return result;
    }
    if (result == -2) {
      MenuArrow_Release(arrowSlot);
      Window_Close(window, 2);
      return result;
    }

    MenuArrow_SetPosition(&arrow, MENU_TEXT_X, field * 14 + MENU_TEXT_Y);
    scene = result;
    wait(1);
  }
}
#undef DEBUG_ROW_COLON_X
#undef DEBUG_ROW_SECOND_X
