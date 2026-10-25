#include "types.h"

struct Walker {
  u8 unk00[0x63];
  u8 arrived;
  u16 frames;
  u8 unk66[6];
  void (*update)(void);
};

struct Walker *Object_GetById(s32 id);
void Func_020044cc(s32 actor, s32 x, s32 z);
void Func_020003bc(void);

void Func_0200279c(s32 actor, s32 x, s32 z, s32 frames) {
  struct Walker *object = Object_GetById(actor);

  object->frames = frames;
  object->arrived = 0;
  object->update = Func_020003bc;
  Func_020044cc(actor, x << 16, z << 16);
}
