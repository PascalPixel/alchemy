// Draft, differ 16 of 206 (2026-10-26, 20 before): counts[3] cleared after the
// count of 16 is set. Earlier notes follow.
// Under -mthumb-slow-loads (2026-10-26): differ 20 of 206 in tla-en,
// instruction order and register choice. As in Func_080ae6e4, the flag loop's
// counter is unsigned (bls for the native's ble). Empty asms before counts[2] =
// 0 and elsewhere lower it to 14 at best. 8 minutes of permute (2026-10-26)
// found nothing.
// Near miss, differ 22 of 206, 2026-10-18, written from the listing: unless
// flag 0x800 is set, sets the party aside, makes the first four owners the
// party, gives them their first djinn, then hands out 16 random djinn still
// unowned, keeping the elements' counts within one, each with the table's
// percentage chance; then moves the djinn flags to 0x800 (seven per element)
// and puts the party back. As in Func_080ae6e4, the native recomputes
// element * 20 each pass and steps the 0x800 base by 7: a counter k, an empty
// asm on i and an unsigned inner counter give that (element * 7 and
// element * 20 with a signed counter: differ 81). The flag loop runs
// on i rather than element: on element, element and the counts' address
// trade r5 and r6 (differ 72). Left: the initial counts are stored ones
// first in the native; two pool loads and a negs issue a place earlier; the
// 0x800 base is formed a place later; the flag loop closes with bls for the
// native ble.
#include "types.h"
#include "adventure.h"

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
    saved[i] = gAdventure.activeOwners[i];
  for (i = 0; i < count; i++)
    Party_RemoveActiveOwner(saved[i]);
  for (i = 0; i < 4; i++)
    Party_AddActiveOwner(i);
  Djinn_AddFoundToOwner(0, 0, 0);
  Djinn_AddFoundToOwner(3, 1, 0);

  counts[0] = 1;
  counts[1] = 1;
  counts[2] = 0;
  remaining = 16;
  counts[3] = 0;
  do {
    element = (Random16() * 4) >> 16;
    djinn = (Random16() * 7) >> 16;
    if (!GameFlag_Test(element * 20 + djinn + 48)) {
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
    u32 j;

    for (j = 0; j < 7; j++) {
      if (GameFlag_Test(0x30 + i * 20 + j))
        GameFlag_SetBit(0x800 + k + j);
      GameFlag_ClearBit(0x30 + i * 20 + j);
    }
    /* FAKEMATCH: keeps i * 20 from being strength-reduced */
    asm("" : "+r"(i));
  }
  for (i = 0; i < 4; i++)
    Party_RemoveActiveOwner(i);
  for (i = 0; i < count; i++)
    Party_AddActiveOwner(saved[i]);
}
