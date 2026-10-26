#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_080daecc(s32 id, s32 a1);

/// Starts an object's chain (Func_080daecc). An object with no motion flags is
/// first moved -16 pixels in z and 64 pixels in y; one with motion
/// flags starts it only when force is set.
void Func_080db444(s32 id, s32 force) {
  struct ObjectRuntime *object = Object_GetById(id);
  s32 y;
  s32 zero;

  if (object != NULL) {
    if (object->motionFlags == 0) {
      zero = 0;
      object->z += 0xfff00000;
      y = object->y;
      /* FAKEMATCH: ⚓️ loads y before it builds the 64-pixel step and sets the
       * call's zero argument first; without the barrier, differ 2 (movs r2,
       * #128 issues before ldr r3, [r0, #12]). */
      asm("" : : "r"(y), "r"(zero));
      object->y = y + 0x400000;
      Func_080daecc(id, zero);
    } else if (force) {
      Func_080daecc(id, 0);
    }
  }
}
