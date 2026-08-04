#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000b0;
extern u8 Value_000000af;
extern u8 Value_000000ae;
extern u8 Data_0200b694[];
extern u8 Data_0200b754[];
extern u8 Data_0200b784[];
extern u8 Data_0200b88c[];

s32 Func_02000fdc(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000b0) {
        return (s32)Data_0200b694;
    }
    if (v == (s32)&Value_000000af) {
        return (s32)Data_0200b754;
    }
    if (v == (s32)&Value_000000ae) {
        return (s32)Data_0200b784;
    }
    return (s32)Data_0200b88c;
}
