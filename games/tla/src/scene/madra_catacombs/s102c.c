#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Func_02000958(void);

/// Mirrors actors 8 and 9 and puts them on the ground; once flag 0x8c9 is set,
/// each steps 12 pixels outward unless flag 0x109 is set too, and Func_02000958
/// runs.
void Func_0200102c(void) {
  struct ObjectRuntime *left = Object_GetById(8);
  struct ObjectRuntime *right = Object_GetById(9);

  left->scaleX = 0x14000;
  right->scaleX = -0x14000;
  left->motionFlags = 0;
  right->motionFlags = 0;
  left->y = 0;
  right->y = 0;
  if (GameFlag_Test(0x8c9) != 0) {
    if (GameFlag_Test(0x109) == 0) {
      left->x += -0xc0000;
      /* FAKEMATCH: read and written as long, another alias set than the int
       * store before it, so the step is built before right->x loads; as int,
       * 2 instructions differ in tla-en. */
      *(long *)&right->x += 0xc0000;
    }
    Func_02000958();
  }
}
