#include "types.h"
#include "iwram_call.h"

// Several maps (Mercury and Venus Lighthouses' aeries, Tolbi, Vault, Venus
// Lighthouse, Xian, and every map with object_staged_object_event.h): the
// distance between two object events.

/// The distance in whole pixels between two 16.16 positions of x, y and z.
/// Callers take byte views of complete object events; their coordinate members
/// do not form an array.
UNUSED static s32 FixedPoint_Distance(const u8 *firstPosition,
                                      const u8 *secondPosition) {
  s32 deltaX =
      (*(const s32 *)firstPosition - *(const s32 *)secondPosition) >> 16;
  s32 deltaY;
  s32 deltaZ;
  s32 deltaXSquared;
  s32 deltaYSquared;
  s32 deltaZSquared;

  firstPosition += sizeof(s32);
  secondPosition += sizeof(s32);
  deltaY = (*(const s32 *)firstPosition - *(const s32 *)secondPosition) >> 16;
  firstPosition += sizeof(s32);
  secondPosition += sizeof(s32);
  deltaZ = (*(const s32 *)firstPosition - *(const s32 *)secondPosition) >> 16;
  deltaXSquared = deltaX * deltaX;
  deltaYSquared = deltaY * deltaY;
  deltaZSquared = deltaZ * deltaZ;
  return Iwram_Sqrt(deltaXSquared + deltaYSquared + deltaZSquared);
}
