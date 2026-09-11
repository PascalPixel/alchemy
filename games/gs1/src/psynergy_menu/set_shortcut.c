#include "scene.h"
#include "abi/psynergy_menu/set_shortcut.h"
#include "psynergy_menu.h"

s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut)
{
    s32 id = psynergy & 0x3fff;
    s32 code =
        (s32)(((u32)owner << 10) | (u32)id);

    if (shortcut == 0) {
        gCell.psynergy_shortcuts[0] = code;
    } else {
        gCell.psynergy_shortcuts[1] = code;
    }
    return 1;
}
