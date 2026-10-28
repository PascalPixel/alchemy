#include "types.h"
#include "heap_release.h"
#include "resource.h"

extern u8 gSceneState[];
extern const u8 gField_CachedTilesA[];
void Func_020055cc(const u8 *data, void *buffer);

s32 Func_020038f4(u32 x, u32 z) {
  u8 *cells = gSceneState;

  if (x > 10 || z > 7)
    return 0;
  {
    u8 *cell = cells + (z * 11 + x) * 4;
    s32 code;

    if (cell[3] != 0 || cell[1] != 0)
      return 1;
    code = cell[0];
    code ^= 10;
    return 1 - ((u32)(-code | code) >> 31);
  }
}
s32 Func_02003934(s32 x, s32 z) {
  s32 found;

  if (Func_020038f4(x, z - 1) || Func_020038f4(x, z + 1) ||
      Func_020038f4(x - 1, z))
    return 1;
  found = Func_020038f4(x + 1, z);
  return (u32)(-found | found) >> 31;
}
