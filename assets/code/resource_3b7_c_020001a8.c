#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000bd;
extern u8 Data_02009f30[];
extern u8 Data_02009e1c[];

s32 Func_020001a8(void) {
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009f30;
    }
    return (s32)Data_02009e1c;
}
