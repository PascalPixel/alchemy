// Matches: differ 0 of 96 (tla-en, 2026-10-30), through one barrier. Plain, the
// retry loop ties the stored channel (strb r0) with the used mask's address copy
// (adds r3, r7) at equal priority in sched2, and the tie-break issues the copy
// first (differ 2). Tried before: an empty asm at the top (differ 4), an empty
// one-pass block there (2), alchemy permute 4 minutes (2). Tried 2026-10-30: a
// one-pass block around the store or the test (94, 99: loop notes inside the
// retry loop), the same store in both arms of a test (81), a volatile store
// (115), temporaries for the mask or the bit (12, 2).
#include "types.h"
#include "ram_buffer.h"

#define CH_CNT 9

struct SummonChargeState {
  u8 unk00[0x10];
  u16 classIds[6];
  u32 usedMasks[6];
  s8 channels[6];
  u8 unk3a[6];
  u8 count;
};

s32 BattleEnemy_ClaimNameLetter(s32 no, s32 n) {
  struct SummonChargeState *w;
  s32 num;
  s32 i;
  s32 retry;

  w = (struct SummonChargeState *)Ram_HeapSlots->battleWork;
  num = w->count;
  for (i = 0; i < num; i++) {
    if (w->classIds[i] == no)
      break;
  }
  if (i != num) {
    retry = 0;
    if (w->channels[i] < 0) {
      w->channels[i] = 1;
      w->usedMasks[i] = 3;
      return 0x8001;
    }
    for (; retry <= 31; retry++) {
      w->channels[i] = (w->channels[i] + 1) % CH_CNT;
      /* FAKEMATCH(barrier): an empty asm after the store keeps the mask's
       * address from issuing before it; as the plain loop, 2 of 96
       * instructions differ in tla-en. */
      asm("");
      if ((w->usedMasks[i] & (1 << w->channels[i])) == 0)
        break;
    }
    w->usedMasks[i] |= 1 << w->channels[i];
    return w->channels[i];
  }
  if (num <= 4) {
    w->channels[num] = -1;
    w->classIds[num] = no;
    w->usedMasks[num] = 0;
    w->count = num + 1;
    return CH_CNT;
  }
  return -1;
}
