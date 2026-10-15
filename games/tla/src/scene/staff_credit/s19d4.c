#include "types.h"
#include "frame.h"
#include "object_runtime.h"

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02001ad0(struct ObjectRuntime *object);

void Func_020019d4(struct ObjectRuntime *object) {
  /* FAKEMATCH: a volatile first read keeps the second read of the frame counter
   * after the call; plain, it is hoisted above the branch */
  u32 frame = *(volatile u32 *)&gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    Func_02001ad0(object);
}
