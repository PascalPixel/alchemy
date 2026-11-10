#include "local.h"

void Func_02001464(s32 entrance) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 *owner = &gMainRoutineWork.currentOwner;

  Func_02001cc4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(228);
  leader->update = (void (*)(void))Func_020013d8;
  leader->motionFlags = 0;
  leader->speedLimit = 0x3333;
  Actor_SetAnimation(*owner, 2);
  Actor_SetDestinationOffset(*owner, 0, -8);
  FieldActor_WaitForMoveById(*owner);
  Actor_SetSpriteColor(Actor_Get(*owner), 9);
  Actor_SetSingleChildFlags(Actor_Get(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(entrance);
  Func_02001ccc();
}

void Func_02001500(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 flag = Event_GetFlag(0x109);
  s32 *owner = &gMainRoutineWork.currentOwner;
  s32 offset;

  if (flag == 0) {
    Func_02001cc4();
    FieldEvent_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    leader->motionFlags = flag;
    Actor_SetPosition(*owner, ((s16 *)&leader->x)[1] << 16,
                                           (((s16 *)&leader->z)[1] << 16) -
                                               0x100000);
    Actor_SetSpriteColor(Actor_Get(*owner), 9);
    Actor_SetSingleChildFlags(Actor_Get(*owner), 0);
    Event_StartScreenTransition();
    Event_PlaySound(228);
    leader->update = (void (*)(void))Func_020013d8;
    Actor_SetSpeed(*owner, 0x6666, 0x3333);
    Actor_WalkByAndWait(*owner, 0, 8);
    Actor_SetSpriteColor(Actor_Get(*owner), 0);
    Actor_SetSingleChildFlags(Actor_Get(*owner), 1);
    ((struct FieldSprite *)leader->animation)->priority = 1;
    offset = 10;
    Actor_WalkByAndWait(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Func_02001df4();
    Event_WaitTransitionFrames();
    Func_02001ccc();
  }
}
