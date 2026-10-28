#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "object_runtime.h"
#include "input.h"

void *FieldMessage_ShowAtActor(s32 object, s32 message);
void WaitFrames(s32 frames);
s32 ObjectTable_ReadActiveValue(s32 key);
void *Scene_FindActorPlacement(s32 id);
void Window_CloseMatchingIdFar(s32 id);
s32 Window_IsClosedFar(s32 window);
void Message_CloseFinishedWindowsFar(void);
void Func_080d4330(s32 key);

/* FAKEMATCH: each test reads the held keys again, as a volatile read; plain
 * reads are combined into one test of 0x305 (differ 52). */
#define HELD (*(volatile u32 *)&gInput.held)

/// Opens the message at the object and waits for its window; after ten
/// seconds, or with Select, R, L and A held, the text is finished at once.
void Func_080d3fb0(s32 object, s32 message) {
  s32 masked;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  void *window = FieldMessage_ShowAtActor(object, message);
  s32 id;
  u32 frames;

  frames = 0;
  WaitFrames(1);
  id = ObjectTable_ReadActiveValue(object);
  if (object <= 7) {
    masked = object & 0x0fff;
    if (Scene_FindActorPlacement(masked) == 0)
      id = masked;
  }
  Window_CloseMatchingIdFar(id);

  if (work->waitMode == 0) {
    while (Window_IsClosedFar((s32)window) == 0) {
      WaitFrames(1);
      frames++;
      if (frames > 600 ||
          ((HELD & 4) && (HELD & 0x100) && (HELD & 0x200) && (HELD & 1)))
        Message_CloseFinishedWindowsFar();
    }
  }

  Func_080d4330(0x80000000);
  WaitFrames(1);
}

void EventRuntime_Wait(s32 frames);

void EventRuntime_ShowMessageThenWait(s32 object, s32 message, s32 frames) {
  Func_080d3fb0(object, message);
  EventRuntime_Wait(frames);
}

void Func_080d3fb0(s32 object, s32 message);

void EventRuntime_ShowMessageAndWait(s32 object, s32 message) {
  Func_080d3fb0(object, message);
}

void *FieldMessage_ShowAtActor(s32 object, s32 message);
s32 EventRuntime_GetControlledOwner(void);
s32 Event_AskYesNoWithActorAnimation(s32 owner, s32 mode);
void Func_080d3fb0(s32 object, s32 message);

s32 EventRuntime_PromptInventoryMessage(s32 object, s32 message) {
  s32 result;

  FieldMessage_ShowAtActor(object, message);
  result =
      Event_AskYesNoWithActorAnimation(EventRuntime_GetControlledOwner(), 0);
  if (result == 0) {
    Func_080d3fb0(object, message);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    Func_080d3fb0(object, message);
  }
  return result;
}

void EventRuntime_Noop(void) {}

s32 Message_OpenWindowFar(s32 message, s32 x, s32 y, s32 mode);

/// Opens the next message for the speaker and waits until its window is idle.
void Func_080d40dc(s32 speaker) {
  // BUG: the position is never set; the window is placed from whatever the
  // two registers hold.
  struct {
    s32 x, y;
  } pos;
  struct EventWork *work = Ram_HeapSlots->eventWork;

  speaker &= 0xfff;
  ObjectTable_Get(speaker);
  work->currentOwner = speaker;
  if (work->waitMode == 0) {
    s32 px = pos.x;
    s32 py = pos.y;
    s32 window;

    if (py > 119)
      py += 32;
    else
      py -= 32;
    px = px < 8 ? 8 : px;
    if (px > 312)
      px = 312;
    py = py < 20 ? 20 : py;
    if (py > 220)
      py = 220;
    window = Message_OpenWindowFar((s16)work->messageId, px, py, 1);
    work->objectStatus = window;
    while (Window_IsClosedFar(window) == 0)
      WaitFrames(1);
  }
  work->messageId++;
}
