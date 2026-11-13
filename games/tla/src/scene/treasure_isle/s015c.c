/// @file
/// Treasure Isle's copy of the field effect spawner.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../../field/field_effect/spawn.c"
