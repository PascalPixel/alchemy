#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000bd;
extern u8 Data_02009aec[];
extern u8 Data_02009cfc[];

s32 Func_02000044(void) {
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009aec;
    }
    return (s32)Data_02009cfc;
}
