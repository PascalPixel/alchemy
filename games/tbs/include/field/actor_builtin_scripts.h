#ifndef ALCHEMY_ACTOR_BUILTIN_SCRIPTS_H
#define ALCHEMY_ACTOR_BUILTIN_SCRIPTS_H

// The built-in scripts an actor script can hand its actor to: key checks, the player, the world map
// and the camera.

#include "types.h"
#include "field/field.h"

extern const Sint32 ActorKeyCheckScript[];
extern const Sint32 ActorKeysScript[];
extern const Sint32 ActorPlayerScript[];
extern const Sint32 ActorWorldMapScript[];
extern const Sint32 ActorCameraScript[];
extern const Sint32 ActorApproachWatchedScript[];
extern const Sint32 ActorIdleScript[];

#endif
