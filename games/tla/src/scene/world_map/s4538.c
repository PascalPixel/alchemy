#include "types.h"
#include "frame.h"
#include "actor.h"

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02004634(struct Actor *object);

void Func_02004538(struct Actor *object) {
  /* FAKEMATCH: a volatile first read keeps the second read of the frame counter
   * after the call; plain, it is hoisted above the branch */
  u32 frame = *(volatile u32 *)&gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    Func_02004634(object);
}
