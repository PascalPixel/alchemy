/// @file
/// The Izumo Ruins' copy of the effect spawner, FieldEffect_Spawn.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_LifeScripts
#include "../../../../games/tla/src/field/field_effect/spawn.c"
