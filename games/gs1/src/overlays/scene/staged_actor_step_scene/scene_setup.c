#include "types.h"

#define FieldScene_RunActor15ZeroStep Func_02000ec8
#define FieldScene_RunLateIndexedStep0 Func_020015cc
#define FieldScene_RunLateIndexedStep1 Func_020015d8
#define FieldScene_RunLateIndexedStep2 Func_020015e4
#define SceneState_ApplyRectAndSetSlotEightByte35 Func_02001a9c
#define FieldScene_RunTwoStepSequence Func_02001c5c
#define SceneState_ApplyRectAndClearSlotTenByte85 Func_02001ca0
#define FieldScene_RunThreeCallSequenceB Func_02001d64
#define FieldScene_RunThreeCallSequence Func_02001e60
#define FieldScene_RunThreeStepSequence Func_02001e80
void Func_020033f4(void);
void Func_02003454(s32, s32);
void Func_02003408(void);
void Func_02002afe(s32);
void Func_02002b0a(s32);
void Func_02002b16(s32);
s32 Func_02003f8a(s32, s32, s32, s32, s32, s32);
u8 *Func_02003ff8(s32);
void Func_02001d24(void);
void Func_0200388c(void);
s32 Func_0200418e(s32, s32, s32, s32, s32, s32);
u8 *Func_020041fc(s32);
void Func_020039d4(void);
void Func_02001e30(void);
void Func_02003a68(void);
void Func_02003c04(void);
void Func_02001f2c(void);
void Func_02003cc0(void);
void Func_02003c24(void);
void Func_02001f4c(void);
void Func_02003d00(void);

/* Contiguous unnamed leaf-owner run for resource_3b4. */

void FieldScene_RunActor15ZeroStep(void)
{
    Func_020033f4();
    Func_02003454(15, 0);
    Func_02003408();
}

void FieldScene_RunLateIndexedStep0(void)
{
    Func_02002afe(0);
}

void FieldScene_RunLateIndexedStep1(void)
{
    Func_02002b0a(1);
}

void FieldScene_RunLateIndexedStep2(void)
{
    Func_02002b16(2);
}

void SceneState_ApplyRectAndSetSlotEightByte35(void)
{
    s32 width = 40;
    s32 height = 42;
    u8 *entry;

    Func_02003f8a(39, 42, 1, 1, width, height);
    entry = Func_02003ff8(8) + 35;
    *entry = 2;
}

void FieldScene_RunTwoStepSequence(void)
{
    Func_02001d24();
    Func_0200388c();
}

void SceneState_ApplyRectAndClearSlotTenByte85(void)
{
    s32 width = 38;
    s32 height = 55;
    u8 *entry;

    Func_0200418e(40, 54, 1, 1, width, height);
    entry = Func_020041fc(10) + 85;
    *entry = 0;
}

void FieldScene_RunThreeCallSequenceB(void)
{
    Func_020039d4();
    Func_02001e30();
    Func_02003a68();
}

void FieldScene_RunThreeCallSequence(void)
{
    Func_02003c04();
    Func_02001f2c();
    Func_02003cc0();
}

void FieldScene_RunThreeStepSequence(void)
{
    Func_02003c24();
    Func_02001f4c();
    Func_02003d00();
}
