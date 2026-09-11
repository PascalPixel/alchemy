#include "types.h"

#define FieldScene_RunBranchedSteps1FF1 Func_020000c8
#define FieldScene_RunBranchedSteps2006 Func_02000348
#define FieldScene_RunStepWithValue29e0 Func_0200400c
#define SceneData_SelectTableD004ByStateAndFlags Func_02004034
extern u8 Value_00001ff1;
extern u8 Value_00002006;
extern u8 Value_00000105;
extern s16 Data_02000240[];
extern u8 Value_0000008b;
extern u8 Data_0200dad8[];
extern u8 Data_0200da48[];
extern u8 Data_0200d9e8[];
extern u8 Data_0200d688[];
extern u8 Data_0200d394[];
extern u8 Data_0200d004[];

void Func_0200456a(s32);
s32 Func_0200457a(s32, s32);
s32 Func_020044da(s32, s32);
void Func_02004584(s32);
void Func_0200458c(s32);
s32 Func_020045a4(s32, s32);
void Func_020047ea(s32);
s32 Func_020047fa(s32, s32);
s32 Func_0200475a(s32, s32);
void Func_02004744(s32);
void Func_02004830(s32, s32, s32);
void Func_02004816(s32);
void Func_0200475e(s32);
void Func_02004848(s32, s32, s32);
void Func_0200482e(s32);
void Func_02004846(s32, s32);
void Func_020083f0(void);
void Func_02008398(s32, s32);
void Func_020083a0(s32, s32);
void Func_0200840c(void);
s32 Func_020083ee(s32);
s32 Func_020083fc(s32);
s32 Func_02008410(s32);
s32 Func_0200841e(s32);

/* Contiguous unnamed leaf-owner run for resource_3b8. */

void FieldScene_RunBranchedSteps1FF1(s32 a)
{
    s32 k = (s32)&Value_00001ff1;

    Func_0200456a(k);
    Func_0200457a(a, 0);
    if (Func_020044da(0, 0) == 0)
        Func_02004584(k + 1);
    else
        Func_0200458c(k + 2);
    Func_020045a4(a, 0);
}

void FieldScene_RunBranchedSteps2006(s32 a)
{
    s32 k = (s32)&Value_00002006;

    Func_020047ea(k);
    Func_020047fa(a, 0);
    if (Func_0200475a(0, 0) == 0) {
        Func_02004744(10);
        Func_02004830(a, 0x102, 0x28);
        Func_02004816(k + 1);
    } else {
        Func_0200475e(10);
        Func_02004848(a, (s32)&Value_00000105, 0x28);
        Func_0200482e(k + 2);
    }
    Func_02004846(a, 0);
}

void FieldScene_RunStepWithValue29e0(void)
{
    Func_020083f0();
    Func_02008398(0x947, 1);
    Func_020083a0(0x29e0, 1);
    Func_0200840c();
}

s32 SceneData_SelectTableD004ByStateAndFlags(void)
{
    if (Data_02000240[224] == (s32)&Value_0000008b) {
        if (Func_020083ee(0x950) != 0) {
            return (s32)Data_0200dad8;
        }
        if (Func_020083fc(0x962) != 0) {
            return (s32)Data_0200da48;
        }
        return (s32)Data_0200d9e8;
    }
    if (Func_02008410(0x950) != 0) {
        return (s32)Data_0200d688;
    }
    if (Func_0200841e(0x962) != 0) {
        return (s32)Data_0200d394;
    }
    return (s32)Data_0200d004;
}
