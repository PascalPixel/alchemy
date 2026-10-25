#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_020048ec(s32 id, u32 mode, s32 flag, s32 layer);

void Func_020047fc(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct ObjectRuntime *object;
  s16 mode;
  s16 flag;
  s32 limit;
  s32 first;
  limit = 0xff;
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
next:
  id = spec[0];
  object = Object_GetById(id);
  mode = spec[1];
  flag = spec[2];
  object->collisionFlags &= 0xfe;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Func_020048ec(id, (u16)mode, (u16)flag, object->terrainId);
  limit = 0xff;
  spec += 3;
  first = spec[0];
  if (first != ((limit << 8) + 0xff)) {
    goto next;
  }
}
