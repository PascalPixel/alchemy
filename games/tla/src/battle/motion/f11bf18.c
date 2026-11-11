#include "types.h"
#include "actor.h"
#include "io_reg.h"

/// Sends the actor of startSlot leaping progress percent of the way toward
/// the actor of endSlot, in actor mode 2. Also clears REG_BLDCNT. Nothing in
/// the source calls it.
void BattleMotion_LeapToward(struct Actor **startSlot, struct Actor **endSlot,
                             s32 progress) {
  struct Actor *start = *startSlot;
  struct Actor *end = *endSlot;
  s32 x;
  s32 z;

  x = start->x + (end->x - start->x) * progress / 100;
  z = start->z + (end->z - start->z) * progress / 100;
  REG_BLDCNT = 0;
  start->acceleration = 0x20000;
  start->speedLimit = 0x80000;
  start->velocityY = 0x40000;
  start->gravity = 0xab85;
  start->bounce = 0;
  start->actionFlags = 1;
  Actor_SetTargetPosition(start, x, 0, z);
  Actor_SetMode(start, 2);
}
