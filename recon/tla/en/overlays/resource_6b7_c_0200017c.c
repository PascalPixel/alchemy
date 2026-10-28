// Draft, differ 22 of 19: the native keeps the index in r5 and reloads the
// array base each pass (lsls, mov r3 sp, ldrh [r3, r2]) and tests i = 0
// against count at entry; agscc strength-reduces and reverses every for,
// while and guarded do/while tried, with u16 or s16 elements; a goto loop keeps
// the native's shape (differ 20) but holds sp and delta in r7 and r8. An s8,
// u8, s16, u16 or u32 index measures 29, 24, 33, 29 and 26. The index
// pinned to r5 gives the native's loop (the entry test of 0 against count,
// no strength reduction) at differ 20: GCSE replaces the loop's array base
// with the call argument's, kept in r7, where the native rebuilds it
// (mov r3, sp) each pass; pinning count and delta as well gives r7 to two
// values at once (13), a form that does not hold delta.
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/sequence_parent_work.h"

void Func_02000160(s32 owner, s32 delta);

void Func_0200017c(s32 delta) {
  u16 owners[16];
  s32 count;
  s32 i;

  count = Party_ListActiveOwners((s16 *)owners);
  for (i = 0; i < count; i++)
    Func_02000160(owners[i], delta);
}
