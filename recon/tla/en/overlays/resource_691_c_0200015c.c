// Matches: differ 0 (tla-en, 2026-11-01)
// Twin of The Broken Seal's FieldEffect_SpawnResident (EFFECT_SPAWN in
// scene/shared/field_effect_spawn.h): the overlay's own copy of the spawner in
// games/tla/src/field/field_effect/spawn.c. Its listing is overlay 692's copy
// with only the set-mode, set-script and script-table symbols changed, so it
// is taken whole with EFFECT_SPRITE_PRIORITY, and carries that file's two
// tagged devices (wrapper, one-pass). Nothing is left but adoption. Differ
// compares instructions only; the twelve-edition compare still has to confirm
// the pool.
#define EFFECT_SPAWN Func_0200015c
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Func_02003a54
#define EFFECT_SET_SCRIPT Func_02003a5c
#define EFFECT_SCRIPTS gData_02003e6c
#define EFFECT_SPRITE_PRIORITY
#include "../../../../games/tla/src/field/field_effect/spawn.c"
