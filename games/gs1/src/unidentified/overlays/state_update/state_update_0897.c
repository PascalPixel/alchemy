#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000aa;
extern u8 Value_000000ab;
extern u8 Data_02008a40[];
extern u8 Data_02008ad0[];
extern u8 Data_02008998[];

s32 Func_02000040(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000aa) {
        return (s32)Data_02008a40;
    }
    if (v == (s32)&Value_000000ab) {
        return (s32)Data_02008ad0;
    }
    return (s32)Data_02008998;
}
