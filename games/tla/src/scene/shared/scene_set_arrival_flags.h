/// @file
/// Scene_SetArrivalFlags, compiled inside each Jupiter Lighthouse scene that
/// calls it and inside Mars Lighthouse room F, at its place in each overlay.
#include "types.h"
#include "flag_call.h"

void Scene_SetArrivalFlags(s32 unused) {
  GameFlag_SetBit(0x113);
  if (GameFlag_Test(0xa25)) {
    GameFlag_SetBit(0x161);
    GameFlag_SetBit(0x144);
  }
}
