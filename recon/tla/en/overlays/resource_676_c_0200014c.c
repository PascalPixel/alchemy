// Japanese draft (compare with --edition tla-ja), differ 4 of 64: the other
// editions' Func_0200014c, adopted in yallam/s014c.c, without the stop beside
// the leader. The native loads the window tiles before it copies the object
// to r5, and the flags before the event work; here both pairs come the other
// way round. Tried 2026-10-19: the flags read into a local ahead of the event
// work (10), an empty asm holding the tiles (37).
#include "../../../../games/tla/src/scene/yallam/yallam.h"

s32 Func_0200014c(struct Actor *object) {
  u8 *tiles = Ram_HeapSlots->windowTiles;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 force = 0;
  struct Actor *target;
  u16 *flags = (u16 *)&object->action;

  if (*flags & 1)
    target = Actor_GetById(10);
  else
    target = Actor_GetById(9);
  if (Func_02000088(object, target, 24, 0) == 0) {
    struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

    if ((*(s16 *)(event + 352) != 0 || tiles[4] != 0) && (*flags & 2))
      force = 1;
    Func_02000088(object, leader, 24, force);
  }
  return 0;
}
