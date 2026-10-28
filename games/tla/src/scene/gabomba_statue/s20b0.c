#include "types.h"

#include "actor.h"

extern const s8 gData_02005940[];

struct Actor *Actor_GetById(s32 id);
void Func_0200561c(void *animation, s32 a);
void Func_02005624(struct Actor *object, s32 mode);

void Func_020020b0(s32 id, s32 direction, s32 value) {
  struct Actor *object = Actor_GetById(id);
  void *animation = object->animation;

  Func_02005624(object, gData_02005940[(direction + 1) & 3]);
  object->unk5c[0] = 2;
  Func_0200561c(animation, value);
}
