#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
s32 Func_02004040();
s32 Func_020041b8();
s32 Func_020040c8();
s32 Actor_Stop();
s32 wait();
s32 Func_02004100();
s32 Actor_ArmCallback();
s32 Func_020040d0();
s32 Func_02004130();
s32 Func_02004118();
s32 Camera_MoveWithinMapBounds();
void Func_02004128();
s32 Actor_Jump();
s32 Motion_SetModeAndWaitAnimation();
s32 Actor_WatchAndSetScript();
s32 Actor_SetSpeedParameters();
s32 Actor_SetMovementTypeById();
s32 FieldEvent_Wait();
s32 Actor_SetMovementTypeAndWaitById();
s32 Func_02004090();
s32 Func_02004098();
s32 GameFlag_SetBit();
s32 Func_02004048();
extern u8 MsgHeyLookUp[];
extern u8 gData_0200468c[];
extern u8 gData_020046dc[];
extern u8 gData_02004718[];

void Func_02001690(s32 p0) {
  s32 *owner;
  s32 angle;

  Call1((void (*)())Func_02004040, p0);
  Call1((void (*)())Func_020041b8, 0);
  Call1((void (*)())Func_020040c8, (s32)MsgHeyLookUp);
  Call1((void (*)())Actor_Stop, 9);
  Call1((void (*)())wait, 1);
  Value3((s32 (*)())Func_02004100, 9, 0x100, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0, 20);
  Call2((void (*)())Func_020040d0, 9, 0);
  Call1((void (*)())Actor_Stop, 8);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Actor_ArmCallback, 8, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 13, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 14, 0xd000, 0);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  Call3((void (*)())Func_02004100, *owner, 0x100, 0);
  Call3((void (*)())Actor_ArmCallback, *owner, 0x8000, 0);
  ((struct Actor *)Func_02004130())->motionFlags = 0;
  Call2((void (*)())Func_02004118, 0x19999, SPLIT_CONSTANT(0x3333));
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xd40000, 0, 0xd40000, 1);
  Func_02004128();
  Call2((void (*)())Func_020040d0, 12, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0, 10);
  Call2((void (*)())Func_020040d0, 11, 0);
  Call3((void (*)())Actor_ArmCallback, 13, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 14, 0x5000, 20);
  Call3((void (*)())Actor_Jump, 14, 4, 40);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 13, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 14, 3);
  Call2((void (*)())Actor_WatchAndSetScript, 8, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 9, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 10, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 11, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 12, 14);
  Call2((void (*)())Actor_WatchAndSetScript, 13, 14);
  Call3((void (*)())Actor_SetSpeedParameters, 13, 0x10000, 0x8000);
  Call2((void (*)())Actor_SetMovementTypeById, 14, (s32)gData_0200468c);
  Call2((void (*)())Func_02004118, 0xcccc, SPLIT_CONSTANT(0x1999));
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xf00000, -1, 0x1040000, 1);
  Func_02004128();
  Call1((void (*)())Actor_Stop, 8);
  Call1((void (*)())Actor_Stop, 9);
  Call1((void (*)())Actor_Stop, 10);
  Call1((void (*)())Actor_Stop, 11);
  Call1((void (*)())Actor_Stop, 12);
  Call1((void (*)())Actor_Stop, 13);
  Call2((void (*)())Func_02004118, 0x26666, SPLIT_CONSTANT(0x4ccc));
  Call4((void (*)())Camera_MoveWithinMapBounds, 0xd40000, -1, 0xd40000, 1);
  Func_02004128();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 8, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 20);
  Call2((void (*)())Func_020040d0, 9, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 8, 3);
  Call2((void (*)())Func_020040d0, 8, 0);
  Call3((void (*)())Actor_Jump, 10, 4, 0);
  Call3((void (*)())Actor_ArmCallback, 10, 0, 0);
  Call2((void (*)())Actor_SetModeById, 10, 4);
  Call2((void (*)())Func_020040d0, 10, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0x8000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x8000, 0);
  Value3((s32 (*)())Func_02004100, 8, 0x102, 0);
  Func_02004100(9, 0x102, 80);
  Call2((void (*)())Func_020040d0, 14, 0);
  Call3((void (*)())Actor_ArmCallback, 8, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 10, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, 11, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 13, 0x8000, 20);
  Call2((void (*)())Actor_SetModeById, 8, 3);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Call2((void (*)())Actor_SetModeById, 10, 3);
  Call2((void (*)())Actor_SetModeById, 11, 3);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 13, 3);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 13, 0x19999, SPLIT_CONSTANT(0xcccc));
  Call2((void (*)())Actor_WatchAndSetScript, *owner, 13);
  {
    s32 path = (s32)gData_020046dc;

    Call2((void (*)())Actor_SetMovementTypeById, 13, path);
    Call1((void (*)())FieldEvent_Wait, 10);
    Call2((void (*)())Actor_SetMovementTypeById, 12, path);
    Call2((void (*)())Func_02004118, SPLIT_CONSTANT(0x6666), SPLIT_CONSTANT(0xccc));
    Call4((void (*)())Camera_MoveWithinMapBounds, 0xf00000, -1, 0x1180000, 1);
    Call2((void (*)())Actor_SetMovementTypeById, 8, path);
    Call1((void (*)())FieldEvent_Wait, 10);
    Call2((void (*)())Actor_SetMovementTypeById, 9, path);
  }
  {
    s32 path = (s32)gData_02004718;

    Call2((void (*)())Actor_SetMovementTypeById, 10, path);
    Call1((void (*)())FieldEvent_Wait, 20);
    Call2((void (*)())Actor_SetMovementTypeAndWaitById, 11, path);
  }
  Call1((void (*)())Actor_Stop, *owner);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Func_02004090, 8, 0, 0);
  Call3((void (*)())Func_02004090, 9, 0, 0);
  Call3((void (*)())Func_02004090, 10, 0, 0);
  Call3((void (*)())Func_02004090, 11, 0, 0);
  Call3((void (*)())Func_02004090, 12, 0, 0);
  Call3((void (*)())Func_02004090, 13, 0, 0);
  Call3((void (*)())Func_02004090, 14, 0, 0);
  angle = 0xb000;
  Call4((void (*)())Func_02004098, 8, 0x2080000, 0xf20000, angle);
  {
    s32 facing = 0x8000;

    Call4((void (*)())Func_02004098, 9, 0x2150000, 0x10f0000, facing);
  }
  {
    s32 facing = 0x3000;

    Call4((void (*)())Func_02004098, 10, 0x1b30000, 0xe80000, facing);
  }
  {
    s32 facing = 0x5000;

    Call4((void (*)())Func_02004098, 11, 0x1d30000, 0xf80000, facing);
  }
  {
    s32 facing = 0xd000;

    Call4((void (*)())Func_02004098, 12, 0x1ce0000, 0x1540000, facing);
    Call4((void (*)())Func_02004098, 13, 0x1e20000, 0x1540000, facing);
  }
  Call4((void (*)())Func_02004098, 14, 0x1e90000, 0x1400000, angle);
  Call2((void (*)())Actor_SetModeById, 8, 1);
  Call2((void (*)())Actor_SetModeById, 9, 1);
  Call2((void (*)())Actor_SetModeById, 10, 1);
  Call2((void (*)())Actor_SetModeById, 11, 1);
  Call2((void (*)())Actor_SetModeById, 12, 1);
  Call2((void (*)())Actor_SetModeById, 13, 1);
  Call2((void (*)())Actor_SetModeById, 14, 1);
  Call1((void (*)())GameFlag_SetBit, 0x8f0);
  Call1((void (*)())GameFlag_SetBit, SPLIT_CONSTANT(0x88f));
  Func_02004048();
}
