#include "types.h"
#include "event_inline.h"

struct Walker {
  u8 unk00[0x63];
  u8 arrived;
  u16 frames;
  u8 unk66[6];
  void (*update)(void);
};

void Func_020003bc(void);

void Func_0200279c(s32 actor, s32 x, s32 z, s32 frames) {
  struct Walker *object = ((struct Walker *)Actor_GetById(actor));

  object->frames = frames;
  object->arrived = 0;
  object->update = Func_020003bc;
  Actor_SetHorizontalPositionWithTerrain(actor, x << 16, z << 16);
}
