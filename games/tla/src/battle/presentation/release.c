#include "types.h"
#include "io_reg.h"
#include "heap_release.h"
#include "iwram_call.h"
#include "task.h"

void Func_08127038(void);
void Func_08126e00(void);

/// Ends a star field: clears the tiles in character block 1, releases its
/// four heap slots, turns BG2 off and removes both its tasks. Called by
/// Func_08127308 once mode 0's field has played. Returns Task_Remove's
/// result, which the caller does not read.
s32 ElementGlow_End(s32 unused) {
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  HeapSlot_Release(96);
  HeapSlot_Release(92);
  REG_DISPCNT = DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  Task_Remove(Func_08127038);
  return Task_Remove(Func_08126e00);
}

void Func_08127304(void) {}
