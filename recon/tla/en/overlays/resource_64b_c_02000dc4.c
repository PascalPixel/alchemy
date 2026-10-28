// Draft, differ 104 of 270: the world map's entrance handler: restores the
// ship's place, then runs the script for the entrance taken. Left: register
// choice in the ship block (the native keeps gAdventure in r5, the override
// in r6, savedX's address in r7) and the pool reload it does at the join.
#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "actor.h"
#include "event_work.h"
#include "call.h"

extern char MsgIVeHeard[];
extern char SceneId_WorldMap;
extern char SceneId_Ov6a5;
extern u8 gData_0200632c[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetMovementTypeById(s32 id, void *callback);
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
void Func_02004b60(s32 a, s32 b, s32 c, s32 d);
void Func_02004c00(void);
void Func_02004c88(s32 id, s32 x, s32 z);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 id, s32 mode);
void Func_02004d20(s32 a, s32 b);
void Func_02004d38(s32 scene, s32 entrance);
void Func_02004df8(s32 a, s32 b);

s32 Func_02000dc4(void) {
  struct EventWork *work;

  if (GameFlag_Test(0x109) == 0 && GameFlag_Test(0x8de) != 0 &&
      gAdventure.ownerOverride == 0 && gAdventure.savedX != 0) {
    struct Actor *ship = Actor_GetById(8);

    ship->x = gAdventure.savedX;
    ship->z = gAdventure.savedZ;
    ship->angle = gAdventure.unk280;
    ship->action = 0;
    ship->unk66 = 0;
  }
  if (gAdventure.ownerOverride != 0)
    Func_02004df8(0x10000, 1);
  GameFlag_SetBit(0x144);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x400;
  work->delay = 16;
  wait(1);
  Func_02004d20(0x80000, 0x10000);
  GameFlag_ClearBit(0x12f);
  Func_02003960();
  if (GameFlag_Test(0x90f))
    Call4((void (*)())Func_02004b60, 0, 240, 336, 288);
  if (GameFlag_Test(0xa21)) {
    Call4((void (*)())Func_02004b60, 0, 224, 208, 256);
    Call4((void (*)())Func_02004b60, 16, 224, 224, 256);
  }
  if (GameFlag_Test(0xa77))
    Func_02000d4c();
  switch (gAdventure.entrance) {
  case 39:
    if (GameFlag_Test(0x927))
      GameFlag_SetBit(0x9ab);
    break;
  case 9:
  case 73:
  case 74:
    if (GameFlag_Test(0xa76))
      Func_020012bc(gAdventure.entrance);
    break;
  case 77:
    Func_020029a4();
    break;
  case 78:
    Func_02003490();
    break;
  case 59:
    if (GameFlag_Test(0x90a) == 0 && GameFlag_Test(0xa5f) != 0) {
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
      Actor_SetMovementTypeById(9, gData_0200632c);
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
    Actor_SetPartAttribute(Actor_GetById(7), 15);
    Func_02004c88(7, 0x1c6d0000, 0x1c840000);
    wait(1);
    Func_02004cd8((s32)MsgIVeHeard);
    Func_02004cf0(7, 0);
    FieldEvent_Wait(20);
    Func_02004cf0(7, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004d38((s32)&SceneId_Ov6a5, 9);
    break;
  case 98:
    gAdventure.ownerOverride = 1;
    GameFlag_SetBit(0x8de);
    GameFlag_SetBit(0x8df);
    GameFlag_SetBit(0x163);
    GameFlag_SetBit(0x11d);
    Func_02004d38((s32)&SceneId_WorldMap, 61);
    break;
  case 99:
    GameFlag_SetBit(0x163);
    GameFlag_SetBit(0x11d);
    break;
  }
  return 0;
}
