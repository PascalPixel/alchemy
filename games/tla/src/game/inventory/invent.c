#include "types.h"
#include "object_runtime.h"
#include "system.h"

void Inventory_ReservedNoOp(void) {}

void Object_WaitUntilChildValueDiffers(s32 objectId, s32 value) {
  struct ObjectRuntime *obj = ObjectTable_Get(objectId);

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
