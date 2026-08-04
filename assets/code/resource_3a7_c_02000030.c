#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_020098cc[];
extern u8 Data_02009a34[];
extern u8 Data_02009b9c[];
extern u8 Data_0200989c[];

s32 Func_02000030(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000060) {
        return (s32)Data_020098cc;
    }
    if (v == (s32)&Value_00000061) {
        return (s32)Data_02009a34;
    }
    if (v == (s32)&Value_00000062) {
        return (s32)Data_02009b9c;
    }
    return (s32)Data_0200989c;
}
