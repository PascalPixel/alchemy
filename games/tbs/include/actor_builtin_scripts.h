#ifndef ALCHEMY_ACTOR_BUILTIN_SCRIPTS_H
#define ALCHEMY_ACTOR_BUILTIN_SCRIPTS_H

/// @file
/// The built-in scripts an actor script can hand its actor to: key checks,
/// the player, the world map and the camera.

#include "unit/field/field.h"
#include "types.h"

extern const s32 gActor_KeyCheckScript[];
extern const s32 gActor_KeysScript[];
extern const s32 gActor_PlayerScript[];
extern const s32 gActor_WorldMapScript[];
extern const s32 gActor_CameraScript[];
extern const s32 gActor_ApproachWatchedScript[];
extern const s32 gActor_IdleScript[];

#endif
