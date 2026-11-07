#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
struct Actor *Actor_GetById();
s32 Func_02000988();
s32 Func_02000a70();
s32 Func_02000a00();
s32 Actor_CommitPositionAndActivate();
s32 FieldEvent_Wait();
s32 Func_020009d0();
s32 Actor_SetPositionAndReset();
s32 Actor_ArmCallback();
s32 Motion_SetVarCbAndRefresh();
s32 Func_02000a08();
s32 Motion_SetModeAndWaitAnimation();
s32 Party_SetFields1eeAnd1f0();
s32 Party_SetFields1f2And1f4();
void Func_02000a40();
void Func_02000990();
extern char SceneId_SuhallaGate2;
extern char SceneId_VenusLighthouseEntrance;
extern u8 MsgHaThatWas[];

void Func_02000494(s32 p0) {
  struct MainRoutineWork *work;
  s32 *owner;

  Call1((void (*)())Func_02000988, p0);
  Call1((void (*)())Func_02000a70, 0);
  Call1((void (*)())Func_02000a00, (s32)MsgHaThatWas);
  work = &gMainRoutineWork;
  owner = &work->currentOwner;
  Call3((void (*)())Actor_CommitPositionAndActivate, *owner, 0, -4);
  Call1((void (*)())FieldEvent_Wait, 30);
  Actor_GetById(*owner)->actionFlags &= 254;
  Call3((void (*)())Actor_CommitPositionAndActivate, *owner, 0, 12);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_CommitPositionAndActivate, *owner, 0, 16);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_020009d0, 13, 0x2680000, 0x3900000);
  Call3((void (*)())Actor_SetPositionAndReset, 13, 0x258, 0x398);
  Call3((void (*)())Func_020009d0, 15, 0x2680000, 0x3900000);
  Call3((void (*)())Actor_ArmCallback, 13, 0x4000, 0);
  Call3((void (*)())Actor_SetPositionAndReset, 15, 0x278, 0x398);
  Call3((void (*)())Actor_ArmCallback, 15, 0x4000, 0);
  Call3((void (*)())Func_020009d0, 14, 0x2680000, 0x3900000);
  Call3((void (*)())Actor_SetPositionAndReset, 14, 0x268, 0x398);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 13, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02000a08, 13, 0, 10);
  Call3((void (*)())Func_02000a08, 14, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 15, 3);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02000a08, 15, 0, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 13, 3);
  Call2((void (*)())Actor_SetModeById, 14, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 15, 3);
  Call2((void (*)())Party_SetFields1eeAnd1f0, (s32)&SceneId_SuhallaGate2, 99);
  Call2((void (*)())Party_SetFields1f2And1f4,
        (s32)&SceneId_VenusLighthouseEntrance, 98);
  *((u8 *)work + 0x24b) = 2;
  Func_02000a40(9, 2);
  Func_02000990();
}
