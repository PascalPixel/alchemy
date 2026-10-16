#include "unit/field/griddata.h"
#include "types.h"

// The sprite viewer's (x, y) sprite scale, 1.0 each (Q16), which
// Battle_PlaceActorsByFormationKind reads.
const s32 gBattle_FormationPlacementScale[2] = {
    0x10000,
    0x10000,
};
