#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000044;
extern u8 Value_00000045;
extern u8 Value_00000046;
extern u8 Data_0200b6a0[];
extern u8 Data_0200b790[];
extern u8 Data_0200b8b0[];
extern u8 Data_0200ba30[];

s32 Func_02000f40(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000044) {
        return (s32)Data_0200b6a0;
    }
    if (v == (s32)&Value_00000045) {
        return (s32)Data_0200b790;
    }
    if (v == (s32)&Value_00000046) {
        return (s32)Data_0200b8b0;
    }
    return (s32)Data_0200ba30;
}
