#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000aa;
extern u8 Value_000000ab;
extern u8 Data_02008ddc[];
extern u8 Data_02008e54[];
extern u8 Data_02008d10[];

s32 Func_020000e4(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000aa) {
        return (s32)Data_02008ddc;
    }
    if (v == (s32)&Value_000000ab) {
        return (s32)Data_02008e54;
    }
    return (s32)Data_02008d10;
}
