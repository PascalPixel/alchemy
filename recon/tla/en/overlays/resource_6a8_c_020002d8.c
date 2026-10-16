// Draft, differ 4 of 107: the facing check of 6a8, written as ☀️'s
// SceneActor_CheckFacingAndRange (vault_range.h) with ⚓️'s early exit on unk63
// and the event work's count over 120. The native loads the facing between the
// right and left angles (adds r1; ldrh; adds r4), this form after both. Tried:
// the four masks in eight other orders (14 to 86), three minutes of permute
// (nothing better); the paused pointer before the count test is what brought it
// from 20 to 4. Also tried: the right angle before the left, masked inline or
// after (12 to 55); a barrier after the right angle (18 to 28). sched2 ties
// left and right (priority 4, four dependents each) and takes left, the earlier
// insn.
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"

struct FacingActor {
  u8 unk00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x47];
  u8 paused;
  u8 unk5c[7];
  u8 unk63;
};

s32 SceneActor_CalculateFixedPointDistance(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);
void Func_020043c4(struct FacingActor *actor, s32 mode);

s32 Func_020002d8(struct FacingActor *actor, struct FacingActor *target,
                  s32 range, s32 force) {
  s32 result = 0;
  s32 *targetPos;
  s32 *actorPos;

  if (actor->unk63 != 0) {
    actor->paused = 1;
    return 1;
  }
  targetPos = &target->x;
  actorPos = &actor->x;
  if (SceneActor_CalculateFixedPointDistance(targetPos, actorPos) < range ||
      force != 0) {
    u32 angle = (u16)ArcTan2(target->z - actor->z, *targetPos - *actorPos);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 forward = angle & 0xf000;
    u32 facing = actor->facing & 0xf000;

    if (forward == facing || right == facing || left == facing || force != 0) {
      s16 count = ((struct EventWork *)Ram_HeapSlots->eventWork)->unk184;
      u8 *paused = &actor->paused;

      if (count > 120) {
        *paused = 0;
        Func_020043c4(actor, 2);
      } else {
        *paused = 1;
        Func_020043c4(actor, 1);
        result = 1;
      }
    }
  } else {
    actor->paused = 0;
    Func_020043c4(actor, 2);
  }
  return result;
}
