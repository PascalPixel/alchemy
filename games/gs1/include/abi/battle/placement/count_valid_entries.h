#ifndef ALCHEMY_ABI_BATTLE_PLACEMENT_COUNT_VALID_ENTRIES
#define ALCHEMY_ABI_BATTLE_PLACEMENT_COUNT_VALID_ENTRIES

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6c08(s32, u16 *);

#define Battle_Apply Func_080b6c08
#define Battle_Run Func_08077000

#endif
