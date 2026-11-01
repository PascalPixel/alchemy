// Matches: differ 0 (tla-en, 2026-11-01)
// Twin of The Broken Seal's FieldEffect_SpawnResident (EFFECT_SPAWN in
// scene/shared/field_effect_spawn.h), and overlay 692's own copy of the spawner
// already adopted for other overlays in
// games/tla/src/field/field_effect/spawn.c. Taken whole with
// EFFECT_SPRITE_PRIORITY, as overlays 663, 664, 66b, 679 and 67b do; without
// it, 78 of 219 differ. This replaces the untagged r10 pin of the earlier
// draft (differ 24, skeleton gap 7). It carries that file's two tagged devices
// (wrapper, one-pass). Nothing is left but adoption: a scene file of these same
// lines, as games/tla/src/scene/aqua_rock/s015c.c is. Differ compares
// instructions only; the twelve-edition compare still has to confirm the pool.
#define EFFECT_SPAWN Func_0200015c
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Func_02004c98
#define EFFECT_SET_SCRIPT Func_02004ca0
#define EFFECT_SCRIPTS gData_02005698
#define EFFECT_SPRITE_PRIORITY
#include "../../../../games/tla/src/field/field_effect/spawn.c"
