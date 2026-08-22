#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000039;
extern u8 Value_00000038;
extern u8 Value_00000037;
extern u8 Data_0200e50c[];
extern u8 Data_0200e35c[];
extern u8 Data_0200e23c[];
extern u8 Data_0200e0ec[];

s32 Func_02000e20(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000039) {
        return (s32)Data_0200e50c;
    }
    if (v == (s32)&Value_00000038) {
        return (s32)Data_0200e35c;
    }
    if (v == (s32)&Value_00000037) {
        return (s32)Data_0200e23c;
    }
    return (s32)Data_0200e0ec;
}
