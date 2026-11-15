#ifndef ALCHEMY_EVENT_LAYER_H
#define ALCHEMY_EVENT_LAYER_H

/// @file
/// The event commands the scenes call that event_inline.h does not have yet,
/// written in its form: the inline takes the Field prefix, or Event where the
/// routine's name already has Field, and passes the call on to the main
/// image's routine.

#include "types.h"

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

/// Gives the cell at (x, z) of a layer the ground height `height`, in 8-pixel
/// steps (MapGrid_SetCellHeight).
static inline void FieldMapGrid_SetCellHeight(s32 x, s32 z, s32 layer,
                                              s32 height) {
  MapGrid_SetCellHeight(x, z, layer, height);
}

void Field_StartColorTransition(s32 frames, s32 color);

/// Fades the palettes to `color` over `frames` frames, keeping the field's
/// colours to fade back to (Field_StartColorTransition).
static inline void Event_StartColorTransition(s32 frames, s32 color) {
  Field_StartColorTransition(frames, color);
}

#endif
