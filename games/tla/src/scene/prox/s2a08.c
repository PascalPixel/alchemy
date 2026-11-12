/// @file
/// Scene code of overlay 0x6a8, in ROM order, from s2a08's offset up to the next object the overlay links.

#include "types.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "local.h"
#include "input.h"
#include "scene_tables.h"

void Func_02002a08(void) {
  s32 variant =
      ((struct FieldActor *)Actor_Get(gMainRoutineWork.currentOwner))
          ->sprite->priority;

  FieldActor_SetSpritePriority(8, variant);
  FieldActor_SetSpritePriority(10, variant);
}

// Unless flag 0x109 is set, calls Func_02001c24 with 0 for entrance 6 and with
// 1 for entrances 7 and 8, then schedules Func_02002a08. The entrance is read
// unsigned first and tested signed against 6, as the native reads it twice.

void Func_02001c24(s32 value);
void Func_020045dc(s32 value);
void Func_02002a08(void);

void Func_02002a38(void) {
  s32 priority;

  if (Event_GetFlag(0x109) == 0) {
    s32 entrance = (u16)gMainRoutineWork.entrance;

    if (gMainRoutineWork.entrance == 6)
      Func_02001c24(0);
    else if ((u32)((entrance - 7) << 16) <= 0x10000)
      Func_02001c24(1);
  }
  Func_020045dc(0);
  // The priority in a local is built before the task function's address (passed
  // directly, the address loads first).
  priority = 0x480;
  Task_AddFrame(Func_02002a08, priority);
}

s32 Func_02004414();
s32 Func_020044dc();
s32 Func_02004464();
void Func_0200457c(void);
extern u8 gData_02005ee8[];
extern u8 gData_02005f2c[];
extern u8 gData_02005f70[];
extern u8 gData_02005fac[];
extern u8 gData_02005fe8[];
extern u8 gData_02006038[];
extern u8 gData_02006088[];
extern u8 gData_020060cc[];
extern u8 gData_020060fc[];
extern u8 gData_0200617c[];
extern u8 gData_020061fc[];
extern u8 gData_0200622c[];
extern u8 gData_02006268[];
extern u8 MsgProx153[];

