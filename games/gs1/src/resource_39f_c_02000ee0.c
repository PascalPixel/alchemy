#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000044;
extern u8 Value_00000045;
extern u8 Value_00000046;
extern u8 Data_0200b0f4[];
extern u8 Data_0200b1e4[];
extern u8 Data_0200b334[];
extern u8 Data_0200b4b4[];

s32 Func_02000ee0(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000044) {
        return (s32)Data_0200b0f4;
    }
    if (v == (s32)&Value_00000045) {
        return (s32)Data_0200b1e4;
    }
    if (v == (s32)&Value_00000046) {
        return (s32)Data_0200b334;
    }
    return (s32)Data_0200b4b4;
}
