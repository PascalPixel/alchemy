#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "task.h"
#include "call.h"
#include "flag_call.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"

struct Actor *Func_02001f0c(s32 type, s32 x, s32 y, s32 z);
void Func_02001f4c(s32 a);
u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 VramCache_Load(s32 block, s32 count, u8 *source);

struct Actor *Func_02001210(void) {
  struct Actor *object =
      Func_02001f0c(0x1e9, 0x1680000, 0x400000, 0x1a80000);
  u8 *buffer = 0;
  struct FieldSprite *sprite;

  if (object == 0)
    return 0;
  sprite = object->animation;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  object->motionFlags = 0;
  object->unk5c[0] = 1;
  sprite->nextFrame = 0;
  sprite->unk1b[0] = 0;
  buffer = HeapSlot_AllocFar(68, 0x608);
  Func_02001f4c(242);
  VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
  HeapSlot_Release(68);
  return object;
}

extern u8 MsgAtLastI[];
extern const u8 gData_02002120[];

void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Task_RemoveFar(void (*func)(void));
void Actor_ArmCallback(s32 id, s32 a, s32 b);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 speed, s32 frames);
void Actor_SetAngleToward(s32 id, s32 target, s32 frames);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_02001188(void);
void Func_02001f14(struct Actor *object);
void Func_02001f2c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_02001fc4(s32 id, s32 a, s32 b);
void Func_02001fd4(s32 id, s32 target);
void Func_0200201c(const void *message);
void Func_02002034(s32 id, s32 a);
void Func_02002044(s32 id, s32 angle);
void Func_02002084(s32 a, s32 b);
void Func_0200208c(s32 frames);
void Func_020020ac(s32 a);
void Func_020020d4(s32 cue);

/// Once flag 7 is set, and only until flag 0x985 is: a scene of actor 7 and the
/// leader, which ends by setting flags 0x201 and 0x985.
void Func_0200128c(void) {
  struct Actor *handle;
  void (*func)(void);
  struct Actor *object;

  if (Flag_IsSet(0x985) != 0)
    return;
  if (Flag_IsSet(7) == 0)
    return;
  Func_02001f5c();
  Call1((void (*)())Func_020020ac, 0);
  Call2((void (*)())Func_02001fd4, 7, gMainRoutineWork.currentOwner);
  Call1((void (*)())wait, 1);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetPositionAndReset, 7, 412, 424);
  Call2((void (*)())Func_02002044, 7, 0xa000);
  Call1((void (*)())Func_0200201c, (s32)MsgAtLastI);
  Call3((void (*)())Actor_Jump, 7, 4, 10);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Func_02002044, 7, 0xe000);
  Call2((void (*)())Func_02002034, 7, 0);
  Call3((void (*)())Actor_SetPositionAndReset, 7, 386, 424);
  Call3((void (*)())Actor_SetAngleToward, 7, gMainRoutineWork.currentOwner,
        0);
  Call2((void (*)())Actor_SetModeById, gMainRoutineWork.currentOwner, 3);
  Call2((void (*)())Func_02002034, 7, 0);
  Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
        0xcccc, 0x6666);
  Call2((void (*)())Actor_SetMovementTypeById,
        gMainRoutineWork.currentOwner, (s32)gData_02002120);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 7, 360, 424);
  Call2((void (*)())Func_02002044, 7, 0xc000);
  Call1((void (*)())Func_020020d4, 182);
  handle = Func_02001210();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())Func_020020d4, 220);
  func = Func_02001188;
  Task_Add(func, TASK_PASS_VBLANK);
  Call2((void (*)())Func_02002084, 0x10003, 1);
  Call1((void (*)())Func_0200208c, 20);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02002084, 0x10006, 1);
  Call1((void (*)())Func_0200208c, 20);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02002084, 0x7fff, 0);
  Call1((void (*)())Func_0200208c, 20);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call6((void (*)())Func_02001f2c, 51, 38, 70, 22, 1, 2);
  Flag_Set(0x201);
  Call1((void (*)())Task_RemoveFar, (s32)func);
  if (handle != 0)
    Call1((void (*)())Func_02001f14, handle);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02002084, 0x10000, 0);
  Call1((void (*)())Func_0200208c, 20);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02002044, 7, 0x4000);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Actor_SetModeById, 7, 2);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != 0)
    Call3((void (*)())Actor_ResetAndSetPosition, 7,
          ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  Call1((void (*)())Actor_WaitForMoveById, 7);
  Call3((void (*)())Func_02001fc4, 7, 0, 0);
  Flag_Set(0x985);
  Func_02001f64();
}

extern const u8 gData_020020e4[];
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Actor_CommitPositionAndActivate(s32 id, s32 dx, s32 dz);
void Actor_SetMovementTypeAndWaitById(s32 id, const void *script);
void Actor_StartRepeatedMotion(s32 id, s32 variant);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Motion_SetVarCbAndRefresh(s32 id, s32 variant);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 flags);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 flags);
void Func_0200202c(s32 id, s32 a, s32 b);
s32 Func_0200204c(s32 id, s32 emote, s32 frames);
void Func_02002054(s32 id, s32 a);
void Func_0200205c(s32 speed, s32 acceleration);
void Func_020020b4(s32 id, s32 dx, s32 dz, s32 angle);

