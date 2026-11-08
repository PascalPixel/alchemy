#include "../../../../../../games/tla/src/scene/islets/islets.h"
#include "scene_tables.h"

void Func_02003fbc(void);
void Func_02003fc4(void);
void *HeapSlot_AllocEwram(s32 id, s32 size);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
extern const u8 gData_020045a4[];
extern const u8 gData_020045f0[];

void Func_02001a68(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(9);
  void (*update)(void);
  s32 result;
  u8 *done;

  Func_02003e64();
  Func_02003fa4(0);
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  Func_02003f24((s32)MsgYouAllSet);
  Func_02003f2c(9, 0);
  Func_02003fc4();
  result = Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (result == 0) {
    Func_02003fbc();
    Func_02003f2c(9, 0);
    ((struct {
      u8 unk000[0x295];
      u8 unk295;
    } *)HeapSlot_AllocEwram(88, 0x298))
        ->unk295 = 99;
    Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
    Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
          0x19999, SPLIT_CONSTANT(0xcc00, 204));
    Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 152, 768);
    leader->update = (void (*)(void))Func_02001278;
    Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
    Actor_SetModeById(9, 3);
    FieldEvent_Wait(20);
    Func_02003fc4();
    Call3((void (*)())Actor_SetSpeedParameters, 9, 0x19999, SPLIT_CONSTANT(0xcc00, 204));
    Call3((void (*)())Actor_SetSpeedParameters, 10, 0x19999, SPLIT_CONSTANT(0xcc00, 204));
    update = (void (*)(void))Func_02000df8;
    done = (u8 *)object + 99;
    *done = 0;
    object->update = update;
    Actor_SetMovementTypeById(9, gData_020045a4);
    FieldEvent_Wait(30);
    Actor_GetById(10)->update = update;
    Actor_SetMovementTypeById(10, gData_020045f0);
    do {
      wait(1);
    } while (*done == 0);
    Func_020034ec();
    wait(1);
    ((struct IsletsEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x201;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02003fbc();
    Func_02003f7c(4);
  } else {
    Func_02003fbc();
    Func_02003f24((s32)MsgYouAllSet + 4);
    Func_02003f2c(9, 0);
    Call2((void (*)())Func_02003f3c, 9, FACING_SOUTH);
  }
  Func_02003e6c();
}

void Func_02001bf4(void) {
  Func_02003e64();
  Func_02003fa4(0);
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  Func_02003f24((s32)MsgMyPopS);
  Func_02003f2c(9, 0);
  Call2((void (*)())Func_02003f3c, 9, FACING_NORTHWEST + FACING_STEP);
  Func_02003e6c();
}

void Func_02001c38(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  work->field15a = 1;
}

void Func_02001c4c(void) {
  if (Actor_GetById(gMainRoutineWork.currentOwner)->y < 0x280000) {
    Actor_GetById(11)->collisionFlags |= 8;
    Actor_GetById(12)->collisionFlags |= 8;
  } else {
    Actor_GetById(11)->collisionFlags &= ~8;
    Actor_GetById(12)->collisionFlags &= ~8;
  }
}
