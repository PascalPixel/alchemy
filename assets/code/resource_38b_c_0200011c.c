#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000001e;
extern u8 Value_00000023;
extern u8 Value_00000020;
extern u8 Data_02009498[];
extern u8 Data_02009600[];
extern u8 Data_020096f0[];
extern u8 Data_02009480[];

s32 Func_0200011c(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000001e) {
        return (s32)Data_02009498;
    }
    if (v == (s32)&Value_00000023) {
        return (s32)Data_02009600;
    }
    if (v == (s32)&Value_00000020) {
        return (s32)Data_020096f0;
    }
    return (s32)Data_02009480;
}
