#ifndef ALCHEMY_FIELD_MAP_GRID_H
#define ALCHEMY_FIELD_MAP_GRID_H

/// @file
/// The map-grid event commands the scenes call that event_inline.h does not
/// have yet, written in its form: the inline takes the Field prefix and passes
/// the call on to the main image's routine.

#include "types.h"

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

/// Gives the cell at (x, z) of a layer the ground height `height`, in 8-pixel
/// steps (MapGrid_SetCellHeight).
static inline void FieldMapGrid_SetCellHeight(s32 x, s32 z, s32 layer,
                                              s32 height) {
  MapGrid_SetCellHeight(x, z, layer, height);
}

#endif
