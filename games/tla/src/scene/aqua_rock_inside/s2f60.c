#include "local.h"

void Func_02002f60(s32 entrance) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 mode;
  s32 *owner = &gPartyState.currentOwner;

  Func_02003d7c();
  Func_02003eac(0);
  Func_02003ef4(228);
  leader->update = (void (*)(void))Func_02002ed4;
  leader->motionFlags = 0;
  leader->speedLimit = 0x3333;
  mode = 2;
  /* FAKEMATCH: the native sets the mode before it loads the owner (differ 2
   * without the barrier). */
  __asm__("" : "+r"(mode), "+r"(owner));
  Object_SetModeById(*owner, mode);
  ObjectMotion_OffsetPositionAndResetMotion(*owner, 0, -8);
  ObjectMotion_CommitCurrentPositionAndActivate(*owner);
  Object_SetPartAttribute(Object_GetById(*owner), 9);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(*owner), 0);
  leader->update = 0;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003e5c(entrance);
  Func_02003d84();
}

void Func_02002ffc(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 flag = GameFlag_Test(0x109);
  s32 *owner = &gPartyState.currentOwner;
  s32 offset;

  if (flag == 0) {
    Func_02003d7c();
    Func_02003eac(0);
    Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
    leader->motionFlags = flag;
    Func_02003ddc(*owner, ((s16 *)&leader->x)[1] << 16,
                  (((s16 *)&leader->z)[1] << 16) - 0x100000);
    Object_SetPartAttribute(Object_GetById(*owner), 9);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(*owner), 0);
    Event_StartScreenTransition();
    Func_02003ef4(228);
    leader->update = (void (*)(void))Func_02002ed4;
    ObjectMotion_SetSpeedParameters(*owner, 0x6666, 0x3333);
    ObjectMotion_CommitPositionAndActivate(*owner, 0, 8);
    Object_SetPartAttribute(Object_GetById(*owner), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(*owner), 1);
    ((struct FieldSprite *)leader->animation)->priority = 1;
    offset = 10;
    /* FAKEMATCH: the native sets the offset before it loads the owner (differ 2
     * without the barrier). */
    __asm__("" : "+r"(offset), "+m"(*(u8 *)owner));
    ObjectMotion_CommitPositionAndActivate(*owner, 0, offset);
    leader->motionFlags = 3;
    leader->update = (void (*)(void))flag;
    Func_02003ed4();
    Event_WaitTransitionFrames();
    Func_02003d84();
  }
}
