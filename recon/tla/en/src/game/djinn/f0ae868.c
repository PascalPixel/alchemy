/// @file
/// Unless flag 0x800 is set, sets the party aside, makes the first four owners
/// the party, gives them their first djinn, then hands out 16 random djinn
/// still unowned, keeping the elements' counts within one, each with the
/// table's percentage chance; then moves the djinn flags to 0x800 (seven per
/// element) and puts the party back.
#include "types.h"
#include "main_routine_work.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 Party_CountActiveOwners(void);
s32 Party_AddActiveOwner(s32 owner);
s32 Party_RemoveActiveOwner(s32 owner);
void Djinn_AddFoundToOwner(s32 owner, s32 element, s32 djinn);
s32 Djinn_AddToLeastLoadedOwner(s32 element, s32 djinn);
void Djinn_ActivateOfferedForParty(void);
u32 Random16(void);

extern const s16 Data_080b1290[4][7];

void Func_080ae868(void) {
  s8 saved[8];
  s8 counts[4];
  s32 count;
  s32 remaining;
  s32 element;
  s32 djinn;
  s32 i;
  s32 k;

  if (GameFlag_Test(0x800))
    return;

  count = Party_CountActiveOwners();
  for (i = 0; i < count; i++)
    saved[i] = gMainRoutineWork.activeOwners[i];
  for (i = 0; i < count; i++)
    Party_RemoveActiveOwner(saved[i]);
  for (i = 0; i < 4; i++)
    Party_AddActiveOwner(i);
  Djinn_AddFoundToOwner(0, 0, 0);
  Djinn_AddFoundToOwner(3, 1, 0);

  counts[0] = 1;
  counts[1] = 1;
  counts[2] = 0;
  counts[3] = 0;
  remaining = 16;
  do {
    element = (Random16() * 4) >> 16;
    djinn = (Random16() * 7) >> 16;
    if (!GameFlag_Test(element * 20 + djinn + 0x30)) {
      s8 low;
      s8 high;

      counts[element]++;
      low = 9;
      high = 0;
      for (i = 0; i < 4; i++) {
        if (low > counts[i])
          low = counts[i];
        if (high < counts[i])
          high = counts[i];
      }
      counts[element]--;
      if (high - low <= 1 &&
          ((Random16() * 100) >> 16) < Data_080b1290[element][djinn]) {
        Djinn_AddToLeastLoadedOwner(element, djinn);
        counts[element]++;
        remaining--;
      }
    }
  } while (remaining != 0);
  Djinn_ActivateOfferedForParty();

  for (i = 0, k = 0; i < 4; i++, k += 7) {
    s32 j;
    s32 flag;

    for (j = 0; j < 7; j++) {
      /* FAKEMATCH(temporary): the form kept; as GameFlag_SetBit(0x800 + k + j),
         8 of 206 instructions differ in tla-en. */
      flag = 0x800 + k + j;
      /* FAKEMATCH(temporary): element reused for the flag; with its own local
         or inline, 13 of 206 instructions differ in tla-en. */
      element = 0x30 + i * 20 + j;
      if (GameFlag_Test(element))
        GameFlag_SetBit(flag);
      GameFlag_ClearBit(element);
    }
  }
  for (i = 0; i < 4; i++)
    Party_RemoveActiveOwner(i);
  for (i = 0; i < count; i++)
    Party_AddActiveOwner(saved[i]);
}
