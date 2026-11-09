#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "event_inline.h"

extern char MsgIVeHeard[];
extern char SceneId_WorldMap;
extern char SceneId_Ov6a5;
extern u8 gData_0200632c[];

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000d4c(void);
void Func_020012bc(s32 entrance);
void Func_02001c5c(void);
void Func_02001e34(void);
void Func_02002698(void);
void Func_020029a4(void);
void Func_02002ab0(void);
void Func_02002e00(void);
void Func_02002e78(void);
void Func_0200310c(void);
void Func_02003490(void);
void Func_02003960(void);
void Func_02003d44(s32 mode);
void Func_02004c00(void);
void Func_02004d38(s32 scene, s32 entrance);

s32 Func_02000dc4(void) {
  struct EventWork *work;

  if (GameFlag_Test(0x109) == 0 && GameFlag_Test(0x8de) != 0 &&
      gMainRoutineWork.ownerOverride == 0 && gMainRoutineWork.savedX != 0) {
    struct Actor *ship = Actor_GetById(8);

    ship->x = gMainRoutineWork.savedX;
    ship->z = gMainRoutineWork.savedZ;
    ship->angle = gMainRoutineWork.unk280;
    ship->action = 0;
    ship->unk66 = 0;
  }
  if (gMainRoutineWork.ownerOverride != 0)
    Func_080d46a4(0x10000, 1);
  GameFlag_SetBit(0x144);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x400;
  work->delay = 16;
  wait(1);
  FieldCamera_SetSpeed(0x80000, 0x10000);
  GameFlag_ClearBit(0x12f);
  Func_02003960();
  /* FAKEMATCH(wrapper): the three Map_SetWindowCellTile calls forwarded through
   * Call4, so each builds its own constants (the 256 is not shared); as direct
   * calls, 12 of 270 instructions differ in tla-en. */
  if (GameFlag_Test(0x90f))
    Field_SetWorldMapWindowCellTile(0, 240, 336, 288);
  if (GameFlag_Test(0xa21)) {
    Field_SetWorldMapWindowCellTile(0, 224, 208, 256);
    Field_SetWorldMapWindowCellTile(16, 224, 224, 256);
  }
  if (GameFlag_Test(0xa77))
    Func_02000d4c();
  switch (gMainRoutineWork.entrance) {
  case 39:
    if (GameFlag_Test(0x927))
      GameFlag_SetBit(0x9ab);
    break;
  case 9:
  case 73:
  case 74:
    if (GameFlag_Test(0xa76))
      Func_020012bc(gMainRoutineWork.entrance);
    break;
  case 77:
    Func_020029a4();
    break;
  case 78:
    Func_02003490();
    break;
  case 59:
    /* FAKEMATCH(wrapper): the flag test here and case 98's four sets forwarded
     * through flag_call.h's Flag_IsSet and Flag_Set; as direct GameFlag_Test
     * and GameFlag_SetBit calls, 75 of 270 instructions differ in tla-en. */
    if (!Event_GetFlag(0x90a) && GameFlag_Test(0xa5f) != 0) {
      GameFlag_SetBit(0x90a);
      Func_02001c5c();
    }
    break;
  case 64:
    Func_02002698();
    break;
  case 65:
    Func_02002ab0();
    break;
  case 70:
    Func_02004c00();
    break;
  case 2:
    if (GameFlag_Test(55) == 0) {
      Actor_SetScript(9, gData_0200632c);
      Func_02001e34();
    }
    break;
  case 67:
    Func_02002e00();
    break;
  case 68:
    Func_02002e78();
    break;
  case 69:
    Func_0200310c();
    break;
  case 72:
    Func_02003d44(2);
    Actor_SetSpriteColor(Actor_GetById(7), 15);
    Actor_SetPosition(7, 0x1c6d0000, 0x1c840000);
    wait(1);
    FieldEvent_SetMessageId((s32)MsgIVeHeard);
    Event_ShowMessage(7, 0);
    FieldEvent_Wait(20);
    Event_ShowMessage(7, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004d38((s32)&SceneId_Ov6a5, 9);
    break;
  case 98:
    gMainRoutineWork.ownerOverride = 1;
    Event_SetFlag(0x8de);
    Event_SetFlag(0x8df);
    Event_SetFlag(0x163);
    Event_SetFlag(0x11d);
    Func_02004d38((s32)&SceneId_WorldMap, 61);
    break;
  case 99:
    GameFlag_SetBit(0x163);
    GameFlag_SetBit(0x11d);
    break;
  }
  return 0;
}
