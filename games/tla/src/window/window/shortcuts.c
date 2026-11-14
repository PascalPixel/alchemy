/// @file
/// The area name notice, and removing an item from the inventory.
// Part of window.c.

#include "../window_internal.h"

struct TimedNoticeWork {
  u8 unk000[0x21c];
  struct Window *window;
  u16 countdown;
};

#define TIMED_NOTICE_WORK ((struct TimedNoticeWork *)Ram_HeapSlots->fieldWork)

void AreaNameWindow_Update(void);
/// Opens the area-name window, which closes after ninety frames.
void AreaNameWindow_Show(void) {
  struct TimedNoticeWork *work;
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  s32 resource;
  struct Window *window;
  struct MainRoutineWork *state;

  work = TIMED_NOTICE_WORK;
  state = &gMainRoutineWork;
  x = 8;
  y = 8;
  resource = Area_GetNameIndexFar(state->scene, state->entrance) +
             (s32)&MsgDebugAreaName;
  Text_GetMessageSize(resource, &x, &y, &width, &height);
  x = (30 - width) >> 1;
  y = (10 - height) >> 1;
  window = Window_Create(x, y, width, height, 2);
  work->window = window;
  Text_DrawMessage(resource, (s32)window, 0, 0);
  work->countdown = 90;
  Task_Add(AreaNameWindow_Update, TASK_PASS_VBLANK);
}

/// Counts the area name down, and closes it at 0.
void AreaNameWindow_Update(void) {
  struct TimedNoticeWork *work;
  u16 *countdown;
  s32 count;

  work = TIMED_NOTICE_WORK;
  countdown = &work->countdown;
  count = *countdown;
  count += 0xffff;
  *countdown = count;
  if ((count << 16) == 0) {
    Window_Close(work->window, 2);
    Task_Remove(AreaNameWindow_Update);
  }
}

/// Closes the area name at once, if it is shown.
void AreaNameWindow_Hide(void) {
  struct Window *window;

  window = TIMED_NOTICE_WORK->window;
  if (window != NULL && window->flags != 0) {
    Window_Close(window, 2);
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
