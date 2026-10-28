#include "types.h"
#include "field_event.h"
#include "flag_call.h"

struct FieldActor *Actor_GetById(s32 id);
void WaitFrames(s32 frames);
void Func_02000b28(s32 id);

void Func_02000680(s32 unused, s32 id) {
  struct FieldActor *object = Actor_GetById(id);
  s32 i;

  if (Flag_IsSet(0xa00) && object->x >> 20 == 18 && object->z >> 20 == 6) {
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      WaitFrames(1);
    }
    Func_02000b28(id);
    Flag_Set(0xa03);
  }
}
