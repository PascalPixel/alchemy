#include "types.h"

extern u8 *Data_03001ebc;

void Func_0801c2d0(void);
s32 Func_08028920(s32);
s32 Func_0801c2e4(void);
s32 Func_0808a280(void);
s32 Func_080a1000(void);
s32 Func_080a1008(void);
s32 Func_080a1010(void);
s32 Func_080a1040(void);

s32 Func_0801c244(void)
{
    s32 result;
    s32 selection;
    u8 *state;

    state = Data_03001ebc;
    selection = 0;

loop:
    Func_0801c2d0();
    selection = Func_08028920(selection);
    result = Func_0801c2e4();

    switch (selection) {
    case 0:
        result = Func_0808a280();
        if (result == 0)
            result = 0xff;
        *(u16 *)(state + 0x17a) = result;
        break;
    case 1:
        result = Func_080a1008();
        if (result == -1)
            goto loop;
        break;
    case 2:
        result = Func_080a1040();
        if (result != 0)
            goto loop;
        break;
    case 3:
        result = Func_080a1000();
        if (result == -1)
            goto loop;
        break;
    case 4:
        result = Func_080a1010();
        if (result == -1)
            goto loop;
        break;
    default:
        break;
    }

    return result;
}
