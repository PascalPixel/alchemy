#ifndef ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_TO_INT
#define ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_TO_INT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0200b982(FloUnion *packed, SoftFloatRecord *record);
u32 Func_0200b77c(SoftFloatRecord *record);
u32 Func_0200b766(SoftFloatRecord *record);
u32 Func_0200b784(SoftFloatRecord *record);
u64 Func_0200b7ce(u64 fraction, u32 count);

#define Map_Apply Func_0200b982
#define Map_Apply2 Func_0200b7ce
#define Map_Do Func_0200b77c
#define Map_Do2 Func_0200b766
#define Map_Do3 Func_0200b784

#endif
