#include "types.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"

struct DisplayBlock {
  u8 unk000[0x18a];
  s8 mode;
  s8 pending;
  u8 unk18c[0x40];
  u8 *object;
};

struct DisplayBlock *HeapSlot_AllocEwram(s32 slot, s32 size);
void Func_08020270(void);

void Func_080cb8a4(void) {
  struct DisplayBlock *block = HeapSlot_AllocEwram(108, 0xd50);

  if (block->mode == 3 && block->pending != 0) {
    block->pending = 0;
    block->object[91] = 0;
    Func_08020270();
  }
}

void *Func_080cb8e8(void) {
  struct SequenceParentWork *party = &gSequenceParentWork;
  void **event;

  /* FAKEMATCH: gSequenceParentWork's address is loaded first; without the barrier the
   * two constants beside it are begun before it (2 differ) */
  asm volatile("" : "+l"(party));
  event = Ram_HeapSlots->eventWork;
  if (party->ownerOverride == 0) {
    s32 offset = party->currentOwner * 4;

    offset += 20;
    event = *(void **)((u8 *)event + offset);
  } else {
    event = event[13];
  }
  return event;
}
