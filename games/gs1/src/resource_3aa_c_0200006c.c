#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000067;
extern u8 Data_02009df4[];
extern u8 Data_02009ddc[];

extern void Func_02001b1a();

s32 Func_0200006c(void) {
    if (Data_02000240[224] == (s32)&Value_00000067) {
        Func_02001b1a(Data_02009df4);
        return (s32)Data_02009df4;
    }
    return (s32)Data_02009ddc;
}
