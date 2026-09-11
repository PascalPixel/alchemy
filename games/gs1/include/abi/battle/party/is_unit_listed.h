#ifndef ALCHEMY_ABI_BATTLE_PARTY_IS_UNIT_LISTED
#define ALCHEMY_ABI_BATTLE_PARTY_IS_UNIT_LISTED

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6c08(s32, u16 *);

#define Battle_Apply Func_080b6c08

#endif
