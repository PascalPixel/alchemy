// Draft, differ 22 of 86: as a goto loop the scale loop builds its turn step
// 0x1000 inside it, as the native does (a for (;;) loop hoists it into r6 from
// the literal pool: differ 28). Left: the native compares the scale with 0xffff
// (ble) and adds 1 for the full size that follows, where this compares with
// 0x10000 (blt), however the test is written; the loads of the scale and the
// angle come before their constants.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void WaitFrames(s32 frames);
struct ObjectRuntime *Func_02000da4(s32 type, s32 x, s32 y, s32 z);
void Func_02000ebc(s32 scene, s32 entrance);

void Func_02000a10(s32 mode, s32 x, s32 z, s32 scene, s32 entrance) {
  struct ObjectRuntime *object;

  if (mode == 1) {
    object = Func_02000da4(354, x << 16, 0, z << 16);
    if (object != NULL) {
      ObjectDispatch_SetSingleChildFlags(object, 0);
      object->priorityFlags = 2;
    }
  }
  if (mode == 2) {
    object = Func_02000da4(252, x << 16, 0, z << 16);
    if (object != NULL) {
      s32 scale;

      ObjectDispatch_SetSingleChildFlags(object, 0);
      object->velocityY = 0x60000;
      object->scaleX = 0x6666;
      object->scaleY = 0x6666;
    grow:
      WaitFrames(1);
      scale = object->scaleX + 0x51e;
      object->angle += 0x1000;
      object->scaleX = scale;
      object->scaleY = scale;
      if (scale >= 0x10000)
        goto grown;
      goto grow;
    grown:
      object->scaleX = 0x10000;
      object->scaleY = 0x10000;
      object->angle = Object_GetById(gPartyState.currentOwner)->angle + 0x8000;
      Func_02000ebc(scene, entrance);
    }
  }
}
