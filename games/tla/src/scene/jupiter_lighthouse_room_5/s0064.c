#include "types.h"
#include "field_event.h"

struct FieldActor *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void WaitTicks(s32 ticks);
void Func_02000e04(s32 id);
void Func_020029d8(void);

void Func_02000064(s32 unused, s32 id) {
  struct FieldActor *object;
  s32 i;

  object = Object_GetById(id);
  if (id == 11) {
    if (!GameFlag_Test(0xa00) || object->x >> 20 != 13 || object->z >> 20 != 34)
      return;
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      WaitTicks(1);
    }
    Func_02000e04(id);
    GameFlag_SetBit(0xa09);
  } else {
    Func_020029d8();
  }
}
