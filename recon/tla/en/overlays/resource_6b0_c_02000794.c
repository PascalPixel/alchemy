// Near miss, differ 2 of 39 (tla-en, 2026-10-31): scheduler order. The
// listing copies the object out of r0 after it stores params.kind (add r4;
// movs; str; adds r5, r0); this draft copies it first. In sched2 the copy
// (insn 4), add r4, sp and movs r3, #2 tie at priority 110 and the copy's
// earlier RTL place wins; the listing's order needs the str -> ldrb memory
// dependence to cost one cycle more, a cost the C cannot move.
// Tried: a pointer local for the params (same); a local copy of the object
// after the first store (CSE merges it, same); u8 * for the object (same); an
// s32 array for the params (same); reading the byte before storing kind (10
// to 12 differ); permute 3 minutes, then 4 minutes, found nothing.
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
