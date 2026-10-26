#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "edition.h"

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
// European far-call tables use a different name for this entry.
#define Func_080201d0 Func_080201a0
#endif

s32 GetWorldMapCollisionFar(s32 *position);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);
s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbda8(s32 x, s32 z, s32 layer);
s32 Func_080dbd5c(s32 x, s32 z, s32 layer);
s32 Func_080201d0(s32 layer, s32 x, s32 z);

s32 Func_080dbc04(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;

  if (*(s8 *)(event + 394) == 3)
    return Func_080dbcd8(x, z, layer);
  if (Func_080dbda8(x, z, layer) != 0 || Func_080dbd5c(x, z, layer) != 0 ||
      Func_080201d0(layer, x, z) != 0)
    return 0;
  if (Func_080dbcd8(x, z, layer) != 0)
    return 1;
  if (*(s8 *)(event + 0xd36) == 0)
    return Func_080dbb78(x, z, layer)->attributeB & 1;
  return ~Func_080dbb78(x, z, layer)->attributeB & 1;
}

s32 Func_080dbca8(s32 x, s32 z) {
  return Func_080dbb78(x, z, 2)->collisionCode == 231;
}

s32 Func_080dbcc0(s32 x, s32 z) {
  return Func_080dbb78(x, z, 2)->collisionCode == 236;
}
