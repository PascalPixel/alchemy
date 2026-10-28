#include "types.h"
#include "io_reg.h"
#include "mem.h"
#include "iwram_call.h"
#include "task.h"

void Func_08127038(void);
void Func_08126e00(void);

void Func_081272b0(void) {
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  REG_DISPCNT = 0x1341;
  Task_Remove(Func_08127038);
  Task_Remove(Func_08126e00);
}

void Func_08127304(void) {}
