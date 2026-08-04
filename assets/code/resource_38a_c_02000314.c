#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000001d;
extern u8 Data_020088d8[];
extern u8 Data_02008818[];

s32 Func_02000314(void) {
    if (Data_02000240[224] == (s32)&Value_0000001d) {
        return (s32)Data_020088d8;
    }
    return (s32)Data_02008818;
}
