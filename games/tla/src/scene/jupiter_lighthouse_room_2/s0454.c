#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void WaitTicks(s32 ticks);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void ObjectDispatch_SetAnimationSpeed(struct ObjectRuntime *object,
                                         s32 value);
void Func_020022bc(s32 a);

/// Once the leader stands at height 32 or more, it rises and spins for 110
/// frames.
void Func_02000454(void) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 i;
  s32 angle;
  s32 y;

  /* FAKEMATCH: the one-pass block loads the height before the 0x200000 limit is
   * built, as the native does (one reordered row without it). */
  do {
    y = object->y;
  } while (0);
  if (y >= 0x200000) {
    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x20000, 0x10000);
    ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
    ObjectMotion_ResetAndSetPositionInMode2(gPartyState.currentOwner, 216, 680);
    ObjectDispatch_SetAnimationSpeed(object, 64);
    object->motionFlags = 0;
    for (i = 109; i >= 0; i--) {
      object->y += 0x30000;
      /* FAKEMATCH: the one-pass block loads the angle before its 0x1800 step is
       * built (one reordered row without it; as one statement loop.c hoists the
       * step out of the loop, differ 20). */
      do {
        angle = object->angle;
      } while (0);
      angle += 0x1800;
      object->angle = angle;
      WaitTicks(2);
    }
    Func_020022bc(12);
  }
}
