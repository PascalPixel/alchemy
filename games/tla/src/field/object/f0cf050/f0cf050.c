/// @file
/// One routine not yet understood.
// Part of f0cf050.c.

#include "../f0cf050_internal.h"
#include "module/system/system.h"

/* VIEW(f0cf050): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 distance, u16 angle, s32 *position); module/system/system.h has void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position). */
extern void Vector_AddPolarOffset__f0cf050(s32 distance, u16 angle, s32 *position) __asm__("Vector_AddPolarOffset");

void Func_080cf050(void) {
  struct Actor *leader;
  struct Actor *other;
  s32 dx;
  s32 dz;
  s32 i;

  leader = ObjectTable_Get(Party_GetLeaderActor());
  for (i = 0; i <= 15; i++) {
    other = ObjectTable_Get(i + 64);
    dx = leader->x - other->x;
    dz = leader->z - other->z;
    if (dx > -0x100000 && dx < 0x100000)
      if (dz > -0x100000 && dz < 0x100000) {
        leader->x = other->x;
        leader->z = other->z;
        Vector_AddPolarOffset__f0cf050(0x140000, ArcTan2(dz, dx), &leader->x);
        leader->targetX = 0x80000000;
        leader->targetY = 0x80000000;
        leader->targetZ = 0x80000000;
        break;
      }
  }
}
