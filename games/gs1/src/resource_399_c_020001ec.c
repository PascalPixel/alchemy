#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000033;
extern u8 Data_0200adb8[];
extern u8 Data_0200ac80[];

s32 Func_020001ec(void) {
    if (Data_02000240[224] == (s32)&Value_00000033) {
        return (s32)Data_0200adb8;
    }
    return (s32)Data_0200ac80;
}