/// Once flag 0x8ff is set, and only until flag 0x989 is: the crew talks
/// over the ship, the leader answers two prompts, and everyone leaves by
/// script gData_020020e4; it sets flag 0x981.
void Func_020014c0(void) {
  if (Flag_IsSet(0x8ff) == 0)
    return;
  if (Flag_IsSet(0x989) != 0)
    return;
  Func_02001f5c();
  Call1((void (*)())Func_020020ac, 0);
  Call1((void (*)())Func_0200201c, (s32)MsgMyShipAt);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 4, 264, 176);
  Call1((void (*)())Actor_WaitForMoveById, 4);
  Call4((void (*)())Func_020020b4, 7, 16, 8, 0x4000);
  Call2((void (*)())Func_0200205c, 0xcccc, 0x1999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1200000, -1, 0xd00000, 1);
  Call4((void (*)())Func_020020b4, 9, 32, 0, 0x4000);
  Call4((void (*)())Func_020020b4, 6, 48, -8, 0x4000);
  Call4((void (*)())Func_020020b4, 5, -16, -8, 0x4000);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x26666, 0x13333);
  Call3((void (*)())Actor_SetPositionAndReset, 7, 280, 224);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value3((s32 (*)())Func_0200204c, 7, 264, 0);
  Call3((void (*)())Actor_Jump, 7, 4, 10);
  Call3((void (*)())Actor_Jump, 7, 4, 20);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3((s32 (*)())Func_0200204c, 9, 265, 20);
  Call2((void (*)())Func_02002034, 9, 0);
  Call2((void (*)())Func_0200205c, 0xcccc, 0x1999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1200000, -1, 0xb80000, 1);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0xcccc, 0x6666);
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, 0, -16);
  Call2((void (*)())Func_02002044, 7, 0xe000);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3((s32 (*)())Func_0200204c, 9, 258, 20);
  Call2((void (*)())Func_02002044, 6, 0x6000);
  Call2((void (*)())Actor_SetModeById, 6, 4);
  Call2((void (*)())Func_02002034, 6, 0);
  Value3((s32 (*)())Func_0200204c, 7, 257, 20);
  Call3((void (*)())Actor_ArmCallback, 5, 0x2000, 0);
  Value3((s32 (*)())Func_0200204c, 5, 261, 20);
  Call2((void (*)())Func_02002034, 5, 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0xa000, 0);
  Value3((s32 (*)())Func_0200204c, 7, 261, 20);
  Call2((void (*)())Actor_SetModeById, 6, 4);
  Call2((void (*)())Func_02002034, 6, 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0xe000, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002044, 7, 0xa000);
  Call2((void (*)())Func_02002054, 7, 258);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02002034, 0x2007, 0);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 6, 3);
  Call2((void (*)())Actor_SetModeById, 9, 4);
  Call2((void (*)())Func_02002034, 9, 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0xe000, 0);
  Call2((void (*)())Func_02002034, 9, 0);
  Call2((void (*)())Func_02002044, 9, 0x8000);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 9, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0, 0);
  if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0) == 0) {
    Value3((s32 (*)())Func_0200204c, 5, 263, 20);
    Call2((void (*)())Func_02002034, 5, 0);
    Call2((void (*)())Func_02002034, 6, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
    Call3((void (*)())Actor_ArmCallback, 7, 0xa000, 0);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
    Call2((void (*)())Func_02002034, 5, 0);
    Call3((void (*)())Actor_ArmCallback, 7, 0xe000, 0);
    Call2((void (*)())Actor_SetModeById, 6, 3);
    Call2((void (*)())Func_02002034, 6, 0);
  }
  Value3((s32 (*)())Func_0200204c, 9, 265, 0);
  Call2((void (*)())Func_02002044, 9, 0xe000);
  Call2((void (*)())Func_02002034, 0x9009, 0);
  Call2((void (*)())Func_02002044, 7, 0xe000);
  Call2((void (*)())Actor_SetModeById, 7, 4);
  Call2((void (*)())Func_02002034, 7, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0x2000,
        0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 0);
  Value3((s32 (*)())Func_0200204c, 5, 257, 0);
  Value3((s32 (*)())Func_0200204c, 6, 257, 40);
  Call2((void (*)())Func_02002044, 7, 0xc000);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3((s32 (*)())Func_0200204c, 9, 265, 20);
  Call2((void (*)())Func_02002034, 9, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 6, 0x8000, 40);
  Call3((void (*)())Actor_ArmCallback, 5, 0x2000, 0);
  Call2((void (*)())Func_02002044, 6, 0x6000);
  Call2((void (*)())Func_02002044, 7, 0xe000);
  Value3((s32 (*)())Func_0200204c, 7, 257, 20);
  Call2((void (*)())Func_02002054, 9, 258);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02002034, 9, 0);
  Call3((void (*)())Actor_ArmCallback, 7, 0xe000, 0);
  Value3((s32 (*)())Func_0200204c, 7, 264, 20);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3((s32 (*)())Func_0200204c, 5, 264, 0);
  Value3((s32 (*)())Func_0200204c, 6, 264, 40);
  Value3((s32 (*)())Func_0200204c, 9, 261, 20);
  Call2((void (*)())Func_02002034, 9, 0);
  Value3((s32 (*)())Func_0200204c, 5, 0x100, 0);
  Value3((s32 (*)())Func_0200204c, 6, 0x100, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0, 20);
  Call2((void (*)())Func_02002044, gMainRoutineWork.currentOwner, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x8000, 40);
  Call2((void (*)())Func_02002044, 9, 0x4000);
  Call2((void (*)())Func_02002034, 9, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Call2((void (*)())Func_02002034, 7, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0x4000,
        0);
  Value3((s32 (*)())Func_0200204c, 9, 0x100, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 9, 3);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0, 0);
  Call2((void (*)())Func_02002054, 5, 258);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002044, 9, 0x8000);
  Call2((void (*)())Actor_StartRepeatedMotion, 9, 2);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 3);
  Value3((s32 (*)())Func_0200204c, 7, 265, 0);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Func_02002044, 9, 0x5000);
  Call2((void (*)())Actor_StartRepeatedMotion, 9, 2);
  Value3((s32 (*)())Func_0200204c, 9, 261, 40);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 3);
  Call2((void (*)())Func_02002034, 9, 0);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Func_02002034, 7, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0x2000,
        0);
  Call3((void (*)())Actor_ArmCallback, 5, 0x2000, 0);
  Value3((s32 (*)())Func_0200204c, 6, 257, 20);
  Call2((void (*)())Func_02002034, 6, 0);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3((s32 (*)())Func_0200204c, 4, 257, 20);
  Call2((void (*)())Func_02002044, 7, 0xa000);
  Call2((void (*)())Func_02002034, 0x2007, 0);
  Call2((void (*)())Func_02002044, 7, 0xc000);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Func_02002034, 7, 0);
  Value3((s32 (*)())Func_0200204c, 5, 265, 40);
  Call2((void (*)())Func_02002034, 5, 0);
  Call2((void (*)())Func_02002044, 7, 0xa000);
  Call2((void (*)())Actor_SetModeById, 7, 4);
  Call2((void (*)())Func_02002034, 0x2007, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
  Call2((void (*)())Func_02002044, 7, 0xe000);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 3);
  Call2((void (*)())Func_02002044, 7, 0xa000);
  Call2((void (*)())Func_02002034, 0x2007, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x8000, 0);
  Value3((s32 (*)())Func_0200204c, 9, 264, 20);
  Call2((void (*)())Func_02002034, 9, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0, 0);
  Call2((void (*)())Func_02002044, 6, 0x8000);
  Call2((void (*)())Actor_SetModeById, 6, 3);
  Call2((void (*)())Func_02002034, 6, 0);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call2((void (*)())Func_02002034, 5, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, -16, 0);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Actor_CommitPositionAndActivate, 7, -16, 0);
  Value3((s32 (*)())Func_0200204c, 7, 261, 20);
  Call3((void (*)())Actor_ArmCallback, 5, 0x4000, 0);
  Value3((s32 (*)())Func_0200204c, 5, 257, 40);
  Call3((void (*)())Actor_CommitPositionAndActivate, 5, 0, 16);
  Call2((void (*)())Func_02002034, 5, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0x4000,
        0);
  Call3((void (*)())Actor_ArmCallback, 6, 0x6000, 0);
  Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Call3((void (*)())Func_0200202c, 7, 0, 20);
  Call2((void (*)())Func_02002034, 7, 0);
  Call2((void (*)())Func_02002044, 7, 0xe000);
  Call2((void (*)())Func_02002034, 7, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0, 0);
  Call2((void (*)())Func_02002044, 9, 0x8000);
  Call2((void (*)())Actor_SetModeById, 9, 3);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 9, 0);
  if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0) == 1) {
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 4);
    Call2((void (*)())Func_02002034, 9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 3);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Call2((void (*)())Func_02002034, 9, 0);
  }
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0x4000,
        0);
  Call2((void (*)())Func_02002044, 9, 0x5000);
  Value3((s32 (*)())Func_0200204c, 9, 264, 20);
  Call2((void (*)())Func_02002034, 9, 0);
  Value3((s32 (*)())Func_0200204c, 7, 264, 0);
  Call2((void (*)())Func_02002034, 7, 0);
  Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner, 0, 0);
  Call2((void (*)())Func_02002044, 9, 0x8000);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 9, 3);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call2((void (*)())Actor_SetModeById, 6, 3);
  Call2((void (*)())Actor_SetModeById, 4, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 3);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x13333, 0x9999);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x13333, 0x9999);
  Actor_SetMovementTypeById(5, gData_020020e4);
  Actor_SetMovementTypeById(9, gData_020020e4);
  Actor_SetMovementTypeById(6, gData_020020e4);
  Actor_SetMovementTypeAndWaitById(7, gData_020020e4);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call1((void (*)())GameFlag_SetBit, 0x981);
  Func_02001f64();
}
