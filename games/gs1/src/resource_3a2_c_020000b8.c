#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000004a;
extern u8 Data_02009a38[];
extern u8 Data_02009918[];

s32 Func_020000b8(void) {
    if (Data_02000240[224] == (s32)&Value_0000004a) {
        return (s32)Data_02009a38;
    }
    return (s32)Data_02009918;
}
