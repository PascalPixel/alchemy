#include "scene.h"
void Func_02003820(const void *script) {
  const u16 *entry;

  if (*(const u16 *)script == 0xffff)
    return;
  entry = script;
loop:
  {
    s32 id = entry[0];
    struct FieldActor *object = Actor_GetById(id);
    s32 kind = (s16)entry[1];
    s32 flag = (s16)entry[2];

    object->collisionFlags &= 0xfe;
    Actor_SetSingleChildFlags(object, 0);
    Func_02003910(id, (u16)kind, (u16)flag, object->unk18[10]);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}
