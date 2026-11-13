/// @file
/// Scene code of overlay 0x6af, in ROM order.

#define EFFECT_SPAWN Func_0200015c
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gData_02001170
#define EFFECT_SPRITE_PRIORITY
#include "../../field/field_effect/spawn.c"
