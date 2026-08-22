#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000a4;
extern u8 Value_000000a5;
extern u8 Value_000000a6;
extern u8 Data_02009488[];
extern u8 Data_020094d0[];
extern u8 Data_02009548[];
extern u8 Data_02009458[];

s32 Func_0200034c(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000a4) {
        return (s32)Data_02009488;
    }
    if (v == (s32)&Value_000000a5) {
        return (s32)Data_020094d0;
    }
    if (v == (s32)&Value_000000a6) {
        return (s32)Data_02009548;
    }
    return (s32)Data_02009458;
}
