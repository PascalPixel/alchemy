#include "types.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || \
    defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define HAS_LOCALIZED_MENU_GUARD 1
#endif

extern u8 *Data_03001ebc;

void Func_0801c2d0(void);
s32 Func_08028920(s32);
s32 Func_0801c2e4(void);
s32 Func_0808a280(void);
s32 Func_080a1000(void);
s32 Func_080a1008(void);
s32 Func_080a1010(void);
s32 Func_080a1040(void);
void WaitFrames(s32);
void Func_0808a5f8(void);

s32 Menu_RunTopSelection(void)
{
    s32 ret;
    s32 sel;
    u8 *state;

    state = Data_03001ebc;
    sel = 0;

loop:
    Func_0801c2d0();
    sel = Func_08028920(sel);
#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 1;
    if (*(s16 *)(state + 0xcb8) != 0) {
        Func_0808a5f8();
        WaitFrames(1);
    }
#endif
    ret = Func_0801c2e4();

    switch (sel) {
    case 0:
        ret = Func_0808a280();
        if (ret == 0)
            ret = 0xff;
        *(u16 *)(state + 0x17a) = ret;
        break;
    case 1:
        ret = Func_080a1008();
        if (ret == -1)
            goto loop;
        break;
    case 2:
        ret = Func_080a1040();
        if (ret != 0)
            goto loop;
        break;
    case 3:
        ret = Func_080a1000();
        if (ret == -1)
            goto loop;
        break;
    case 4:
        ret = Func_080a1010();
        if (ret == -1)
            goto loop;
        break;
    default:
        break;
    }

#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 0;
#endif
    return ret;
}
