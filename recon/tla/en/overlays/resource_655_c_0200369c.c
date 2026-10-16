// Draft of Func_0200369c (overlay 655), differ 12 of 86: the room entry of
// Func_02003aa0 with its own cells. One spot is left: the native reads the
// entrance as an unsigned halfword (ldrh r5), tests 11 to 39 on it, and
// sign-extends it only after the call, adding the flag base into r5 before
// copying it to r0; an s16 local reads it sign-extended once (ldrsh).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001804(void);
void Func_02001fdc(void);
void Func_0200353c(void);
void Func_02003764(void);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);

void Func_0200369c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Object_GetById(gPartyState.currentOwner)->priorityFlags |= 32;
  Func_02001fdc();
  if (GameFlag_Test(0x86c) != 0) {
    Func_02003be0(3, 8, 30, 33, 1, 1);
    Func_02003be0(67, 3, 94, 28, 1, 5);
    Func_02003be0(3, 69, 30, 93, 1, 4);
  }
  {
    s16 entrance = gPartyState.entrance;

    if (entrance >= 11 && entrance <= 39) {
      Func_02001804();
      GameFlag_SetBit(entrance + 789);
    }
  }
  Func_0200353c();
  if (GameFlag_Test(265) == 0) {
    if (gPartyState.entrance > 10) {
      if (gPartyState.entrance <= 39)
        Func_02003764();
    }
  }
}
