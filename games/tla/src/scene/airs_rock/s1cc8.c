#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event_inline.h"

extern u8 gData_02003d94[];
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003cc0(void);
void Func_020003e8(const u8 *list);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void OverlayObject_WaitUntilIdle(struct Actor *object);

void Func_02001cc8(void) {
  struct Actor *object =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 x, z;

  Func_02003bc8();
  FieldEvent_ResolvePendingActions(0);
  Func_02003cc0();
  Func_020003e8(gData_02003d94);
  if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    OverlayObject_WaitUntilIdle(object);
    OverlayObject_WaitUntilIdle(object);
    Event_PlaySound(188);
    object->motionFlags = 0;
    object->terrainId = 0;
  }
  x = object->x >> 20;
  z = object->z >> 20;
  if (x == 16 && z == 43)
    Event_SetFlag(0x85f);
  else if (x == 45 && z == 45)
    Event_SetFlag(0x860);
  else if (x == 21 && z == 48)
    Event_SetFlag(0x861);
  else if (x == 39 && z == 49)
    Event_SetFlag(0x862);
  else if (x == 49 && z == 18)
    Event_SetFlag(0x887);
  Func_02003bd0();
}
