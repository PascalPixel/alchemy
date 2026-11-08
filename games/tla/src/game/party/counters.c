#include "types.h"
#include "main_routine_work.h"

s32 Party_AdjustCoins(s32 amount) {
  s32 value;

  value = gMainRoutineWork.coins;
  value += amount;
  if (value > 0xf423f)
    value = 0xf423f;
  if (value < 0)
    value = 0;
  gMainRoutineWork.coins = value;
  return value;
}

s32 Party_AdjustShopCredit(s32 amount) {
  s32 value;

  value = gMainRoutineWork.counterB;
  value += amount;
  if (value > 0xf423f)
    value = 0xf423f;
  if (value < 0)
    value = 0;
  gMainRoutineWork.counterB = value;
  return value;
}

s32 Party_AdjustShopGifts(s32 amount) {
  s32 value;

  value = gMainRoutineWork.counterC;
  value += amount;
  if (value > 28)
    value = 28;
  if (value < 0)
    value = 0;
  gMainRoutineWork.counterC = value;
  return value;
}
