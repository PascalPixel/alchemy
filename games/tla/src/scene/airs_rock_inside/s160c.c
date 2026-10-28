#include "airs_rock_inside.h"

s32 Random16Far(void);
void Func_02001458(void);

void Func_0200160c(s32 x, s32 y, s32 z) {
  struct FieldActor *object = Import_ActorCreate(0x11d, x, y, z);

  Func_02005d5c(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Actor_SetPartAttribute(object, 6);
  Animation_SetStateFlags(object, 1);
  object->timer = 27;
  object->motionFlags = 0;
  BYTE_AT(object, 90) = 0;
  WORD_AT(object, 24) = 0x8000;
  object->speed = 0x40000;
  object->acceleration = ((Random16Far() & 0x7fff) << 1) + 0xffff8001;
  WORD_AT(object, 76) = 0x3800;
  WORD_AT(object, 108) = (s32)Func_02001458;
}
