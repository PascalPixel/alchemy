#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000022;
extern u8 Data_02008c7c[];
extern u8 Data_02008c64[];
s32 Func_02000b68(s32);
s32 Func_02000b7a(s32);

u8 *SceneData_SelectRecordByScene22(void)
{
    if (Data_02000240[224] == (s32)&Value_00000022) {
        if (Func_02000b68(0x84f) != 0)
            Data_02008c7c[118] = 1;
        if (Func_02000b7a(0x845) != 0)
            Data_02008c7c[70] = 0;
        return Data_02008c7c;
    }
    return Data_02008c64;
}

extern void Func_02000bd0(void);
extern s32 Func_02000c2a(s32, s32, s32);
extern void Func_02000bc8(s32);
extern void Func_02000cd8(s32, s32);
extern s32 Func_02000c00(s32, s32);
extern void Func_02000bfc(void);

void FieldScene_RunStepWithValueFd2(void)
{
    Func_02000bd0();
    Func_02000c2a(0xD, 0, 0);
    Func_02000bc8(0xFD2);
    Func_02000cd8(0xB5, 3);
    Func_02000c00(0xB5, 0);
    Func_02000bfc();
}

extern void Func_02000c04(void);
extern void Func_02000bdc(s32, s32);
extern void Func_02000be4(s32, s32);
extern void Func_02000c20(void);

void FieldScene_RunStepWithValue29de(void)
{
    Func_02000c04();
    Func_02000bdc(0x947, 1);
    Func_02000be4(0x29DE, 1);
    Func_02000c20();
}

extern u8 Data_02008d30[];
extern u8 Data_02008d24[];

s32 SceneData_SelectTable8d24ByState(void) {
    if (Data_02000240[224] == (s32)&Value_00000022) {
        return (s32)Data_02008d30;
    }
    return (s32)Data_02008d24;
}

extern u8 Value_000013c0;

extern void Func_02000c5c(void);
extern void Func_02000ce2(s32);
extern s32 Func_02000d0a(s32, s32);
extern void Func_02000c76(void);

void SceneDialogue_RunActor9Message13c0(void)
{
    Func_02000c5c();
    Func_02000ce2((s32)&Value_000013c0);
    Func_02000d0a(9, 0);
    Func_02000c76();
}

extern void Func_02000c7c(void);
extern void Func_02000d02(s32);
extern s32 Func_02000d2a(s32, s32);
extern void Func_02000c96(void);

void SceneDialogue_RunActor10Message13c3(void)
{
    Func_02000c7c();
    Func_02000d02(0x13C3);
    Func_02000d2a(10, 0);
    Func_02000c96();
}

extern void Func_02000c9c(void);
extern void Func_02000d22(s32);
extern s32 Func_02000d4a(s32, s32);
extern void Func_02000cb6(void);

void SceneDialogue_RunActor11Message1751(void)
{
    Func_02000c9c();
    Func_02000d22(0x1751);
    Func_02000d4a(11, 0);
    Func_02000cb6();
}
