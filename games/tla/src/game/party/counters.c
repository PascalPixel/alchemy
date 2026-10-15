#include "types.h"
#include "party_state.h"

s32 Party_AdjustCoins(s32 amount) {
  s32 value;

  value = gPartyState.coins;
  value += amount;
  if (value > 0xf423f)
    value = 0xf423f;
  if (value < 0)
    value = 0;
  gPartyState.coins = value;
  return value;
}

s32 Party_AdjustSixDigitCounterB(s32 amount) {
  s32 value;

  value = gPartyState.counterB;
  value += amount;
  if (value > 0xf423f)
    value = 0xf423f;
  if (value < 0)
    value = 0;
  gPartyState.counterB = value;
  return value;
}

s32 Party_AdjustCounterCappedAt28(s32 amount) {
  s32 value;

  value = gPartyState.counterC;
  value += amount;
  if (value > 28)
    value = 28;
  if (value < 0)
    value = 0;
  gPartyState.counterC = value;
  return value;
}
