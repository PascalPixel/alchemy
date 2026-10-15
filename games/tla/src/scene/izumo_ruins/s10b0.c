#include "izumo_ruins.h"

u8 *Func_020010b0(void) {
  s16 scene = gPartyState.scene;

  if (scene == (s32)&SceneId_Ov67FRoom1)
    return gData_0200176c;
  if (scene == (s32)&SceneId_Ov67FRoom2)
    return gData_02001814;
  return gData_02001754;
}

// Starts the effect for this scene's room, then, for a first argument of 1,
// runs Func_02000f30 on the effect work's object.
void Func_020010f0(s32 aArg, s32 b) {
  struct IzumoRuinsEffectWork *work = Ram_HeapSlots->effectWork;
  struct PartyState *party = &gPartyState;
  s16 scene;

  /* FAKEMATCH: the loop note makes the scene offset's first instruction a
   * barrier: ⚓️ loads the party state's address before it copies b (plain, the
   * copy comes first: differ 2). */
  do {
  } while (0);
  scene = party->scene;
  {
    /* FAKEMATCH: the first argument pinned to r5 after the scene read stays
     * after it, as in ⚓️; as the parameter it is copied above the barrier
     * (differ 2). */
    register s32 a asm("r5") = aArg;

    if (scene == (s32)&SceneId_Ov67FRoom1)
      Func_020000a4(gData_02001674, a, b);
    else
      Func_020000a4(gData_02001688, a, b);
    if (a == 1)
      Func_02000f30(Object_GetById(work->objectId));
  }
}
