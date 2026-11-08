/// @file
/// Deals the Trial Road's stage order from a seed: shuffles 0 to 7 into the eight
/// slots at the start of the stage state, again until both the even and the odd
/// slots hold at least one of the four values in gData_02005be0; the random state
/// is restored after.
#include "trial_road.h"

extern u32 gRandomState;
extern const s8 gData_02005be0[4];

u32 Random16Far(void);

void Func_02002c30(u32 seed) {
  s16 *order = (s16 *)gTrialRoadState;
  u32 saved = gRandomState;
  s32 counts[2];
  s32 i;
  s32 k;
  s32 offset;
  s8 value;

  gRandomState = seed;
  do {
    for (i = 0; i <= 7; i++)
      order[i] = -1;
    for (i = 0; i <= 7; i++) {
      do {
        offset = ((Random16Far() << 3) >> 16) * 2;
      } while (*(s16 *)((u8 *)order + offset) != -1);
      *(s16 *)((u8 *)order + offset) = i;
    }
    counts[1] = 0;
    counts[0] = 0;
    for (i = 0; i <= 7; i++) {
      for (k = 0; k <= 3; k++) {
        value = gData_02005be0[k];
        if (order[i] == value)
          counts[i & 1]++;
      }
    }
  } while (counts[0] == 0 || counts[1] == 0);
  gRandomState = saved;
}
