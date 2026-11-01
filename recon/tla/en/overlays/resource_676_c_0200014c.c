// Matches: differ 0 of 64 (tla-ja, 2026-11-01), its literal pool included.
// Japanese draft (compare with --edition tla-ja): the other editions'
// Func_0200014c, adopted in yallam/s014c.c, without the stop beside the
// leader. Its default tla-en diff (46 of 82) measures that other function.
// Twin of The Broken Seal's Vault_UpdateWalkingVillager: same partner choice
// by bit 0, same stop beside the partner, then the leader with bit 1 forcing.
// Nothing is left to port: the code already matches, the twin has no stop
// beside the leader, and its range of 18 or 26 is a flat 24 here.
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
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

    if ((*(s16 *)(event + 352) != 0 || tiles[4] != 0) && (*flags & 2))
      force = 1;
    Func_02000088(object, leader, 24, force);
  }
  return 0;
}
