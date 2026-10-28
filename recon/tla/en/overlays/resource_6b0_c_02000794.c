// Under -mthumb-slow-loads (2026-10-26): differ 2 of 39; left: the parameter's
// copy (adds r5, r0) comes first, after the first store in the native.
// Draft, differ 2 of 39: the listing copies the object out of r0 after it
// stores params.kind (add r4; movs; str; adds r5, r0); this draft copies it
// first. Tried: a pointer local for the params (same); permute 3 minutes found
// nothing.
#include "types.h"
#include "actor.h"

struct SpawnParams {
  s32 kind;
  s32 part;
  u8 unk08[32];
};

void Func_02000480(s32 x, s32 y, s32 z, s32 targetX, s32 targetY, s32 targetZ,
                   s32 flags, struct SpawnParams *params);
void Func_02003328(s32 srcX, s32 srcY, s32 dst_x, s32 dst_y, s32 width,
                   s32 height);

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
  Func_02003328(x, y + 26, x, y, 1, 1);
  return 0;
}
