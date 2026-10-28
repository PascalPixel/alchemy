// Under -mthumb-slow-loads (2026-10-26): differ 2 of 58 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Near miss, differ 4 of 58 (was 10 before the activeOwners stores went
// through one pointer local, 6 before the barrier below). The snapshot's
// clearing zero (movs r0, #0), which the native sets before the last halfword
// load, is an input of an empty asm that rewrites src, so that load waits for
// it (to be tagged FAKEMATCH(barrier) on adoption; plain, 6). Left: the movs
// of packedEvents' offset comes before the first halfword load (the native
// loads first), and the barrier's other input, activeOwners[2]'s address
// (adds r2, r1, r4), is formed before the last load where the native forms
// it after the pool load of the snapshot; without that input the zero comes
// before the third store (differ 6). Tried: the zero as u16, "r" operands
// (differ 6); writing the clear as gData_020023c4[0] = 0 differs 42; permute
// (4 min twice, before the barrier) found nothing better.
#include "types.h"
#include "owner_state.h"
#include "main_routine_work.h"

extern u16 gData_020023c4[];

void Unit_Calculate(s32 owner);
void GameFlag_ClearBit(s32 flag);

void Party_RestoreSnapshot(void) {
  u16 *src = gData_020023c4;
  s32 owner;
  u16 *owners;

  if (*src++ == 0x6774) {
    for (owner = 4; owner <= 7; owner++) {
      u16 *dst = (u16 *)((u8 *)Unit_Get(owner) + 216);
      s32 i;

      for (i = 14; i >= 0; i--)
        *dst++ = *src++;
      Unit_UpdateClass(owner);
      Unit_Calculate(owner);
    }
    gMainRoutineWork.packedEvents[0] = *src++;
    gMainRoutineWork.packedEvents[1] = *src++;
    owners = (u16 *)&gMainRoutineWork.activeOwners[0];
    *owners = src[0];
    owners = (u16 *)&gMainRoutineWork.activeOwners[2];
    {
      s32 zero = 0;
      __asm__("" : "+l"(src) : "l"(zero), "l"(owners));
      *owners = src[1];
      src = gData_020023c4;
      *src = zero;
    }
  }
  GameFlag_ClearBit(0x9ff);
}
