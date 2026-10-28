#include "types.h"
#include "sequence_parent_work.h"

s32 Party_AdjustCoins(s32 amount) {
  s32 value;

  value = gSequenceParentWork.coins;
  value += amount;
  if (value > 0xf423f)
    value = 0xf423f;
  if (value < 0)
    value = 0;
  gSequenceParentWork.coins = value;
  return value;
}

s32 Party_AdjustShopCredit(s32 amount) {
  s32 value;

  value = gSequenceParentWork.counterB;
  value += amount;
  if (value > 0xf423f)
    value = 0xf423f;
  if (value < 0)
    value = 0;
  gSequenceParentWork.counterB = value;
  return value;
}

s32 Party_AdjustShopGifts(s32 amount) {
  s32 value;

  value = gSequenceParentWork.counterC;
  value += amount;
  if (value > 28)
    value = 28;
  if (value < 0)
    value = 0;
  gSequenceParentWork.counterC = value;
  return value;
}
