#include "local.h"

void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);

void Func_02001f04(void) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  u8 *flags = &object->motionFlags;
  s32 saved = *flags;
  s32 speed;
  s32 i;

  Func_02003d7c();
  Func_02003eac(0);
  Event_StartScreenTransition();
  Animation_ApplyChildValues(object, 9);
  *flags = 0;
  object->y = 0x500000;
  /* FAKEMATCH: the native stores the height before it starts the speed; without
   * the barrier the speed comes first (differ 2). */
  __asm__("");
  speed = 0x30000;
  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the turn step on every pass, and a for loop hoists it
   * (differ 43). */
  i = 31;
fall:
  object->y -= speed;
  object->angle += 0x2000;
  speed += -0xccc;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  WaitTicks(1);
  i--;
  if (i >= 0)
    goto fall;
  Func_02002bd4(object);
  Animation_ApplyChildValues(object, 0);
  ObjectMotion_SetSpeedParameters_(gPartyState.currentOwner, 0x20000, 0x10000);
  *flags = saved;
  object->velocityY = 0x80000;
  Func_02003ef4(215);
  ObjectMotion_SetPositionAndCommit(gPartyState.currentOwner, 168, 200);
  Func_02003d84();
}
