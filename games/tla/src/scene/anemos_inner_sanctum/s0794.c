#include "types.h"
#include "actor.h"
#include "event_inline.h"

struct SpawnParams {
  s32 kind;
  s32 part;
  u8 unk08[32];
};

void Func_02000480(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ,
                   s32 flags, struct SpawnParams *params);

s32 Func_02000794(struct Actor *object) {
  struct SpawnParams params;
  s32 x;
  s32 y;

  params.kind = 2;
  params.part = *((u8 *)object + 98);
  Func_02000480(object->x, object->y, object->z, 0, 0x10000, 0, 0x30000,
                &params);
  x = (object->bounce >> 20) + 64;
  y = object->gravity >> 20;
  Map_CopyCellPictureIndicesRect(x, y + 26, x, y, 1, 1);
  return 0;
}
