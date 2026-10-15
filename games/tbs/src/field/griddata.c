#include "unit/field/griddata.h"
#include "types.h"

// The (x, y) scale for battle formation placement: 1.0 each (Q16).
const s32 gBattle_FormationPlacementScale[2] = {
    0x10000,
    0x10000,
};
