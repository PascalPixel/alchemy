#include "psynergy_menu.h"

s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut)
{
    s32 masked_psynergy = psynergy & 0x3fff;
    s32 encoded_shortcut =
        (s32)(((u32)owner << 10) | (u32)masked_psynergy);

    if (shortcut == 0) {
        Data_02000240.psynergy_shortcuts[0] = encoded_shortcut;
    } else {
        Data_02000240.psynergy_shortcuts[1] = encoded_shortcut;
    }
    return 1;
}
