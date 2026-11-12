/// @file
/// Scene code of overlay 0x671, in ROM order, from setup's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "input.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "field.h"

void Func_020022d0(s32 id, s32 x, s32 z, s32 facing);
void Func_020011bc(s32 message, s32 value);
void Func_020056b4(void);
void Func_0200533c(void);
void Func_020057b4(void);
s32 Func_02004d48(s32 a, s32 b);
void Func_0200581c(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02000f50(s32 id) {
  Actor_SetPosition(id, 0, 0);
  FieldActor_Stop(id);
  Actor_SetAnimation(id, 1);
}

void Func_02000f6c(void) {
  Func_02000f50(11);
  Func_02000f50(12);
  Func_02000f50(13);
  Func_02000f50(14);
  Func_020022d0(11, 11, 9, 0);
  Func_020022d0(12, 11, 9, 0x8000);
  Func_020022d0(13, 10, 10, 0x4000);
  Func_020022d0(14, 10, 10, 0xc000);
}

void Func_02000fc0(void) {
  s32 result;

  Event_SetFlag(0x205);
  Func_020056b4();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(78);
  Func_0200533c();
  FieldEvent_Wait(40);
  FieldEvent_SetMessageId(MsgGabombaAcceptOffering);
  Event_ShowMessage(24, 0);
  FieldCamera_MoveTo(PIXELS(392), -1, PIXELS(256), 1);
  Func_020057b4();
  Event_PlaySound(220);
  System_Wait(1);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteColor(Actor_Get(gMainRoutineWork.currentOwner), 15);
  Func_02000f6c();
  System_Wait(1);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Event_PlaySound(130);
  Actor_SetSpriteColor(Actor_Get(9), 7);
  Actor_SetSpriteColor(Actor_Get(10), 7);
  System_Wait(8);
  Actor_SetSpriteColor(Actor_Get(10), 3);
  Actor_SetSpriteColor(Actor_Get(9), 0);
  System_Wait(8);
  Event_PlaySound(11);
  result = Func_02004d48(11, 1);
  Func_0200581c();
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  switch (result) {
  case 3:
    FieldEvent_SetExitRequest(14);
    break;
  case 1:
    FieldEvent_SetExitRequest(15);
    break;
  case 2:
    FieldEvent_SetExitRequest(16);
    break;
  case 0:
    FieldEvent_SetExitRequest(17);
    break;
  }
}

#include "scene_ids.h"

extern const u8 gData_02006e9c[];
extern const u8 gData_02006b54[];
extern const u8 gData_02006b6c[];
extern const u8 gData_02006bd8[];
extern const u8 gData_02006c80[];
extern const u8 gData_02006d1c[];
extern const u8 gData_02006db8[];
extern const u8 gData_02006e78[];

const u8 *Func_02001124(void) {
  s16 scene = gMainRoutineWork.scene;
  const u8 *table = gData_02006e9c;

  if (scene == (s32)&SceneId_Ov671Room1)
    table = gData_02006b54;
  else if (scene == (s32)&SceneId_Ov671Room2)
    table = gData_02006b6c;
  else if (scene == (s32)&SceneId_Ov671Room3)
    table = gData_02006bd8;
  else if (scene == (s32)&SceneId_Ov671Room4)
    table = gData_02006c80;
  else if (scene == (s32)&SceneId_Ov671Room5)
    table = gData_02006d1c;
  else if (scene == (s32)&SceneId_Ov671Room6)
    table = gData_02006db8;
  else if (scene == (s32)&SceneId_Ov671Room7)
    table = gData_02006e78;
  return table;
}

