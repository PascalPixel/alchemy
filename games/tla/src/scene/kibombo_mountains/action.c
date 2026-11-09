#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_0200114c(void);
void Func_0200103c(struct Actor *object, s32 value);
void Func_02000da8(s32 value);
void Func_02000e28(s32 value);

void Func_02000190(s32 unused, s32 id) {
  if (id == 15)
    FieldActor_SetSpritePriority(15, 1);
}
