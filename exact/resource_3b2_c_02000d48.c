#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000071;
extern u8 Value_0000007b;
extern u8 Value_00000086;
extern u8 Value_0000007e;
extern u8 Data_0200b610[];
extern u8 Data_0200b718[];
extern u8 Data_0200b850[];
extern u8 Data_0200b5f8[];

s32 Func_02000d48(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000071) {
        return (s32)Data_0200b610;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)Data_0200b718;
    }
    if (v <= (s32)&Value_00000086 && v >= (s32)&Value_0000007e) {
        return (s32)Data_0200b850;
    }
    return (s32)Data_0200b5f8;
}
