#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000073;
extern u8 Value_00000074;
extern u8 Value_00000077;
extern u8 Value_00000079;
extern u8 Value_0000007a;
extern u8 Data_0200b3a8[];
extern u8 Data_0200b438[];
extern u8 Data_0200b498[];
extern u8 Data_0200b51c[];
extern u8 Data_0200b618[];
extern u8 Data_0200b39c[];

s32 Func_02002580(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200b3a8;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200b438;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200b498;
    }
    if (v == (s32)&Value_00000079) {
        return (s32)Data_0200b51c;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200b618;
    }
    return (s32)Data_0200b39c;
}
