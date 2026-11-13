/// @file
/// The scene code of overlay 0x690, in ROM order.

#define EFFECT_SPAWN Func_020000b8
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Import_ActorSelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gData_020022d8
#include "../field/field_effect/spawn.c"
