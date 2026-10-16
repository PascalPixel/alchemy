// Draft, differ 2 of 32: the native stores motionFlags = 3 before it builds
// the frame count (strb, movs r6 #60, mov r8 r2); this form schedules the
// store after them. Assigning frames after the store, a for loop, a do/break
// loop and a counting-up loop all swap the object and the count (r5/r6),
// differ 14 to 16. Also tried: an empty asm or one-pass block after
// the store (2); the count pinned to r6 and set after the store keeps the
// order but for the address's copy to r8 (2); the address passed to a
// long-lived copy through a volatile asm after the store orders all three
// but builds the address as adds r2, r5, #0; adds r2, #85 (differ 4); the
// offset through an asm (10 and 17).
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Func_02002eec(s32 a);

void Func_02000b30(s32 id, s32 cue) {
  struct ObjectRuntime *object = Object_GetById(id);
  s32 frames = 60;

  object->motionFlags = 3;
  while (frames != 0) {
    WaitFrames(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
  if (cue != 0)
    Func_02002eec(cue);
  WaitFrames(10);
  object->motionFlags = 0;
}
