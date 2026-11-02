// Matches: differ 0 (tla-en, 2026-11-02)
// Twin of The Broken Seal's FieldEffect_SpawnResident (EFFECT_SPAWN in
// scene/shared/field_effect_spawn.h), confirmed by its flag tests, option reads
// and pool: the overlay's own copy of the spawner in
// games/tla/src/field/field_effect/spawn.c, with default type 285. Taken whole
// with EFFECT_SPRITE_PRIORITY, as overlays 663, 664, 66b, 679 and 67b do;
// without it, 78 of 219 differ (skeleton gap 19/7). It carries that file's
// tagged devices. Only the set-mode, set-script and script-table symbols below
// are this overlay's, so the form carries to its copies in 681 and 687. Nothing
// is left but adoption. Differ compares instructions only; the twelve-edition
// compare still has to confirm the pool.
#define EFFECT_SPAWN Func_0200015c
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Func_02002dd4
#define EFFECT_SET_SCRIPT Func_02002ddc
#define EFFECT_SCRIPTS gData_02003040
#define EFFECT_SPRITE_PRIORITY
#include "../../../../games/tla/src/field/field_effect/spawn.c"
