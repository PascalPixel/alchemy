// Near miss, differ 2 of 142 (tla-en, 2026-10-30): the first inner loop
// closes with bls for the native's ble. The native's flag loop counts up with
// a signed counter (ble) that loop.c does not reverse; here a signed counter
// is reversed (subs, bge: differ 8) and an unsigned one closes with bls. The
// loop dumps say why: loop.c's second pass reverses a counter that has no
// givs left (check_dbra_loop's no_use_except_counting), and with k and i * 20
// invariant in the first pass both flag numbers are reduced there. Written
// plainly, i * 7 and i * 20 with no k and no asm (differ 20), the inner loop
// matches exactly: i * 7 stays in the inner loop in pass one ("not
// desirable"), so its giv is only reduced in pass two and the counter is
// kept; but the outer loop then strength-reduces i * 20 to a register
// stepped by 20, which the native recomputes from i every pass. Tried
// 2026-10-30, none closer: do-while inner or outer loop, continue, a flag and
// bit pair stepped with j (22), an inline helper per element (20), an outer
// loop built from labels with k (8, reversed) or i * 7 (81), k from 0x800
// (28), k stepped at the outer body's end (28), the dead update pair on i
// (58). A counter that stays live after the loop (djinni) is reversed too; no
// single empty asm lowers it (2026-10-26).
// Draft, differ 6 of 142 (2026-10-18): applies data carried over from The
// Broken Seal: each of the 28 djinn flags from 0x800 (seven per element) sets
// the matching flag from 0x30 (twenty per element), item 222 is removed,
// owners 0 to 3 join, the djinn are set and the eight units recomputed, flag
// 34 set; then for each element a random djinni still unowned (plus 1, or 0
// for none) is stored at gData_02001000 + 0x1950, five Func_080ae6c8 calls
// follow, and the coins at 0x28c of gMainRoutineWork are added to the coins.
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
#include "main_routine_work.h"

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
    u32 j;

    for (j = 0; j < 7; j++) {
      if (GameFlag_Test(0x800 + k + j))
        GameFlag_SetBit(0x30 + i * 20 + j);
    }
    /* FAKEMATCH(barrier): an empty asm on i keeps i * 20 from being
     * strength-reduced; as the same loops without it, 22 of 142
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
}
