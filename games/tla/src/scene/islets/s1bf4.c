#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02003fbc(void);
void Func_02003fc4(void);
void *HeapSlot_AllocEwram(s32 id, s32 size);
extern const u8 gData_020045a4[];
extern const u8 gData_020045f0[];

void Func_02001a68(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_Get(9);
  void (*update)(void);
  s32 result;
  u8 *done;

  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgYouAllSet);
  Event_ShowMessage(9, 0);
  Func_02003fc4();
  result = FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (result == 0) {
    Func_02003fbc();
    Event_ShowMessage(9, 0);
    ((struct {
      u8 unk000[0x295];
      u8 unk295;
    } *)HeapSlot_AllocEwram(88, 0x298))
        ->unk295 = 99;
    FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
    Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 768);
    leader->update = (void (*)(void))Func_02001278;
    Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
    Actor_SetAnimation(9, 3);
    FieldEvent_Wait(20);
    Func_02003fc4();
    Actor_SetSpeed(9, 0x19999, 0xcccc);
    Actor_SetSpeed(10, 0x19999, 0xcccc);
    update = (void (*)(void))Func_02000df8;
    done = (u8 *)object + 99;
    *done = 0;
    object->update = update;
    Actor_SetScript(9, gData_020045a4);
    FieldEvent_Wait(30);
    Actor_Get(10)->update = update;
    Actor_SetScript(10, gData_020045f0);
    do {
      System_Wait(1);
    } while (*done == 0);
    Func_020034ec();
    System_Wait(1);
    ((struct IsletsEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x201;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02003fbc();
    FieldEvent_SetExitRequest(4);
  } else {
    Func_02003fbc();
    FieldEvent_SetMessageId((s32)MsgYouAllSet + 4);
    Event_ShowMessage(9, 0);
    Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  }
  Func_02003e6c();
}

void Func_02001bf4(void) {
  Func_02003e64();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(9, gMainRoutineWork.currentOwner, 0);
  FieldEvent_SetMessageId((s32)MsgMyPopS);
  Event_ShowMessage(9, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHWEST + FACING_STEP);
  Func_02003e6c();
}

void Func_02001c38(void) {
  struct IsletsEventWork *work = Ram_HeapSlots->eventWork;

  work->field15a = 1;
}

void Func_02001c4c(void) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y < 0x280000) {
    Actor_Get(11)->collisionFlags |= 8;
    Actor_Get(12)->collisionFlags |= 8;
  } else {
    Actor_Get(11)->collisionFlags &= ~8;
    Actor_Get(12)->collisionFlags &= ~8;
  }
}
