// Draft, differ 17 of 29: raises each active owner by levels. A goto loop keeps
// loop.c from strength-reducing the index (the for loop: 22), so the native's
// lsls, mov sp, ldrh comes out; but GCSE's PRE then reuses the array address
// computed for Party_ListActiveOwners's argument (insn 31 becomes a copy of
// 34), keeping sp in r8 across the calls and moving levels to r10. Tried: a u16
// list, owners[i++] in a while loop, i stepped after the load, one minute
// (earlier) and three minutes of permute (nothing valid). Tried 2026-10-15:
// levels pinned to r8 (13: the array address then takes sl), the argument
// address or the loop base laundered through an empty asm (17, 19), a for loop
// with s32, u32, s16, u16 or u8 index over s16 or u16 owners (22 to 43), four
// more minutes of permute (17).
#include "types.h"
#include "owner_state.h"
#include "adventure.h"

void Unit_Calculate(s32 owner);

void Func_02000704(s32 levels) {
  s16 owners[16];
  s32 count = Party_ListActiveOwners(owners);
  s32 i = 0;

  if (i < count) {
  loop:
    {
      u16 owner = owners[i++];

      Unit_LevelUpTo(owner,
                     ((struct BattleUnit *)Unit_Get(owner))->level + levels);
      Unit_Calculate(owner);
    }
    if (i < count)
      goto loop;
  }
}
