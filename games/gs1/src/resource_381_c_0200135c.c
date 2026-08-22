#include "types.h"

extern s32 Data_0200bb70;
extern s32 Data_0200bb6c;
extern s32 Data_0200bb68;
extern s32 Func_02004778(void);
extern s32 Func_0200479e(void);
extern s32 Func_020047b6(void);
extern s32 Func_020047ce(void);
extern s32 Func_020047e6(void);

void Func_0200135c(void)
{
    u32 v;

    if (Data_0200bb70 != 0) {
        Data_0200bb70--;
        return;
    }
    if (Data_0200bb6c != 0) {
        Data_0200bb6c--;
    } else {
        Data_0200bb6c = (u32)(Func_02004778() << 2) >> 16;
    }
    v = Data_0200bb6c;
    switch (v) {
    case 3:
        Data_0200bb68 = v;
        Data_0200bb70 = ((u32)(Func_0200479e() * 20) >> 16) + 40;
        break;
    case 2:
        Data_0200bb68 = 15;
        Data_0200bb70 = ((u32)(Func_020047b6() * 40) >> 16) + 80;
        break;
    case 1:
        Data_0200bb68 = 63;
        Data_0200bb70 = ((u32)(Func_020047ce() * 80) >> 16) + 160;
        break;
    default:
        Data_0200bb68 = 127;
        Data_0200bb70 = ((u32)(Func_020047e6() * 160) >> 16) + 320;
        break;
    }
}
