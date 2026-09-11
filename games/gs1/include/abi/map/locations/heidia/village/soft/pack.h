#ifndef ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_PACK
#define ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_PACK

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0200ba24(SoftFloatRecord *record);
s32 Func_0200ba4c(SoftFloatRecord *record);
s32 Func_0200ba66(SoftFloatRecord *record);
u64 Func_0200b8c2(u64 value, u32 count);

#define Map_Apply Func_0200b8c2
#define Map_Check Func_0200ba24
#define Map_Check2 Func_0200ba4c
#define Map_Check3 Func_0200ba66

#endif
