// Matches (tla-en, 2026-11-07).
// The list's count comes through an inline helper, which keeps the list's
// address out of a register so each pass rebuilds it from sp, as natively.
// Differ 0 in all six editions.
#include "types.h"
#include "main_routine_work.h"

void Func_02000160(s32 owner, s32 delta);

static __inline__ s32 ListActiveOwners(u16 *owners) {
  return Party_ListActiveOwners((s16 *)owners);
}

void Func_0200017c(s32 delta) {
  u16 owners[16];
  /* FAKEMATCH(wrapper): the count through an inline call layer; as
   * Party_ListActiveOwners called directly, 22 of 19 instructions differ in
   * tla-en. */
  s32 count = ListActiveOwners(owners);
  s32 i;

  for (i = 0; i < count; i++)
    Func_02000160(owners[i], delta);
}
