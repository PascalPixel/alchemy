#ifndef ALCHEMY_DEBUG_PARAMETER_SET_H
#define ALCHEMY_DEBUG_PARAMETER_SET_H

/// @file
/// The debug parameter sets.

#include "types.h"
#include "battle_unit.h"

/// One row of the debug parameter sets: a party member, the level to raise them
/// to, the Djinn of each element to give them, the items to equip and up to two
/// abilities to teach. A row whose party member is DEBUG_PARAMETER_SET_END ends
/// a parameter set. Read only by DebugParameterSet_Load.
struct DebugParameterSetRow {
  s8 unitId;                     ///< the party member, or the end of a set
  u8 level;                      ///< the level the party member is raised to
  s8 djinnCounts[ELEMENT_COUNT]; ///< Djinn of each element to give and Set
  u16 items[4];                  ///< items to add and equip; 0 for none
  u16 abilities[2];              ///< abilities to teach; 0 for none
  u16 unused;                    ///< 0 in every row
};

#define DEBUG_PARAMETER_SET_END -1

#endif
