/// @file
/// The area name notice, and removing an item from the inventory.
// Part of window.c.

#include "window_internal.h"
#include "field/field_work.h"

#define TIMED_NOTICE_WORK ((struct FieldWork *)Ram_HeapSlots->fieldWork)

void AreaNameWindow_Update(void);

/// Shows the area's name in a window near the top of the screen for
/// AREA_NAME_WINDOW_FRAMES. The field work owns the window and countdown until
/// a task closes it.
void AreaNameWindow_Show(void) {
  struct FieldWork *work;
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  s32 areaMessage;
  struct Window *nameWindow;
  struct MainRoutineWork *state;

  work = TIMED_NOTICE_WORK;
  state = &gMainRoutineWork;
  x = 8;
  y = 8;
  areaMessage = Area_GetNameIndexFar(state->next, state->entrance) +
                (s32)&MsgDebugAreaName;
  Text_GetMessageSize(areaMessage, &x, &y, &width, &height);
  x = (DISPLAY_TILE_WIDTH - width) >> 1;
  y = (10 - height) >> 1;
  nameWindow = Window_Create(x, y, width, height, WINDOW_FLAG_INSTANT);
  work->noticeWindow = nameWindow;
  Text_DrawMessage(areaMessage, (s32)nameWindow, 0, 0);
  work->noticeFrames = AREA_NAME_WINDOW_FRAMES;
  Task_Add(AreaNameWindow_Update, TASK_PASS_FRAME);
}

/// Counts the area name down, and closes it at 0.
void AreaNameWindow_Update(void) {
  struct FieldWork *work;
  u16 framesLeft;

  work = TIMED_NOTICE_WORK;
  work->noticeFrames = framesLeft = work->noticeFrames - 1;
  if (framesLeft == 0) {
    Window_Close(work->noticeWindow, WINDOW_FLAG_INSTANT);
    Task_Remove(AreaNameWindow_Update);
  }
}

/// Closes the area name at once, if it is shown.
void AreaNameWindow_Hide(void) {
  struct Window *window;

  window = TIMED_NOTICE_WORK->noticeWindow;
  if (window != NULL && window->flags != 0) {
    Window_Close(window, WINDOW_FLAG_INSTANT);
    Task_Remove(AreaNameWindow_Update);
  }
}

void PartyInventory_RemoveFar(s32 item);

/// Removes itemId from the party's inventory and returns 0.
s32 Item_RemoveFromInventory(s32 unused, s32 itemId) {
  PartyInventory_RemoveFar(itemId);
  return 0;
}

#undef TIMED_NOTICE_WORK
