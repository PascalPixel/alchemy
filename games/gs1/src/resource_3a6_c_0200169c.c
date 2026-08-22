#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000005d;
extern u8 Value_0000005e;
extern u8 Value_0000005f;
extern u8 Data_0200a420[];
extern u8 Data_0200a450[];
extern u8 Data_0200a624[];
extern u8 Data_0200a414[];

s32 Func_0200169c(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000005d) {
        return (s32)Data_0200a420;
    }
    if (v == (s32)&Value_0000005e) {
        return (s32)Data_0200a450;
    }
    if (v == (s32)&Value_0000005f) {
        return (s32)Data_0200a624;
    }
    return (s32)Data_0200a414;
}
