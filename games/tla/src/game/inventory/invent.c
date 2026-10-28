#include "types.h"
#include "actor.h"
#include "system.h"

void Inventory_ReservedNoOp(void) {}

void Actor_WaitForAnimationEnd(s32 objectId, s32 value) {
  struct Actor *obj = ObjectTable_Get(objectId);

  if (obj != 0 && obj->animationKind == 1) {
    s32 cnt = 0;
    u8 *anim = (u8 *)obj->animation;

    while (cnt <= 89) {
      WaitFrames(1);
      if (value != anim[0x18]) {
        break;
      }
      cnt++;
    }
  }
}
