/* Matches the five international editions (differ 0). Japanese differs: no
 * yes/no prompt, and its three messages (8787, 7788, 6671) are literal-pool
 * words in the listing but have no name in the Japanese catalog, so this
 * draft builds them as split constants there (differ 28). Adopt once the
 * Japanese catalog names them. */
#include "../../../../games/tla/src/field/overlay_659/ov659.h"

void Func_020000fc(void)
{
    u16 facing = Object_GetById(gPartyState.current_owner)->angle;

    /* FAKEMATCH: keeps the quadrant offset after the facing load. */
    __asm__("");

    if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
        Func_02001f08(1, 22);
    } else {
#ifdef TLA_EDITION_JA
        // 日本語版は選択肢なし
        if (FlagChk(0x8ff))
            Func_02001e18(0x2253);
        else if (FlagChk(0x8ab))
            Func_02001e18(0x1e6c);
        else
            Func_02001e18(0x1a0f);
        Func_02001e30(22, 0);
#else
        if (FlagChk(0x8ff)) {
            s32 msg = (s32)MsgScene659No155;

            Func_02001e18(msg);
            UiText_OpenMessageAtObject(22, 0);
            if (Inventory_PromptAndSetObjectMode(Func_02001ee8(), 0) == 0) {
                Battle_WaitMode0(10);
                Func_02001e18(msg + 1);
            } else {
                Battle_WaitMode0(20);
                Func_02001e18(msg + 2);
            }
            Func_02001e30(22, 0);
        } else {
            if (FlagChk(0x8ab))
                Func_02001e18((s32)MsgScene659No87);
            else
                Func_02001e18((s32)MsgScene659No39);
            Func_02001e30(22, 0);
        }
#endif
    }
}
