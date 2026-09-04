#include "types.h"

#define SceneData_SelectTableByScene9f Func_02000388

extern s16 Data_02000240[];
extern u8 Value_00000068;
extern u8 Value_0000009f;
extern u8 Data_02009f64[];
extern u8 Data_02009e14[];

s32 SceneData_SelectTableByScene9f(void) {
    s16 v = Data_02000240[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)Data_02009f64;
    }
    return (s32)Data_02009e14;
}
