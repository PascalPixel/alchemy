// Near miss, differ 2 of 152 (the English listing; the Japanese one does not set
// flag 0x200 by the leader's position or place actors 31 and 32): Alhafra's
// interiors' setup: marks actor 16, gives actors 9, 14, 15 and 31 action
// variant 3, starts the two drifting lists (Func_02000bb4) and actors 14 and
// 15's updates; with flag 0x8a7 it sets flag 0x200 when the leader is right
// of x 512 and places actors 31 and 32 by flags 0x200, 0x823 and 0x903; with
// flag 0x8ab and 0x971 it clears actor 31; otherwise it sets four actors'
// action variants. Returns 0. Left: the native loads the leader's x before building 0x2000000 (one reordered row). Tried: the call layer for that lookup, 1 minute of alchemy permute.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "call.h"

extern u8 gData_02001c90[], gData_02001c60[];
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02000bac(s32 mode);
void Func_02000bb4();
void Func_02000578(void);
void Func_02000598(void);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02000aec();

s32 Func_020005b8(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Object_GetById(16)->collisionFlags |= 4;
  Object_GetById(16)->collisionFlags |= 16;
  ObjectMotion_SetActionVariant(9, 3);
  ObjectMotion_SetActionVariant(31, 3);
  ObjectMotion_SetActionVariant(14, 3);
  ObjectMotion_SetActionVariant(15, 3);
  Func_02000bac(0);
  Call4(Func_02000bb4, (s32)gData_02001c90, 8, 15, 0x40000000);
  Call4(Func_02000bb4, (s32)gData_02001c60, 8, 15, 0x40000000);
  *(s32 *)(gData_02001c60 + 24) = 0x18000;
  Object_GetById(14)->update = Func_02000578;
  Object_GetById(15)->update = Func_02000598;
  if (GameFlag_Test(0x8a7)) {
    if (Object_GetById(gPartyState.currentOwner)->x >= 0x2000000)
      GameFlag_SetBit(0x200);
    else
      GameFlag_ClearBit(0x200);
    if (GameFlag_Test(0x200) && GameFlag_Test(0x823) &&
        !GameFlag_Test(0x903)) {
      Call3(Func_02000aec, 31, 0x1780000, 0x13c0000);
      Call3(Func_02000aec, 32, 0x1880000, 0x1580000);
    } else {
      Call3(Func_02000aec, 31, 0, 0);
      Call3(Func_02000aec, 32, 0, 0);
    }
  } else if (GameFlag_Test(0x8ab)) {
    if (GameFlag_Test(0x971))
      Call3(Func_02000aec, 31, 0, 0);
  } else {
    ObjectMotion_SetActionVariant(24, 2);
    ObjectMotion_SetActionVariant(19, 3);
    ObjectMotion_SetActionVariant(28, 2);
    ObjectMotion_SetActionVariant(25, 3);
  }
  return 0;
}
