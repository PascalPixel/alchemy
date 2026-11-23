/// @file
/// The characters a new game sets up with their starting equipment, class and
/// level.

#include "game/starting_equipment.h"
#include "types.h"
#include "game/character_ids.h"

/// The five who fight in this game, ended by -1: Party_InitializeCharacters
/// equips each with their definition's items and raises them to their
/// starting level. Felix, Sheba and Piers are left out.
// clang-format off
const s32 gCharacter_StartingEquipUnitIds[6] = {
    ISAAC, GARET, IVAN, MIA, JENNA, -1,
};
// clang-format on
