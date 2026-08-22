#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000001e;
extern u8 Value_00000023;
extern u8 Value_00000020;
extern u8 Data_02009744[];
extern u8 Data_02009a2c[];
extern u8 Data_02009bc4[];
extern u8 Data_02009738[];

s32 Func_02000198(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000001e) {
        return (s32)Data_02009744;
    }
    if (v == (s32)&Value_00000023) {
        return (s32)Data_02009a2c;
    }
    if (v == (s32)&Value_00000020) {
        return (s32)Data_02009bc4;
    }
    return (s32)Data_02009738;
}
