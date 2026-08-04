#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000a0;
extern u8 Value_000000a1;
extern u8 Value_000000a2;
extern u8 Data_0200eff4[];
extern u8 Data_0200f258[];
extern u8 Data_0200f528[];
extern u8 Data_0200f63c[];

s32 Func_02000af8(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000a0) {
        return (s32)Data_0200eff4;
    }
    if (v == (s32)&Value_000000a1) {
        return (s32)Data_0200f258;
    }
    if (v == (s32)&Value_000000a2) {
        return (s32)Data_0200f528;
    }
    return (s32)Data_0200f63c;
}
