// Matches (tla-en, 2026-11-07).
// The count is read straight from the work in the for loop's test, so the
// duplicated first test keeps its own address (copied to r7 after it); the
// cell is written through struct MapCell, whose own alias set lets the next
// count load go straight after the store.
#include "types.h"
#include "map.h"

extern u8 gData_02001000[];
void Func_080201f0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);

void Func_080e2718(void) {
  u8 *base = gData_02001000;
  s32 i;

  for (i = 0; i < (s8)gData_02001000[0x194c]; i++) {
    u8 *entry = base + 0x174c + i * 8;

    Func_080201f0(entry[0], entry[1], 1, 1, entry[2], entry[3]);
    Func_080dbb78(entry[4] << 20, entry[5] << 20, 2)->collisionCode = 0;
  }
}
