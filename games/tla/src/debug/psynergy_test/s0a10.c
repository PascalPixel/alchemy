#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"
#include "fixed_math.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void wait(s32 frames);
struct Actor *Func_02000da4(s32 type, s32 x, s32 y, s32 z);
void Func_02000ebc(s32 scene, s32 entrance);

/// Mode 1 places object kind 354 at (x, z) in pixels; mode 2 grows object
/// kind 252 there from 0x6666 to full size while it turns, then turns it
/// opposite the leader's facing and passes scene and entrance on to
/// Func_02000ebc.
void Func_02000a10(s32 mode, s32 x, s32 z, s32 scene, s32 entrance) {
  struct Actor *object;

  if (mode == 1) {
    object = (struct Actor *)Value4((s32 (*)())Func_02000da4, 354,
                                            x << 16, 0, z << 16);
    if (object != NULL) {
      Actor_SetSingleChildFlags(object, 0);
      object->priorityFlags = 2;
    }
  }
  if (mode == 2) {
    object = Func_02000da4(252, x << 16, 0, z << 16);
    if (object != NULL) {
      s32 scale;

      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = 0x60000;
      object->scaleX = 0x6666;
      object->scaleY = 0x6666;
      while (object->scaleX < Q16_ONE) {
        wait(1);
        scale = object->scaleX + 0x51e;
        {
          /* FAKEMATCH(temporary): the turn is added as a word, so its step
           * is built inside the loop as in the native; as
           * object->angle += 0x1000, 22 of 86 instructions differ in
           * tla-en. */
          s32 angle = object->angle + 0x1000;

          object->angle = angle;
        }
        object->scaleX = scale;
        object->scaleY = scale;
      }
      object->scaleX = Q16_ONE;
      object->scaleY = Q16_ONE;
      object->angle = Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x8000;
      Func_02000ebc(scene, entrance);
    }
  }
}
