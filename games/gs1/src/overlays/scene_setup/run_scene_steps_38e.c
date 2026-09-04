/* Contiguous unnamed leaf-owner run for resource_38e. */

#include "types.h"

extern void Func_02000bd0(void);
extern s32 Func_02000c2a(s32, s32, s32);
extern void Func_02000bc8(s32);
extern void Func_02000cd8(s32, s32);
extern s32 Func_02000c00(s32, s32);
extern void Func_02000bfc(void);

#define FieldScene_RunStepWithValueFd2 Func_020000f8

void FieldScene_RunStepWithValueFd2(void)
{
    Func_02000bd0();
    Func_02000c2a(0xD, 0, 0);
    Func_02000bc8(0xFD2);
    Func_02000cd8(0xB5, 3);
    Func_02000c00(0xB5, 0);
    Func_02000bfc();
}

#include "types.h"

extern void Func_02000c04(void);
extern void Func_02000bdc(s32, s32);
extern void Func_02000be4(s32, s32);
extern void Func_02000c20(void);

#define FieldScene_RunStepWithValue29de Func_0200012c

void FieldScene_RunStepWithValue29de(void)
{
    Func_02000c04();
    Func_02000bdc(0x947, 1);
    Func_02000be4(0x29DE, 1);
    Func_02000c20();
}

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000022;
extern u8 Data_02008d30[];
extern u8 Data_02008d24[];

#define SceneData_SelectTable8d24ByState Func_02000154

s32 SceneData_SelectTable8d24ByState(void) {
    if (Data_02000240[224] == (s32)&Value_00000022) {
        return (s32)Data_02008d30;
    }
    return (s32)Data_02008d24;
}

#include "types.h"

#define SceneDialogue_RunActor11Message1751 Func_020001c4

#define SceneDialogue_RunActor10Message13c3 Func_020001a4

#define SceneDialogue_RunActor9Message13c0 Func_02000184

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

#include "types.h"

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

#include "types.h"

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
