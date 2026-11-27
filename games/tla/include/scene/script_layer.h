#ifndef ALCHEMY_SCRIPT_LAYER_H
#define ALCHEMY_SCRIPT_LAYER_H

/* The script commands the scenes call that script_inline.h does not have yet,
   written in its form: the inline takes the Field prefix, or Script where the
   function's name already has Field, and passes the call on to the main image's
   function. */

/* The inlines below are compiled against their own forms of
   fdCycloneCopyTouchedMetatiles and fdTileCopiesCopyTouchedMetatiles. */
#define OWN_fdCycloneCopyTouchedMetatiles
#define OWN_fdTileCopiesCopyTouchedMetatiles

#include "types.h"
#include "field/field.h"
#include "field/field_mode.h"
#include "field/effect_sprite.h"

/* Gives the metatile at (x, z) of a layer the ground height `height`, in
   8-pixel steps (fdMapGridSetMetatileHeight). */
static inline void syFieldMapGridSetMetatileHeight(
	Sint32 x, Sint32 z, Sint32 layer, Sint32 height)
{
	fdMapGridSetMetatileHeight(x, z, layer, height);
}

Sint32 scMapMetatileSetAttributes(
	Uint32 layer, Sint32 x, Sint32 z, Sint32 value);

/* Replaces a metatile's collision code and attributes on a layer
   (scMapMetatileSetAttributes). */
static inline void syFieldMapMetatileSetAttributes(
	Sint32 layer, Sint32 x, Sint32 z, Sint32 value)
{
	scMapMetatileSetAttributes(layer, x, z, value);
}

/* Fades the palettes to `col` over `ticks` ticks, keeping the field
   engine's colours to fade back to (fdStartColTransition). */
static inline void fdScriptStartColTransition(Sint32 ticks, Sint32 col)
{
	fdStartColTransition(ticks, col);
}

/* The ROM's callers are compiled against this form, which takes layerIdx,
   width and height as Sint32 where the definition takes them as Sint16. */
void fdCycloneCopyTouchedMetatiles(Sint32 x, Sint32 z, Sint32 layerIdx,
	Sint32 width, Sint32 height, Sint32 toX, Sint32 toZ);

/* Cyclone scatters the bushes or weeds it touches: the metatiles of the width
   by height rectangle at (x, z) of the layer take those of the scattered
   plants, kept at (toX, toZ) in the map (fdCycloneCopyTouchedMetatiles). */
static inline void syFieldCycloneCopyTouchedMetatiles(Sint32 x, Sint32 z,
	Sint32 layerIdx, Sint32 width, Sint32 height, Sint32 toX, Sint32 toZ)
{
	fdCycloneCopyTouchedMetatiles(x, z, layerIdx, width, height, toX, toZ);
}

/* The ROM's callers are compiled against this form, which takes winZ as an
   Sint32 where the definition takes an Sint16. */
void fdTileCopiesCopyTouchedMetatiles(Sint32 srcX, Sint32 srcZ, Sint32 width,
	Sint32 height, Sint32 winX, Sint32 winZ, Sint32 layer);

/* Opens the metatiles of a window that the leader can reach, taking their
   pictures from the same place of another window, as Cyclone clears a bush
   (fdTileCopiesCopyTouchedMetatiles). */
static inline void syFieldTileCopiesCopyTouchedMetatiles(Sint32 srcX,
	Sint32 srcZ, Sint32 width, Sint32 height, Sint32 winX, Sint32 winZ,
	Sint32 layer)
{
	fdTileCopiesCopyTouchedMetatiles(
		srcX, srcZ, width, height, winX, winZ, layer);
}

/* A Mad Plant leaps out of a cleared bush at the leader
   (fdMadPlantLeapAtLeader). */
static inline void syFieldMadPlantLeapAtLeader(Sint32 spr, Sint32 x, Sint32 z)
{
	fdMadPlantLeapAtLeader(spr, x, z);
}

/* Sets up an effect sprite: its pivot, its OAM attributes and its first tile
   (fdEffectSprInit). */
static inline void syFieldEffectSprInit(EFFECT_SPR *spr, Sint32 pivotX,
	Sint32 pivotY, Uint32 attributes, Sint32 tile)
{
	fdEffectSprInit(spr, pivotX, pivotY, attributes, tile);
}

#endif
