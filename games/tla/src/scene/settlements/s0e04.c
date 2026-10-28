#include "local.h"

void Func_02000e04(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 514;
  Task_Add(Func_02000d14, 0x480);
  Func_02002168(170);
  Func_02000ed0();
}
