#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_0200114c(void);
void Func_0200103c(struct Actor *object, s32 value);
void Func_02000da8(s32 value);
void Func_02000e28(s32 value);

void Func_02000ad4(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Func_0200103c(object, 1);
  object->acceleration = 0x4000;
  object->speedLimit = 0x8000;
}
