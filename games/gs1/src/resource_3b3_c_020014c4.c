#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000073;
extern u8 Value_00000074;
extern u8 Value_00000077;
extern u8 Value_0000007a;
extern u8 Data_0200b06c[];
extern u8 Data_0200b0e4[];
extern u8 Data_0200b174[];
extern u8 Data_0200b2dc[];
extern u8 Data_0200b264[];

s32 Func_020014c4(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200b06c;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200b0e4;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200b174;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200b2dc;
    }
    return (s32)Data_0200b264;
}
