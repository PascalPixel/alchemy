#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "call.h"
#include "adventure.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

struct ObjectRuntime *Object_GetById(s32 id);
void Vector_AddPolarOffsetFar();
void Func_0200120c(void);
void Func_020012e4(s32);
void Object_TurnToFaceEachOther(s32, s32, s32);
void Func_0200128c(s32);
void Func_02001294(s32, s32);
void ObjectMotion_ArmCallback();
void Func_02001214(void);

void Func_02000660(void) {
  struct ObjectRuntime *object = Object_GetById(gAdventure.currentOwner);
  struct {
    s32 x, y, z;
  } position;
  s32 *out = (s32 *)&position;

  out[0] = object->x;
  out[1] = object->y;
  out[2] = object->z;
  Call3(Vector_AddPolarOffsetFar, 0x100000, object->angle, (s32)out);
  if ((position.x >> 20) == 119 && (position.z >> 20) == 45) {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  } else {
    Func_0200120c();
    Func_020012e4(0);
    Object_TurnToFaceEachOther(16, gAdventure.currentOwner, 0);
    Func_0200128c((s32)MsgRrruffRrruff2);
    Func_02001294(16, 0);
    Call3(ObjectMotion_ArmCallback, 16, 0x8000, 0);
    Func_02001214();
  }
}
