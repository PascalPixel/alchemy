#include "types.h"

#define SceneData_SelectTableByWord224B Func_020001ec

extern s16 Data_02000240[];
extern u8 Value_00000033;
extern u8 Data_0200adb8[];
extern u8 Data_0200ac80[];

s32 SceneData_SelectTableByWord224B(void) {
    if (Data_02000240[224] == (s32)&Value_00000033) {
        return (s32)Data_0200adb8;
    }
    return (s32)Data_0200ac80;
}
