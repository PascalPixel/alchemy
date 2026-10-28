/// @file
/// Battle: identify effects that apply additional effects to defeated targets.

#include "types.h"

/// Returns 1 for an effect that still applies when the ability's damage has
/// just knocked its target out: the drains, which give the damage they dealt
/// back to the user.
/// effect: effect ID.
s32 BattleEffect_CheckDead(s32 effect) {
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
