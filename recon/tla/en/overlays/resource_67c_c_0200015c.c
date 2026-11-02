// Matches: differ 0 (tla-en, 2026-11-01)
// Twin of The Broken Seal's FieldEffect_SpawnResident (EFFECT_SPAWN in
// scene/shared/field_effect_spawn.h), confirmed by its calls and pool: the
// overlay's own copy of the spawner in
// games/tla/src/field/field_effect/spawn.c, with default type 285. Taken whole
// with EFFECT_SPRITE_PRIORITY, as overlays 663, 664, 66b, 679 and 67b do;
// without it, 78 of 219 differ. Only the set-mode, set-script and script-table
// symbols below are this overlay's, so the form carries to its copies.
// It carries that file's two tagged devices. Measured here on the new compiler:
// the one-pass block is stale (differ 0 without it, spawn.c's tag still says 4);
// the wrapper is needed (36 without it, 57 with one call taking a chosen type);
// so is the low-byte priority read (12 as (u8), 14 plain or through a local).
// Nothing is left but adoption. Differ compares instructions only; the
// twelve-edition compare still has to confirm the pool.
#define EFFECT_SPAWN Func_0200015c
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Func_02003cc4
#define EFFECT_SET_SCRIPT Func_02003ccc
#define EFFECT_SCRIPTS gData_020044f8
#define EFFECT_SPRITE_PRIORITY
#include "../../../../games/tla/src/field/field_effect/spawn.c"
