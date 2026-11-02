// Matches: differ 0 (tla-en, 2026-11-02)
// The Broken Seal's FieldEffect_Spawn, through the shared field_effect/spawn.c
// with no device; the overlay's copies differ only in these five names. Differ
// compares instructions only; the twelve-edition compare still has to confirm
// the pool.
#define EFFECT_SPAWN Func_020000b8
#define EFFECT_CREATE Func_02002f98
#define EFFECT_SET_MODE Func_02002f80
#define EFFECT_SET_SCRIPT Func_02002f90
#define EFFECT_SCRIPTS gData_02003334
#include "../../../../games/tla/src/field/field_effect/spawn.c"
