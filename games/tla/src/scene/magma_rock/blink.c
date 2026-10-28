#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "frame.h"
#include "actor.h"
#include "main_routine_work.h"

void Func_02003bac(s32 a, s32 b);
void Func_02003bb4(s32 owner, u16 value);
void Field_DispatchTypeHandler(s32 type);
void Func_02003bbc(void);
void Func_02003bc4(void);
void Actor_SetPartAttribute(struct Actor *object, s32 value);

void Func_02000fcc(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    Actor_SetPartAttribute(object, 10);
  else
    Actor_SetPartAttribute(object, 0);
}
