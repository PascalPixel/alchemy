#include "types.h"
#include "ram_buffer.h"

struct OwnerObject {
  u8 unk00[8];
  s32 x;
  u8 unk0c[4];
  s32 z;
  u8 unk14[14];
  u8 layer;
};

struct EventWork {
  u8 unk000[0x166];
  u16 state;
};

s32 EventRuntime_GetControlledOwner(void);
struct OwnerObject *ObjectTable_Get(s32 id);
s32 Func_080dbca8(s32 x, s32 z, s32 layer);

/// Scheduler callback of Cloak: when the controlled character does not
/// stand on a tile Cloak can hide on, requests CLOAK_END (0x2092).
void FieldFx_CheckCloakTile(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct OwnerObject *owner =
      ObjectTable_Get(EventRuntime_GetControlledOwner());

  if (Func_080dbca8(owner->x, owner->z, owner->layer) == 0)
    event->state = 0x2092;
}
