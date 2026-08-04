#include "resource_3a9.h"

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000064;
extern u8 Value_00000065;
extern u8 Data_020088d4[];
extern u8 Data_0200879c[];
extern u8 Data_02008a0c[];
extern u8 Data_02008784[];

extern void Func_020004d0(s32);

s32 Func_0200007c(void) {
    s16 v = Data_02000240[224];
    s32 w;
    s32 table;

    if (v == (s32)&Value_00000064) {
        w = Data_02000240[225];
        if (!(w < 9) && (w <= 15 || w == 17)) {
            table = (s32)Data_020088d4;
        } else {
            table = (s32)Data_0200879c;
        }
        Func_020004d0(table);
        return table;
    }
    if (v == (s32)&Value_00000065) {
        return (s32)Data_02008a0c;
    }
    return (s32)Data_02008784;
}
