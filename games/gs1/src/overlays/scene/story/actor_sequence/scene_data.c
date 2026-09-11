#include "types.h"

#define SceneData_SelectDataByRuntimeSelector Func_02000314
#define SceneData_ReturnZero Func_0200034c
#define SceneData_SelectTableBySelector224 Func_02000350
#define SceneData_SelectTableByScene9f Func_02000388
extern s16 Data_02000240[];
extern u8 Value_00000068;
extern u8 Value_0000009f;
extern u8 Data_02009d3c[];
extern u8 Data_02009bec[];
extern u8 Data_02009e04[];
extern u8 Data_02009dcc[];
extern u8 Data_02009f64[];
extern u8 Data_02009e14[];

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)Data_02009d3c;
    }
    return (s32)Data_02009bec;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_SelectTableBySelector224(void)
{
    s16 v = Data_02000240[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)Data_02009e04;
    }
    return (s32)Data_02009dcc;
}

s32 SceneData_SelectTableByScene9f(void)
{
    s16 v = Data_02000240[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)Data_02009f64;
    }
    return (s32)Data_02009e14;
}
