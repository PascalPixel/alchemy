#ifndef ALCHEMY_ACTOR_BUILTIN_SCRIPTS_H
#define ALCHEMY_ACTOR_BUILTIN_SCRIPTS_H

/* @file */
/* The built-in scripts an actor script can hand its actor to: key checks, */
/* the player, the world map and the camera. */

#include "types.h"
#include "field/field.h"

extern const s32 Actor_KeyCheckScript[];
extern const s32 Actor_KeysScript[];
extern const s32 Actor_PlayerScript[];
extern const s32 Actor_WorldMapScript[];
extern const s32 Actor_CameraScript[];
extern const s32 Actor_ApproachWatchedScript[];
extern const s32 Actor_IdleScript[];

#endif
