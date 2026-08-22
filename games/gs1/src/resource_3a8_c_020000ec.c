#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000063;
extern u8 Value_00000066;
extern u8 Value_00000099;
extern u8 Value_0000009a;
extern u8 Value_0000009b;
extern u8 Value_0000009c;
extern u8 Data_0200c2c8[];
extern u8 Data_0200c448[];
extern u8 Data_0200c4a8[];
extern u8 Data_0200c520[];
extern u8 Data_0200c580[];
extern u8 Data_0200c628[];
extern u8 Data_0200c298[];

s32 Func_020000ec(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000063) {
        return (s32)Data_0200c2c8;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)Data_0200c448;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)Data_0200c4a8;
    }
    if (v == (s32)&Value_0000009a) {
        return (s32)Data_0200c520;
    }
    if (v == (s32)&Value_0000009b) {
        return (s32)Data_0200c580;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)Data_0200c628;
    }
    return (s32)Data_0200c298;
}
