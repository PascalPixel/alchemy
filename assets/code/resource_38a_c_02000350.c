#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000001d;
extern u8 Data_02008978[];
extern u8 Data_02008948[];

s32 Func_02000350(void) {
    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_02008978;
    }
    return (s32)Data_02008948;
}
