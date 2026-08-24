#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000021;
extern u8 Data_0200aca8[];
extern u8 Data_0200ac9c[];

s32 Func_02000100(void) {
    if (Data_02000240[224] == (s32)&Value_00000021) {
        return (s32)Data_0200aca8;
    }
    return (s32)Data_0200ac9c;
}
