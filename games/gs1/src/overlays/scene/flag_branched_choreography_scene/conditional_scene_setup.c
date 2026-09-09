#include "types.h"

#define FieldScene_RunStepWithValue174d Func_020003ec
#define FieldScene_RunStepWithValue174e Func_02000408
#define FieldScene_RunBranchedStep Func_02000424
#define FieldScene_RunStep210ByFlag84e Func_0200043c
#define FieldScene_RunStep211ByFlag84e Func_02000468
#define FieldScene_RunStep212ByFlag84e Func_02000494
#define FieldScene_RunStep213ByFlag84e Func_020004c0
void Func_02002860(void);
void Func_02002840(s32, s32);
void Func_02002874(void);
void Func_0200287c(void);
void Func_0200285c(s32, s32);
void Func_02002890(void);
s32 Func_02002878(void);
void Func_0200081c(void);
void Func_0200083e(void);
s32 Func_02002892(s32);
void Func_02000872(s32);
s32 Func_02000948(s32, s32, s32);
s32 Func_020028be(s32);
void Func_0200089c(s32);
s32 Func_02000970(s32, s32, s32);
s32 Func_020028ea(s32);
void Func_020008c8(s32);
s32 Func_0200099c(s32, s32, s32);
s32 Func_02002916(s32);
void Func_020008f4(s32);
s32 Func_020009c8(s32, s32, s32);

void FieldScene_RunStepWithValue174d(void)
{
    Func_02002860();
    Func_02002840(0x174D, 1);
    Func_02002874();
}

void FieldScene_RunStepWithValue174e(void)
{
    Func_0200287c();
    Func_0200285c(0x174E, 1);
    Func_02002890();
}

void FieldScene_RunBranchedStep(void)
{
    if (Func_02002878() != 0) {
        Func_0200081c();
    } else {
        Func_0200083e();
    }
}

void FieldScene_RunStep210ByFlag84e(void)
{
    if (Func_02002892(0x84E) != 0) {
        Func_02000872(0x210);
    } else {
        Func_02000948(21, 182, 0x210);
    }
}

void FieldScene_RunStep211ByFlag84e(void)
{
    if (Func_020028be(0x84E) != 0) {
        Func_0200089c(0x211);
    } else {
        Func_02000970(22, 183, 0x211);
    }
}

void FieldScene_RunStep212ByFlag84e(void)
{
    if (Func_020028ea(0x84E) != 0) {
        Func_020008c8(0x212);
    } else {
        Func_0200099c(23, 186, 0x212);
    }
}

void FieldScene_RunStep213ByFlag84e(void)
{
    if (Func_02002916(0x84E) != 0) {
        Func_020008f4(0x213);
    } else {
        Func_020009c8(24, 189, 0x213);
    }
}
