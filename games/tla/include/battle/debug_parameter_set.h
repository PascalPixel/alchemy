#ifndef ALCHEMY_DEBUG_PARAMETER_SET_H
#define ALCHEMY_DEBUG_PARAMETER_SET_H

/// @file
/// The debug parameter sets.

#include "types.h"

/// A row of the debug parameter sets: a party member, its level, its Djinn of
/// each element, its equipped items and two Psynergy it learns. A row whose
/// party member is DEBUG_PARAMETER_SET_END ends a parameter set.
struct DebugPartyEntry {
  s8 owner;
  u8 level;
  s8 djinnCounts[4];
  u16 items[6];
  u16 actions[2];
  u16 unused; ///< 0 in every row; nothing reads it
};

/// The owner that ends the debug parameter sets.
#define DEBUG_PARAMETER_SET_END -1

/// The debug parameter sets DebugParameterSet_Load loads, a party member a row
/// (debug_parameter_sets.c).
extern struct DebugPartyEntry gDebug_ParameterSets[];

#endif
