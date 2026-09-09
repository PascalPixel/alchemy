#include "types.h"

#define NULL ((void *)0)
#define FieldScene_CallWhenCheck9_31_9 Func_02001574
#define ConfigureSceneAndCheckActors Func_020015d4
#define FieldScene_ApplyRect13_31_12_30_12 Func_02001a24
#define FieldScene_ApplyRect10_14_7_13_7 Func_02001a64
#define FieldScene_ApplyRect12_21_7_22_7 Func_02001ad4

s32 Func_02003166(s32, s32, s32);
void Func_0200319a(void);
void Func_02003a08(s32, s32, s32, s32);
s32 Func_020031d6();
void Func_0200383a();
s32 Func_020031fe();
void Func_02003912();
void Func_02003a30();
void Func_0200357a(s32, s32, s32, s32, s32);
void Func_0200384a(void);
void Func_020035ba(s32, s32, s32, s32, s32);
void Func_02003cbe(void);
void Func_0200362a(s32, s32, s32, s32, s32);
void Func_02003dde(void);

static __inline__ void ConfigureScene(s32 actor, s32 x, s32 y, s32 mode)
{
    Func_02003a08(actor, x, y, mode);
}

static __inline__ void ConfigureScene_02003a30(s32 actor, s32 x, s32 y, s32 mode)
{
    Func_02003a30(actor, x, y, mode);
}

void FieldScene_CallWhenCheck9_31_9(void)
{
    if (Func_02003166(9, 31, 9) != 0) {
        Func_0200319a();
    }
}

void ConfigureSceneAndCheckActors(void)
{
    ConfigureScene(2, 0x00d00000, 0x00700000, 0);
    if (Func_020031d6(10, 14, 7) != 0) {
        Func_0200383a();
    }
}

void Func_020015fc(void)
{
    ConfigureScene_02003a30(2, 23068672, 7340032, 0);
    if (Func_020031fe(12, 21, 7) != 0) {
        Func_02003912();
    }
}

void FieldScene_ApplyRect13_31_12_30_12(void)
{
    Func_0200357a(13, 31, 12, 30, 12);
    Func_0200384a();
}

void FieldScene_ApplyRect10_14_7_13_7(void)
{
    Func_020035ba(10, 14, 7, 13, 7);
    Func_02003cbe();
}

void FieldScene_ApplyRect12_21_7_22_7(void)
{
    Func_0200362a(12, 21, 7, 22, 7);
    Func_02003dde();
}
