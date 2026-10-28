#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

extern const u8 gData_02003f92[];
extern const u8 gData_02003fb8[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Func_02000038(const void *data);
void Func_020001d0(const void *data);
void Func_02001ff4(void);
void Func_02002140(void);
void Func_02003764(void);

void Func_02003aa0(void) {
  s32 *route = (s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac);

  /* FAKEMATCH(barrier): an empty asm between the route's address and its
   * store, so gMainRoutineWork's address loads after the address is added; as
   * one assignment, 2 of 57 instructions differ in tla-en. */
  __asm__("");
  *route = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (GameFlag_Test(95) != 0)
    GameFlag_SetBit(521);
  Func_02000038(gData_02003fb8);
  Func_020001d0(gData_02003f92);
  if (GameFlag_Test(519) == 0) {
    Func_02002140();
    wait(10);
  }
  Func_02001ff4();
  if (GameFlag_Test(265) == 0) {
    if (gMainRoutineWork.entrance > 5) {
      if (gMainRoutineWork.entrance <= 39)
        Func_02003764();
    }
  }
}
