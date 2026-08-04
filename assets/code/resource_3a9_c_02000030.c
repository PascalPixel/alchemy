#include "resource_3a9.h"

typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000064;
extern u8 Value_00000065;
extern u8 Data_020084d0[];
extern u8 Data_020086c8[];
extern u8 Data_020084a0[];

s32 Func_02000030(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000064) {
        return (s32)Data_020084d0;
    }
    if (v == (s32)&Value_00000065) {
        return (s32)Data_020086c8;
    }
    return (s32)Data_020084a0;
}
