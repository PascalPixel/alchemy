#include "types.h"
#include "scene.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || \
    defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define HAS_LOCALIZED_MENU_GUARD 1
#endif

extern u8 *gWork;

void WaitFrames(s32);

s32 Menu_RunTopSelection(void)
{
    s32 ret;
    s32 sel;
    u8 *state;

    state = gWork;
    sel = 0;

loop:
    Menu_Run();
    sel = Menu_Check(sel);
#if defined(HAS_LOCALIZED_MENU_GUARD)
    state[0xcca] = 1;
    if (*(s16 *)(state + 0xcb8) != 0) {
        Menu_unk2_4();
        WaitFrames(1);
    }
#endif
    ret = Menu_unk2();

    switch (sel) {
    case 0:
        ret = Menu_unk3();
        if (ret == 0)
            ret = 0xff;
        *(u16 *)(state + 0x17a) = ret;
        break;
    case 1:
        ret = Menu_unk4();
        if (ret == -1)
            goto loop;
        break;
    case 2:
        ret = Menu_unk5();
        if (ret != 0)
            goto loop;
        break;
    case 3:
        ret = Menu_unk6();
        if (ret == -1)
            goto loop;
        break;
    case 4:
        ret = Menu_unk7();
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
