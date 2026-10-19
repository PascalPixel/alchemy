#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "event_work.h"

extern struct ObjectRuntime *gData_02006c08[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004e30(s32 value);

// The event countdown takes before when the party leader's x is less than
// that of the object in slot - 100 of the overlay's table, else after.
void Func_02003600(s32 slot, s32 before, s32 after) {
  /* FAKEMATCH(pin): after copied to r6 on entry; as plain after, 18 of 33
   * instructions differ in tla-en. */
  register s32 elseValue asm("r6") = after;
  struct ObjectRuntime *target = gData_02006c08[slot - 100];
  struct ObjectRuntime *owner;
  struct EventWork *work;

  /* FAKEMATCH(barrier): ⚓️ loads gPartyState's address before it builds the
   * currentOwner offset; as no barrier, 2 of 33 instructions differ in
   * tla-en. */
  asm volatile("");
  {
    /* FAKEMATCH(pin): before copied to r7 after the barrier, beside the
     * call; as plain before, 4 of 33 instructions differ in tla-en. */
    register s32 thenValue asm("r7") = before;

    owner = Object_GetById(gPartyState.currentOwner);
    work = Ram_HeapSlots->eventWork;
    if (owner->x < target->x)
      work->countdown = thenValue;
    else
      work->countdown = elseValue;
  }
  Func_02004e30(123);
}

// The same on z.
void Func_02003654(s32 slot, s32 before, s32 after) {
  /* FAKEMATCH(pin): after copied to r6 on entry; as plain after, 18 of 33
   * instructions differ in tla-en. */
  register s32 elseValue asm("r6") = after;
  struct ObjectRuntime *target = gData_02006c08[slot - 100];
  struct ObjectRuntime *owner;
  struct EventWork *work;

  /* FAKEMATCH(barrier): ⚓️ loads gPartyState's address before it builds the
   * currentOwner offset; as no barrier, 2 of 33 instructions differ in
   * tla-en. */
  asm volatile("");
  {
    /* FAKEMATCH(pin): before copied to r7 after the barrier, beside the
     * call; as plain before, 4 of 33 instructions differ in tla-en. */
    register s32 thenValue asm("r7") = before;

    owner = Object_GetById(gPartyState.currentOwner);
    work = Ram_HeapSlots->eventWork;
    if (owner->z < target->z)
      work->countdown = thenValue;
    else
      work->countdown = elseValue;
  }
  Func_02004e30(123);
}
