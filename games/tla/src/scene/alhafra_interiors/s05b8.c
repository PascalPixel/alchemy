#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "call.h"
#include "edition.h"

// The Japanese edition sets no flag 0x200 by the leader's position and places
// no actors 31 and 32; its form is still on the listing.
#if (LANGUAGE != LANGUAGE_JAPANESE)
extern u8 gData_02001c90[], gData_02001c60[];
struct Actor *Actor_GetById(u32 id);
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
  Actor_GetById(16)->collisionFlags |= 4;
  Actor_GetById(16)->collisionFlags |= 16;
  Actor_SetSpritePriority(9, 3);
  Actor_SetSpritePriority(31, 3);
  Actor_SetSpritePriority(14, 3);
  Actor_SetSpritePriority(15, 3);
  Func_02000bac(0);
  Call4(Func_02000bb4, (s32)gData_02001c90, 8, 15, 0x40000000);
  Call4(Func_02000bb4, (s32)gData_02001c60, 8, 15, 0x40000000);
  *(s32 *)(gData_02001c60 + 24) = 0x18000;
  Actor_GetById(14)->update = Func_02000578;
  Actor_GetById(15)->update = Func_02000598;
  if (GameFlag_Test(0x8a7)) {
    if (Actor_GetById(gAdventure.currentOwner)->x >= 0x2000000)
      GameFlag_SetBit(0x200);
    else
      GameFlag_ClearBit(0x200);
    if (GameFlag_Test(0x200) && GameFlag_Test(0x823) && !GameFlag_Test(0x903)) {
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
    Actor_SetSpritePriority(24, 2);
    Actor_SetSpritePriority(19, 3);
    Actor_SetSpritePriority(28, 2);
    Actor_SetSpritePriority(25, 3);
  }
  return 0;
}
#endif
