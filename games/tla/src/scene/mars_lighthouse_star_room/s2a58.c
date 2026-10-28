#include "mars_lighthouse_star_room.h"

void Func_02002a58(s32 sourceId, s32 id, s32 kind, s32 value,
                   const void *script) {
  struct Actor *source = Actor_GetById(sourceId);
  struct Actor *object = Actor_GetById(id);

  Actor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Func_020041dc(object, script);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  Func_020041cc(object, 1);
  *(struct Actor **)((u8 *)object + 0x68) = source;
  object->action = 0;
  *(s16 *)((u8 *)object + 0x66) = value;
  *(u8 *)((u8 *)object + 0x62) = kind;
  object->update = Func_02002738;
}
