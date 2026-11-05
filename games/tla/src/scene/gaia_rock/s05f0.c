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
