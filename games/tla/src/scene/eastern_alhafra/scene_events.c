#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

struct DoorStep {
  s32 script;
  u16 x;
  u16 z;
};

extern struct DoorStep gData_02005290[];

s32 GameFlag_Test();
void GameFlag_SetBit();
struct ObjectRuntime *Object_GetById();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_ArmCallback();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_CommitCurrentPositionAndActivate();
void Motion_SetVarCbAndRefresh();
void Battle_WaitMode0();
s32 UiText_OpenMessageAtObject();
s32 Event_AskYesNoWithActorAnimation();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_0200112c(void);
void Func_020017ec(void);
void Func_02001cb0(void);
void Func_02000394(s32, s32);
void Func_0200042c(s32, s32);
void Func_02000514(void);
s32 Func_02004a44();
s32 Func_02004a84();
void Func_02004a94();
void Func_02004a9c();
s32 Func_02004b04();
s32 Func_02004b3c();
s32 Func_02004b4c();
s32 Func_02004b5c();
s32 Func_02004b6c();
s32 Func_02004bac();
s32 Func_02004bb4();
s32 Func_02004be4();
s32 Func_02004bec();
s32 Func_02004c44();
s32 Func_02004c4c();
void Func_02004c9c(s32);

void Func_020006ec(s32 mode) {
  s32 tile;

  tile = Func_02004bb4(gPartyState.currentOwner);
  if (tile == 16) {
    Func_02000394(mode, 16);
    if (Func_02004c44() != 0)
      Func_02004c4c();
    Func_0200042c(mode, 16);
  } else if (tile == 15) {
    Func_02000514();
  }
}

void Func_02000730(void) {
  GameFlag_SetBit(0x8a8);
  Func_02004a94();
  Func_02004be4(0);
  Func_02004b3c(MsgWhatSYour);
  Func_02004b6c(22, 0x101, 40);
  ObjectMotion_SetAngleToward(22, gPartyState.currentOwner, 0);
  Battle_WaitMode0(20);
  Call3((void (*)())Func_02004b4c, 22, 0, 10);
  Motion_SetVarCbAndRefresh(22, 2);
  Battle_WaitMode0(10);
  UiText_OpenMessageAtObject(22, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) != 0) {
    Battle_WaitMode0(30);
    Call3((void (*)())Func_02004b4c, 22, 0, 0);
  } else {
    Battle_WaitMode0(20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    UiText_OpenMessageAtObject(22, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) != 0) {
      Battle_WaitMode0(30);
      Call3((void (*)())Func_02004b4c, 22, 0, 0);
    } else {
      Battle_WaitMode0(20);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
      UiText_OpenMessageAtObject(22, 0);
      if (Event_AskYesNoWithActorAnimation(4, 0) != 0) {
        Battle_WaitMode0(30);
        Call3((void (*)())Func_02004b4c, 22, 0, 0);
      } else {
        Battle_WaitMode0(20);
        ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
        Call3((void (*)())Func_02004b4c, 22, 0, 0);
      }
    }
  }
  Call3(ObjectMotion_ArmCallback, 22, 0, 0);
  Func_02004a9c();
}

void Func_02000838(void) {
  if (Value1(GameFlag_Test, 0x8a7)) {
    if (Value1(GameFlag_Test, 0x904) == 0) {
      Call1(GameFlag_SetBit, 0x904);
      Func_02001cb0();
    }
  } else if (Value1(GameFlag_Test, 0x8ab)) {
    if (Value1(GameFlag_Test, 0x8a0) && Func_02004a84(151) == -1 &&
        Value1(GameFlag_Test, 0x8ad) == 0) {
      Call1(GameFlag_SetBit, 0x8ad);
      Func_020017ec();
    }
  } else if (Value1(GameFlag_Test, 0x8a9) == 0) {
    Call1(GameFlag_SetBit, 0x8a9);
    Func_0200112c();
  }
}

void Func_020008cc(struct DoorStep *step, s32 countdown, s32 unused) {
  Func_02004a94();
  Func_02004be4(0);
  Func_02004c9c(158);
  Func_02004a44(step->script, step->x, step->z);
  Object_GetById(gPartyState.currentOwner)->motionFlags = 2;
  ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
  Object_SetModeById(gPartyState.currentOwner, 2);
  Call3((void (*)())ObjectMotion_CenterOnColumn, gPartyState.currentOwner, 2,
        -8);
  Battle_WaitMode0(10);
  Func_02004bac(countdown);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004a9c();
}

void Func_02000940(s32 countdown) {
  Func_020008cc(gData_02005290, countdown, 0);
}

void Func_02000954(void) {}

void Func_02000958(void) { GameFlag_SetBit(0x8be); }

void Func_02000968(void) {
  struct ObjectRuntime *leader;

  goto check;
run:
  if (Func_02004a84(151) != -1)
    return;
  GameFlag_SetBit(0x8bf);
  Func_02004a94();
  Func_02004be4(0);
  Call3(ObjectMotion_SetPositionAndReset, 4, 488, 440);
  Call4((void (*)())Func_02004bec, 5, -16, 0, 0);
  Func_02004b5c(4, 0x8000);
  Battle_WaitMode0(20);
  Func_02004b3c(MsgIDonTKnowWhatWe);
  Call3((void (*)())Func_02004b4c, 5, 0, 5);
  Object_SetModeById(5, 2);
  leader = Object_GetById(gPartyState.currentOwner);
  if (leader != NULL)
    ObjectMotion_ResetAndSetPosition(5, ((s16 *)leader)[5], ((s16 *)leader)[9]);
  ObjectMotion_CommitCurrentPositionAndActivate(5);
  Call3((void (*)())Func_02004b04, 5, 0, 0);
  Func_02004a9c();
  return;
check:
  if (GameFlag_Test(0x8ab) && GameFlag_Test(0x8a7) == 0 && GameFlag_Test(0x8be))
    goto run;
}

const struct SceneEvent *Scene_GetEvents(void) { return gSceneEvents; }
