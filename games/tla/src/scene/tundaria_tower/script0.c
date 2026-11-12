#include "types.h"
#include "event_inline.h"

void Func_02000280(void) {
  GameFlag_SetBit(513);
  Func_080d2b4c(8, 52);
}

#include "../../field/object/wait_until_idle.c"

#include "../../field/map/cell_get.c"
