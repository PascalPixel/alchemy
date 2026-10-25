// Draft, differ 50 of 93: fills record `index` of the table at
// gData_020023c4 + 0x188 (28 bytes, from byte 4) for an actor that swings
// between two limits: unless flag 265 is set it records the actor, its
// start, the limits in order and the swing's constants (360, 60, 240) and
// makes the actor solid; it always creates the actor's shadow (type 0x8000)
// and keeps it in the record. Returns the record. The same listing as
// overlay 661's Func_02003e3c and 697's Func_02004224. Left: the native
// stores the zeros from GameFlag_Test's result and a zero loaded from the
// literal pool, and keeps the arguments in r8 to r10.
#include "types.h"
#include "object_runtime.h"

extern u8 gData_020023c4[];
struct ObjectRuntime *Object_GetById(u32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);

u16 *Func_02001068(s32 index, s32 actor, s32 start, s32 low, s32 high) {
  u16 *record = (u16 *)(gData_020023c4 + 0x188 + index * 28 + 4);
  struct ObjectRuntime *object = Object_GetById(actor);
  struct ObjectRuntime *shadow;
  s32 swap;

  Object_GetById(actor);
  ObjectDispatch_SetSingleChildFlags(object, 0);
  if (!GameFlag_Test(265)) {
    if (high < low) {
      swap = low;
      low = high;
      high = swap;
    }
    record[0] = actor;
    record[1] = start;
    record[3] = 360;
    record[4] = 60;
    record[5] = 240;
    record[7] = low;
    record[8] = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    object->priorityFlags |= 2;
    record[2] = 0;
    record[6] = high;
    record[9] = 0;
    record[11] = 0;
    record[10] = 0;
  }
  shadow = Engine_ObjectCreate(0x8000, object->x, object->y, object->z);
  shadow->collisionFlags = 0;
  shadow->motionFlags = 0;
  shadow->priorityFlags |= 128;
  *(struct ObjectRuntime **)(record + 12) = shadow;
  return record;
}
