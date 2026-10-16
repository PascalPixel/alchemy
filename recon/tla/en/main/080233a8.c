// Near miss, differ 2 of 16 (one reordered row): The Broken Seal's
// ObjectDispatch_Initialize with the header's signature, which the earlier
// draft did not compile against. The native loads the halfword zero from the
// pool (ldr r3, =0) before the first store's zero (movs r2, #0; strh r2, [r0,
// #4]); here the pool load comes after that store. Measured: an empty asm after
// the first store moves the pool load between movs and strh (still differ 2); a
// one-pass block around the first store is differ 25.
#include "object_dispatch.h"

void ObjectDispatch_Initialize(struct DispatchObject *object,
                               const s32 *script) {
  if (object != 0) {
    object->value04 = 0;
    object->value00 = (u32)script;
    object->value5b = 0;
    object->value5d = 0;
    object->value57 = 0;
  }
}
