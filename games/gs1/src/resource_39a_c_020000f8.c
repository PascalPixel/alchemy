#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000034;
extern u8 Value_0000003e;
extern u8 Value_0000003f;
extern u8 Value_00000040;
extern u8 Value_00000041;
extern u8 Value_00000043;
extern u8 Data_0200a9bc[];
extern u8 Data_0200a9ec[];
extern u8 Data_0200aa4c[];
extern u8 Data_0200aac4[];
extern u8 Data_0200ab3c[];
extern u8 Data_0200ab9c[];
extern u8 Data_0200a9a4[];

s32 Func_020000f8(void) {
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
