#include "trial_road.h"

// While the party's render mode is 0, clears the map cell flag under the
// object.
void Func_02000728(s32 a, s32 id) {
  struct PartyState *party = &gPartyState;
  u8 mode;

  /* FAKEMATCH: the loop note makes the offset's first instruction a barrier: ⚓️
   * loads the party state's address before it builds the render mode's offset
   * (plain, the offset starts first: differ 2). */
  do {
  } while (0);
  mode = party->renderMode;
  {
    /* FAKEMATCH: the id copy pinned to r0 after the mode read stays in the
     * load's delay slot; as a plain local it joins the parameter and rises
     * above the barrier (differ 2). */
    register s32 target asm("r0") = id;

    if (mode == 0)
      Func_02000660(target);
  }
}

// While the party's render mode is 0, sets the map cell flag under the object.
void Func_02000748(s32 a, s32 id) {
  struct PartyState *party = &gPartyState;
  u8 mode;

  /* FAKEMATCH: the loop note makes the offset's first instruction a barrier: ⚓️
   * loads the party state's address before it builds the render mode's offset
   * (plain, the offset starts first: differ 2). */
  do {
  } while (0);
  mode = party->renderMode;
  {
    /* FAKEMATCH: the id copy pinned to r0 after the mode read stays in the
     * load's delay slot; as a plain local it joins the parameter and rises
     * above the barrier (differ 2). */
    register s32 target asm("r0") = id;

    if (mode == 0)
      Func_0200062c(target);
  }
}
