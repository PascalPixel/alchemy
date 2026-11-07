#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

void HeapSlot_Release(s32 slot);

struct BattlePresentationTransition {
  u8 unk00[8];
  s32 mode;
};

void *HeapSlot_AllocEwram(s32 slot, s32 size);

void Func_081263c4(void) {
  void *block = HeapSlot_AllocEwram(40, 0x2a0);
  struct BattlePresentationTransition *work = Ram_HeapSlots->transitionWork;

  /* FAKEMATCH: a one-pass block after the work load copies the size after the
   * entry address is loaded; without it the size is copied first (differ 4). */
  do {
  } while (0);
  Iwram_FillWords(block, 0x2a0, 0);
  work->mode = 0;
}

void Func_081263f0(void) { HeapSlot_Release(40); }
