/// @file
/// Places the event work's listed blocks.

#include "types.h"
#include "map.h"

extern u8 gData_02001000[];
void Func_080201f0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);

/// For each entry of the event work's list at 0x174c, calls Func_080201f0 with
/// its bytes and clears the collision code of the cell at its position.
void Func_080e2718(void) {
  u8 *base = gData_02001000;
  s32 i;

  for (i = 0; i < (s8)gData_02001000[0x194c]; i++) {
    u8 *entry = base + 0x174c + i * 8;

    Func_080201f0(entry[0], entry[1], 1, 1, entry[2], entry[3]);
    Func_080dbb78(entry[4] << 20, entry[5] << 20, 2)->collisionCode = 0;
  }
}
