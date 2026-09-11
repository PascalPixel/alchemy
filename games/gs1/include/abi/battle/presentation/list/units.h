#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_LIST_UNITS
#define ALCHEMY_ABI_BATTLE_PRESENTATION_LIST_UNITS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6b40(s32 side, u16 *out_units);
s32 Func_080151c0(void *entries, u16 *excluded_units, s32 excluded_count);
void Func_08002df0(void *ptr);

#define BattlePres_BuildUnitEntries Func_080b920c
#define Battle_Apply Func_080b6b40
#define Battle_Do Func_08002df0
#define Battle_Place Func_080151c0
#define Battle_Run Func_08004970
#define Battle_Run2 Func_08077008

#endif
