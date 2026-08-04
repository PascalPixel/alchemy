#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000022;
extern u8 Data_02008d30[];
extern u8 Data_02008d24[];

s32 Func_02000154(void) {
    if (Data_02000240[224] == (s32)&Value_00000022) {
        return (s32)Data_02008d30;
    }
    return (s32)Data_02008d24;
}
