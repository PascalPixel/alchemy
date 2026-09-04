#include "types.h"
#include "add_parts_record.h"

extern s32 Func_0200259a(SoftFloatRecord *record);
extern s32 Func_020025a8(SoftFloatRecord *record);
extern s32 Func_020025c2(SoftFloatRecord *record);
extern SoftFloatRecord *Func_020025c8(void);
extern s32 Func_020025cc(SoftFloatRecord *record);
extern s32 Func_020025e8(SoftFloatRecord *record);
extern s32 Func_02002602(SoftFloatRecord *record);
extern s32 Func_0200260c(SoftFloatRecord *record);
extern s32 Func_02002632(SoftFloatRecord *record);

#define AddSoftFloatParts Func_02001158
#define SoftFloatLeftIsNaN Func_0200259a
#define SoftFloatRightIsNaN Func_020025a8
#define SoftFloatLeftIsInfinity Func_020025c2
#define GetSoftFloatNaNRecord Func_020025c8
#define SoftFloatRightIsInfinityWithLeftInfinity Func_020025cc
#define SoftFloatRightIsInfinity Func_020025e8
#define SoftFloatRightIsZero Func_02002602
#define SoftFloatLeftIsZeroWithRightZero Func_0200260c
#define SoftFloatLeftIsZero Func_02002632
SoftFloatRecord *AddSoftFloatParts(SoftFloatRecord *left,
                                   SoftFloatRecord *right,
                                   SoftFloatRecord *result)
{
#include "add_parts_body.inc"
}
