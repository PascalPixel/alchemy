/// @file
/// The distance between two positions, by which Yallam's and Prox's actors
/// react to who is near; Magma Rock and Venus Lighthouse keep a copy nothing
/// calls.

#include "types.h"
#include "fixed_math.h"
#include "system/iwram_call.h"

/// The distance in whole pixels between two positions, each given by its x
/// and read on through the y and z that follow it, as an actor holds them.
// UB: an actor's x, y and z are separate members, read here as an array.
s32 SceneActor_CalculateFixedPointDistance(s32 *first, s32 *second) {
  s32 deltaX = FIXEDtoINT(*first++ - *second++);
  s32 deltaY = FIXEDtoINT(*first++ - *second++);
  s32 deltaZ = FIXEDtoINT(*first - *second);

  return Iwram_Sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}
