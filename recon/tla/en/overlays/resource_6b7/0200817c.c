// Near miss, differ 22 of 19: the native indexes the list from sp on every
// pass (lsls r2, r5, #1; mov r3, sp; ldrh r0, [r3, r2]) and counts i up to
// the count; this draft walks a pointer and counts down. A u16 or s16 list
// compiles the same.
#include "types.h"
#include "main_routine_work.h"

void Func_02000160(s32 owner, s32 delta);

void Func_0200017c(s32 delta) {
  u16 owners[16];
  s32 count = Party_ListActiveOwners((s16 *)owners);
  s32 i;

  for (i = 0; i < count; i++)
    Func_02000160(owners[i], delta);
}
