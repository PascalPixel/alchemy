#include "types.h"
#include "iwram_call.h"

// Several scenes (Mercury and Venus Lighthouses' aeries, Tolbi, Vault, Venus
// Lighthouse, Xian, and every scene with object_staged_actor.h): the distance
// between two actors.

/// The distance in whole pixels between two 16.16 positions of x, y and z.
static s32 FixedPoint_Distance(s32 *first_position, s32 *second_position) {
  s32 delta_x = (*first_position++ - *second_position++) >> 16;
  s32 delta_y = (*first_position++ - *second_position++) >> 16;
  s32 delta_z = (*first_position - *second_position) >> 16;
  s32 delta_x_squared = delta_x * delta_x;
  s32 delta_y_squared = delta_y * delta_y;
  s32 delta_z_squared = delta_z * delta_z;

  return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}
