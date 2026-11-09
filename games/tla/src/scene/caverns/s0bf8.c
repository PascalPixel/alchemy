#include "caverns.h"

void Func_02000ba8(void) {
  struct FieldActor *object;
  s32 arg;

  ((struct FieldActor *)Actor_GetById(4))->priorityFlags |= 0x20;
  Func_0200114c();
  Func_02001154(0, 0x200, 8, 9);
  object = ((struct FieldActor *)Actor_GetById(10));
  object->motionFlags = 0;

  arg = 10;
  Map_CopyCellAttributeRect(16, arg, 1, 1, object->x >> 20, arg);
}

void Func_02000bf8(s32 unused, s32 id) {
  struct FieldActor *object = ((struct FieldActor *)Actor_GetById(id));

  object->motionFlags = 0;
  Map_CopyCellAttributeRect(13, 12, 2, 1, 13, 10);
  Map_CopyCellAttributeRect(16, 10, 1, 1, object->x >> 20, 10);
}
