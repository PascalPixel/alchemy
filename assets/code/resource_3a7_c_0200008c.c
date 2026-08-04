#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_02009c80[];
extern u8 Data_02009cc0[];
extern u8 Data_02009cfc[];
extern u8 Data_02009c7c[];

s32 Func_0200008c(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000060) {
        return (s32)Data_02009c80;
    }
    if (v == (s32)&Value_00000061) {
        return (s32)Data_02009cc0;
    }
    if (v == (s32)&Value_00000062) {
        return (s32)Data_02009cfc;
    }
    return (s32)Data_02009c7c;
}
