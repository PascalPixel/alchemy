#include "sea_of_time.h"

s32 Func_02001ec8(void) {
  void *block = Ram_HeapSlots->block32;

  Dma_Fill32(-1, block, 0x1c0);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  return 0;
}
