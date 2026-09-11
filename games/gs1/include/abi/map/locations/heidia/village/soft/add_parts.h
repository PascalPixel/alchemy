#ifndef ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_ADD_PARTS
#define ALCHEMY_ABI_MAP_LOCATIONS_HEIDIA_VILLAGE_SOFT_ADD_PARTS

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_0200b2c2(SoftFloatRecord *record);
extern s32 Func_0200b2d0(SoftFloatRecord *record);
extern s32 Func_0200b2ea(SoftFloatRecord *record);
extern s32 Func_0200b2f4(SoftFloatRecord *record);
extern s32 Func_0200b310(SoftFloatRecord *record);
extern s32 Func_0200b32a(SoftFloatRecord *record);
extern s32 Func_0200b334(SoftFloatRecord *record);
extern s32 Func_0200b35a(SoftFloatRecord *record);

#define GetSoftFloatNaNRecord Func_0200b2f0
#define SoftFloatLeftIsInfinity Func_0200b2ea
#define SoftFloatLeftIsNaN Func_0200b2c2
#define SoftFloatLeftIsZero Func_0200b35a
#define SoftFloatLeftIsZeroWithRightZero Func_0200b334
#define SoftFloatRightIsInfinity Func_0200b310
#define SoftFloatRightIsInfinityWithLeftInfinity Func_0200b2f4
#define SoftFloatRightIsNaN Func_0200b2d0
#define SoftFloatRightIsZero Func_0200b32a

#endif
