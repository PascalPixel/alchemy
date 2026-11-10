#include "local.h"

void Func_020020bc(s32 entrance) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 *owner = &gMainRoutineWork.currentOwner;

  Func_02002998();
  Event_ResolvePendingActions(0);
  Sound_Play(228);
  leader->update = (void (*)(void))Func_02002030;
  leader->motionFlags = 0;
  leader->speedLimit = 0x3333;
  Actor_SetModeById(*owner, 2);
  Actor_OffsetPositionAndResetMotion(*owner, 0, -8);
  Actor_WaitForMoveById(*owner);
  Actor_SetPartAttribute(Actor_GetById(*owner), 9);
  Actor_SetSingleChildFlags(Actor_GetById(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(entrance);
  Func_020029a0();
}

void Func_02002158(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 flag = GameFlag_Test(0x109);
  s32 *owner = &gMainRoutineWork.currentOwner;
  s32 offset;

  if (flag == 0) {
    Func_02002998();
    Event_ResolvePendingActions(0);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    leader->motionFlags = flag;
    Actor_SetHorizontalPositionWithTerrain(*owner, ((s16 *)&leader->x)[1] << 16,
                                           (((s16 *)&leader->z)[1] << 16) -
                                               0x100000);
    Actor_SetPartAttribute(Actor_GetById(*owner), 9);
    Actor_SetSingleChildFlags(Actor_GetById(*owner), 0);
    Event_StartScreenTransition();
    Sound_Play(228);
    leader->update = (void (*)(void))Func_02002030;
    Actor_SetSpeedParameters(*owner, 0x6666, 0x3333);
    Actor_CommitPositionAndActivate(*owner, 0, 8);
    Actor_SetPartAttribute(Actor_GetById(*owner), 0);
    Actor_SetSingleChildFlags(Actor_GetById(*owner), 1);
    ((struct FieldSprite *)leader->animation)->priority = 1;
    offset = 10;
    Actor_CommitPositionAndActivate(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Func_02002a60();
    Event_WaitTransitionFrames();
    Func_020029a0();
  }
}
