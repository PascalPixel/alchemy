#include "types.h"
#include "ram_buffer.h"
#include "edition.h"

// The letters cycle through CH_CNT values: 26 in the Japanese release, 9
// elsewhere, as first_digit.c reads them back.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CH_CNT 26
#else
#define CH_CNT 9
#endif

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
