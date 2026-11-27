/* @file */
/* Declarations Tundaria Tower shares with Dehkan Plateau and Magma Rock's */
/* inside, whose pillars and rubble it copies: the field engine calls their */
/* overlays make, some through their own import rows. */
#ifndef ALCHEMY_TUNDARIA_TOWER_H
#define ALCHEMY_TUNDARIA_TOWER_H

#include "fixed_math.h"
#include "types.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "system/field_effect.h"
#include "field/actor.h"
#include "scene_ids.h"
#include "field/field_sequence_event.h"
#include "graphics/resource.h"

#include "field/scene_tables.h"

s32 scMapCellSetAttributes(u32 layer, s32 x, s32 z, s32 value);
void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options);
/* Stops the leader where it stands (fdLeaderStopMoving, through the overlay's
 */
/* own import row). */
void syImportLeaderStopMoving(void);

void syRockSpawn(s32 x, s32 y, s32 z, s32 frames);

/* fdPaletteFadeSetSource, under the name the tower's fades call it by. */
static inline void scTundariaTowerFadeFrom(s32 color, s32 mode) {
	fdPaletteFadeSetSource(color, mode);
}

#endif
