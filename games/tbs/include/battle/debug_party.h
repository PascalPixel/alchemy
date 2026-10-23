#ifndef ALCHEMY_DEBUG_PARTY_H
#define ALCHEMY_DEBUG_PARTY_H

/// @file
/// The debug party presets.

#include "types.h"
#include "battle_unit.h"

/// One row of the debug party presets: a party member, the level to raise them
/// to, the Djinn of each element to give them, the items to equip and up to two
/// abilities to teach. A row whose member is DEBUG_PRESET_END ends a preset.
/// Read only by DebugParty_LoadPreset.
struct DebugPartyEntry {
  s8 unitId;                     ///< the party member, or DEBUG_PRESET_END
  u8 level;                      ///< the level the member is raised to
  s8 djinnCounts[ELEMENT_COUNT]; ///< Djinn of each element to give and Set
  u16 items[4];                  ///< items to add and equip; 0 for none
  u16 abilities[2];              ///< abilities to teach; 0 for none
  u16 unused;                    ///< 0 in every row
};

#define DEBUG_PRESET_END -1

#endif
