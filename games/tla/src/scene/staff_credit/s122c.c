#include "types.h"
#include "frame.h"
#include "actor.h"

extern s32 gData_02003db8;
extern const s32 gData_02002eac[];
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02001168(struct Actor *object);

void Func_0200122c(struct Actor *object) {
  /* FAKEMATCH(volatile): the first read of the frame counter made volatile
   * keeps its second read after the call; as a plain read of gFrameCount,
   * the second read is hoisted above the branch. */
  if (*(volatile u32 *)&gFrameCount & 1) {
    Animation_ApplyChildValues(object, gData_02002eac[gData_02003db8]);
    gData_02003db8 = (gData_02003db8 + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    Func_02001168(object);
  }
}
