#include "islets.h"

void Func_02002d0c(s32 id);
void Func_02002d7c(void);
void Func_02003684(s32 actor);
void Func_02003ee4(s32 actor, s32 x, s32 z, s32 angle);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetMovementTypeById(s32 actor, const void *script);
void Actor_WatchAndSetScript(s32 actor, s32 other);
extern const u8 gData_02004274[];

/// The room's setup: flags, the objects' places and the scene state.
void Func_02002f1c(void) {
  struct Actor *object;
  struct Actor *other;
  u16 *state;
  u16 *next;
  s32 angle;
  s32 i;

  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  if (gMainRoutineWork.entrance == 2)
    GameFlag_ClearBit(0x12f);
  i = 11;
  do {
    Func_02002d0c(i++);
  } while (i <= 18);
  state = gSceneState;
  next = &gSceneState[1];
  if (!GameFlag_Test(0x109)) {
    *state = 0;
    *next = 0;
  }
  Task_Add(Func_02002d7c, 0x480);
  if (GameFlag_Test(0xa2c)) {
    object = Actor_GetById(10);
    Call3((void (*)())Func_02003edc, 10, 0xb00000, 0xa80000);
    Actor_SetModeById(10, 4);
    object->priorityFlags |= 2;
    object->motionFlags = 0;
    object->y = -0x80000;
    object->terrainHeight = -0x80000;
    Func_02003e24(9, 1, 4, 1, 9, 10);
    Func_02003e24(9, 64, 4, 1, 9, 74);
  } else {
    Func_02003684(10);
  }
  if (!GameFlag_Test(0xaa2)) {
    Actor_WatchAndSetScript(9, 19);
    Actor_SetModeById(19, 0);
    Actor_GetById(19)->actionFlags &= ~1;
  } else if (!GameFlag_Test(0x109)) {
    other = Actor_GetById(9);
    other->angle = 0x5000;
    other->collisionFlags |= 0x80;
    /* FAKEMATCH: the angle in a temporary is built before the position (differ
     * 4 as a constant argument). */
    angle = 0x3000;
    Func_02003ee4(19, 0xea0000, 0xcc0000, angle);
    Actor_GetById(19)->collisionFlags |= 0x80;
    Call3((void (*)())Actor_SetSpeedParameters, 9, 0x20000, 0x10000);
    Call3((void (*)())Actor_SetSpeedParameters, 19, 0x19999, 0xcccc);
    Actor_SetMovementTypeById(9, gData_02004274);
    Actor_SetMovementTypeById(19, gData_02004274);
    GameFlag_SetBit(0x200);
  }
}

void Func_020030b4(void) {
  struct Actor *spawned;

  if (gMainRoutineWork.entrance == 3)
    GameFlag_ClearBit(0x12f);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  Actor_SetModeById(10, 4);
  if (GameFlag_Test(0xaa3)) {
    spawned = (struct Actor *)Value4((s32 (*)())Func_02003df4, 489,
                                             0x680000, 0x180000, 0xa00000);
    if (spawned != 0) {
      spawned->motionFlags = 0;
      Func_02003e3c(spawned, 453);
      Actor_SetSingleChildFlags(spawned, 0);
      spawned->unk5c[0] = 1;
    }
    Call3((void (*)())Func_02003edc, 10, 0x680000, 0xcc0000);
  }
  Actor_SetSpritePriority(11, 1);
  Actor_SetPartAttribute(Actor_GetById(11), 15);
  Actor_GetById(11)->priorityFlags |= 2;
}
