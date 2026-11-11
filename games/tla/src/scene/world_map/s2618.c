#include "types.h"
#include "actor.h"
#include "event_inline.h"

static __inline__ void PosSet(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): as in the scene layer of The Broken Seal, the command
   * takes pixels and shifts them into fixed point inside an inline; as a direct
   * call or a forwarding inline, id and dz take each other's register and 8
   * instructions differ. */
  Actor_SetHorizontalPositionWithTerrain(id, x << 16, z << 16);
}

void Func_02002618(s32 id, s32 dx, s32 dz) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL)
    PosSet(id, *(s16 *)((u8 *)object + 10) + dx,
           *(s16 *)((u8 *)object + 18) + dz);
}
