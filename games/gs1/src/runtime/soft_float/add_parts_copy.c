#include "types.h"
#include "add_parts_record.h"

/* Second embedded copy of the compiler runtime's soft-float addition core. */
extern s32 Func_0200b2c2(SoftFloatRecord *record);
extern s32 Func_0200b2d0(SoftFloatRecord *record);
extern s32 Func_0200b2ea(SoftFloatRecord *record);
extern SoftFloatRecord *Func_0200b2f0(void);
extern s32 Func_0200b2f4(SoftFloatRecord *record);
extern s32 Func_0200b310(SoftFloatRecord *record);
extern s32 Func_0200b32a(SoftFloatRecord *record);
extern s32 Func_0200b334(SoftFloatRecord *record);
extern s32 Func_0200b35a(SoftFloatRecord *record);

#define AddSoftFloatParts Func_020057ec
#define SoftFloatLeftIsNaN Func_0200b2c2
#define SoftFloatRightIsNaN Func_0200b2d0
#define SoftFloatLeftIsInfinity Func_0200b2ea
#define GetSoftFloatNaNRecord Func_0200b2f0
#define SoftFloatRightIsInfinityWithLeftInfinity Func_0200b2f4
#define SoftFloatRightIsInfinity Func_0200b310
#define SoftFloatRightIsZero Func_0200b32a
#define SoftFloatLeftIsZeroWithRightZero Func_0200b334
#define SoftFloatLeftIsZero Func_0200b35a
SoftFloatRecord *AddSoftFloatParts(SoftFloatRecord *left,
                                   SoftFloatRecord *right,
                                   SoftFloatRecord *result)
{
#include "add_parts_body.inc"
}
