#ifndef ALCHEMY_EVENT_LAYER_H
#define ALCHEMY_EVENT_LAYER_H

/* @file */
/* The event commands the scenes call that event_inline.h does not have yet, */
/* written in its form: the inline takes the Field prefix, or Event where the */
/* function's name already has Field, and passes the call on to the main */
/* image's function. */

/* The inlines below are compiled against their own forms of */
/* fdCycloneCopyTouchedCells and fdTileCopiesCopyTouchedCells. */
#define OWN_Cyclone_CopyTouchedCells
#define OWN_TileCopies_CopyTouchedCells

#include "types.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/effect_sprite.h"

/* Gives the cell at (x, z) of a layer the ground height `height`, in 8-pixel */
/* steps (fdMapGridSetCellHeight). */
static inline void syFieldMapGridSetCellHeight(s32 x, s32 z, s32 layer,
											   s32 height) {
	fdMapGridSetCellHeight(x, z, layer, height);
}

s32 scMapCellSetAttributes(u32 layer, s32 x, s32 z, s32 value);

/* Replaces a cell's collision code and attributes on a layer */
/* (scMapCellSetAttributes). */
static inline void syFieldMapCellSetAttributes(s32 layer, s32 x, s32 z,
											   s32 value) {
	scMapCellSetAttributes(layer, x, z, value);
}

/* Fades the palettes to `color` over `frames` frames, keeping the field */
/* engine's colours to fade back to (fdStartColTransition). */
static inline void fdEventStartColTransition(s32 frames, s32 color) {
	fdStartColTransition(frames, color);
}

/* The ROM's callers are compiled against this form, which takes layerIndex, */
/* width and height as s32 where the definition takes them as s16. */
void fdCycloneCopyTouchedCells(s32 x, s32 z, s32 layerIndex, s32 width,
							   s32 height, s32 toX, s32 toZ);
/* Cyclone scatters the bushes or weeds it touches: the cells of the width by */
/* height rectangle at (x, z) of the layer take those of the scattered plants,
 */
/* kept at (toX, toZ) in the map (fdCycloneCopyTouchedCells). */
static inline void syFieldCycloneCopyTouchedCells(s32 x, s32 z, s32 layerIndex,
												  s32 width, s32 height,
												  s32 toX, s32 toZ) {
	fdCycloneCopyTouchedCells(x, z, layerIndex, width, height, toX, toZ);
}

/* The ROM's callers are compiled against this form, which takes windowZ as an
 */
/* s32 where the definition takes an s16. */
void fdTileCopiesCopyTouchedCells(s32 sourceX, s32 sourceZ, s32 width,
								  s32 height, s32 windowX, s32 windowZ,
								  s32 layer);
/* Opens the cells of a window that the leader can reach, taking their pictures
 */
/* from the same place of another window, as Cyclone clears a bush */
/* (fdTileCopiesCopyTouchedCells). */
static inline void syFieldTileCopiesCopyTouchedCells(s32 sourceX, s32 sourceZ,
													 s32 width, s32 height,
													 s32 windowX, s32 windowZ,
													 s32 layer) {
	fdTileCopiesCopyTouchedCells(sourceX, sourceZ, width, height, windowX,
								 windowZ, layer);
}

/* A Mad Plant leaps out of a cleared bush at the leader */
/* (fdMadPlantLeapAtLeader). */
static inline void syFieldMadPlantLeapAtLeader(s32 sprite, s32 x, s32 z) {
	fdMadPlantLeapAtLeader(sprite, x, z);
}

/* Sets up an effect sprite: its pivot, its OAM attributes and its first tile */
/* (fdEffectSprInit). */
static inline void syFieldEffectSprInit(EFFECT_SPR *sprite, s32 pivotX,
										s32 pivotY, u32 attributes, s32 tile) {
	fdEffectSprInit(sprite, pivotX, pivotY, attributes, tile);
}

#endif
