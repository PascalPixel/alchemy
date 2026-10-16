// Draft, differ 10 of 53: the native adds the cell offset to the layer's cells
// (adds r2, r2, r3) and shifts z after building 0x1a8; this draft adds them the
// other way and the party state's pool load moves one slot. Tried: the index
// either way round, pointer arithmetic, z * 128, a u8 code, a minute of
// alchemy permute (differ 8 at best, with registers changed).
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/ram_buffer.h"
#include "../../../../games/tla/include/map.h"
#include "../../../../games/tla/include/object_runtime.h"
#include "../../../../games/tla/include/party_state.h"

struct ObjectRuntime *Object_GetById(u32 id);

void Func_020049e4(void) {
  if (gPartyState.renderMode == 2) {
    u8 *event = Ram_HeapSlots->eventWork;
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
    s32 x = object->x / 0x100000;
    s32 z = (object->z - object->y + 0x7fff) / 0x100000;
    struct MapCell *cells = map->layers[2].cells;
    s32 code = cells[x + (z << 7)].collisionCode;

    if (code >= 1 && code <= 230)
      *(u16 *)(event + 340) = code;
  }
}
