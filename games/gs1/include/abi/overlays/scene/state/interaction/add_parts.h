#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_ADD_PARTS
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_ADD_PARTS

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_0200259a(SoftFloatRecord *record);
extern s32 Func_020025a8(SoftFloatRecord *record);
extern s32 Func_020025c2(SoftFloatRecord *record);
extern s32 Func_020025cc(SoftFloatRecord *record);
extern s32 Func_020025e8(SoftFloatRecord *record);
extern s32 Func_02002602(SoftFloatRecord *record);
extern s32 Func_0200260c(SoftFloatRecord *record);
extern s32 Func_02002632(SoftFloatRecord *record);

#define GetSoftFloatNaNRecord Func_020025c8
#define SoftFloatLeftIsInfinity Func_020025c2
#define SoftFloatLeftIsNaN Func_0200259a
#define SoftFloatLeftIsZero Func_02002632
#define SoftFloatLeftIsZeroWithRightZero Func_0200260c
#define SoftFloatRightIsInfinity Func_020025e8
#define SoftFloatRightIsInfinityWithLeftInfinity Func_020025cc
#define SoftFloatRightIsNaN Func_020025a8
#define SoftFloatRightIsZero Func_02002602

#endif
