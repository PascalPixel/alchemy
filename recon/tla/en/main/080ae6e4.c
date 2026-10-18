// Draft, differ 18 of 142: applies data carried over from The Broken Seal:
// each of the 28 djinn flags from 0x800 (seven per element) sets the matching
// flag from 0x30 (twenty per element), item 222 is removed, owners 0 to 3
// join, the djinn are set and the eight units recomputed, flag 34 set; then
// for each element a random djinni still unowned (plus 1, or 0 for none) is
// stored at gData_02001000 + 0x1950, five Func_080ae6c8 calls follow, and
// the coins at 0x28c of the party state are added to the coins. Left: in the
// first loop the native recomputes element * 20 from the counter each pass
// while the draft strength-reduces it into a second induction register (sl);
// the rest is the register (r1 against r3) of the frame address. Tried: the
// set flag written as i * 20 + djinni + 0x30 (same).
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

  for (i = 0; i < 4; i++) {
    for (djinni = 0; djinni < 7; djinni++) {
      if (GameFlag_Test(0x800 + i * 7 + djinni))
        GameFlag_SetBit(0x30 + i * 20 + djinni);
    }
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
