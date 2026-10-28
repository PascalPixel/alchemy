#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_020022bc(s32 a);

/// Once the leader stands at height 32 or more, it rises and spins for 110
/// frames.
void Func_02000454(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 i;
  s32 angle;
  s32 y;

  /* FAKEMATCH: the one-pass block loads the height before the 0x200000 limit is
   * built, as the native does (one reordered row without it). */
  do {
    y = object->y;
  } while (0);
  if (y >= 0x200000) {
    Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
    Actor_ResetAndSetPositionInMode2(gMainRoutineWork.currentOwner, 216, 680);
    Actor_SetAnimationSpeed(object, 64);
    object->motionFlags = 0;
    for (i = 109; i >= 0; i--) {
      object->y += PIXELS(3);
      /* FAKEMATCH: the one-pass block loads the angle before its 0x1800 step is
       * built (one reordered row without it; as one statement loop.c hoists the
       * step out of the loop, differ 20). */
      do {
        angle = object->angle;
      } while (0);
      angle += 0x1800;
      object->angle = angle;
      wait(2);
    }
    Func_020022bc(12);
  }
}
