#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "task.h"
#include "call.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);

struct Actor *Func_02001210(void) {
  struct Actor *object = (struct Actor *)Import_ActorCreate(0x1e9, 0x1680000, 0x400000, 0x1a80000);
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
  Icon_DrawItem(242);
  VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
  HeapSlot_Release(68);
  return object;
}

extern u8 MsgAtLastI[];
extern const u8 gData_02002120[];

void Func_02001188(void);
void Func_02001f14(struct Actor *object);
void Func_02001f5c(void);
void Func_02001f64(void);
void Func_02001fd4(s32 id, s32 target);

/// Once flag 7 is set, and only until flag 0x985 is: a scene of actor 7 and the
/// leader, which ends by setting flags 0x201 and 0x985.
void Func_0200128c(void) {
  struct Actor *handle;
  void (*func)(void);
  struct Actor *object;

  if (Event_GetFlag(0x985) != 0)
    return;
  if (Event_GetFlag(7) == 0)
    return;
  Func_02001f5c();
  FieldEvent_ResolvePendingActions(0);
  Call2((void (*)())Func_02001fd4, 7, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_SetSpeed(7, 0x19999, 0xcccc);
  Actor_WalkToAndWait(7, 412, 424);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  FieldEvent_SetMessageId((s32)MsgAtLastI);
  FieldActor_Jump(7, 4, 10);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Event_ShowMessage(7, 0);
  Actor_WalkToAndWait(7, 386, 424);
  Actor_FaceActor(7, gMainRoutineWork.currentOwner, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Event_ShowMessage(7, 0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetScript(gMainRoutineWork.currentOwner, gData_02002120);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_WalkToAndWait(7, 360, 424);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Event_PlaySound(182);
  handle = Func_02001210();
  FieldEvent_Wait(20);
  Event_PlaySound(220);
  func = Func_02001188;
  Task_Add(func, TASK_PASS_VBLANK);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(0x10006, 1);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  MapGrid_CopyCellsTo(51, 38, 70, 22, 1, 2);
  Event_SetFlag(0x201);
  FieldTask_Remove(func);
  if (handle != 0)
    Call1((void (*)())Func_02001f14, handle);
  FieldEvent_Wait(20);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(20);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(7, FACING_SOUTH);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_SetAnimation(7, 2);
  object = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object != 0)
    Actor_SetDestination(7, ((s16 *)&object->x)[1], ((s16 *)&object->z)[1]);
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Event_SetFlag(0x985);
  Func_02001f64();
}

extern const u8 gData_020020e4[];

/// Once flag 0x8ff is set, and only until flag 0x989 is: the crew talks
/// over the ship, the leader answers two prompts, and everyone leaves by
/// script gData_020020e4; it sets flag 0x981.
void Func_020014c0(void) {
  if (Event_GetFlag(0x8ff) == 0)
    return;
  if (Event_GetFlag(0x989) != 0)
    return;
  Func_02001f5c();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgMyShipAt);
  Actor_WalkTo(4, 264, 176);
  FieldActor_WaitForMoveById(4);
  FieldActor_LaunchFromLeader(7, 16, 8, FACING_SOUTH);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(288), -1, PIXELS(208), 1);
  FieldActor_LaunchFromLeader(9, 32, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(6, 48, -8, FACING_SOUTH);
  FieldActor_LaunchFromLeader(5, -16, -8, FACING_SOUTH);
  FieldEvent_Wait(40);
  Actor_SetSpeed(7, 0x26666, 0x13333);
  Actor_WalkToAndWait(7, 280, 224);
  FieldEvent_Wait(20);
  Actor_ShowEmote(7, 264, 0);
  FieldActor_Jump(7, 4, 10);
  FieldActor_Jump(7, 4, 20);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, 265, 20);
  Event_ShowMessage(9, 0);
  FieldCamera_SetSpeed(0xcccc, 0x1999);
  FieldCamera_MoveTo(PIXELS(288), -1, PIXELS(184), 1);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_WalkByAndWait(7, 0, -16);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Actor_SetAnimationAndWait(7, 3);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, 258, 20);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_ShowEmote(7, 257, 20);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(5, 261, 20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Actor_ShowEmote(7, 261, 20);
  Actor_SetAnimation(6, 4);
  Event_ShowMessage(6, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(5, 3);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Actor_SetAttachedEffect(7, 258);
  FieldEvent_Wait(20);
  Event_ShowMessage(0x2007, 0);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(6, 3);
  Actor_SetAnimation(9, 4);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_ShowMessage(9, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  Event_OpenMessage(9, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Actor_ShowEmote(5, 263, 20);
    Event_ShowMessage(5, 0);
    Event_ShowMessage(6, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
    Actor_FaceDirection(7, FACING_NORTHWEST, 0);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(5, 0);
    Actor_FaceDirection(7, FACING_NORTHEAST, 0);
    Actor_SetAnimation(6, 3);
    Event_ShowMessage(6, 0);
  }
  Actor_ShowEmote(9, 265, 0);
  Actor_FaceDirectionAndWait(9, FACING_NORTHEAST);
  Event_ShowMessage(0x9009, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Actor_SetAnimation(7, 4);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(5, 257, 0);
  Actor_ShowEmote(6, 257, 40);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(9, 265, 20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_WEST, 40);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(6, FACING_SOUTHWEST);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Actor_ShowEmote(7, 257, 20);
  Actor_SetAttachedEffect(9, 258);
  FieldEvent_Wait(20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_ShowEmote(7, 264, 20);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(5, 264, 0);
  Actor_ShowEmote(6, 264, 40);
  Actor_ShowEmote(9, 261, 20);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_FaceDirection(5, FACING_EAST, 20);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_EAST);
  Actor_FaceDirection(9, FACING_WEST, 40);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  Event_ShowMessage(9, 0);
  Actor_SetAnimationAndWait(7, 3);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_ShowEmote(9, 0x100, 20);
  Actor_RunRepeatedMotion(9, 3);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_SetAttachedEffect(5, 258);
  FieldEvent_Wait(20);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  FieldActor_StartRepeatedMotion(9, 2);
  Actor_SetAnimationAndWait(9, 3);
  Actor_ShowEmote(7, 265, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH + FACING_STEP);
  FieldActor_StartRepeatedMotion(9, 2);
  Actor_ShowEmote(9, 261, 40);
  Actor_SetAnimationAndWait(9, 3);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(6, 257, 20);
  Event_ShowMessage(6, 0);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(4, 257, 20);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTH);
  Actor_SetAnimation(7, 3);
  Event_ShowMessage(7, 0);
  Actor_ShowEmote(5, 265, 40);
  Event_ShowMessage(5, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Actor_SetAnimation(7, 4);
  Event_ShowMessage(0x2007, 0);
  Actor_SetAnimationAndWait(5, 3);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Event_ShowMessage(7, 0);
  Actor_SetAnimationAndWait(9, 3);
  Actor_FaceDirectionAndWait(7, FACING_NORTHWEST);
  Event_ShowMessage(0x2007, 0);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_ShowEmote(9, 264, 20);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(6, FACING_WEST);
  Actor_SetAnimation(6, 3);
  Event_ShowMessage(6, 0);
  Actor_SetAnimation(5, 3);
  Event_ShowMessage(5, 0);
  Actor_WalkByAndWait(7, -16, 0);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(7, -16, 0);
  Actor_ShowEmote(7, 261, 20);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_ShowEmote(5, 257, 40);
  Actor_WalkByAndWait(5, 0, 16);
  Event_ShowMessage(5, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 0);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_ShowMessageThenWait(7, 0, 20);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  Actor_SetAnimation(9, 3);
  Event_OpenMessage(9, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 1) {
    Actor_SetAnimationAndWait(9, 4);
    Event_ShowMessage(9, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Actor_SetAnimationAndWait(9, 3);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Event_ShowMessage(9, 0);
  }
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(9, FACING_SOUTH + FACING_STEP);
  Actor_ShowEmote(9, 264, 20);
  Event_ShowMessage(9, 0);
  Actor_ShowEmote(7, 264, 0);
  Event_ShowMessage(7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(9, FACING_WEST);
  Actor_SetAnimationAndWait(9, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimationAndWait(7, 3);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetScript(5, gData_020020e4);
  Actor_SetScript(9, gData_020020e4);
  Actor_SetScript(6, gData_020020e4);
  Actor_RunScriptAndWait(7, gData_020020e4);
  FieldEvent_Wait(20);
  Event_SetFlag(0x981);
  Func_02001f64();
}