void Func_02002a8c(void) {
  u32 i;

  if (Event_GetFlag(0x9c5) != 0) {
    Func_02004464();
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(78);
    FieldActor_Jump(5, 2, 10);
    FieldActor_Jump(5, 4, 10);
    FieldEvent_SetMessageId((s32)MsgProx153);
    Event_ShowMessage(5, 0);
    FieldCamera_SetSpeed(0x33333, 0x6666);
    FieldCamera_MoveTo(PIXELS(144), -1, PIXELS(228), 1);
    Func_0200457c();
    Prox_Jump(10, 0x108, 0);
    Event_ShowMessage(0x200a, 0);
    Event_PlaySound(37);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 136, 196);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Func_020044dc(0, gMainRoutineWork.currentOwner);
    Func_020044dc(0xb, gMainRoutineWork.currentOwner);
    System_Wait(1);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetScript(0, (s32)gData_02005ee8);
    Actor_RunScriptAndWait(11, (s32)gData_02005f2c);
    Actor_SetAnimationAndWait(9, 3);
    Event_ShowMessage(9, 0);
    Actor_FaceDirectionAndWait(5, FACING_SOUTHWEST);
    Actor_ShowEmote(5, 0x103, 20);
    Event_ShowMessage(5, 0);
    Actor_FaceDirection(6, FACING_NORTHEAST, 0);
    Prox_Jump(6, 0x108, 20);
    Event_ShowMessage(6, 0);
    Actor_FaceDirectionAndWait(7, FACING_WEST);
    Actor_SetAnimationAndWait(7, 3);
    Actor_FaceDirectionAndWait(7, FACING_NORTHEAST);
    Event_ShowMessage(7, 0);
    Actor_ShowEmote(5, 0x107, 20);
    Event_ShowMessage(5, 0);
    Actor_SetAnimation(3, 4);
    Event_ShowMessage(3, 0);
    Actor_ShowEmote(10, 0x108, 0);
    Actor_FaceDirectionAndWait(10, 0xd000);
    Event_ShowMessage(10, 0);
    Actor_FaceDirectionAndWait(5, FACING_SOUTH);
    FieldActor_Jump(5, 4, 10);
    Event_ShowMessage(5, 0);
    Prox_Jump(1, 0x109, 0);
    Actor_FaceDirectionAndWait(1, FACING_NORTHWEST);
    Event_ShowMessage(1, 0);
    Actor_ShowEmote(2, 0x105, 40);
    Actor_FaceDirectionAndWait(2, FACING_NORTHWEST);
    Event_ShowMessage(2, 0);
    Actor_ShowEmote(0, 0x109, 40);
    Actor_FaceDirectionAndWait(0, 0);
    FieldEvent_ShowMessageThenWait(0x8000, 0, 0xa);
    Actor_FaceDirection(8, 0x3000, 20);
    FieldEvent_ShowMessageThenWait(0x4008, 0, 0x14);
    if (Event_GetFlag(0x824) != 0) {
      Actor_FaceDirectionAndWait(0, FACING_SOUTH);
      Actor_SetAnimation(0, 4);
      Event_ShowMessage(0, 0);
      Actor_FaceDirectionAndWait(8, 0xd000);
      Event_ShowMessage(8, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId += 2;
      Actor_FaceDirectionAndWait(8, 0xd000);
      Event_ShowMessage(8, 0);
    }
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Actor_SetAnimationAndWait(0, 3);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(0x200b, 0);
    Actor_FaceDirection(11, 0x3000, 20);
    Actor_FaceDirection(11, FACING_WEST, 20);
    Actor_FaceDirectionAndWait(11, 0x5000);
    Event_ShowMessage(0x200b, 0);
    Actor_FaceDirectionAndWait(10, FACING_WEST);
    Event_ShowMessage(0x600a, 0);
    Actor_FaceDirectionAndWait(9, 0);
    Actor_SetAnimation(9, 3);
    Event_ShowMessage(0x5009, 0);
    Actor_FaceDirectionAndWait(8, 0);
    Event_ShowMessage(0x5008, 0);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(0x200b, 0);
    Actor_FaceDirectionAndWait(0, 0);
    Actor_SetAnimation(0, 3);
    Prox_MessageOpen(0x8000, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(0, 3);
      Event_ShowMessage(0x8000, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(0, 4);
      Event_ShowMessage(0x8000, 0);
    }
    Actor_FaceDirectionAndWait(5, FACING_NORTHWEST);
    FieldActor_Jump(5, 2, 10);
    FieldActor_Jump(5, 4, 10);
    Event_ShowMessage(0x8005, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimation(6, 3);
    Actor_SetAnimation(7, 3);
    Actor_SetAnimation(8, 3);
    Actor_SetAnimation(9, 3);
    Actor_SetAnimation(10, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimation(2, 3);
    Actor_SetAnimationAndWait(1, 3);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetSpeed(5, 0xcccc, 0x6666);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_SetSpeed(7, 0xcccc, 0x6666);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_SetSpeed(3, 0xcccc, 0x6666);
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    FieldActor_SetSpritePriority(0, 2);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    FieldActor_SetSpritePriority(11, 2);
    FieldActor_SetSpritePriority(5, 2);
    FieldActor_SetSpritePriority(6, 2);
    FieldActor_SetSpritePriority(7, 2);
    FieldActor_SetSpritePriority(8, 2);
    FieldActor_SetSpritePriority(9, 2);
    FieldActor_SetSpritePriority(10, 2);
    FieldActor_SetSpritePriority(3, 2);
    FieldActor_SetSpritePriority(2, 2);
    FieldActor_SetSpritePriority(1, 2);
    FieldCamera_SetSpeed(0x6666, 0xccc);
    FieldCamera_MoveTo(PIXELS(144), -1, PIXELS(246), 1);
    Actor_SetScript(9, (s32)gData_02005f70);
    Actor_SetScript(10, (s32)gData_02005fac);
    FieldEvent_Wait(10);
    Actor_SetScript(8, (s32)gData_02005fe8);
    Actor_SetScript(3, (s32)gData_02006038);
    FieldEvent_Wait(10);
    Actor_SetScript(6, gData_02005fe8);
    Actor_SetScript(7, (s32)gData_02005fe8);
    FieldEvent_Wait(10);
    Actor_SetScript(2, gData_02006038);
    Actor_SetScript(1, (s32)gData_02006088);
    FieldEvent_Wait(30);
    Actor_SetScript(5, (s32)gData_020060cc);
    FieldEvent_Wait(20);
    Actor_SetScript(0, (s32)gData_020060fc);
    Actor_SetScript(gMainRoutineWork.currentOwner,
                              (s32)gData_0200617c);
    FieldEvent_Wait(10);
    Actor_RunScriptAndWait(11, (s32)gData_020061fc);
    Actor_ShowEmote(11, 0x101, 20);
    Actor_FaceDirection(11, 0xb000, 0);
    FieldActor_RefreshSelectorById(5);
    Actor_FaceDirection(5, FACING_WEST, 0);
    Actor_FaceDirection(1, 0, 40);
    Actor_FaceDirection(5, FACING_NORTHWEST, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_ShowEmote(11, 0x108, 40);
    Prox_MessageOpen(0xa00b, 0);
    Actor_FaceDirection(0, 0, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(0xa00b, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(0xa00b, 0);
    }
    Actor_FaceDirection(0, FACING_NORTH, 20);
    Event_ShowMessage(0, 0);
    Actor_SetAnimation(11, 4);
    Event_ShowMessage(0xa00b, 0);
    FieldActor_Jump(1, 2, 10);
    Event_ShowMessage(1, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_FaceDirection(11, 0x3000, 20);
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(5, 0);
    Actor_SetAnimationAndWait(11, 4);
    Event_ShowMessage(11, 0);
    Prox_Jump(1, 0x103, 0);
    FieldEvent_ShowMessageThenWait(1, 0, 0x14);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(11, 0);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Actor_ShowEmote(11, 0x103, 0);
    Prox_MessageOpen(0xa00b, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(0xa00b, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(0xa00b, 0);
    }
    Prox_Jump(1, 0x101, 20);
    Event_ShowMessage(1, 0);
    Actor_FaceDirectionAndWait(11, 0x3000);
    Event_ShowMessage(11, 0);
    Actor_ShowEmote(5, 0x101, 0);
    Event_ShowMessage(5, 0);
    Actor_SetAnimationAndWait(11, 3);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(0xa00b, 0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimationAndWait(5, 3);
    FieldEvent_Wait(20);
    Event_ShowMessage(1, 0);
    Actor_ShowEmote(11, 0x103, 0);
    Actor_FaceDirectionAndWait(11, 0x3000);
    Event_ShowMessage(11, 0);
    Actor_SetAttachedEffect(1, 0x102);
    Actor_SetAnimationAndWait(1, 4);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(11, 0);
    Actor_SetAnimationAndWait(5, 3);
    Event_ShowMessage(5, 0);
    FieldActor_Jump(1, 2, 10);
    Event_ShowMessage(1, 0);
    Actor_SetAnimationAndWait(11, 3);
    Actor_FaceDirectionAndWait(0, FACING_SOUTH);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(5, FACING_WEST);
    Actor_SetAnimation(5, 3);
    Event_ShowMessage(5, 0);
    Actor_FaceDirectionAndWait(1, 0);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimationAndWait(1, 3);
    FieldEvent_Wait(10);
    Actor_SetSpeed(5, 0x19999, 0xcccc);
    Actor_SetSpeed(1, 0x19999, 0xcccc);
    Actor_SetScript(5, (s32)gData_0200622c);
    Actor_RunScriptAndWait(1, (s32)gData_02006268);
    FieldEvent_Wait(20);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(0xa00b, 0);
    Actor_WalkToAndWait(11, 156, 256);
    Actor_ShowEmote(11, 0x100, 0);
    Event_ShowMessage(0, 0);
    Actor_SetAnimation(0, 4);
    Event_ShowMessage(0, 0);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Event_ShowMessage(0, 0);
    Actor_ShowEmote(11, 0x109, 40);
    Prox_MessageOpen(0xa00b, 0);
    Actor_FaceDirection(0, 0, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      Actor_SetAnimationAndWait(11, 3);
      Event_ShowMessage(0xa00b, 0);
      EVENT_WORK->messageId++;
    } else {
      EVENT_WORK->messageId++;
      Actor_SetAnimationAndWait(11, 4);
      Event_ShowMessage(0xa00b, 0);
    }
    Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
    Actor_SetAnimation(11, 4);
    FieldEvent_ShowMessageThenWait(0xa00b, 0, 0x14);
    Event_ShowMessage(0, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(11, 3);
    FieldEvent_Wait(20);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 40);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
    Actor_FaceDirectionAndWait(0, FACING_SOUTHEAST);
    Event_ShowMessage(0, 0);
    Actor_SetAnimation(11, 4);
    Event_ShowMessage(0xa00b, 0);
    Prox_Jump(0, 0x101, 0);
    Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x101, 40);
    Event_ShowMessage(0xa00b, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimation(11, 3);
    Event_ShowMessage(0xa00b, 0);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_FaceDirection(11, 0x3000, 20);
    FieldActor_Jump(11, 4, 10);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirectionAndWait(11, 0x3000);
    FieldActor_Jump(11, 2, 10);
    FieldActor_Jump(11, 4, 10);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_WEST);
    Actor_FaceDirectionAndWait(11, 0xb000);
    Event_ShowMessage(0xa00b, 0);
    Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_ShowEmote(11, 0x108, 0);
    Actor_FaceDirectionAndWait(11, 0x3000);
    Event_ShowMessage(0xa00b, 0);
    Actor_SetAnimation(11, 9);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 0x21);
    Actor_SetAnimation(0, 0x28);
    Func_02004414(2);
    Func_02004414(3);
    Func_02004414(4);
    Func_02004414(5);
    Func_02004414(6);
    Event_PlaySound(78);
    FieldEvent_Wait(40);
    PaletteFade_ApplyTarget(0x10009, 0);
    PaletteFade_Interpolate(0x78);
    System_Wait(120);
    // Waits up to 120 frames, or until a key is held.
    i = 0;
    goto test_wait;
  next_wait:
    i++;
  test_wait:
    if (i < 120) {
      System_Wait(1);
      if (gInput.held == 0)
        goto next_wait;
    }
    PaletteFade_ApplyTarget(0, 0);
    PaletteFade_Interpolate(0x10);
    System_Wait(16);
    Event_SetFlag(0x9cc);
    FieldEvent_SetExitRequest(3);
  }
}
