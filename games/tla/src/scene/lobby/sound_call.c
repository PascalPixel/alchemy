#include "types.h"
#include "io_reg.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_0200191c(void);
void Func_02001914(void);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001abc(s32 value);

void Func_02000f50(s32 value) {
  Func_02001a14();
  Event_ResolvePendingActions(0);
  Func_02001abc(value);
  Func_02001a1c();
}

/// Whether owner is one of the first three active owners, or the first four
/// while flag 370 is clear; an 0xff entry ends the list.
s32 Func_02000f6c(s32 owner) {
  s32 count = Party_CountActiveOwners();
  s32 limit = 3;
  s32 i;
  struct MainRoutineWork *party;

  if (!GameFlag_Test(370))
    limit = 4;
  if (count > limit)
    count = limit;
  i = 0;
  if (i >= count)
    goto none;
next:
  party = &gMainRoutineWork;
  if (party->activeOwners[i] == 0xff)
    goto none;
  /* FAKEMATCH(barrier): the empty asm with a memory clobber, so the owner byte
   * is read again for the second test; as no asm, 8 instructions differ. */
  __asm__ volatile("" ::: "memory");
  if (gMainRoutineWork.activeOwners[i] == owner)
    return 1;
  i++;
  if (i < count)
    goto next;
none:
  return 0;
}

void Func_02000fb8(void) {}
