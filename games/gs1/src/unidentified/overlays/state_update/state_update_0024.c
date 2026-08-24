#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_02009d34[];
extern u8 Data_02009d4c[];
extern u8 Data_02009ecc[];
extern u8 Data_02009d1c[];

s32 Func_020000e0(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000060) {
        return (s32)Data_02009d34;
    }
    if (v == (s32)&Value_00000061) {
        return (s32)Data_02009d4c;
    }
    if (v == (s32)&Value_00000062) {
        return (s32)Data_02009ecc;
    }
    return (s32)Data_02009d1c;
}
