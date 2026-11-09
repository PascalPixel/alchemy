#include "local.h"

void Func_0200030c(void) {
  Event_SetFlag(0x205);
  Event_SetFlag(0x206);
  Field_RequestDjinnBattle(17, 35);
}

s32 Func_0200032c(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  ((struct FieldSprite *)object->animation)->priority = 2;
  object->motionFlags = 0;
  return 0;
}
