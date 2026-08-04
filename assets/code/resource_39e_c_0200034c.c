#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000003c;
extern u8 Data_0200c7a8[];
extern u8 Data_0200c838[];

s32 Func_0200034c(void) {
    if (Data_02000240[224] == (s32)&Value_0000003c) {
        return (s32)Data_0200c7a8;
    }
    return (s32)Data_0200c838;
}
