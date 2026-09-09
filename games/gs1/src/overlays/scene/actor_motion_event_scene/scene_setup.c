#include "types.h"

#define FieldScene_SetupDescriptor9740 Func_02000ac8
#define FieldScene_SetupWithDescriptor976C Func_02000b18
#define FieldScene_SetupWithDescriptor97AE Func_02000bf8
#define SceneState_SetFlag200AndConfigureRegion55_26 Func_02000d24
/* Complete scene-variant five actor-zero setup wrapper through its pool. */
extern const u8 Data_02009740[];
/* Complete scene-variant seven actor-zero setup wrapper through its pool. */
extern const u8 Data_0200976c[];
/* Complete scene-variant ten actor-zero setup wrapper through its pool. */
extern const u8 Data_020097ae[];

void Func_02001c7e(s32 value);
void Func_02001b60(const void *tbl, s32 value, s32 actor);
void Func_02001566(s32 x, s32 y, s32 value);
void Func_02001cce(s32 value);
void Func_02001bb0(const void *descriptor, s32 value, s32 actor);
void Func_020015b4(s32 x, s32 y, s32 value);
void Func_02001dae(s32 value);
void Func_02001c90(const void *descriptor, s32 value, s32 actor);
void Func_02001692(s32 x, s32 y, s32 value);
void Func_02001df6(s32 flag);
void Func_02001dda(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 a, s32 b);

void FieldScene_SetupDescriptor9740(void)
{
    Func_02001c7e(158);
    Func_02001b60(Data_02009740, 56, 19);
    Func_02001566(408, 320, 5);
}

void FieldScene_SetupWithDescriptor976C(void)
{
    Func_02001cce(158);
    Func_02001bb0(Data_0200976c, 44, 17);
    Func_020015b4(216, 288, 7);
}

void FieldScene_SetupWithDescriptor97AE(void)
{
    Func_02001dae(158);
    Func_02001c90(Data_020097ae, 38, 6);
    Func_02001692(120, 144, 10);
}

void SceneState_SetFlag200AndConfigureRegion55_26(void)
{
    Func_02001df6(0x200);
    {
        s32 a = 23;
        s32 b = 26;
        Func_02001dda(55, 26, 4, 2, a, b);
    }
}
