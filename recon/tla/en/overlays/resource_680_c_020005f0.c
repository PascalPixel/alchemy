// Draft, differ 26 of 31, skeleton gap 0 (tla-en, 2026-11-01)
// Func_020005f0 differs in 26 of 31 and Func_02000630 in 26 of 32, registers
// only. The lookup is Func_080dae70 with its id fixed at 8; the index's default
// gives the native's copy from found (adds r0, r4, #0). Left: the native keeps
// work, offset and index in r2, r1, r0; global allocation here ranks the offset
// (r2), then the index (r1), then work (r0), and work's 6 refs over 15 insns
// cannot outrank them, so the native's order needs a preference nothing in the
// source shows. Measured the same: work as an inline parameter, the lookup in
// the body, found declared first, i <= 7, the slot type's own pointer. An entry
// pointer in the loop is worse (differ 37). The permuter (3 minutes, one
// function) found nothing nearer.
#include "types.h"
#include "worka0.h"

struct A4SlotValues {
  s16 id;
  s16 unk02;
  s32 value04;
  u8 unk08[8];
  s32 value10;
  u8 unk14[8];
};

static inline struct A4SlotValues *FindSlot(s32 id) {
  struct WorkA4 *work = WORK_A4;
  struct A4SlotValues *found = NULL;
  s32 i;

  // Default: as Func_080dae70, the index zero is copied from found.
  i = 0;

  for (i = 0; i < 8; i++) {
    if (work->slots[i].id == id) {
      found = (struct A4SlotValues *)&work->slots[i];
      break;
    }
  }
  return found;
}

void Func_020005f0(void) {
  struct A4SlotValues *slot = FindSlot(8);

  slot->value10 = 0x4000;
  slot->value04 = 64;
}

void Func_02000630(void) {
  struct A4SlotValues *slot = FindSlot(8);

  slot->value10 = 0x4000;
  slot->value04 += 128;
}
