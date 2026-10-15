#include "trial_road.h"

extern const s8 gData_02005a88[];

void Func_020009ac(s32 a, s32 b) {
  s32 id;

  id = 18;
  /* FAKEMATCH: both loops are written with goto: as for or do-while loops the
   * table address and the 1 are moved out of them (differ 29), which the native
   * does not do. */
next:
  {
    struct ObjectRuntime *object = Object_GetById(id);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    u8 *covered =
        &OBJECT_BYTE(Object_GetById(gData_02005a88[(id - 18) & 3]), 99);
    s32 other;

    other = 15;
    *covered = 0;
    goto test;
  step:
    other++;
  test:
    if (other <= 17) {
      object = Object_GetById(other);
      if (x != object->x >> 20)
        goto step;
      if (z != object->z >> 20)
        goto step;
      *covered = 1;
    }
  }
  id++;
  if (id <= 21)
    goto next;
}
