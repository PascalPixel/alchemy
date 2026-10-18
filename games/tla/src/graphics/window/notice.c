#include "types.h"
#include "graphics.h"
#include "window.h"
#include "callback_scheduler.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "scene.h"
#include "edition.h"

struct TimedNoticeWork {
  u8 unk000[0x21c];
  struct UiWindow *window;
  u16 countdown;
};

#define TIMED_NOTICE_WORK ((struct TimedNoticeWork *)Ram_HeapSlots->eventWork)

void UiTimedNotice_Tick(void);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 style);
extern u8 MsgDebugEntryName;

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_ITALIAN)
// The Spanish and Italian far-call tables use different entry names for
// Scene_GetAreaNameIndex.
s32 Func_080c8638(s32 scene, s32 entrance);
#define Scene_GetAreaNameIndexFar Func_080c8638
#endif
void UiText_DrawResource(s32 resource, struct UiWindow *window, s32 x, s32 y);

// Open the area-name window, then close it after ninety frames.
void UiTimedNotice_Create(void) {
  struct TimedNoticeWork *work;
  s32 height;
  s32 width;
  s32 y;
  s32 x;
  s32 resource;
  struct UiWindow *window;
  struct PartyState *state;

  work = TIMED_NOTICE_WORK;
  state = &gPartyState;
  /* FAKEMATCH: Without it the party state's address loads after the 8 (differ
   * 2); keep the load before it. */
  __asm__("" : : "r"(state));
  x = 8;
  y = 8;
  /* FAKEMATCH: Without it the scene's offset is built before the two stores of
   * 8 (differ 4); keep it after them. */
  __asm__("" : : "r"(state));
  resource = Scene_GetAreaNameIndexFar(state->scene, state->entrance) +
             (s32)&MsgDebugEntryName;
  UiText_GetResourceDimensions(resource, &x, &y, &width, &height);
  x = (30 - width) >> 1;
  y = (10 - height) >> 1;
  window = UiWindow_Create(x, y, width, height, 2);
  work->window = window;
  UiText_DrawResource(resource, window, 0, 0);
  work->countdown = 90;
  Scheduler_AddOrUpdateCallback(UiTimedNotice_Tick, 0x480);
}

void UiTimedNotice_Tick(void) {
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
    UiWork_Finalize(work->window, 2);
    Scheduler_RemoveCallback(UiTimedNotice_Tick);
  }
}

void UiTimedNotice_CloseIfActive(void) {
  struct UiWindow *window;

  window = TIMED_NOTICE_WORK->window;
  if (window != NULL && window->flags != 0) {
    UiWork_Finalize(window, 2);
    Scheduler_RemoveCallback(UiTimedNotice_Tick);
  }
}
