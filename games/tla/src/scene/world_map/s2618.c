#include "types.h"
#include "actor.h"
#include "event_inline.h"

static __inline__ void PosSet(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: as in ☀️'s scene layer, the command takes pixels and shifts them
   * into fixed point inside an inline; a direct call and a forwarding inline
   * give id and dz each other's register (8 differ). */
  Actor_SetHorizontalPositionWithTerrain(id, x << 16, z << 16);
}

void Func_02002618(s32 id, s32 dx, s32 dz) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL)
    PosSet(id, *(s16 *)((u8 *)object + 10) + dx,
           *(s16 *)((u8 *)object + 18) + dz);
}
