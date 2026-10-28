#include "local.h"

void Func_020007ac(struct Actor *object) {
  u8 *work = (u8 *)Ram_HeapSlots->eventWork;
  u8 *state = &object->movementState;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0 ||
      *(u16 *)(work + 434) == 0)
    *state = 1;
  else
    *state = 0;
}
