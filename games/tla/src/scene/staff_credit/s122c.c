#include "types.h"
#include "frame.h"
#include "object_runtime.h"

extern s32 gData_02003db8;
extern const s32 gData_02002eac[];
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02001168(struct ObjectRuntime *object);

void Func_0200122c(struct ObjectRuntime *object) {
  /* FAKEMATCH: a volatile first read keeps the second read of the frame counter
   * after the call; plain, it is hoisted above the branch */
  if (*(volatile u32 *)&gFrameCount & 1) {
    Animation_ApplyChildValues(object, gData_02002eac[gData_02003db8]);
    gData_02003db8 = (gData_02003db8 + 1) & 3;
  }
  if (!(gFrameCount & 3)) {
    Func_02001168(object);
  }
}
