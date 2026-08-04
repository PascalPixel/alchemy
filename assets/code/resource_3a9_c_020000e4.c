#include "resource_3a9.h"

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000064;
extern u8 Value_00000065;
extern u8 Data_02008c88[];
extern u8 Data_02008a48[];
extern u8 Data_02008eb0[];
extern u8 Data_02008a3c[];

s32 Func_020000e4(void) {
    s16 v = Data_02000240[224];
    s32 w;

    if (v == (s32)&Value_00000064) {
        w = Data_02000240[225];
        if (!(w < 9) && (w <= 15 || w == 17)) {
            return (s32)Data_02008c88;
        }
        return (s32)Data_02008a48;
    }
    if (v == (s32)&Value_00000065) {
        return (s32)Data_02008eb0;
    }
    return (s32)Data_02008a3c;
}
