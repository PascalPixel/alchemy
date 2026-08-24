#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000067;
extern u8 Data_02009c04[];
extern u8 Data_02009bd4[];

s32 Func_02000030(void) {
    if (Data_02000240[224] == (s32)&Value_00000067) {
        return (s32)Data_02009c04;
    }
    return (s32)Data_02009bd4;
}
