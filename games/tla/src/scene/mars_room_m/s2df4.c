#include "mars_room_m.h"

void Func_02002df4(s32 sourceId, s32 id, s32 value, s32 angle,
                   const void *script) {
  struct ObjectRuntime *source = Object_GetById(sourceId);
  struct ObjectRuntime *object = Object_GetById(id);

  Object_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Func_02003120(object, script);
  ObjectDispatch_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  Func_02003118(object, 1);
  object->linkedObject = source;
  object->action = 0;
  object->unk66 = angle;
  ((u8 *)object)[0x62] = value;
  object->update = Func_02002ad4;
}
