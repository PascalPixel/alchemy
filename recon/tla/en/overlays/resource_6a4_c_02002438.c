// Near miss: differ 2 of 49 (one reordered row): the native loads x before it
// copies the mask 64; the draft copies the mask first.
// Tried: an x temporary, the mask as the first operand of the OR.
#include "../../../../games/tla/src/scene/jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"

void Func_02002438(struct JupiterLighthouseAerieWork *work,
                   struct JupiterLighthouseAerieEntry *entry) {
  struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
  s32 x = (object->x >> 20) | (s16)(work->x & 64);
  s32 z = (object->z >> 20) | (s16)(work->z & 64);

  Func_020023b0();
  Func_02002b68(161);
  Func_02002a40(work->x, work->z + 1, 1, 1, x, z);
  FieldEvent_WaitFrames(12);
}
