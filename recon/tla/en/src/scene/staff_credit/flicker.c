#include "types.h"
#include "frame.h"
#include "actor.h"

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_020010b0(struct Actor *object);

void Func_020011f4(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValues(object, 7);
  } else {
    Animation_ApplyChildValues(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    Func_020010b0(object);
  }
}
