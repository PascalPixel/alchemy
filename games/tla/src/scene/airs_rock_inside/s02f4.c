/// @file
/// Scene code of overlay 0x664, in ROM order.

#define EFFECT_SPAWN Func_020002f4
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Import_ActorSelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gData_02006188
#define EFFECT_SPRITE_PRIORITY
#include "../../field/field_effect/spawn.c"
