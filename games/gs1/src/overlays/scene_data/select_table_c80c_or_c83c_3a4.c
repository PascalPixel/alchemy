#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000055;
extern u8 Value_00000056;
extern u8 Data_0200c80c[];
extern u8 Data_0200c83c[];

s32 SceneData_SelectTableC80cOrC83c(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c80c;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c83c;
    }
    return 0;
}
