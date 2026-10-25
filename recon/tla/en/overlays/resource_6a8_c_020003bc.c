// Draft of overlay 6a8's Func_020003bc in Japanese, the one edition still on
// its listing (English to Italian link games/tla/src/scene/prox/s03bc.c):
// differ 2 of 67 in tla-ja. Left: the native copies the actor into r5 after
// it loads the window tiles, where this copies it first; a barrier after that
// load, a pin on it and the locals assigned apart do not move the copy.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 Func_020002d8(struct ObjectRuntime *actor, struct ObjectRuntime *target,
                  s32 distance, s32 flag);

s32 Func_020003bc(struct ObjectRuntime *actor) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *work = Ram_HeapSlots->eventWork;
  s32 flag = 0;
  struct ObjectRuntime *target;

  if (actor->action == 0)
    target = Object_GetById(18);
  else if (actor->action == 1)
    target = Object_GetById(16);
  else
    target = Object_GetById(17);
  if (Func_020002d8(actor, target, 24, 0) == 0) {
    target = Object_GetById(gPartyState.currentOwner);
    if ((*(s16 *)(work + 352) != 0 || tiles[4] != 0) && (actor->action & 2))
      flag = 1;
    Func_020002d8(actor, target, 32, flag);
  }
  return 0;
}
