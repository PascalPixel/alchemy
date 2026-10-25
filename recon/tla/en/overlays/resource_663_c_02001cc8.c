// Near miss, differ 2 of 95: inside an event, settles the object the event
// work holds at 0xcbc after a push (Func_020003e8 with gData_02003d94): if
// it is not on layer 2's ground it falls there (cue 188); then where it
// stops sets one of five flags (0x85f to 0x862, 0x887) for its five target
// cells. Left: the native loads the event work before starting the 0xcbc
// constant (one reordered row). Tried: 2 minutes of alchemy permute.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

extern u8 gData_02003d94[];
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003c88(s32 skip);
void Func_02003cc0(void);
void Func_020003e8(const u8 *list);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void OverlayObject_WaitUntilIdle(struct ObjectRuntime *object);
void Func_02003cd8(s32 cue);
void GameFlag_SetBit(s32 flag);

void Func_02001cc8(void) {
  struct ObjectRuntime *object =
      *(struct ObjectRuntime **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 x, z;

  Func_02003bc8();
  Func_02003c88(0);
  Func_02003cc0();
  Func_020003e8(gData_02003d94);
  if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    OverlayObject_WaitUntilIdle(object);
    OverlayObject_WaitUntilIdle(object);
    Func_02003cd8(188);
    object->motionFlags = 0;
    object->terrainId = 0;
  }
  x = object->x >> 20;
  z = object->z >> 20;
  if (x == 16 && z == 43)
    GameFlag_SetBit(0x85f);
  else if (x == 45 && z == 45)
    GameFlag_SetBit(0x860);
  else if (x == 21 && z == 48)
    GameFlag_SetBit(0x861);
  else if (x == 39 && z == 49)
    GameFlag_SetBit(0x862);
  else if (x == 49 && z == 18)
    GameFlag_SetBit(0x887);
  Func_02003bd0();
}
