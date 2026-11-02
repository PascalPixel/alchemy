// Matches: differ 0 (tla-en, 2026-11-01)
// The Broken Seal's FieldEffect_Spawn, through the shared field_effect/spawn.c
// with no device; the overlay's copies differ only in these five names. Differ
// compares instructions only; the twelve-edition compare still has to confirm
// the pool.
#define EFFECT_SPAWN Func_02000ce4
#define EFFECT_CREATE Func_02001438
#define EFFECT_SET_MODE Func_02001428
#define EFFECT_SET_SCRIPT Func_02001430
#define EFFECT_SCRIPTS gData_02001668
#include "../../../../games/tla/src/field/field_effect/spawn.c"
