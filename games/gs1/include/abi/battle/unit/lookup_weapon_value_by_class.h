#ifndef ALCHEMY_ABI_BATTLE_UNIT_LOOKUP_WEAPON_VALUE_BY_CLASS
#define ALCHEMY_ABI_BATTLE_UNIT_LOOKUP_WEAPON_VALUE_BY_CLASS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077228(s32, s32);
u32 Func_080b6e7c(s32 arg0);

#define Battle_Apply Func_08077228
#define Battle_Do Func_080b6e7c
#define gRom Data_080c2a1c
#define gRom2 Data_080c2a2a
#define gRom3 Data_080c2a38
#define gRom4 Data_080c2a46
#define gRom5 Data_080c2a54

#endif
