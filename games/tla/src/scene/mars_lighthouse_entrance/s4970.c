#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02004a60(s32 id, u32 mode, s32 flag, s32 layer);

void Func_02004970(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *object;
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
  do {
    id = spec[0];
    object = Actor_Get(id);
    mode = spec[1];
    flag = spec[2];
    object->collisionFlags &= 0xfe;
    Actor_SetSingleChildFlags(object, 0);
    Func_02004a60(id, (u16)mode, (u16)flag, object->terrainId);
    limit = 0xff;
    spec += 3;
    first = spec[0];
  } while (first != ((limit << 8) + 0xff));
}
