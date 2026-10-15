#include "garoh.h"

extern s32 Data_0200632c;

void Func_02000a68(void)
{
    if (!Flag_IsSet(0x91b) && Flag_IsSet(0x310)) {
        Data_0200632c++;
        if ((Data_0200632c & 0x3ff) == 0)
            Func_02005394(0x20d);
    }
}
