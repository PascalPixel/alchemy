#include "types.h"

#define FieldScene_RunIndexedStep0 Func_02000ae8
#define FieldScene_RunIndexedStep1 Func_02000af4
#define FieldScene_RunIndexedStep2 Func_02000b00
#define FieldScene_RunIndexedStep3 Func_02000b0c
#define FieldScene_RunIndexedStep4 Func_02000b18
#define FieldScene_RunIndexedStep5 Func_02000b24
#define FieldScene_RunTwoCallSequence Func_02000d48
#define FieldScene_RunGuardedSixWordStep Func_02000eac
#define FieldScene_RunTwoArmSequenceWithValue217f Func_020034bc
typedef struct {
    s32 a, b, c, d, e, f;
} S6;

extern u8 Value_0000217f;

void Func_020015aa(s32);
void Func_020015b6(s32);
void Func_020015c2(s32);
void Func_020015ce(s32);
void Func_020015da(s32);
void Func_020015e6(s32);
void Func_02000e10(void);
void Func_02001a48(void);
void Func_02004b62(void);
s32 Func_0200132e(S6 *);
void Func_020014da(S6);
void Func_02004b8e(void);
void Func_02007170(void);
void Func_02007228(s32 arg0);
s32 Func_02007238(s32 arg0, s32 arg1);
s32 Func_02007198(s32 arg0, s32 arg1);
void Func_0200718a(s32 arg0);
void Func_02007248(s32 arg0);
void Func_02007260(s32 arg0, s32 arg1);
void Func_020071a0(s32 arg0);
void Func_0200725e(s32 arg0);
void Func_02007276(s32 arg0, s32 arg1);
void Func_020071c2(void);

/* Contiguous unnamed leaf-owner run for resource_3bd. */

void FieldScene_RunIndexedStep0(void)
{
    Func_020015aa(0);
}

void FieldScene_RunIndexedStep1(void)
{
    Func_020015b6(1);
}

void FieldScene_RunIndexedStep2(void)
{
    Func_020015c2(2);
}

void FieldScene_RunIndexedStep3(void)
{
    Func_020015ce(3);
}

void FieldScene_RunIndexedStep4(void)
{
    Func_020015da(4);
}

void FieldScene_RunIndexedStep5(void)
{
    Func_020015e6(5);
}

void FieldScene_RunTwoCallSequence(void)
{
    Func_02000e10();
    Func_02001a48();
}

void FieldScene_RunGuardedSixWordStep(void)
{
    S6 s;

    Func_02004b62();
    if (Func_0200132e(&s) != 0) {
        Func_020014da(s);
    }
    Func_02004b8e();
}

void FieldScene_RunTwoArmSequenceWithValue217f(void)
{
    s32 val;

    Func_02007170();
    val = (s32)&Value_0000217f;
    Func_02007228(val);
    Func_02007238(8, 0);
    if (Func_02007198(0, 0) == 0) {
        Func_0200718a(20);
        Func_02007248(val + 1);
        Func_02007260(8, 0);
    } else {
        Func_020071a0(20);
        Func_0200725e(val + 2);
        Func_02007276(8, 0);
    }
    Func_020071c2();
}
