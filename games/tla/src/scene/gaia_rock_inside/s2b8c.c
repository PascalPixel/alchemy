/// @file
/// The frame callback of the room where persons 8 to 16 sway: all of them
/// stop while flag 0x106 is set or the event work holds them; otherwise, once
/// per pending step, at frame 60 of every 240 they lean one way and at frame
/// 180 they come back.

#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "event_inline.h"

struct SwayState {
  u16 frame;
  u16 pending;
};

extern struct SwayState gSceneState;

void Func_02002b58(s32 id, s32 x, s32 z);

void Func_02002b8c(void) {
  // Pointers that stay on one global for their whole life.
  struct SwayState *state = &gSceneState;
  u16 *pending = &state->pending;
  u16 *frame = &state->frame;
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 stopped = Event_GetFlag(0x106);
  s32 id;

  if (work->unk166 != 0)
    stopped |= 1;
  id = 8;
  do {
    Actor_Get(id++)->movementState = stopped;
  } while (id <= 16);
  if (stopped != 0)
    return;
  ++*pending;
  if (work->unk156 != 0)
    return;
  if (*pending == 0)
    return;
step:
  switch (*frame) {
  case 60:
    Func_02002b58(8, 0, 3);
    Func_02002b58(9, 3, 0);
    Func_02002b58(10, 0, 3);
    Func_02002b58(11, 0, 3);
    Func_02002b58(12, 0, -3);
    Func_02002b58(13, 0, -3);
    Func_02002b58(14, 0, 3);
    Func_02002b58(15, 0, -3);
    Func_02002b58(16, 3, 0);
    break;
  case 180:
    id = 8;
    do {
      Func_02002b58(id, 0, 0);
      id++;
    } while (id <= 16);
    break;
  }
  if (++*frame == 240)
    *frame = 0;
  if (--*pending != 0)
    goto step;
}
