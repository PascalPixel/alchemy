// Draft, differ 2 of 32 (tla-en, 2026-11-05). Left: the native copies the index's
// zero from the slot pointer's (adds r0, r4, #0), here movs r0, #0; starting the
// index from the slot pointer or a shared zero local measures the same.
#include "types.h"
#include "ram_buffer.h"

struct Slot02000630 {
  s16 kind;
  s16 unk02;
  s32 position;
  u8 unk08[8];
  s32 speed;
  u8 unk14[8];
};

struct Work02000630 {
  s32 unk00;
  struct Slot02000630 slots[8];
};

// Finds a type-eight slot and advances it.
void Func_02000630(void) {
  struct Work02000630 *work = Ram_HeapSlots->unk84[8];
  struct Slot02000630 *slot = NULL;
  s32 i;

  for (i = 0; i < 8; i++) {
    if (work->slots[i].kind == 8) {
      slot = &work->slots[i];
      break;
    }
  }
  slot->speed = 0x4000;
  slot->position += 128;
}
