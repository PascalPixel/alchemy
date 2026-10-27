// Twin: The Broken Seal's BattleEffect_ConfigureEffectDisplay.
#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "system.h"

void Func_0813ba50(void) {
  REG_BLDCNT = 0;
  REG_BLDALPHA = 0x100e;
  REG_WIN0H = 0x00f0;
  REG_WIN0V = 0x1088;
  REG_WIN1H = 0x00f0;
  REG_WIN1V = 0x1088;
  REG_WININ = 0x3537;
  REG_WINOUT = 0x3f21;
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  WaitTicks(1);
}
