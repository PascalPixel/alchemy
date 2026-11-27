/* Declarations Tundaria Tower shares with Dehkan Plateau and Magma Rock's
   inside, whose pillars and rubble it copies: the field engine calls their
   overlays make, some through their own import rows. */
#ifndef ALCHEMY_TUNDARIA_TOWER_H
#define ALCHEMY_TUNDARIA_TOWER_H

#include "fixed_math.h"
#include "types.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "system/field_effect.h"
#include "field/actor.h"
#include "scene_ids.h"
#include "field/field_mode_script.h"
#include "graphics/resource.h"

#include "field/scene_tables.h"

Sint32 scMapMetatileSetAttributes(
	Uint32 layer, Sint32 x, Sint32 z, Sint32 value);
void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velocityX,
	Sint32 velocityY, Sint32 velocityZ, Uint32 flags,
	const EFFECT_OPTIONS *options);
/* Stops the leader where it stands (fdLeaderStopMoving, through the overlay's
   own import row). */
void syImportLeaderStopMoving(void);

void scRockSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 ticks);

/* fdPaletteFadeSetSource, under the name the tower's fades call it by. */
static inline void scTundariaTowerFadeFrom(Sint32 color, Sint32 mode)
{
	fdPaletteFadeSetSource(color, mode);
}

#endif
