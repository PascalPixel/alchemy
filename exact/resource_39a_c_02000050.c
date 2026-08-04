#include "types.h"

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

s32 Func_02000050(void) {
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
