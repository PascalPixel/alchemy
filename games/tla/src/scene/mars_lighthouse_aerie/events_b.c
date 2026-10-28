#include "mars_lighthouse_aerie.h"

void Func_02000610(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Actor_Stop(id);
  object->unk5c[6] = (((u32)Random16Far() * 40) >> 16) + 20;
  object->update = (void (*)(void))Func_02000038;
}

void Func_02000640(s32 id) {
  Actor_GetById(id)->update = NULL;
  Actor_ArmCallback(id, 0xc000, 0);
}
