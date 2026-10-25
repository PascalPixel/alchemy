// Draft, differ 75 of 131: the frame callback of the room where persons 8 to 16
// sway: all stop while flag 0x106 is set or the event work holds them; else,
// once per pending step, at frame 60 of each 240 they lean one way and at 180
// come back.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

struct SwayState {
  u16 frame;
  u16 pending;
};

extern struct SwayState gSceneState;

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02002b58(s32 id, s32 x, s32 z);

void Func_02002b8c(void) {
  struct SwayState *state = &gSceneState;
  u16 *pending = &state->pending;
  void *work = Ram_HeapSlots->eventWork;
  s32 stopped = GameFlag_Test(0x106);
  s32 id;

  if (FIELD_AT(work, s16, 358) != 0)
    stopped |= 1;
  id = 8;
  do {
    Object_GetById(id++)->movementState = stopped;
  } while (id <= 16);
  if (stopped != 0)
    return;
  ++*pending;
  if (FIELD_AT(work, s16, 342) != 0)
    return;
  if (*pending == 0)
    return;
step:
  if (state->frame == 60) {
    Func_02002b58(8, 0, 3);
    Func_02002b58(9, 3, 0);
    Func_02002b58(10, 0, 3);
    Func_02002b58(11, 0, 3);
    Func_02002b58(12, 0, -3);
    Func_02002b58(13, 0, -3);
    Func_02002b58(14, 0, 3);
    Func_02002b58(15, 0, -3);
    Func_02002b58(16, 3, 0);
  } else if (state->frame == 180) {
    id = 8;
    do {
      Func_02002b58(++id, 0, 0);
    } while (id <= 16);
  }
  if (++state->frame == 240)
    state->frame = 0;
  if (--*pending != 0)
    goto step;
}
