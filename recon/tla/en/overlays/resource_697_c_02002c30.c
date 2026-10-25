// Draft, differ 80 of 80: deals the Trial Road's stage order from seed: shuffles
// 0 to 7 into the eight slots at the start of the stage state, again until both
// the even and the odd slots hold at least one of the four values in
// gData_02005be0; the random state is restored after.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

extern u32 gRandomState;
extern const s8 gData_02005be0[4];

u32 Random16Far(void);

void Func_02002c30(u32 seed) {
  s16 *order = (s16 *)gTrialRoadState;
  u32 saved = gRandomState;
  s32 counts[2];
  s16 *slot;
  const s8 *special;
  s32 i;
  s32 k;
  s32 index;

  gRandomState = seed;
deal:
  slot = order;
  i = 7;
clear:
  *slot++ = -1;
  if (--i >= 0)
    goto clear;
  i = 0;
draw:
  index = (Random16Far() << 3) >> 16;
  if (*(order + index) != -1)
    goto draw;
  *(order + index) = i;
  if (++i <= 7)
    goto draw;
  counts[1] = 0;
  counts[0] = 0;
  slot = order;
  i = 0;
count:
  special = gData_02005be0;
  k = 0;
compare:
  if (*slot == *special++)
    counts[i & 1]++;
  if (++k <= 3)
    goto compare;
  i++;
  slot++;
  if (i <= 7)
    goto count;
  if (counts[0] == 0 || counts[1] == 0)
    goto deal;
  gRandomState = saved;
}
