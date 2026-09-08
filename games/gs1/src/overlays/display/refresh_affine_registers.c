#include "types.h"

extern u16 Data_04000006[];
extern u32 Data_03001ad4[];
extern u32 Data_04000014[];
extern u32 Data_0200b738[];
extern u32 Data_0200b72c[];
extern u32 Data_04000018[];

s32 Func_02003fd8(void);

void OverlayDisplay_RefreshAffineRegisters(void)
{
    u16 vcount = Data_04000006[0];
    u32 *src = Data_03001ad4;
    u32 *dst = Data_04000014;

    if (vcount == 227 || vcount <= 46) {
        s32 rec = Func_02003fd8();

        if (((u32)(100 * rec) >> 16) < Data_0200b738[0])
            src = Data_0200b72c;
    }
    *dst = *src++;
    dst = Data_04000018;
    *dst++ = *src++;
    *dst = *src;
}
