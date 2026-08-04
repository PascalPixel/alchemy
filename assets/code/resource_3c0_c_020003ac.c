#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000a4;
extern u8 Value_000000a5;
extern u8 Value_000000a6;
extern u8 Data_02009610[];
extern u8 Data_020096b8[];
extern u8 Data_02009790[];
extern u8 Data_020095f8[];

s32 Func_020003ac(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000a4) {
        return (s32)Data_02009610;
    }
    if (v == (s32)&Value_000000a5) {
        return (s32)Data_020096b8;
    }
    if (v == (s32)&Value_000000a6) {
        return (s32)Data_02009790;
    }
    return (s32)Data_020095f8;
}
