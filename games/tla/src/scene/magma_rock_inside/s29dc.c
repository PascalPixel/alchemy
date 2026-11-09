#include "local.h"


void Func_020029dc(void) {
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!Event_GetFlag(0x305))
      Actor_SetPosition(11, 0, 0);
  }
}
