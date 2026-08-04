#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000001e;
extern u8 Value_00000023;
extern u8 Value_00000020;
extern u8 Data_020091ec[];
extern u8 Data_0200930c[];
extern u8 Data_0200936c[];
extern u8 Data_020091d4[];

s32 Func_02000088(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000001e) {
        return (s32)Data_020091ec;
    }
    if (v == (s32)&Value_00000023) {
        return (s32)Data_0200930c;
    }
    if (v == (s32)&Value_00000020) {
        return (s32)Data_0200936c;
    }
    return (s32)Data_020091d4;
}
