#include "types.h"
#include "frame.h"
#include "actor.h"

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02001ad0(struct Actor *object);

void Func_020019d4(struct Actor *object) {
  /* FAKEMATCH(volatile): the first read of the frame counter made volatile
   * keeps its second read after the call; as a plain read of gFrameCount, 0
   * of 22 instructions differ in tla-en. */
  u32 frame = *(volatile u32 *)&gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    Func_02001ad0(object);
}
