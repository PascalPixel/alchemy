#include "types.h"
#include "actor.h"
#include "flag_call.h"
#include "call.h"

struct Actor *Actor_GetById(s32 id);
void Actor_Stop(s32 actor);
void Actor_WaitForMoveById(s32 actor);
void Actor_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void Actor_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02001084(void);
void Func_0200108c(void);
void Func_02001124(s32 actor, s32 value);
void Func_02001164(s32 value);

void Func_0200036c(void) {
  /* FAKEMATCH: the placements go through call.h's Call3 and the flags through
   * flag_call.h, which build each constant again at each call (direct calls
   * share 764 and 380 in saved registers, differ 36). */
  Func_02001084();
  Func_02001164(0);
  Call3((void (*)())Actor_ResetAndSetPosition, 17, 764, 384);
  Call3((void (*)())Actor_SetPositionAndCommit, 16, 746, 380);
  Actor_GetById(17)->velocityY = 0x40000;
  Actor_WaitForMoveById(17);
  Actor_GetById(17)->priorityFlags |= 2;
  if (!Flag_IsSet(0x8ff) && !Flag_IsSet(0x201)) {
    Actor_Stop(10);
    Call3((void (*)())Actor_SetPositionAndCommit, 10, 764, 388);
    Call3((void (*)())Actor_ResetAndSetPosition, 17, 764, 376);
    Call3((void (*)())Actor_SetPositionAndReset, 10, 764, 380);
    Actor_SetModeById(10, 4);
    Func_02001124(10, 257);
    Flag_Set(0x202);
  }
  Func_0200108c();
}
