#ifndef ALCHEMY_ACTOR_BUILTIN_SCRIPTS_H
#define ALCHEMY_ACTOR_BUILTIN_SCRIPTS_H

/* The built-in scripts an actor script can hand its actor to: key checks, the
   player, the world map and the camera. */

#include "types.h"
#include "field/field.h"

extern const Sint32 Actor_KeyCheckScript[];
extern const Sint32 Actor_KeysScript[];
extern const Sint32 Actor_PlayerScript[];
extern const Sint32 Actor_WorldMapScript[];
extern const Sint32 Actor_CameraScript[];
extern const Sint32 Actor_ApproachWatchedScript[];
extern const Sint32 Actor_IdleScript[];

#endif
