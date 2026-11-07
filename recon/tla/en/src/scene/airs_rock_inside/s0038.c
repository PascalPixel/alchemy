#include "../../../../../../games/tla/src/scene/airs_rock_inside/airs_rock_inside.h"
#include "actor.h"

u32 Random16Far(void);
s32 Func_02005f64(struct FieldActor *object);
void Func_020006cc(struct FieldActor *object);
void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer);

void Func_02000038(s16 *list) {
  s16 *entry;

  if ((u16)list[0] == 0xffff)
    return;
  entry = list;
next:
  {
    s32 id = (u16)entry[0];
    struct FieldActor *object = Actor_GetById(id);
    s32 kind = entry[1];
    s32 flag = entry[2];

    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlags(object, 0);
    Func_02000128(id, (u16)kind, (u16)flag, BYTE_AT(object, 34));
    entry += 3;
  }
  if ((u16)entry[0] != 0xffff)
    goto next;
}
