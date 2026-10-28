#include "types.h"
#include "graphics.h"
#include "window.h"
#include "task.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene.h"
#include "edition.h"

struct TimedNoticeWork {
  u8 unk000[0x21c];
  struct Window *window;
  u16 countdown;
};

#define TIMED_NOTICE_WORK ((struct TimedNoticeWork *)Ram_HeapSlots->eventWork)

void UiTimedNotice_Update(void);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);
extern u8 MsgDebugAreaName;

void Text_DrawMessage(s32 resource, struct Window *window, s32 x, s32 y);

/// Opens the area-name window, which closes after ninety frames.
void UiTimedNotice_Create(void) {
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
  /* FAKEMATCH: Without it gMainRoutineWork's address loads after the 8 (differ
   * 2); keep the load before it. */
  __asm__("" : : "r"(state));
  x = 8;
  y = 8;
  /* FAKEMATCH: Without it the scene's offset is built before the two stores of
   * 8 (differ 4); keep it after them. */
  __asm__("" : : "r"(state));
  resource = Scene_GetAreaNameIndexFar(state->scene, state->entrance) +
             (s32)&MsgDebugAreaName;
  Text_GetMessageSize(resource, &x, &y, &width, &height);
  x = (30 - width) >> 1;
  y = (10 - height) >> 1;
  window = Window_Create(x, y, width, height, 2);
  work->window = window;
  Text_DrawMessage(resource, window, 0, 0);
  work->countdown = 90;
  Task_Add(UiTimedNotice_Update, TASK_PASS_VBLANK);
}

void UiTimedNotice_Update(void) {
  struct TimedNoticeWork *work;
  u16 *countdown;
  s32 count;

  work = TIMED_NOTICE_WORK;
  countdown = &work->countdown;
  count = *countdown;
  /* FAKEMATCH: Without it the 0xffff starts before the countdown loads (differ
   * 2); keep the load first. */
  __asm__("" : : "r"(count));
  count += 0xffff;
  *countdown = count;
  if ((count << 16) == 0) {
    Window_Close(work->window, 2);
    Task_Remove(UiTimedNotice_Update);
  }
}

void UiTimedNotice_CloseIfActive(void) {
  struct Window *window;

  window = TIMED_NOTICE_WORK->window;
  if (window != NULL && window->flags != 0) {
    Window_Close(window, 2);
    Task_Remove(UiTimedNotice_Update);
  }
}
