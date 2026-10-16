#include "unit/field/griddata.h"
#include "types.h"
#include "render_placement.h"

// The sprite viewer's (x, y) sprite scale, 1.0 each (Q16), which
// Battle_PlaceActorsByFormationKind reads.
const struct SpriteScale gBattle_FormationPlacementScale = {
    {{0x10000, 0x10000}}};
