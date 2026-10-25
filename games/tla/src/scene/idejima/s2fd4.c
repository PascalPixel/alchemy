#include "types.h"
#include "field_event.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
s32 GameFlag_Test(s32 flag);
void Func_02003190();
void Func_02003228();
void Func_02002c64(void);

void Func_02002fd4(void) {
  s32 flag;

  if (!GameFlag_Test(0x863)) {
    struct ObjectRuntime *actor;

    Call3(Func_02003228, 8, 0x1a00000, 0x2a80000);
    Call6(Func_02003190, 38, 32, 2, 1, 25, 42);
    actor = Object_GetById(8);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
    actor->angle = 0x4000;
    actor->motionFlags = 0;
    actor->y = 0xc0000;
    actor->collisionFlags = 0;
    Engine_ActorSetAnimation(8, 5);
  }
  if (!GameFlag_Test(0x864)) {
    struct ObjectRuntime *actor;

    Call3(Func_02003228, 5, 0x1200000, 0x2e80000);
    Call6(Func_02003190, 38, 33, 2, 1, 17, 46);
    actor = Object_GetById(5);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(5), 0);
    actor->angle = 0x4000;
    actor->motionFlags = 0;
    actor->y = 0xc0000;
    actor->collisionFlags = 0;
    Engine_ActorSetAnimation(5, 18);
  }
  flag = GameFlag_Test(0x865);
  if (flag == 0) {
    struct ObjectRuntime *actor;

    actor = Object_GetById(6);
    Call3(Func_02003228, 6, 0xf00000, 0x2880000);
    Call6(Func_02003190, 38, 34, 2, 1, 14, 40);
    Engine_ActorSetAnimation(6, 19);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(6), 0);
    actor->motionFlags = 0;
    actor->y = 0xc0000;
  }
  Engine_MapCopyCellsTo(72, 42, 75, 40, 1, 1);
  Task_Wait(1);
  if (!GameFlag_Test(0x844))
    Func_02002c64();
}
