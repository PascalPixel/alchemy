#ifndef ALCHEMY_ABI_BATTLE_PARTY_LIST_ALL_UNITS_AND_SUBMIT
#define ALCHEMY_ABI_BATTLE_PARTY_LIST_ALL_UNITS_AND_SUBMIT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6c08(s32, void *);
s32 Func_080b7b6c(void *, s32);

#define Battle_Apply Func_080b6c08
#define Battle_Apply2 Func_080b7b6c

#endif
