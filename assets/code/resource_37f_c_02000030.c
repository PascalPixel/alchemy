#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000013;
extern u8 Value_00000010;
extern u8 Data_02009d04[];
extern u8 Data_02009d64[];
extern u8 Data_02009cd4[];

s32 Func_02000030(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000013) {
        return (s32)Data_02009d04;
    }
    if (v == (s32)&Value_00000010) {
        return (s32)Data_02009d64;
    }
    return (s32)Data_02009cd4;
}
