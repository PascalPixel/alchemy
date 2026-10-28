#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02004a60(s32 id, u32 mode, s32 flag, s32 layer);

void Func_02004970(const u16 *list) {
  const u16 *spec;
  u16 id;
  struct Actor *object;
  s16 mode;
  s16 flag;
  register s32 limit asm("r2");
  s32 first;
  do {
    limit = 0xff;
  } while (0);
  first = list[0];
  if (first == ((limit << 8) + 0xff)) {
    return;
  }
  spec = list;
next:
  asm("" : : : "memory");

  id = spec[0];
  object = Actor_GetById(id);
  mode = spec[1];
  flag = spec[2];
  object->collisionFlags &= 0xfe;
  Actor_SetSingleChildFlags(object, 0);
  Func_02004a60(id, (u16)mode, (u16)flag, object->terrainId);
  limit = 0xff;
  spec += 3;
  first = spec[0];
  asm volatile("" : "+l"(limit));
  if (first != ((limit << 8) + 0xff)) {
    goto next;
  }
}
