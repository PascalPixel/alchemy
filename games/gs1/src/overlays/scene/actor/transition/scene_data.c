#include "types.h"

#define SceneData_SelectTableBySceneId Func_02000050
#define SceneData_ReturnZero Func_020000ec
#define SceneData_GetTableA8f4 Func_020000f0
#define SceneData_SelectDataByRuntimeSelector Func_020000f8
#define SceneData_SelectDataByRuntimeSelectorB Func_02000f30
extern s16 Data_02000240[];
extern u8 Value_00000034;
extern u8 Value_0000003e;
extern u8 Value_0000003f;
extern u8 Value_00000040;
extern u8 Value_00000041;
extern u8 Value_00000042;
extern u8 Value_00000043;
extern u8 Data_0200a4bc[];
extern u8 Data_0200a504[];
extern u8 Data_0200a5f4[];
extern u8 Data_0200a63c[];
extern u8 Data_0200a6cc[];
extern u8 Data_0200a744[];
extern u8 Data_0200a7bc[];
extern u8 Data_0200a48c[];
extern u8 Data_0200a8f4;
extern u8 Data_0200a9bc[];
extern u8 Data_0200a9ec[];
extern u8 Data_0200aa4c[];
extern u8 Data_0200aac4[];
extern u8 Data_0200ab3c[];
extern u8 Data_0200ab9c[];
extern u8 Data_0200a9a4[];
extern u8 Data_0200abd8[];
extern u8 Data_0200ac08[];
extern u8 Data_0200ad1c[];
extern u8 Data_0200ae24[];
extern u8 Data_0200b058[];
extern u8 Data_0200b130[];
extern u8 Data_0200b184[];
extern u8 Data_0200abcc[];

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200a4bc;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200a504;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200a5f4;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200a63c;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200a6cc;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)Data_0200a744;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200a7bc;
    }
    return (s32)Data_0200a48c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTableA8f4(void)
{
    return &Data_0200a8f4;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200a9bc;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200a9ec;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200aa4c;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200aac4;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200ab3c;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200ab9c;
    }
    return (s32)Data_0200a9a4;
}

s32 SceneData_SelectDataByRuntimeSelectorB(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200abd8;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200ac08;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200ad1c;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200ae24;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200b058;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)Data_0200b130;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200b184;
    }
    return (s32)Data_0200abcc;
}
