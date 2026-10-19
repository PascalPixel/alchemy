/// @file
/// Battle: identify effects that apply additional effects to defeated targets.

#include "types.h"

/// Return 1 if the additional effect applies even when the target is defeated.
/// Drain effects return damage dealt to the user.
/// effect: effect ID.
s32 BattleEffectDeadCheck(s32 effect) {
  switch (effect) {
  case 31: // Drain HP.
  case 32: // Drain PP.
  case 60: // Drain HP (half).
  case 69: // Drain PP (one tenth).
    return 1;
  default:
    return 0;
  }
}
