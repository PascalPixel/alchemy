#include "types.h"
#include "main_routine_work.h"
#include "version.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void PartyInventory_Remove(s32 item);
void Party_AddActiveOwner(s32 owner);
void Func_080ae358(s32 mode);
void Djinn_ActivateOfferedForParty(void);
void Unit_Calculate(s32 unitId);
void Func_080ae16c(s32 mode);
void Func_080ae6c8(s32 a, s32 b);
u32 Random16(void);

extern u8 gData_02001000[];

void Func_080ae6e4(void) {
  s32 i;
  s32 djinni;
  s8 missing[8];
  s32 count;
  s32 k;

  for (i = 0, k = 0; i < 4; i++, k += 7) {
    s32 j;

    for (j = 0; j < 7; j++) {
      if (GameFlag_Test(0x800 + k + j))
        GameFlag_SetBit(0x30 + i * 20 + j);
      /* FAKEMATCH(barrier): an empty asm reading j keeps loop.c from
       * reversing the signed counter; as the same loop without it, 8 of
       * 142 instructions differ in tla-en. */
      asm("" : : "r"(j));
    }
    /* FAKEMATCH(barrier): an empty asm on i keeps i * 20 from being
     * strength-reduced; as the same loops without it, 20 of 142
     * instructions differ in tla-en. */
    asm("" : "+r"(i));
  }
  PartyInventory_Remove(222);
  Party_AddActiveOwner(0);
  Party_AddActiveOwner(1);
  Party_AddActiveOwner(2);
  Party_AddActiveOwner(3);
  Func_080ae358(0);
  Djinn_ActivateOfferedForParty();
  for (i = 0; i < 8; i++)
    Unit_Calculate(i);
  Func_080ae16c(1);
  GameFlag_SetBit(34);

  for (i = 0; i < 4; i++) {
    count = 0;
    for (djinni = 0; djinni < 7; djinni++) {
      if (!GameFlag_Test(0x30 + i * 20 + djinni))
        missing[count++] = djinni;
    }
    if (count != 0)
      djinni = missing[(Random16() * count) >> 16] + 1;
    else
      djinni = 0;
    gData_02001000[0x1950 + i] = djinni;
  }
  Func_080ae6c8(202, 3);
  Func_080ae6c8(201, 3);
  Func_080ae6c8(203, 1);
  Func_080ae6c8(206, 0);
  Func_080ae6c8(207, 2);
  gMainRoutineWork.coins += *(s32 *)&gMainRoutineWork.unk284[8];
#if TLA_PATCH_4
  if ((u32)gMainRoutineWork.coins > 999999)
    gMainRoutineWork.coins = 999999;
#endif
}
