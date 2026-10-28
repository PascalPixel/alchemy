#include "types.h"
#include "field_event.h"
#include "call.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 GameFlag_Test(s32 flag);
void Func_02003190();
void Func_02003228();
void Func_02002c64(void);

void Func_02002fd4(void) {
  s32 flag;

  if (!GameFlag_Test(0x863)) {
    struct Actor *actor;

    Call3(Func_02003228, 8, PIXELS(416), PIXELS(680));
    Call6(Func_02003190, 38, 32, 2, 1, 25, 42);
    actor = Actor_GetById(8);
    Actor_SetSingleChildFlags(Actor_GetById(8), 0);
    actor->angle = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y = PIXELS(12);
    actor->collisionFlags = 0;
    Import_ActorSetAnimation(8, 5);
  }
  if (!GameFlag_Test(0x864)) {
    struct Actor *actor;

    Call3(Func_02003228, 5, PIXELS(288), PIXELS(744));
    Call6(Func_02003190, 38, 33, 2, 1, 17, 46);
    actor = Actor_GetById(5);
    Actor_SetSingleChildFlags(Actor_GetById(5), 0);
    actor->angle = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y = PIXELS(12);
    actor->collisionFlags = 0;
    Import_ActorSetAnimation(5, 18);
  }
  flag = GameFlag_Test(0x865);
  if (flag == 0) {
    struct Actor *actor;

    actor = Actor_GetById(6);
    Call3(Func_02003228, 6, PIXELS(240), PIXELS(648));
    Call6(Func_02003190, 38, 34, 2, 1, 14, 40);
    Import_ActorSetAnimation(6, 19);
    Actor_SetSingleChildFlags(Actor_GetById(6), 0);
    actor->motionFlags = 0;
    actor->y = PIXELS(12);
  }
  Import_MapCopyCellPictureIndicesRect(72, 42, 75, 40, 1, 1);
  Task_Wait(1);
  if (!GameFlag_Test(0x844))
    Func_02002c64();
}
