/* Contiguous unnamed leaf-owner run for resource_38d. */

#include "types.h"

extern void Func_02002860(void);
extern void Func_02002840(s32, s32);
extern void Func_02002874(void);

void FieldScene_RunStepWithValue174d(void)
{
    Func_02002860();
    Func_02002840(0x174D, 1);
    Func_02002874();
}

#include "types.h"

extern void Func_0200287c(void);
extern void Func_0200285c(s32, s32);
extern void Func_02002890(void);

void FieldScene_RunStepWithValue174e(void)
{
    Func_0200287c();
    Func_0200285c(0x174E, 1);
    Func_02002890();
}

#include "types.h"

extern s32 Func_02002878(void);
extern void Func_0200081c(void);
extern void Func_0200083e(void);

void FieldScene_RunBranchedStep(void)
{
    if (Func_02002878() != 0) {
        Func_0200081c();
    } else {
        Func_0200083e();
    }
}

#include "types.h"

extern s32 Func_02002892(s32);
extern void Func_02000872(s32);
extern s32 Func_02000948(s32, s32, s32);

void FieldScene_RunStep210ByFlag84e(void)
{
    if (Func_02002892(0x84E) != 0) {
        Func_02000872(0x210);
    } else {
        Func_02000948(21, 182, 0x210);
    }
}

#include "types.h"

extern s32 Func_020028be(s32);
extern void Func_0200089c(s32);
extern s32 Func_02000970(s32, s32, s32);

void FieldScene_RunStep211ByFlag84e(void)
{
    if (Func_020028be(0x84E) != 0) {
        Func_0200089c(0x211);
    } else {
        Func_02000970(22, 183, 0x211);
    }
}

#include "types.h"

extern s32 Func_020028ea(s32);
extern void Func_020008c8(s32);
extern s32 Func_0200099c(s32, s32, s32);

void FieldScene_RunStep212ByFlag84e(void)
{
    if (Func_020028ea(0x84E) != 0) {
        Func_020008c8(0x212);
    } else {
        Func_0200099c(23, 186, 0x212);
    }
}

#include "types.h"

extern s32 Func_02002916(s32);
extern void Func_020008f4(s32);
extern s32 Func_020009c8(s32, s32, s32);

void FieldScene_RunStep213ByFlag84e(void)
{
    if (Func_02002916(0x84E) != 0) {
        Func_020008f4(0x213);
    } else {
        Func_020009c8(24, 189, 0x213);
    }
}
