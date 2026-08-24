#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000038;
extern u8 Data_0200ead8[];
extern u8 Data_0200ec10[];
extern u8 Data_0200ed60[];
extern u8 Data_0200eec8[];

s32 Func_02000f30(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000036) {
        return (s32)Data_0200ead8;
    }
    if (v == (s32)&Value_00000037) {
        return (s32)Data_0200ec10;
    }
    if (v == (s32)&Value_00000038) {
        return (s32)Data_0200ed60;
    }
    return (s32)Data_0200eec8;
}
