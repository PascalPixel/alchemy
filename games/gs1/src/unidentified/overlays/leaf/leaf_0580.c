#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000033;
extern u8 Data_0200a8a0[];
extern u8 Data_0200a798[];

s32 Func_02000130(void) {
    if (Data_02000240[224] == (s32)&Value_00000033) {
        return (s32)Data_0200a8a0;
    }
    return (s32)Data_0200a798;
}
