#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000003c;
extern u8 Data_0200cb90[];
extern u8 Data_0200d184[];
extern u8 Data_0200cd40[];

s32 Func_02002574(void) {
    if (Data_02000240[224] == (s32)&Value_0000003c) {
        return (s32)Data_0200cb90;
    }
    if (Data_02000240[225] == 3) {
        return (s32)Data_0200d184;
    }
    return (s32)Data_0200cd40;
}
