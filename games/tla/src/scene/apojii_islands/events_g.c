#include "local.h"

void Func_020010b4(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  GameFlag_ClearBit(0x12f);
}

void Func_020010d0(void) { Func_02001d9c(); }
