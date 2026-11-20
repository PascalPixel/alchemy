#ifndef ALCHEMY_EVENT_LAYER_H
#define ALCHEMY_EVENT_LAYER_H

/// @file
/// The event commands the scenes call that event_inline.h does not have yet,
/// written in its form: the inline takes the Field prefix, or Event where the
/// routine's name already has Field, and passes the call on to the main
/// image's routine.

#include "types.h"
#include "field/effect_sprite.h"

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

/// Gives the cell at (x, z) of a layer the ground height `height`, in 8-pixel
/// steps (MapGrid_SetCellHeight).
static inline void FieldMapGrid_SetCellHeight(s32 x, s32 z, s32 layer,
                                              s32 height) {
  MapGrid_SetCellHeight(x, z, layer, height);
}

s32 MapCell_SetAttributes(u32 layer, s32 x, s32 z, s32 value);

/// Replaces a cell's collision code and attributes on a layer
/// (MapCell_SetAttributes).
static inline void FieldMapCell_SetAttributes(s32 layer, s32 x, s32 z,
                                              s32 value) {
  MapCell_SetAttributes(layer, x, z, value);
}

void Field_StartColorTransition(s32 frames, s32 color);

/// Fades the palettes to `color` over `frames` frames, keeping the field's
/// colours to fade back to (Field_StartColorTransition).
static inline void Event_StartColorTransition(s32 frames, s32 color) {
  Field_StartColorTransition(frames, color);
}

void Cyclone_CopyTouchedCells(s32 x, s32 z, s32 layerIndex, s32 width,
                              s32 height, s32 toX, s32 toZ);

/// Cyclone scatters the bushes or weeds it touches: the cells of the width by
/// height rectangle at (x, z) of the layer take those of the scattered plants,
/// kept at (toX, toZ) in the map (Cyclone_CopyTouchedCells).
static inline void FieldCyclone_CopyTouchedCells(s32 x, s32 z, s32 layerIndex,
                                                 s32 width, s32 height, s32 toX,
                                                 s32 toZ) {
  Cyclone_CopyTouchedCells(x, z, layerIndex, width, height, toX, toZ);
}

void TileCopies_CopyTouchedCells(s32 sourceX, s32 sourceZ, s32 width,
                                 s32 height, s32 windowX, s32 windowZ,
                                 s32 layer);

/// Opens the cells of a window that the leader can reach, taking their pictures
/// from the same place of another window, as Cyclone clears a bush
/// (TileCopies_CopyTouchedCells).
static inline void FieldTileCopies_CopyTouchedCells(s32 sourceX, s32 sourceZ,
                                                    s32 width, s32 height,
                                                    s32 windowX, s32 windowZ,
                                                    s32 layer) {
  TileCopies_CopyTouchedCells(sourceX, sourceZ, width, height, windowX, windowZ,
                              layer);
}

void MadPlant_LeapAtLeader(s32 sprite, s32 x, s32 z);

/// A Mad Plant leaps out of a cleared bush at the leader
/// (MadPlant_LeapAtLeader).
static inline void FieldMadPlant_LeapAtLeader(s32 sprite, s32 x, s32 z) {
  MadPlant_LeapAtLeader(sprite, x, z);
}

/// Sets up an effect sprite: its pivot, its OAM attributes and its first tile
/// (EffectSprite_Init).
static inline void FieldEffectSprite_Init(struct EffectSprite *sprite,
                                          s32 pivotX, s32 pivotY,
                                          u32 attributes, s32 tile) {
  EffectSprite_Init(sprite, pivotX, pivotY, attributes, tile);
}

#endif
