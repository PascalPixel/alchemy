// Near miss, differ 2 of 24: the native loads the work pointer before it
// starts the 0x7c4 offset constant (ldr r3, [r3]; movs r2, #224); this draft
// starts the constant first. A local pointer, a struct field, an empty asm
// and a register-use barrier all leave the same order.
#include "../../../../../games/tla/src/scene/yampi_desert/scene.h"

void Func_02002c6c(void) {
  u8 *work = Ram_HeapSlots->effectWork;

  if (*(s16 *)(work + 0x7c4) != 0)
    Func_02003ff0(73, 76, 78, 11, 1, 1);
}
