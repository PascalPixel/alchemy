#include "types.h"
#include "object_runtime.h"
#include "owner_state.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
s32 Unit_AddPp();
void Func_02003c3c(void);
void Func_02004a1c(void);

// Sets render mode 7, clears object 8's child flags, then adds up the PP of
// the active party; when it is 0, the current owner gets 1 PP back. The loop
// is the listing's goto loop, which the loop pass leaves alone (gPartyState
// reloaded each pass); its first test reads pp, which still holds the
// index's 0.
void Func_0200050c(void) {
  s32 count;
  /* FAKEMATCH(pin): the PP total in r6; as a plain local, 19 of 42
   * instructions differ in tla-en. */
  register s32 pp asm("r6");
  /* FAKEMATCH(pin): the index in r5; as a plain local, 19 of 42
   * instructions differ in tla-en. */
  register s32 i asm("r5");

  {
    /* FAKEMATCH(temporary): gPartyState's address loaded before the field's
     * offset, through an empty asm on it (barrier); as
     * gPartyState.renderMode = 7, 2 of 42 instructions differ in tla-en. */
    struct PartyState *state = &gPartyState;

    __asm__ __volatile__("" : "+r"(state));
    state->renderMode = 7;
  }
  Func_02003c3c();
  Func_02004a1c();
  ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
  count = Party_CountActiveOwners();
  pp = 0;
  i = 0;
  if (pp < count) {
  loop:
    // One member's PP.
    {
      /* FAKEMATCH(pin): gPartyState's address in r2, loaded before the
       * table's offset through an empty asm on it (barrier); unpinned, 6 of
       * 42 instructions differ in tla-en, and without the asm, 2. */
      register struct PartyState *state asm("r2") = &gPartyState;

      __asm__ __volatile__("" : "+r"(state));
      pp += ((struct BattleUnit *)UnitGet(state->activeOwners[i]))->pp;
    }
    i++;
    if (i < count)
      goto loop;
  }
  if (pp == 0)
    Unit_AddPp(gPartyState.currentOwner, 1);
}
