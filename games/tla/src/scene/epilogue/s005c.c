#include "types.h"

extern s8 Data_02000a34;
extern s16 Data_02000a38;
extern s16 Data_02000a2c;
extern s32 Data_02000a00[];

void Func_0200083c();

void Func_0200005c(void)
{
    if (Data_02000a34 != 0) {
        Data_02000a00[3] = Data_02000a38 << 16;
        Data_02000a00[4] = Data_02000a2c << 16;
        Func_0200083c(Data_02000a00);
    }
}
