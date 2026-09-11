#ifndef ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_ADD
#define ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_ADD

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0200b85c(const SoftDouble *packed, SoftFloatRecord *record);
void Func_0200b866(const SoftDouble *packed, SoftFloatRecord *record);
SoftDouble Func_0200b6a8(SoftFloatRecord *record);

#define Map_Apply Func_0200b85c
#define Map_Apply2 Func_0200b866
#define Map_Do Func_0200b6a8
#define Map_Run Func_0200b258

#endif
