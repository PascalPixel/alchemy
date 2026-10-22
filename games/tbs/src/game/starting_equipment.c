#include "unit/game/starting_equipment.h"
#include "types.h"
#include "character_ids.h"

/// Characters given starting equipment, terminated by -1.
/// Party_InitializeCharacters reads it at a new game: each listed character
/// gets its definition's items, equipped, its class and its starting level.
// clang-format off
const s32 gCharacter_StartingEquipOwnerIds[6] = {
    ISAAC, GARET, IVAN, MIA, JENNA, -1,
};
// clang-format on
