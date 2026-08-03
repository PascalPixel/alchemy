typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000027;
extern u8 Value_00000026;
extern u8 Data_0200af80[];
extern u8 Data_0200afc8[];
extern u8 Data_0200ae60[];

s32 Func_0200016c(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000027) {
        return (s32)Data_0200af80;
    }
    if (v == (s32)&Value_00000026) {
        return (s32)Data_0200afc8;
    }
    return (s32)Data_0200ae60;
}
#include "resource_38f.h"
