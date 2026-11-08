// Matches: differ 0 (tla-en, 2026-11-08)
// Differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it too. The world map's
// entrance handler: restores the ship's place, then runs the script for the
// entrance taken. diff compares instructions only: the pool words and the
// 98-entry jump table are left to make compare-editions. Case 98's flags go
// through the scene layer's Flag_Set (flag_call.h), a tagged wrapper: called
// directly, each flag id is a pseudo of the case's block, local-alloc's block
// then holds seven quantities and qsorts them, so the constant 1 (shorter life)
// takes r3 before the store's address and the native's r2/r3 turn round (12 of
// 270). Through the inline the ids go straight to r0, the block keeps three
// (address, 1, the scene's pool word), and local-alloc's three-quantity hand
// sort, which compares quantity numbers, not the order array, places the
// address first, as in lemuria_entrance/s0190.c. Case 59's test goes through
// Flag_IsSet (63 of 270 as a direct call); its set is a direct call. Two tagged
// devices: the flag forwarding and the Call4 wrapper round Func_02004b60; tried
// before this round: a pointer to gMainRoutineWork or to the field, *&, a
// constant local for the 1 (case 98 right, reload scratch r0 elsewhere, 36),
// once-only blocks, permuter 18 minutes in all.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "call.h"
#include "flag_call.h"

extern char MsgIVeHeard[];
extern char SceneId_WorldMap;
extern char SceneId_Ov6a5;
extern u8 gData_0200632c[];

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
      gMainRoutineWork.ownerOverride == 0 && gMainRoutineWork.savedX != 0) {
    struct Actor *ship = Actor_GetById(8);

    ship->x = gMainRoutineWork.savedX;
    ship->z = gMainRoutineWork.savedZ;
    ship->angle = gMainRoutineWork.unk280;
    ship->action = 0;
    ship->unk66 = 0;
  }
  if (gMainRoutineWork.ownerOverride != 0)
    Func_02004df8(0x10000, 1);
  GameFlag_SetBit(0x144);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x400;
  work->delay = 16;
  wait(1);
  Func_02004d20(0x80000, 0x10000);
  GameFlag_ClearBit(0x12f);
  Func_02003960();
  /* FAKEMATCH(wrapper): the three Func_02004b60 calls forwarded through Call4,
   * so each builds its own constants (the 256 is not shared); as direct calls,
   * 12 of 270 instructions differ in tla-en. */
  if (GameFlag_Test(0x90f))
    Call4((void (*)())Func_02004b60, 0, 240, 336, 288);
  if (GameFlag_Test(0xa21)) {
    Call4((void (*)())Func_02004b60, 0, 224, 208, 256);
    Call4((void (*)())Func_02004b60, 16, 224, 224, 256);
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
    if (!Flag_IsSet(0x90a) && GameFlag_Test(0xa5f) != 0) {
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
    gMainRoutineWork.ownerOverride = 1;
    Flag_Set(0x8de);
    Flag_Set(0x8df);
    Flag_Set(0x163);
    Flag_Set(0x11d);
    Func_02004d38((s32)&SceneId_WorldMap, 61);
    break;
  case 99:
    GameFlag_SetBit(0x163);
    GameFlag_SetBit(0x11d);
    break;
  }
  return 0;
}
