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

s32 EnemyCountDecrement(s32 no, s32 n) {
  struct SummonChargeState *w;
  s32 num;
  s32 i;
  s32 retry;
  s32 ch;

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
      ch = (w->channels[i] + 1) % CH_CNT;
      w->channels[i] = ch;
      if ((w->usedMasks[i] & (1 << (s8)ch)) == 0)
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
