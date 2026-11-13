/// @file
/// Scene code of overlay 0x66b, in ROM order.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_LifeScripts
#define EFFECT_SPRITE_PRIORITY
#include "../../field/field_effect/spawn.c"
