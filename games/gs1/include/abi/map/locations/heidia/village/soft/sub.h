#ifndef ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_SUB
#define ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_SUB

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0200b894(const SoftDouble *packed, SoftFloatRecord *record);
void Func_0200b89e(const SoftDouble *packed, SoftFloatRecord *record);
SoftDouble Func_0200b6e8(SoftFloatRecord *record);

#define Map_Apply Func_0200b894
#define Map_Apply2 Func_0200b89e
#define Map_Do Func_0200b6e8
#define Map_Run Func_0200b298

#endif
