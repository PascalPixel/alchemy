// Draft, differ 6 of 142 (2026-10-18): applies data carried over from The
// Broken Seal: each of the 28 djinn flags from 0x800 (seven per element) sets
// the matching flag from 0x30 (twenty per element), item 222 is removed,
// owners 0 to 3 join, the djinn are set and the eight units recomputed, flag
// 34 set; then for each element a random djinni still unowned (plus 1, or 0
// for none) is stored at gData_02001000 + 0x1950, five Func_080ae6c8 calls
// follow, and the coins at 0x28c of the party state are added to the coins.
// The native recomputes element * 20 from the counter each pass and steps
// the djinn flag base by 7; written as i * 7 and i * 20 agscc
// strength-reduces both (differ 18), so the base is a counter k and an empty
// asm on i keeps i * 20 unreduced. The first inner counter is unsigned:
// signed, agscc reverses that loop (differ 12). Left: that loop closes with
// bls for the native ble, and the gData_02001000 pool load issues one place
// later. Tried: a copy of i behind the asm instead (the extra copy shifts
// every register: differ 73), the asm on i with i * 7 (differ 105), k
// stepped in the inner loop (differ 37).
#include "types.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void PartyInventory_Remove(s32 item);
void Party_AddActiveOwner(s32 owner);
void Func_080ae358(s32 mode);
void Djinn_ActivateOfferedForParty(void);
void UnitCalc(s32 unitId);
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
    u32 j;

    for (j = 0; j < 7; j++) {
      if (GameFlag_Test(0x800 + k + j))
        GameFlag_SetBit(0x30 + i * 20 + j);
    }
    /* FAKEMATCH: keeps i * 20 from being strength-reduced (differ 18 as
     * i * 7 and i * 20 without it) */
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
    UnitCalc(i);
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
  gPartyState.coins += *(s32 *)&gPartyState.unk284[8];
}
