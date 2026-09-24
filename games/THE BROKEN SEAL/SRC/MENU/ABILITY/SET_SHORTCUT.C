#include "PSYNERGY_MENU.H"
#define PARTY_STATE gGameState

s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut)
{
    s32 id = psynergy & 0x3fff;
    s32 code =
        (s32)(((u32)owner << 10) | (u32)id);

    if (shortcut == 0) {
        PARTY_STATE.psynergy_shortcuts[0] = code;
    } else {
        PARTY_STATE.psynergy_shortcuts[1] = code;
    }
    return 1;
}
